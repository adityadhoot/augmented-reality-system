#ifndef RAVEN_STRUCTS_H_
#define RAVEN_STRUCTS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <vector>
#include <semaphore.h>
#include <math.h>

// VICON DATA ############
struct ViconData
{
    int    viconTime;
    char   name[128];//std::string name;
    int    ID;
    double x;        // x position (m)
    double y;
    double z;
    double roll;     // roll angle (rad)
    double pitch;
    double yaw;
    double dx;       // x velocity (m/s)
    double dy;
    double dz;
    double p;        // roll angle rate (rad/s)
    double q;
    double r;
    double qx;       // quaternion attitude
    double qy;
    double qz;
    double qw;

    // Constructors:
    ViconData()
    {
        viconTime = 0;
        strcpy(name,"n/a");
        ID        = 0;
        x         = 0.0;
        y         = 0.0;
        z         = 0.0;
        roll      = 0.0;
        pitch     = 0.0;
        yaw       = 0.0;
        dx        = 0.0;
        dy        = 0.0;
        dz        = 0.0;
        p         = 0.0;
        q         = 0.0;
        r         = 0.0;
        qx        = 0.0;
        qy        = 0.0;
        qz        = 0.0;
        qw        = 0.0;
    }
};


#endif



