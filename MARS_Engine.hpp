#ifndef MARS_Engine_H_
#define MARS_Engine_H_

#define MIPMAPPING FALSE
#define FARPLANE 100.0f
#define NEARPLANE 0.1f
#define VIDEODIST 95.0f


#include <stdio.h>
#include <string.h>

#include <sys/time.h>
#include <sys/select.h>
#include <semaphore.h>

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

#include "MARS_Math.hpp"
#include "MARS_Camera.hpp"
#include "MARS_Graphic.hpp"
#include "MARS_Datasource.hpp"
#include "MARS_VideoWriter.hpp"

class RenderLoopLimiter{
	public:
		double dt;
		RenderLoopLimiter(int fps);
		~RenderLoopLimiter();
		void wait();
	private:
		timespec releasetime;
		sem_t sem_timer;
};

void MARS_keypressed(unsigned char key, int x, int y);
class MARS_Engine{
	private:
		MARS_Camera* cam;
		//vector<MARS_Actor*> actorlist;
		vector<MARS_Graphic*> graphiclist;
		MARS_Datasource* datasrc;
		MARS_VideoWriter* vidwriter;
		//int nextactorid;
		int nextgraphicid;
		float renderedvideoheight, aspectratio;
		IplImage *rgb_frame, *bgr_frame, *flipped_frame;
		GLuint windowID, vidtex;
		int windowWidth, windowHeight, fps;
		void swapBuffers();
	public:
		MARS_Engine(MARS_Camera* cam, int argc, char** argv);
		~MARS_Engine();
		bool init(int argc, char** argv);
		//int addActor(MARS_Actor* act);
		int addGraphic(MARS_Graphic* gr);
		void addCamera(MARS_Camera* cm);
		void addDatasource(MARS_Datasource* datasrc);
		void renderFrame();
		static bool keep_running;
		static void stop();
		int getWindowWidth();
		int getWindowHeight();
		void renderLoop();
		static void toggleRecord();
		static bool toggle_recording;
};
#endif
