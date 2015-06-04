#include "MARS_Graphic_ImageOnQuad.hpp"

void MARS_Graphic_ImageOnQuad::updateExtra(void* miscData);

MARS_Graphic_ImageOnQuad::~MARS_Graphic_ImageOnQuad(){
			delete[] this->stencilData;
			delete[] this->frameData;
}
		
void MARS_Graphic_ImageOnQuad::render(void* data){		
	//bind & update the texture
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texID);
	if (MIPMAPPING){	//NOTE: MIPMAPPING MIGHT GIVE THE IMAGE A HALO OF WHATEVER COLOR THE TRANSPARENT COLOR IS (DUE TO COMPRESSION ARTIFACTS)
		gluBuild2DMipmaps( GL_TEXTURE_2D, 3, this->imageWidth, this->imageHeight,
                       			GL_RGB, GL_UNSIGNED_BYTE, this->imageData );
	}
	else {						//SEE ABOVE NOTE. MIPMAPPING SHOULD PROBABLY BE DISABLED.
		glTexImage2D(GL_TEXTURE_2D, 0, 3, this->imageWidth, this->imageHeight, 
				0, GL_RGB, GL_UNSIGNED_BYTE, this->imageData);
	}
				
	//bind offscreen rendering buffers & setup drawing to offscreen
	//glPushAttrib(GL_VIEWPORT_BIT | GL_ENABLE_BIT);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboID); //bind the framebuffer
	glClearColor(transparent[0], transparent[1], transparent[2], 1.0f); //clear to the transparent color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, windowWidth, windowHeight);
				
	//offscreen draw the quad transformed properly and with its texture 
	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0,0.0); corner1.makeVertex();
	glTexCoord2d(1.0,0.0); corner2.makeVertex();
	glTexCoord2d(1.0,1.0); corner3.makeVertex();
	glTexCoord2d(0.0,1.0); glVertex3d(corner1.x + (corner3.x - corner2.x), corner1.y + (corner3.y - corner2.y), corner1.z + (corner3.z - corner2.z));
	glEnd();
		
	glReadPixels(0, 0, windowWidth, windowHeight, GL_RGB, GL_UNSIGNED_BYTE, frameData);
			
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0); //unbind the framebuffer (i.e. draw to backbuffer again)
	//glPopAttrib(); //restore viewport and enable bits if they were changed at al
	//glDrawPixels(windowWidth, windowHeight, GL_RGB, GL_UNSIGNED_BYTE, frameData);
				
	//fill our local stencil buffer
	for (int i = 0; i < windowHeight*windowWidth; i++){
			if (fabs(frameData[3*i]-transparent[0]*255.0) <= COLORTOL &&
				fabs(frameData[3*i+1]-transparent[1]*255.0) <= COLORTOL &&
				fabs(frameData[3*i+2]-transparent[2]*255.0) <= COLORTOL){
				stencilData[i] = 0; //shouldn't be drawn when rendering is done
			}
			else{
				stencilData[i] = 1; //should be drawn
			}
	}
	//enable stenciling and load the stencil buffer
				
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_NOTEQUAL, 0, 1); //ref 0, mask 1. All "should be drawns" are 1, masked with 1 is 1, 1 != 0,  therefore passes.
	glClearStencil(0);
	glClear(GL_STENCIL_BUFFER_BIT);
				
	glDrawPixels(windowWidth, windowHeight, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, stencilData); //draw the stencil buffer in based on our render from before

				
	//now actually draw the quad on the screen
	glColor4f(0.0f, 0.0f, 0.0f, this->color[3]);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0,0.0); corner1.makeVertex();
	glTexCoord2d(1.0,0.0); corner2.makeVertex();
	glTexCoord2d(1.0,1.0); corner3.makeVertex();
	glTexCoord2d(0.0,1.0); glVertex3d(corner1.x + (corner3.x - corner2.x), corner1.y + (corner3.y - corner2.y), corner1.z + (corner3.z - corner2.z));
	glEnd();
	glDisable( GL_TEXTURE_2D );	
	glDisable(GL_STENCIL_TEST);
}
		
		//note corner1, 2, 3 must be specified counterclockwise (when looking at the face with the texture)
		//further, corner1 will be the bottom left of the image, corner 2 bottom right, corner 3 top right
MARS_Graphic_ImageOnQuad::MARS_Graphic_ImageOnQuad(const char* name, GLubyte transparentr, GLubyte transparentg, GLubyte transparentb, GLubyte alpha, int windowWidth, int windowHeight, vector3<GLfloat> corner1, vector3<GLfloat> corner2, vector3<GLfloat> corner3, char* imageData, int imageWidth, int imageHeight) : MARS_Graphic(name,0,0,0,alpha) {
	this->corner1 = corner1;
	this->corner2 = corner2;
	this->corner3 = corner3;
	this->imageData = imageData;
	this->imageWidth = imageWidth;
	this->imageHeight = imageHeight;
	this->windowWidth = windowWidth;
	this->windowHeight = windowHeight;
	this->transparent[0] = (GLfloat)transparentr/255.0;
	this->transparent[1] = (GLfloat)transparentg/255.0;
	this->transparent[2] = (GLfloat)transparentb/255.0;	
			
	this->stencilData = new GLubyte[windowHeight*windowWidth];
	this->frameData = new GLubyte[windowHeight*windowWidth*3];
			
			
	//prepare the texture
	glGenTextures(1, &this->texID);
	glBindTexture(GL_TEXTURE_2D, texID);
	// the texture wraps over at the edges (repeat)
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	
	if (MIPMAPPING){
		//mipmapping enabled
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	}
	else {
		//mipmapping disabled
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
				glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	}	
			
	if (glewIsSupported("GL_EXT_framebuffer_object") == GL_FALSE){
		  std::cout << "ERROR: NO FBO support" << std::endl;
		  std::cout << "Did you try to call the ImageOnQuad constructor before calling MARS_Engine.init??" << std::endl;
		  std::cout << "This constructor relies on glew/glut being initialized, which occurs in MARS_Engine.init" << std::endl;
		  std::cout << "If your code is fine, and the problem persists, try rebooting, and/or updating your videocard driver." << std::endl;
	} 
			
	//prepare the framebuffer
	glGenFramebuffersEXT(1, &this->fboID);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, this->fboID); //bind the framebuffer
	//first, depth buffer
	glGenRenderbuffersEXT(1, &this->fbo_depthID); // depth buffer ID
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, fbo_depthID); // bind the render buffer
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, windowWidth, windowHeight); //allocates depth buffer-type storage for this renderbuffer
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, fbo_depthID); //attaches the buffer to the FBO as a depthbuf
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0); // Unbind the depth buffer 
	//next, renderbuffer
	glGenRenderbuffersEXT(1, &this->fbo_renderID); // depth buffer ID
			glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, fbo_renderID); // bind the render buffer
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_RGB8, windowWidth, windowHeight); //allocates depth buffer-type storage for this renderbuffer
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_RENDERBUFFER_EXT, fbo_renderID); //attaches the render buffer to FBO
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0); // Unbind the depth buffer 
	//finally, unbind the framebuffer
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);//unbind (switch back to rendering on backbuffer)	
}
