#ifndef MARS_Graphic_FlatMarker_FillCircle_H_
#define MARS_Graphic_FlatMarker_FillCircle_H_


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


#include "../MARS_Math.hpp"
#include "../MARS_Graphic.hpp"


class MARS_Graphic_FlatMarker_FillCircle : public MARS_Graphic{ //the three corners must be counter clockwise view
	private:
		vector3<GLfloat> centre, normal;
		GLfloat radius;
		int resolution;
	public:
		
		void setRadius(GLfloat radius);
		void setResolution(int resolution);
		void render(void* Vicondata, void* Miscdata);
		void updateMisc(void* data);		
		MARS_Graphic_FlatMarker_FillCircle(const char* name, GLubyte r, GLubyte g, GLubyte b, GLubyte a, 
							vector3<GLfloat> centre, vector3<GLfloat> normal, 
							int resolution, GLfloat radius);
};
#endif
