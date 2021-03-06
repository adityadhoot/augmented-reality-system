#include "MARS_Engine.hpp"

bool MARS_Engine::keep_running = true;
bool MARS_Engine::toggle_recording = false;

RenderLoopLimiter::RenderLoopLimiter(int fps){
	this->dt = 1.0/(double)fps;
	// Build Semaphore Names
    sem_init(&sem_timer, 0, 0);    
    clock_gettime(CLOCK_REALTIME, &releasetime);
}
void RenderLoopLimiter::wait(){ //first time wait is called, it'll pass right through the timedwait.
	sem_timedwait(&sem_timer, &releasetime);
	clock_gettime(CLOCK_REALTIME, &releasetime);
	double t = (double)releasetime.tv_sec + (double)releasetime.tv_nsec/1000000000.0 + dt;
	releasetime.tv_sec = floor(t);
	releasetime.tv_nsec = 1000000000.0*(t-floor(t));
}

RenderLoopLimiter::~RenderLoopLimiter(){
	sem_destroy(&sem_timer);
}

MARS_Engine::MARS_Engine(MARS_Camera* cam, int argc, char** argv){
	string vicon_ip = "192.168.0.255";
	int vicon_port = 9999;
	int vicon_bufsize = 2048;

	string misc_ip = "192.168.0.255";
	int misc_port = 20000;
	int misc_bufsize = 2048;

	MARS_Datasource* viconsrc = new MARS_Datasource(vicon_ip, vicon_port, vicon_bufsize, misc_ip, misc_port, misc_bufsize);
	this->datasrc = viconsrc;

	//this->nextactorid = 0;
	this->nextgraphicid = 0;
	this->windowID = 0;
	this->cam = cam;
	if(!this->init(argc, argv)){
		fprintf(stderr, "ERROR: MARS_Engine::init failed.\n");
	}
}

MARS_Engine::~MARS_Engine(){
	printf("MARS_Engine: Shutting down & cleaning up...\n");
	delete cam;
	datasrc->vicon_stopAndJoin();
	datasrc->misc_stopAndJoin();	
	delete datasrc;
	delete vidwriter;
	//for (vector<MARS_Actor*>::iterator it = this->actorlist.begin(); it < this->actorlist.end(); it++){
	//	delete *it;
	//}
	for (vector<MARS_Graphic*>::iterator it = this->graphiclist.begin(); it < this->graphiclist.end(); it++){
		delete *it;
	}
	glutDestroyWindow(this->windowID);
	printf("MARS_Engine: Successfully shut down.\n");
}

bool MARS_Engine::init(int argc, char** argv){
	
	if (!this->cam) {
		fprintf(stderr, "ERROR: Camera is NULL. Please add a camera before calling init().\n");
		return false;
	}
	
	if (!this->datasrc) {
		fprintf(stderr, "ERROR: Datasource is NULL. Please add a datasource before calling init().\n");
		return false;
	}
	
	if (this->graphiclist.size() != 0){
		fprintf(stderr, "ERROR: Graphics cannot be added prior to calling init, as they may rely on glut/glew having been initialized.\n");
	}
	
	glutInit(&argc, argv);
	
	//get whatever is necessary from a camera frame (window width, height, etc.)
	this->bgr_frame = cvQueryFrame(cam->getCapture()); 
	windowWidth = this->bgr_frame->width;
	windowHeight = this->bgr_frame->height;
	float fovy = 180/M_PI*2*atan(windowHeight/(2*cam->getFocalLength()));
	this->aspectratio = (float)windowWidth/(float)windowHeight;
	this->renderedvideoheight = 2*VIDEODIST*tan(fovy*M_PI/180/2);
	
	//initialize glut window parameters and callbacks
	glutInitWindowPosition(-1,-1); //for now we don't care where the window starts
	glutInitWindowSize(windowWidth, windowHeight); //initialize window size with resolution of video image
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL); //double buffered, RGBA, with depth buffer & stencil buffer
	this->windowID = glutCreateWindow("CameraWindow");
	glutKeyboardFunc(MARS_keypressed); // sets the function to process keyboard input
	
	//initialize the iplimages used to convert opencv crazy flipped bgr to normal orientation rgb
	this->rgb_frame = cvCreateImage(cvSize(this->bgr_frame->width, this->bgr_frame->height), IPL_DEPTH_8U, 3);
	this->flipped_frame = cvCreateImage(cvSize(this->bgr_frame->width, this->bgr_frame->height), IPL_DEPTH_8U, 3);
	
	//setup the projection transform
	glMatrixMode(GL_PROJECTION);                        // Select The Projection Matrix
    	glLoadIdentity();                           // Reset The Projection Matrix
 	gluPerspective(fovy,(GLfloat)windowWidth/(GLfloat)windowHeight,NEARPLANE,FARPLANE);

	//setup the modelview transform
   	 glMatrixMode(GL_MODELVIEW);                     // Select The Modelview Matrix
   	 glLoadIdentity();                           // Reset The Modelview Matrix
    
    //setup OpenGL's shading, clear color, depthtesting and perspective correction
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); 
	glClearDepth(1.0f);                         // Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);                        // Enables Depth Testing
	glDepthFunc(GL_LEQUAL);                         // The Type Of Depth Test To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	

	//prepare the video texture
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL ); //DECAL means just ignore the object's color and only use texture color
	glGenTextures(1, &this->vidtex);
	glBindTexture(GL_TEXTURE_2D, vidtex);
	
	// the texture wraps over at the edges (repeat)
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	
	if (MIPMAPPING){
		//mipmapping enabled
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	}
	else {
		//mipmapping disabled
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	}
	
	//setup glew for drawing to the offscreen framebuffer (see the ImageOnQuad custom graphic for why this is needed)
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
	  /* Problem: glewInit failed, something is seriously wrong. */
	  fprintf(stdout, "Error: %s\n", glewGetErrorString(err));
	  return false;
	}
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

	//start up the datasource thread
	if(!this->datasrc->run()){
		fprintf(stdout, "ERROR: Datasource->run() failed in MARS_Engine::init()\n");
		return false;
	}
	
	this->fps = 30;
	this->vidwriter = new MARS_VideoWriter("MARS_Video_", windowWidth, windowHeight, fps); 

	return true;
}

//int MARS_Engine::addActor(MARS_Actor* act){
//	act->MARS_id = this->nextactorid++;
//	this->actorlist.push_back(act);
//	return act->MARS_id;
//}

int MARS_Engine::addGraphic(MARS_Graphic* gr){
	gr->MARS_id = this->nextgraphicid++;
	this->graphiclist.push_back(gr);
	return gr->MARS_id;
}

void MARS_Engine::addCamera(MARS_Camera* cm){
	this->cam = cm;
}

void MARS_Engine::addDatasource(MARS_Datasource* datasrc){
	this->datasrc = datasrc;
}

void MARS_Engine::renderFrame(){
	//this is the program loop
	//first update cameras
	//update camera and actors
	//put a lock on the datasource so that it doesn't touch the data while cameras and actors are using it
	static char viconBuf[2048]; //static so it doesn't keep allocating it
	static char miscBuf[2048]; //static so it doesn't keep allocating it
	//TO DO: Change the manual miscBuf size entry.

	datasrc->getviconBuffer(viconBuf); //copies the most recent data into the buffer
	datasrc->getmiscBuffer(miscBuf); //copies the most recent data into the buffer

	//printf("Vicon Buffer: %s\n", viconBuf);
	//printf("Misc Buffer: %s\n", miscBuf);

	//cam->update_test(buf);
	cam->update(viconBuf);
	cam->calcViewMat();
//	for(vector<MARS_Actor*>::iterator it = actorlist.begin(); it < actorlist.end(); it++){
//		(*it)->update(buf);
//	}
	
	glutMainLoopEvent(); //signal to GLUT that there's a new frame & process all pending windowing stuff
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); //clear the color, stencil, and depth buffers
	
	//get a camera frame
	bgr_frame = cvQueryFrame(cam->getCapture()); 
	cvCvtColor(bgr_frame, rgb_frame, CV_BGR2RGB);
	cvFlip(rgb_frame, flipped_frame, 0); //0 here means flip only vert
	
	
	//draw it to a texture and onto a quad
	glEnable( GL_TEXTURE_2D );
	glBindTexture(GL_TEXTURE_2D, this->vidtex);
	if (MIPMAPPING){
		gluBuild2DMipmaps( GL_TEXTURE_2D, 3, flipped_frame->width, flipped_frame->height,
              				GL_RGB, GL_UNSIGNED_BYTE, flipped_frame->imageData );
    	}
    	else {
		glTexImage2D(GL_TEXTURE_2D, 0, 3, flipped_frame->width, flipped_frame->height, 
				0, GL_RGB, GL_UNSIGNED_BYTE, flipped_frame->imageData);
	}
    
        glLoadIdentity(); //this causes the video to be facing the camera no matter what else is going on in the screen
    	glBegin(GL_QUADS);
    	glTexCoord2d(0.0,0.0); glVertex3d(-renderedvideoheight*aspectratio/2.0, -renderedvideoheight/2.0, -VIDEODIST);
	glTexCoord2d(1.0,0.0); glVertex3d(renderedvideoheight*aspectratio/2.0, -renderedvideoheight/2.0, -VIDEODIST);
	glTexCoord2d(1.0,1.0); glVertex3d(renderedvideoheight*aspectratio/2.0, renderedvideoheight/2.0, -VIDEODIST);
	glTexCoord2d(0.0,1.0); glVertex3d(-renderedvideoheight*aspectratio/2.0, renderedvideoheight/2.0, -VIDEODIST);
	glEnd();
    	glDisable( GL_TEXTURE_2D );
    	
    	//setup the view matrix
    	glMultMatrixf(cam->getViewMat());  	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	//draw graphics
	//TODO: fix blending with enable depth buffer and draw from farthest to nearest
	for(vector<MARS_Graphic*>::iterator it = graphiclist.begin(); it < graphiclist.end(); it++){
		glPushMatrix();
		(*it)->render(viconBuf, miscBuf);
		glPopMatrix();
	}
	glDisable(GL_BLEND);
}

void MARS_Engine::swapBuffers(){
	glutSwapBuffers();
}

void MARS_Engine::renderLoop(){
	//setup timer stuff for maintaining fps
	RenderLoopLimiter rll(this->fps);
	while (MARS_Engine::keep_running){
		//limit the cycle speed to fps frames per second
		rll.wait();
		//check if we want to toggle recording
		if (MARS_Engine::toggle_recording){
			if(vidwriter->recording){
				vidwriter->stop();
			} else {
				vidwriter->record();
			}
			MARS_Engine::toggle_recording = false;
		}
		
		//render a frame (don't swap buffers yet)
		renderFrame();
		//put the frame in the video writer buffer
		if (vidwriter->recording){
			vidwriter->queueFrameFromBackBuffer();
		}
		swapBuffers();
	}
}

void MARS_Engine::stop(){
	MARS_Engine::keep_running = false;
}

void MARS_Engine::toggleRecord(){
	MARS_Engine::toggle_recording = true;
}

int MARS_Engine::getWindowWidth(){
	return this->windowWidth;
}

int MARS_Engine::getWindowHeight(){
	return this->windowHeight;
}

void MARS_keypressed(unsigned char key, int x, int y){
	if (key == 27) //esc
		MARS_Engine::stop();
	if (key == 32) //spacebar
		MARS_Engine::toggleRecord();
}
