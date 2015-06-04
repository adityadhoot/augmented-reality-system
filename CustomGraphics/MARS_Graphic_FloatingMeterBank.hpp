#ifndef MARS_Graphic_FloatingMeterBank_H_
#define MARS_Graphic_FloatingMeterBank_H_

class MARS_Graphic_FloatingMeterBank : public MARS_Graphic{
	private:

		vector<float> resources;
		vector<float> resource_maxes;
		vector< vector3<GLfloat> > displacements;
		
		Glfloat rh, gh, bh, rm, gm, bm, rl, gl, bl;
		GLfloat radius;
		GLfloat height;
		
		void updateMisc(void* miscdata);
	public:
		void render(void* vicondata, void* miscdata);
		MARS_Graphic_FloatingMeterBank(const char* viconname,
													GLubyte rc, GLubyte gc, GLubyte bc,
													 GLubyte rh, GLubyte gh, GLubyte bh, 
													 GLubyte rm, GLubyte gm, GLubyte bm,
													 GLubyte rl, GLubyte gl, GLubyte bl, GLubyte a, 
					   vector<float> resource_maxes,
					   vector< vector3<GLfloat> > displacements, GLfloat radius, GLfloat height);
			
		void setResourceValue(float resource);
		float getResourceValue();
		float getResourceMax();
};
#endif
