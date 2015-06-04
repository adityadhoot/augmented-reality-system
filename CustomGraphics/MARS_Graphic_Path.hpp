#ifndef MARS_Graphic_Path_H_
#define MARS_Graphic_Path_H

class MARS_Graphic_Path : public MARS_Graphic{
	private:
		int min_extra_ind, extra_ind_length;
		GLfloat lwidth;
		vector < vector3<GLfloat> > path; //this was originally in MARS_ActorProperty_PathPlanner
	public:
		MARS_Graphic_Path(const char* name, GLubyte r, GLubyte g, GLubyte b, GLubyte a, GLfloat lwidth);		
		void render(void* data);
		void setLineWidth(GLfloat lwidth);

		void clearPath();
		vector <vector3<GLfloat> >& getPath();
		void addToPath(vector3<GLfloat>& newpt);
		void removeOldestPoint();

		void updateExtra(void* miscData);
};
#endif
