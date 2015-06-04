#include "MARS_Camera.hpp"

MARS_Camera::MARS_Camera(char *name, int ncam, float focallength, CalcViewMethod calcviewmethod){
	strncpy(this->name, name, 128);
	this->calcviewmethod = CALCVIEWMETHOD_RT;
	this->ncam = ncam;
	vector3<GLfloat> p(0,0,0); eye = p;
	p.set(0, 0, -1); look = p;
	p.set(0, 1, 0); up = p;
	this->capture = cvCaptureFromCAM(ncam);
	if (!capture){
		fprintf(stderr, "ERROR: Capture %d is NULL\n", ncam);
	}
	this->focallength = focallength;
}

MARS_Camera::~MARS_Camera(){
	cvReleaseCapture(&(this->capture));
}

char* MARS_Camera::getName(){
	return this->name;
}

void MARS_Camera::update(void* data){
        // Parse the buffer and find the name 
        char* idx = (char*)data;
        float timestamp;
        sscanf(idx, "%f;", &timestamp);
        if (!index(idx, ';')) //sometimes happens while the datamanager is starting up
        return;
        idx = index(idx, ';') + 1;
        char   name_data[128];
        int    id;
        float  x, y, z, phi, theta, psi, dx, dy, dz;
        float  dphi, dtheta, dpsi, qx, qy, qz, qw;
        while( sscanf(idx,"%s %d,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f;",
                                name_data,&id,&x,&y,&z,&phi,&theta,&psi,&dx,&dy,&dz,&dphi,&dtheta,&dpsi,
                                &qy,&qx,&qz,&qw) == 18 )
       {
                   // if name matches camera update the camera
                                if ( strcmp(name_data,this->name) == 0 )
                                {
                                //printf("Camera: X: %f Y: %f Z: %f\n", x, y, z); fflush(stdout);
                                        float psi_corr = -psi; //correction for weird left-handed psi
                                        float s3 = sin(psi_corr);       float c3 = cos(psi_corr);
                                        float s2 = sin(phi);            float c2 = cos(phi);
                                        float s1 = sin(theta);          float c1 = cos(theta);
                                        //fill the rotation matrix in column major order
                                        //NB: this is a 3-1-2 rotation (i.e. V2 = C2*C1*C3*V1)
                                        this->rotmat[0] = c2*c3-s1*s2*s3; this->rotmat[3] = c3*s1*s2+c2*s3; this->rotmat[6] = -c1*s2;
                                        this->rotmat[1] = -c1*s3;         this->rotmat[4] = c1*c3;          this->rotmat[7] = s1;
                                        this->rotmat[2] = c3*s2+c2*s1*s3; this->rotmat[5] = -c2*c3*s1+s2*s3;this->rotmat[8] = c1*c2;                
                                        this->pos.x = x; this->pos.y = y; this->pos.z = z;
                                }
                                if (!index(idx, ';')){ //if the buffer has no datapackets --THIS SHOULD NEVER BE TRUE -- sscanf above should find the ';'
                                        printf("MARS_Camera Warning: idx was null in while loop.");
                                        return;
                                }
                                // Update the packet received index
                                idx = index(idx, ';') + 1;
                        }
}

void MARS_Camera::update_test(void* data){
	// Parse the buffer and find the name 
	char* idx = (char*)data;
	float timestamp;
	char test_data [] = "0.00;CAM00 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0;";
	sscanf(test_data, "%f;", &timestamp);
	char test_data_1[] = "CAM00 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0;";
	char   name_data[] = "CAM01";
	int    id = 0;
	float  x, y, z, phi, theta, psi, dx, dy, dz;
	float  dphi, dtheta, dpsi, qx, qy, qz, qw;
	x = 1; y = 1; z = 1; phi = 0; theta = 0; psi = 0; dx = 0; dy = 0; dz = 0; dphi = 0; dtheta = 0; dpsi = 0; qx = 0; qy = 0; qw = 0; 
	// if name matches camera update the camera
	if ( strcmp(name_data,this->name) == 0 )
	{
		printf("Camera: X: %f Y: %f Z: %f\n", x, y, z); fflush(stdout);
		float psi_corr = -psi; //correction for weird left-handed psi
		float s3 = sin(psi_corr); 	float c3 = cos(psi_corr);
		float s2 = sin(phi);	  	float c2 = cos(phi);
		float s1 = sin(theta);		float c1 = cos(theta);
		//fill the rotation matrix in column major order
		//NB: this is a 3-1-2 rotation (i.e. V2 = C2*C1*C3*V1)
		this->rotmat[0] = c2*c3-s1*s2*s3; 	this->rotmat[3] = c3*s1*s2+c2*s3;	this->rotmat[6] = -c1*s2;
		this->rotmat[1] = -c1*s3; 		this->rotmat[4] = c1*c3;		this->rotmat[7] = s1;
		this->rotmat[2] = c3*s2+c2*s1*s3; 	this->rotmat[5] = -c2*c3*s1+s2*s3;     this->rotmat[8] = c1*c2;		
		this->pos.x = x; this->pos.y = y; this->pos.z = z;
	}
	// Update the packet received index
	//idx = index(test_data, ';') + 1;
	
}

CvCapture* MARS_Camera::getCapture(){
	if (!this->capture)
		fprintf(stderr, "ERROR: Using capture before it was initialized properly!\n");
	return this->capture;
}

float MARS_Camera::getFocalLength(){
	return this->focallength;
}

void MARS_Camera::setFocalLength(float focallength){
	this->focallength = focallength;
}

vector3<GLfloat> MARS_Camera::getEye(){
	return this->eye;
}

vector3<GLfloat> MARS_Camera::getLook(){
	return this->look;
}

vector3<GLfloat> MARS_Camera::getUp(){
	return this->up;
}

void MARS_Camera::calcViewMat_ELU(){ //calculate viewmat with eye, look and up
	//eye is where camera is
	//look is where it's looking
	//up is up vector in cam frame (y)
	
	vector3<GLfloat> zaxis = (eye - look)/ (eye-look).norm();
	vector3<GLfloat> yaxis = this->up / this->up.norm();
	vector3<GLfloat> xaxis = yaxis.cross(zaxis);
	xaxis = xaxis/xaxis.norm();
	
	viewmat[0] = xaxis.x; viewmat[1] = yaxis.x; viewmat[2] = zaxis.x; viewmat[3] = 0; //col 1
	viewmat[4] = xaxis.y; viewmat[5] = yaxis.y; viewmat[6] = zaxis.y; viewmat[7] = 0;//col 2
	viewmat[8] = xaxis.z; viewmat[9] = yaxis.z; viewmat[10] = zaxis.z; viewmat[11] = 0; //col 3
	//col4 below this - just the rotation matrix above times the negative displacement of the camera
	viewmat[12] = -(xaxis.x*eye.x + xaxis.y*eye.y + xaxis.z*eye.z);
	viewmat[13] = -(yaxis.x*eye.x + yaxis.y*eye.y + yaxis.z*eye.z);		
	viewmat[14] = -(zaxis.x*eye.x + zaxis.y*eye.y + zaxis.z*eye.z);
	viewmat[15] = 1;				   
}

void MARS_Camera::calcViewMat_RT(){ //calculate the viewmat with rotation matrix and pos translation
	viewmat[0] = rotmat[0]; viewmat[1] = rotmat[1]; viewmat[2] = rotmat[2]; viewmat[3] = 0; //col 1
	viewmat[4] = rotmat[3]; viewmat[5] = rotmat[4]; viewmat[6] = rotmat[5]; viewmat[7] = 0;//col 2
	viewmat[8] = rotmat[6]; viewmat[9] = rotmat[7]; viewmat[10] = rotmat[8]; viewmat[11] = 0; //col 3
	
	//similar to above, rotation matrix times -ve displacement
	viewmat[12] = -(viewmat[0]*pos.x + viewmat[4]*pos.y + viewmat[8]*pos.z);
	viewmat[13] = -(viewmat[1]*pos.x + viewmat[5]*pos.y + viewmat[9]*pos.z);
	viewmat[14] = -(viewmat[2]*pos.x + viewmat[6]*pos.y + viewmat[10]*pos.z);
	viewmat[15] = 1;
}

void MARS_Camera::calcViewMat(){
	switch (calcviewmethod){
		case CALCVIEWMETHOD_ELU:
			calcViewMat_ELU();
			break;
		case CALCVIEWMETHOD_RT:
			calcViewMat_RT();
			break;
	}
}

GLfloat* MARS_Camera::getViewMat(){
	return this->viewmat;
}
