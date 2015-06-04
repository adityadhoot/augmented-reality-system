#ifndef MARS_Graphic_H_
#define MARS_Graphic_H_


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

class MARS_Graphic{
	friend class MARS_Engine;
	protected:
		const static int namesize = 128; //from MARS_Actor_ViconTracked
		char name[namesize]; //from MARS_Actor_ViconTracked
		int vicon_ID; //from MARS_Actor_ViconTracked
		int MARS_id;
		
		
		bool enabled;
		GLfloat color[4];
		vector3<GLfloat> position, velocity;
		GLfloat psi, theta, phi, dpsi, dtheta, dphi;
		void updateVicon(void* viconData);//for updating actor positions
		virtual void updateMisc(void* miscData)=0;
	public:

		virtual void render(void* viconData, void* miscData)=0;
		void setEnabled(bool en);
		bool isEnabled();
		void setColor(GLubyte r, GLubyte g, GLubyte b, GLubyte alpha);
		MARS_Graphic(const char* name, GLubyte r, GLubyte g, GLubyte b, GLubyte alpha);
		virtual ~MARS_Graphic(){};
};
#endif









