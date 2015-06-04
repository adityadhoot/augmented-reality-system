#include "viconDataList.hpp"

ViconDataList::ViconDataList()
{
        pthread_rwlock_init(&rwLock,NULL);
        data = ViconData();
}

void ViconDataList::setBuffer(int viconTime,char* name,int ID,double x,double y,double z,
                              double phi,double theta,double psi,
                              double dx,double dy,double dz,
                              double p,double q,double r,
                              double qx,double qy,double qz,double qw)
{
        pthread_rwlock_wrlock(&rwLock);

        dataRX.viconTime = viconTime;
        strcpy(dataRX.name,name);
        dataRX.ID = ID;
        dataRX.x = x;
        dataRX.y = y;
        dataRX.z = z;
        dataRX.roll = phi;
        dataRX.pitch = theta;
        dataRX.yaw = psi;
        dataRX.dx = dx;
        dataRX.dy = dy;
        dataRX.dz = dz;
        dataRX.p = p;
        dataRX.q = q;
        dataRX.r = r;
        dataRX.qx = qx;
        dataRX.qy = qy;
        dataRX.qz = qz;
        dataRX.qw = qw;

        pthread_rwlock_unlock(&rwLock); 
}

void ViconDataList::setBuffer(ViconData vd)
{
        pthread_rwlock_wrlock(&rwLock);

        dataRX.viconTime = vd.viconTime;
        strcpy(dataRX.name,vd.name);
        dataRX.ID = vd.ID;
        dataRX.x = vd.x;
        dataRX.y = vd.y;
        dataRX.z = vd.z;
        dataRX.roll = vd.roll;
        dataRX.pitch = vd.pitch;
        dataRX.yaw = vd.yaw;
        dataRX.dx = vd.dx;
        dataRX.dy = vd.dy;
        dataRX.dz = vd.dz;
        dataRX.p = vd.p;
        dataRX.q = vd.q;
        dataRX.r = vd.r;
        dataRX.qx = vd.qx;
        dataRX.qy = vd.qy;
        dataRX.qz = vd.qz;
        dataRX.qw = vd.qw;

        pthread_rwlock_unlock(&rwLock);
}

void ViconDataList::updateData()
{
        pthread_rwlock_rdlock(&rwLock);

        memcpy(&data, &dataRX, sizeof(ViconData));

        pthread_rwlock_unlock(&rwLock);
}

ViconData ViconDataList::getData()
{
        ViconData vd;

        pthread_rwlock_rdlock(&rwLock);
 
        vd = data;
 
        pthread_rwlock_unlock(&rwLock);

        return vd;
}

