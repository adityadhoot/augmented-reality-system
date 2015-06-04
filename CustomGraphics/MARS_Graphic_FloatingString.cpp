#include "MARS_Graphic_FloatingString.hpp"

void MARS_Graphic_FloatingString::updateExtra(void* miscData);

void MARS_Graphic_FloatingString::render(void* viconData){
			//in OpenGL, the camera is ALWAYS at (0,0,0) and pointing towards (0,0,-1)
			//xaxis right, yaxis up, zaxis out of the screen towards your face
			//in order to make something always face the screen, its face must have a normal of (0,0,1)
			//in this case, we want text to face the camera. Usually when we render a coordinate, it undergoes two transformations: V*M*x, where x is coord,
			//M is model transform, V is view transform. M takes the coordinate to world space, and V transforms it into the camera's frame of reference
			//In this case, we want text to always face the screen, so we will operate in the camera's frame of reference from the start
			//as such, we will ditch the view matrix currently loaded (don't worry we'll pop it back in later in MARS_Engine, it's safe),
			//transform the actor's coordinate manually into view space, and place the text above that.
			updateVicon(viconData);			
			glLoadIdentity(); //the matrix is saved outside of this function in MARS_Engine, so this is OK.
			vector3<GLfloat> textpos = getPos() + displacement;
			GLfloat* view = cam_to_face->getViewMat();
			vector3<GLfloat> textpos_inviewspace(0,0,0);
			//multiply the view matrix manually
			textpos_inviewspace.x = view[0]*textpos.x + view[4]*textpos.y + view[8]*textpos.z + view[12];
			textpos_inviewspace.y = view[1]*textpos.x + view[5]*textpos.y + view[9]*textpos.z + view[13];
			textpos_inviewspace.z = view[2]*textpos.x + view[6]*textpos.y + view[10]*textpos.z + view[14];
			//the last row of the view matrix is just 0001 so don't bother, the result will be 1 (homog. coordinates)
			
			//now we know where to put our text in view space. (textpos_inviewspace)
		
			glPushAttrib(GL_LINE_BIT);
		    	glColor4f(this->color[0], this->color[1], this->color[2], this->color[3]);
		    	glLineWidth(this->textthickness);
		    	glTranslatef(textpos_inviewspace.x, textpos_inviewspace.y, textpos_inviewspace.z);
		    	glScalef(this->textheight/152.38, this->textheight/152.38, this->textheight/152.38);
		    	for (int i = 0; i < text.size(); i++)
		        	glutStrokeCharacter(GLUT_STROKE_ROMAN, text[i]);
		    	glPopAttrib();
}
		
MARS_Graphic_FloatingString::MARS_Graphic_FloatingString(const char* name, GLubyte r, GLubyte g, GLubyte b, GLubyte a, 
									MARS_Camera* cam_to_face, vector3<GLfloat> displacement, 
									string text, GLfloat textthickness, GLfloat textheight) :
									MARS_Graphic(name,r, g, b, a){
			this->cam_to_face = cam_to_face;
			this->displacement = displacement;
			this->textthickness = textthickness;
			this->textheight = textheight;
			this->text = text;
}
		
void MARS_Graphic_FloatingString::setText(string s){
	this->text = s;
}
