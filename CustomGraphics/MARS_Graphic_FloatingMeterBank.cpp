#include "MARS_Graphic_FloatingMeterBank.hpp"

void MARS_Graphic_FloatingMeterBank::render(void* vicondata, void* miscdata){
	updateVicon(vicondata); //updates position
	updateMisc(miscdata);
	//rotate into the coordinate frame of the bank
	glTranslatef(position.x, position.y, position.z);
	glRotatef(psi, 0, 0, 1);
	//loop through the meters, 
	for (int i = 0; i < displacements.size(); i++){
		glPushMatrix();
		GLUquadric* quadric1, *quadric2 = gluNewQuadric();
		glTranslatef(displacements[i].x, displacements[i].y, displacements[i].z);
		glRotatef(-90, 1, 0, 0);
		//resource container cylinder first, with 1/2 the opacity of the resource cylinder
		glColor4f(this->color[0], this->color[1], this->color[2], this->color[3]/2.0);
		gluCylinder(quadric1, this->radius, this->radius, this->height, 15, 1);
		//resource cylinder itself
		//interpolate colours
		Glfloat r,g,b;
		if (resources[i] > resource_maxes[i]){
			resources[i] = resource_maxes[i];
		}
		if (resources[i] < 0){
			resources[i] = 0;
		}
		if (resources[i] / resource_maxes[i] <= 0.5){
			float frac = resources[i] / (resource_maxes[i]/2.0);
			r = rl*(1-frac) + rm*frac;
			g = gl*(1-frac) + gm*frac;
			b = bl*(1-frac) + bm*frac;
		} else {
			float frac = (resources[i] - resource_maxes[i]/2.0) / (resource_maxes[i]/2.0);
			r = rm*(1-frac) + rh*frac;
			g = gm*(1-frac) + gh*frac;
			b = bm*(1-frac) + bh*frac;
		}
		glColor4f(r,g,b, this->color[3]);
		gluCylinder(quadric2, this->radius, this->radius, this->height*resources[i]/resource_maxes[i], 15, 1);
		glPopMatrix();
	}
}
	
MARS_Graphic_FloatingMeterBank::MARS_Graphic_FloatingMeterBank(const char* viconname,
													GLubyte rc, GLubyte gc, GLubyte bc,
													 GLubyte rh, GLubyte gh, GLubyte bh, 
													 GLubyte rm, GLubyte gm, GLubyte bm,
													 GLubyte rl, GLubyte gl, GLubyte bl, GLubyte a, 
					   vector<float> resource_maxes,
					   vector< vector3<GLfloat> > displacements, GLfloat radius, GLfloat height); : MARS_Graphic(viconname, rc, gc, bc, a){
	this->rh = rh/255.0;
	this->rh = gh/255.0;
	this->rh = bh/255.0;
	this->rh = rm/255.0;
	this->rh = gm/255.0;
	this->rh = bm/255.0;
	this->rh = rl/255.0;
	this->rh = gl/255.0;
	this->rh = bl/255.0;
	this->resources = resource_maxes;
	this->resource_maxes = resource_maxes;	

	this->displacements = displacements;
	this->radius = radius;
	this->height = height;
}

void MARS_Graphic_FloatingMeterBank::setResourceValue(float resource){
	this->resource = resource;
}
		
float MARS_Graphic_FloatingMeterBank::getResourceValue(){
	return this->resource;
}
		
float MARS_Graphic_FloatingMeterBank::getResourceMax(){
	return this->resource_max;
}

void MARS_Graphic_FloatingMeterBank::updateMisc(void* miscdata){

		char *idx = buffer;

		// Extract data
		int   dest, src, cmdid;
		float cmd[8];

		while( sscanf(idx,"%d %d %d %f %f %f %f %f %f %f %f;",
				&src,&dest,&cmdid,&cmd[0],&cmd[1],&cmd[2],&cmd[3],
				&cmd[4],&cmd[5],&cmd[6],&cmd[7] ) == 11 ) {

			if (dest == MARS_id){
				for (int i = 0; i < 8; i++){
					resources[i] = cmd[i];
				}
			}

			// Update the packet received index
			idx = index(idx, ';') + 1;

		} // end while(sscanf)

}
