#ifndef MARS_Camera_H_
#define MARS_Camera_H_

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

#include "MARS_Math.hpp"


class MARS_Camera {
	friend class MARS_Engine;
	protected:
		char name[128];
		CvCapture* capture;
		float focallength; //should be in pixels
		GLfloat viewmat[16];
		int ncam;

		//there are two ways of updating the camera
		//1. With eye, look, up vectors
		vector3<GLfloat> eye, look, up;
		//2. Or with pos and rotation matrix
		vector3<GLfloat> pos;
		GLfloat rotmat[9];

	public:
		enum CalcViewMethod {CALCVIEWMETHOD_ELU, CALCVIEWMETHOD_RT};
		CalcViewMethod calcviewmethod;
		MARS_Camera(char *name, int ncam, float focallength,CalcViewMethod calcviewmethod);
		~MARS_Camera();
		void update(void* data);
		void update_test(void* data);
		char* getName();
		CvCapture* getCapture();
		float getFocalLength();
		void setFocalLength(float focallength);
		vector3<GLfloat> getEye();
		vector3<GLfloat> getLook();
		vector3<GLfloat> getUp();
		void calcViewMat();
		void calcViewMat_ELU();
		void calcViewMat_RT();
		GLfloat* getViewMat();
		
		//gui functions
		//virtual QWidget* provideUnAddedPage()=0;
		//virtual QWidget* provideAddedPage()=0;	
};
#endif
