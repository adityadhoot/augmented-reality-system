#ifndef MARS_Graphic_ImageOnQuad_H_
#define MARS_Graphic_ImageOnQuad_H_

#define COLORTOL 2 //tolerance (in ubyte format) for transparent color check

class MARS_Graphic_ImageOnQuad : public MARS_Graphic{ //the three corners must be counter clockwise view
	private:
		vector3<GLfloat> corner1, corner2, corner3;
		char* imageData;
		GLubyte *stencilData, *frameData;
		GLuint fboID, fbo_depthID, fbo_renderID;
		int imageWidth, imageHeight;
		int windowWidth, windowHeight;
		GLuint texID;
		GLfloat transparent[3];
	public:
		~MARS_Graphic_ImageOnQuad();
		void render(void* data);

		void updateExtra(void* miscData);
		
		//note corner1, 2, 3 must be specified counterclockwise (when looking at the face with the texture)
		//further, corner1 will be the bottom left of the image, corner 2 bottom right, corner 3 top right
		MARS_Graphic_ImageOnQuad(const char* name, GLubyte transparentr, GLubyte transparentg, GLubyte transparentb, 
					GLubyte alpha, int windowWidth, int windowHeight, vector3<GLfloat> corner1, 
					vector3<GLfloat> corner2, vector3<GLfloat> corner3, char* imageData, 
					int imageWidth, int imageHeight);
};
#endif
