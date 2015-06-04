#include "MARS_Graphic_FlatMarker_OpenRect.hpp"

void MARS_Graphic_FlatMarker_OpenRect::setThickness(GLfloat thickness){
	this->thickness = thickness;
}
	
void MARS_Graphic_FlatMarker_OpenRect::render(void* data){
	//calculate the norm vectors to construct the various verticies of the rectangle
	vector3<GLfloat> n1 = (corner2 - corner1)/(corner2-corner1).norm();
	vector3<GLfloat> n2 = (corner3 - corner2)/(corner3-corner2).norm();
	//start making the verticies
	vector3<GLfloat> v1 = corner1;
	vector3<GLfloat> v2 = corner1 - n2*thickness;
	vector3<GLfloat> v3 = corner2;
	vector3<GLfloat> v4 = corner2 - n2*thickness;
	vector3<GLfloat> v5 = corner2;
	vector3<GLfloat> v6 = corner2 + n1*thickness;
	vector3<GLfloat> v7 = corner3;
	vector3<GLfloat> v8 = corner3 + n1*thickness;
	vector3<GLfloat> v9 = corner3;
	vector3<GLfloat> v10 = corner3 + n2*thickness;
	vector3<GLfloat> corner4 = corner1 + (corner3 - corner2);
	vector3<GLfloat> v11 = corner4;
	vector3<GLfloat> v12 = corner4 + n2*thickness;
	vector3<GLfloat> v13 = corner4;
	vector3<GLfloat> v14 = corner4 - n1*thickness;
	vector3<GLfloat> v15 = corner1;
	vector3<GLfloat> v16 = corner1 - n1*thickness;
	vector3<GLfloat> v17 = corner1;
	vector3<GLfloat> v18 = corner1 - n2*thickness;
			
	//draw them		
	glColor4f(this->color[0], this->color[1], this->color[2], this->color[3]);
	glBegin(GL_QUAD_STRIP);
	v1.makeVertex();
	v2.makeVertex();
	v3.makeVertex();
	v4.makeVertex();
	v5.makeVertex();
	v6.makeVertex();
	v7.makeVertex();
	v8.makeVertex();
	v9.makeVertex();
	v10.makeVertex();
	v11.makeVertex();
	v12.makeVertex();
	v13.makeVertex();
	v14.makeVertex();
	v15.makeVertex();
	v16.makeVertex();
	v17.makeVertex();
	v18.makeVertex();
	glEnd();
}
		
MARS_Graphic_FlatMarker_OpenRect::MARS_Graphic_FlatMarker_OpenRect(const char* name, GLubyte r, GLubyte g, GLubyte b, GLubyte a,
								vector3<GLfloat> corner1, vector3<GLfloat> corner2, 
								vector3<GLfloat> corner3, GLfloat thickness) : 
								MARS_Graphic(name,r, g, b, a) {
	this->corner1 = corner1;
	this->corner2 = corner2;
	this->corner3 = corner3;
	this->thickness = thickness;
}
