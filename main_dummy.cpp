#include "MARS.hpp"

class MARS_Camera_Dummy : public MARS_Camera{
	public:
		MARS_Camera_Dummy() : MARS_Camera(0,  5.94406677e+02, CALCVIEWMETHOD_ELU){}
		void update( void * data){}
};

class MARS_Actor_Dummy : public MARS_Actor{
	public:
		void update(void * data) { vector3<GLfloat> pose(0, 0, -3); this->pos = pose;}
};

int main(int argc, char** argv){
	
	//create the camera
	MARS_Camera_Dummy* dum = new MARS_Camera_Dummy();
	
	//create the datasource
	string ip = "192.168.0.255";
	int port = 20001;
	int bufsize = 2048;
	MARS_Datasource_UDP* udpsrc = new MARS_Datasource_UDP(ip, port);
	
	//create & initialize mars engine
	MARS_Engine* mars = new MARS_Engine(dum, udpsrc, argc, argv);
	
	//add actors
	MARS_Actor_Dummy* dumdum = new MARS_Actor_Dummy();
	mars->addActor(dumdum);


	//add graphics
	vector3<GLfloat> c1(0.5f, 0.0f, -5.0f), c2(0.5f, 1.0f, -5.0f), c3(-1.5f, 1.0f, -5.0f);
	MARS_Graphic_FlatMarker_OpenRect* rect = new MARS_Graphic_FlatMarker_OpenRect(0, 255, 255, 128, c1, c2, c3, 0.5f);
	
	vector3<GLfloat> centre(0.0f, 0.5f, -6.0f), normal(0.0f, 0.0f, 1.0f);
	MARS_Graphic_FlatMarker_FillCircle* circ = new MARS_Graphic_FlatMarker_FillCircle(255, 255, 0, 255, centre, normal, 30, 0.5f);
	
	vector3<GLfloat> disp(0, 0, 0);
	MARS_Graphic_FloatingString* floater = new MARS_Graphic_FloatingString( 0, 255, 0, 255, dumdum, dum, disp, "string", 4.0f, 0.2f);
	
	IplImage* rawimg, *rgbimg, *flipimg; 
  	rawimg=cvLoadImage("img.png");
	rgbimg = cvCreateImage(cvSize(rawimg->width, rawimg->height), IPL_DEPTH_8U, 3);
	flipimg = cvCreateImage(cvSize(rawimg->width, rawimg->height), IPL_DEPTH_8U, 3);
  	cvCvtColor(rawimg, rgbimg, CV_BGR2RGB);
	cvFlip(rgbimg, flipimg, 0); //0 here means flip only vert
	
  	c1.set(-1.5f, 0.0f, -3.0f);
  	c2.set(0.5f, 0.0f, -3.0f);
  	c3.set(0.5f, 1.0f, -3.0f);
	 
	MARS_Graphic_ImageOnQuad* quadimg = new MARS_Graphic_ImageOnQuad(227, 70, 203, 255, mars->getWindowWidth(), mars->getWindowHeight(), c1, c2, c3, flipimg->imageData, flipimg->width, flipimg->height);

	mars->addGraphic(rect);
	mars->addGraphic(circ);
	mars->addGraphic(floater);
	mars->addGraphic(quadimg);
	
	//render!
	while (MARS_Engine::keep_running){	
		mars->renderFrame();
	}
	delete mars;

}
