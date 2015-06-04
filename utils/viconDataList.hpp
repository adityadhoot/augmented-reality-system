#ifndef VICONDATALIST_HPP_
#define VICONDATALIST_HPP_

#include <semaphore.h>
#include <iostream>
#include <string>
#include "RAVEN_structs.hpp"

class ViconDataList
{
        pthread_rwlock_t rwLock;

        ViconData data;    // for other threads to read from
        ViconData dataRX;  // for receiving from vicon stream

public:

        ViconDataList();

        void setBuffer(ViconData vd);
        void setBuffer(int viconTime,char* name,int ID, double x,double y,double z,
                       double phi,double theta,double psi,
                       double dx,double dy,double dz,
                       double p,double q,double r,
                       double qx,double qy,double qz,double qw);
        void updateData();
        ViconData getData();

};

#endif /*VICONDATALIST_HPP_*/
