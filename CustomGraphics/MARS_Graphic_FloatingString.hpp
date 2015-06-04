#ifndef MARS_Graphic_FloatingString_H_
#define MARS_Graphic_FloatingString_H_

class MARS_Graphic_FloatingString : public MARS_Graphic{
	private:
		MARS_Camera* cam_to_face;
		vector3<GLfloat> displacement;
		GLfloat textthickness;
		GLfloat textheight;
		string text;
	public:
		void render(void* viconData, void* miscData);		
		MARS_Graphic_FloatingString(const char* name, GLubyte r, GLubyte g, GLubyte b, GLubyte a, 
							MARS_Camera* cam_to_face,
							vector3<GLfloat> displacement, string text, GLfloat textthickness, GLfloat textheight);	
		void setText(string s);
		void updateExtra(void* miscData);
};
#endif
