#ifndef MARS_H_
#define MARS_H_
//#include "cvcam.h"


#include <stdio.h>
#include <string.h>

#include <fstream>
#include <string>
#include <sstream>
#include <math.h>
#include <algorithm>

//OpenGL libraries
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include <GL/glut.h>
#include <opencv/highgui.h>
#include <opencv/cv.h>
#include <opencv/cxcore.h>

//MARS header
using namespace std;

class MARS_Engine;


//main includes
#include "MARS_Math.hpp"
//#include "MARS_Actor.hpp"
//#include "MARS_ActorProperty.hpp"
#include "MARS_Camera.hpp"
#include "MARS_Graphic.hpp"
#include "MARS_Datasource.hpp"
#include "MARS_Engine.hpp"

//custom cameras
//#include "CustomCameras/MARS_Camera_ViconTracked.hpp"

//custom actors
//#include "CustomActors/MARS_ActorProperty_PathPlanner.hpp"
//#include "CustomActors/MARS_ActorProperty_ResourceTracker.hpp"
//#include "CustomActors/MARS_Actor_ViconTracked.hpp"


//custom graphics
#include "CustomGraphics/MARS_Graphic_FlatMarker_FillCircle.hpp"
#include "CustomGraphics/MARS_Graphic_FlatMarker_OpenRect.hpp"
#include "CustomGraphics/MARS_Graphic_Path.hpp"
#include "CustomGraphics/MARS_Graphic_FloatingString.hpp"
#include "CustomGraphics/MARS_Graphic_FloatingMeter.hpp"
#include "CustomGraphics/MARS_Graphic_ImageOnQuad.hpp"

//custom datasources
//#include "CustomDatasources/MARS_Datasource_UDP.hpp"
#endif

