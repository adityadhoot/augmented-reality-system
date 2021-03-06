#include "MARS.hpp"
using namespace std;

/*
#include "MARS.hpp"
#include "MARS_Camera.cpp"
#include "MARS_Datasource.cpp"
#include "MARS_Engine.cpp"
#include "MARS_Graphic.cpp"
#include "CustomGraphics/MARS_Graphic_FlatMarker_FillCircle.cpp"
#include "CustomGraphics/MARS_Graphic_FlatMarker_OpenRect.cpp"
#include "CustomGraphics/MARS_Graphic_FloatingMeter.cpp"
#include "CustomGraphics/MARS_Graphic_FloatingString.cpp"
#include "CustomGraphics/MARS_Graphic_ImageOnQuad.cpp"
//#include "CustomGraphics/MARS_Graphic_Path.cpp"
*/
//class MARS_Camera_Dummy : public MARS_Camera{
//	public:
//		MARS_Camera_Dummy() : MARS_Camera("CAM00",0,  5.94406677e+02, CALCVIEWMETHOD_ELU){}
//		void update( void * data){}
//};

//class MARS_Actor_Dummy : public MARS_Graphic{
//	public:
//		MARS_Actor_Dummy(const char* name) : MARS_Graphic(name) {}
//		void update(void * data) { vector3<GLfloat> pose(0, 0, -3); this->pos = pose;}
//		void render(){}
//};


/*
			getline 
			cout <<line;
			found = line.find("camera name");
			if (found!=string::npos){
							
			}
				
				printf ("ADSFAFDASFD");
*/

int main(int argc, char** argv){
	string cam_name;
	float focal_x, focal_y;
	string MARS_filename = "config1.mars";
	ifstream mars_file ("config1.mars");
	/*FILE *mars_file = fopen ("config1.mars", "r+");
	if (mars_file == NULL)
		printf("Error opening MARS Configuration file\n");
	else {
		//while(feof(mars_file)==0){
			fscanf(mars_file, "%s", cam_name);
			printf ("%s", cam_name);
			//fscanf(mars_file, "%f", focal_x);
			//fscanf(mars_file, "%f", focal_y);
			//printf ("%s\n", cam_name[0]); 
		//}
	}*/

	string line;

	//cout << "Enter MARS configuration filename(enter .mars extension): ";
	//cin >> MARS_filename;

/*	if (mars_file.is_open())
	{
		while (mars_file.good())
		{
			getline (mars_file, cam_name);
			getline (mars_file, line);
			focal_x = atof(line.c_str());
			getline (mars_file, line);
			focal_y = atof(line.c_str());
			printf ("%s %f %f\n", cam_name.c_str(), focal_x, focal_y);
			while (mars_file.good()){
				string line1; size_t found;			
				getline (mars_file, line1);
				found = line1.find("newgraphic->")
				if (!found == string::npos){
					size_t = found1;
					while (!found 
					getline (mars_file, line 
					getline (mars_file, line1);
				}
			}
	}

*/

	//create the camera
	//MARS_Camera_Dummy* dum = new MARS_Camera_Dummy();
	MARS_Camera* camera = new MARS_Camera("CAM01", 1, 7.03209778e+02, MARS_Camera::CALCVIEWMETHOD_RT);

	//create & initialize mars engine
	MARS_Engine* mars = new MARS_Engine(camera, argc, argv);
	
	//add actors
	//MARS_Actor_ViconTracked* upucc = new MARS_Actor_ViconTracked(argv[4]);
	//mars->addActor(upucc);

	printf("%d %s %s %s %s %s\n", argc, argv[0], argv[1], argv[2], argv[3], argv[4]);

	//add graphics
	vector3<GLfloat> c1(0.5f, 0.0f, -5.0f), c2(0.5f, 1.0f, -5.0f), c3(-1.5f, 1.0f, -5.0f);
	vector3<GLfloat> centre(0,0,0);
	vector3<GLfloat> normal(0,0,1);
	vector3<GLfloat> disp(0, 0, 0.1);
	vector3<GLfloat> displacement(0, 0, .1);

	//MARS_Graphic_FlatMarker_FillCircle* circle = new MARS_Graphic_FlatMarker_FillCircle("Circle1",255, 22, 2, 255,centre, normal, 30, 1.0f);
	//MARS_Graphic_FloatingMeter* floating_meter = new MARS_Graphic_FloatingMeter("meter1",0, 0, 255, 255, 1,1, displacement, 0.1f, 0.1f);
	

	MARS_Graphic_FloatingMeter* floating_meter = new MARS_Graphic_FloatingMeter("BQ06", 180, 180, 180, 0, 255, 0, 255, 255, 0, 255, 0, 0, 255, 0, 10, displacement, .03, .10);


//MARS_Graphic_FlatMarker_OpenRect* rect = new MARS_Graphic_FlatMarker_OpenRect("meter2",0, 255, 255, 128, c1, c2, c3, 0.5f);
	//MARS_Graphic_FloatingString* floating_name = new MARS_Graphic_FloatingString( "Actor1", 10, 100, 255, 255, camera, disp, "Actor1", 4.0f, 0.2f);
	//MARS_Graphic_FloatingString* floater = new MARS_Graphic_FloatingString("quad2",0, 255, 0, 255, dumdum, dum, disp1, "string", 4.0f, 0.2f);

	IplImage* rawimg, *rgbimg, *flipimg; 
  	rawimg=cvLoadImage("img.png");
	rgbimg = cvCreateImage(cvSize(rawimg->width, rawimg->height), IPL_DEPTH_8U, 3);
	flipimg = cvCreateImage(cvSize(rawimg->width, rawimg->height), IPL_DEPTH_8U, 3);
  	cvCvtColor(rawimg, rgbimg, CV_BGR2RGB);
	cvFlip(rgbimg, flipimg, 0); //0 here means flip only vert
  	c1.set(-1.5f, 0.0f, -3.0f);
  	c2.set(0.5f, 0.0f, -3.0f);
  	c3.set(0.5f, 1.0f, -3.0f);
	 
	//MARS_Graphic_ImageOnQuad* quadimg = new MARS_Graphic_ImageOnQuad("quad1",227, 70, 203, 255, mars->getWindowWidth(), mars->getWindowHeight(), c1, c2, c3, flipimg->imageData, flipimg->width, flipimg->height);

	
	//mars->addGraphic(floating_name);	
	printf("Floating Meter ID: %d\n", mars->addGraphic(floating_meter));
	//mars->addGraphic(circle);
	//mars->addGraphic(quadimg);
	//mars->addGraphic(rect);

	//render!
	mars->renderLoop();
	delete mars;
}
