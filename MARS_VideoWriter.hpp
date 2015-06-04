#ifndef MARS_VideoWriter_H_
#define MARS_VideoWriter_H_

#include "./utils/thread.hpp"
#include "./utils/timer.hpp"
#include "./utils/udpPort.hpp"
#include "semaphore.h"

#include <stdio.h>
#include <string.h>

#include <fstream>
#include <string>
#include <sstream>
#include <math.h>
#include <algorithm>

//OpenGL libraries
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include <GL/glut.h>
#include <opencv/highgui.h>
#include <opencv/cv.h>

using namespace std;

class BufferItem{
	public:
		BufferItem* next;
		char* framedata;	
		BufferItem(int windowWidth, int windowHeight);
		~BufferItem();
};

class MARS_VideoWriter{
	friend class MARS_Engine;
	protected:
		bool recording;
		int windowWidth, windowHeight, fps;
		BufferItem* head, *tail;
		void queueFrameFromBackBuffer();
		pthread_mutex_t buffermutex; //a mutex lock for the bufferlist
		sem_t buffersem; //a semaphore for the bufferlist
		string filename_base;
		int filenum_counter;
		string getNextFilename();
	public:
		BufferItem* dequeue();
		void * (*threadstarter_ptr)(void*);
		Thread videoWriterThread;  //thread that runs the video writer
		void* videoWriteLoop();
		MARS_VideoWriter(const char* filename_base, int windowWidth, int windowHeight, int fps);
		~MARS_VideoWriter();
		bool record();
		void stop();
		
};
#endif
