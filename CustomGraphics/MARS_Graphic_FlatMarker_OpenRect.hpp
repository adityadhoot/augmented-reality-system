#ifndef MARS_Graphic_FlatMarker_OpenRect_H_
#define MARS_Graphic_FlatMarker_OpenRect_H_

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


class MARS_Graphic_FlatMarker_OpenRect : public MARS_Graphic{ //the three corners must be counter clockwise view
	private:
		vector3<GLfloat> corner1, corner2, corner3;
		GLfloat thickness;
	public:
		void setThickness(GLfloat thickness);
		void render(void* data);
		MARS_Graphic_FlatMarker_OpenRect(const char* name, GLubyte r, GLubyte g, GLubyte b, GLubyte a,
						vector3<GLfloat> corner1, vector3<GLfloat> corner2, 
						vector3<GLfloat> corner3, GLfloat thickness);
};
#endif
