#include "MARS_Graphic_FlatMarker_FillCircle.hpp"

void MARS_Graphic_FlatMarker_FillCircle::setRadius(GLfloat radius){
	this->radius = radius;
}
void MARS_Graphic_FlatMarker_FillCircle::setResolution(int resolution){
	this->resolution = resolution;
}

void MARS_Graphic_FlatMarker_FillCircle::updateMisc(void* data){
}

	
void MARS_Graphic_FlatMarker_FillCircle::render(void* Vicondata, void* Miscdata){
	vector3<GLfloat> temp(1, 0, 0); //pick a random vector for cross product
	if (fabs(1 - fabs(temp.dot(normal)/normal.norm())) < 0.1) //if it's too close to being parallel, just pick a different one
		temp.set(0, 1, 0);
	//find a frame with normal as NEGATIVE Z
	vector3<GLfloat> framex = normal.cross(temp);
	vector3<GLfloat> framey = framex.cross(normal);
	framex = framex / framex.norm();
	framey = framey / framey.norm();
						
	//set the view  (and later rotate about the normal)
	GLfloat mat[16] = {framex.x, framex.y, framex.z, 0, //col 1
			   framey.x, framey.y, framey.z, 0, //col 2
			   -normal.x, -normal.y, -normal.z, 0, //col 3
			   centre.x,centre.y,centre.z,1};
	glMultMatrixf(mat);
	
	//draw them
	glColor4f(this->color[0], this->color[1], this->color[2], this->color[3]);
	glBegin(GL_TRIANGLE_FAN);			
	glVertex3f(0, 0, 0); //centre of fan
	double rot_rad = 0;
	double rot_step = 2.0*M_PI/resolution;
	temp.set(1.0f, 0, 0);
	double xtemp, ytemp;
	while (1){
		temp.makeVertex();
		xtemp = temp.x*cos(rot_step) + temp.y*sin(rot_step);
		ytemp = temp.y*cos(rot_step) - temp.x*sin(rot_step);
		temp.x = xtemp;
		temp.y = ytemp;
		rot_rad += rot_step;
		if (rot_rad >= 2.0*M_PI){
			xtemp = temp.x*cos(2.0*M_PI - rot_rad) + temp.y*sin(2.0*M_PI - rot_rad);
			ytemp = temp.y*cos(2.0*M_PI - rot_rad) - temp.x*sin(2.0*M_PI - rot_rad);
			temp.x = xtemp;
			temp.y = ytemp;
			temp.makeVertex();
			break;
		}
	}	
	glEnd();
}
		
MARS_Graphic_FlatMarker_FillCircle::MARS_Graphic_FlatMarker_FillCircle(const char* name, GLubyte r, GLubyte g, GLubyte b, GLubyte a,
				vector3<GLfloat> centre, vector3<GLfloat> normal, int resolution, GLfloat radius) : 
				MARS_Graphic(name, r, g, b, a) {
	this->centre = centre;
	this->normal = normal / normal.norm(); //makes sure it's normalized
	this->radius = radius;
	this->resolution = resolution;
}
