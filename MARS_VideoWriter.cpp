#include "MARS_VideoWriter.hpp"

BufferItem::BufferItem(int windowWidth, int windowHeight){
	framedata = new char[windowHeight*windowWidth*3];
	glReadPixels(0, 0, windowWidth, windowHeight, GL_RGB, GL_UNSIGNED_BYTE, framedata);
	next = NULL;
}
BufferItem::~BufferItem(){
	delete[] framedata;
}


void * videoWriterThreadStarter(void * data) {
	printf ("Starting VideoWriter Thread...\n");
	MARS_VideoWriter* videowriter = (MARS_VideoWriter*) data;
	return videowriter->videoWriteLoop();
}

MARS_VideoWriter::MARS_VideoWriter(const char* filename_base, int windowWidth, int windowHeight, int fps){
	this->windowWidth = windowWidth;
	this->windowHeight = windowHeight;
	this->fps = fps;
	this->recording = false;
	
	pthread_mutex_init(&buffermutex, NULL);
	if (sem_init(&buffersem, 0, 0) == -1){
		printf("Couldn't initialize semaphore in MARS_VideoWriter constructor.\n Please make sure you have administrator privileges.\n");
	}
	
	head = tail = NULL;

	this->threadstarter_ptr = videoWriterThreadStarter;
	this->filename_base = filename_base;
	this->filenum_counter = 0;
}

string MARS_VideoWriter::getNextFilename(){
	stringstream ss;
	ss << setw(4) << setfill('0') << filenum_counter;
	filenum_counter++;
	return this->filename_base + ss.str() + ".mpeg";
}

MARS_VideoWriter::~MARS_VideoWriter(){
	sem_destroy(&buffersem);
	pthread_mutex_destroy(&buffermutex);
}

void * MARS_VideoWriter::videoWriteLoop(){
	string fname = this->getNextFilename();
	printf("Recording in %s...\n", fname.c_str());
   	//## Setup the opencv writer
	CvVideoWriter *writer = NULL;
	int isColor = 1;
	writer=cvCreateVideoWriter(fname.c_str(),CV_FOURCC('P','I','M','1'),
			                   fps,cvSize(windowWidth,windowHeight),isColor);
			 
	IplImage *imgrgb= cvCreateImage(cvSize(windowWidth,windowHeight), IPL_DEPTH_8U, 3),
			 * img = cvCreateImage(cvSize(windowWidth,windowHeight), IPL_DEPTH_8U, 3);
	cvFree(&img->imageDataOrigin); //need to deallocate the image data
	img->dataOrder = 0; //interleaved colour channels
	img->origin = 0; //top left origin? I think. change this to 1 if flipped
		
			             	
	// Main loop
	while( this->recording){
		//dequeue a frame
		BufferItem* bi = this->dequeue(); //openwill wait for the semaphore to be > 0, and then mutex lock/unlock the buffer for access.
		if (!this->recording){ //check right after the dequeue as well, in case sem_post was called to unwait this thread from MARS_VideoWriter::stop
			break;
		}
		//convert to IplImage
		img->imageData = bi->framedata;
		img->imageDataOrigin = bi->framedata;
		cvFlip(img, NULL, 0);
		cvCvtColor(img, imgrgb, CV_BGR2RGB);
		//write the frame to disk
		cvWriteFrame(writer,imgrgb);
		//cleanup the memory associated with the frame
		delete bi; 
	}
	
	cvReleaseVideoWriter(&writer);
	//## Exit cleanly
	sched_yield();
	usleep(1000);
	printf("Closing %s.\n", fname.c_str());
	return(0);
}


bool MARS_VideoWriter::record(){		
	this->recording = true;
	if (!videoWriterThread.start(threadstarter_ptr,this)){
		printf("ERROR: Could not start the VideoWriter thread in MARS_VideoWriter::run().\n");
		printf("NOTE: Please make sure you have administrative privileges when running this.\n");
		printf("\tto allow the VideoWriter thread to start.\n");
		return false;
	}
	return true; 
}


void MARS_VideoWriter::stop(){
	this->recording = false;
	sem_post(&buffersem); //in case the thread is waiting on the buffer to become nonempty
	if (!this->videoWriterThread.join())
		printf("ERROR: VideoWriter Thread did not join correctly in MARS_VideoWriter::stopAndJoin()!\n");
	this->videoWriterThread.close_thread();
}

void MARS_VideoWriter::queueFrameFromBackBuffer(){
	BufferItem* bfi = new BufferItem(windowWidth, windowHeight);
	pthread_mutex_lock(&buffermutex);
		int bufferlen;
		sem_getvalue(&buffersem, &bufferlen);
		if (bufferlen == 0){
			head = tail = bfi;
		} else {
			tail->next = bfi;
			tail = tail->next;
		}
	pthread_mutex_unlock(&buffermutex);
	sem_post(&buffersem);
}

BufferItem* MARS_VideoWriter::dequeue(){
	sem_wait(&buffersem);
	pthread_mutex_lock(&buffermutex);
		BufferItem* dq = head;
		if (head != NULL){ //head might = NULL when MARS_VideoWriter::stop is called.
			head = head->next;
		}
	pthread_mutex_unlock(&buffermutex);
	return dq;
}

