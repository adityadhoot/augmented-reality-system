#ifndef UTILITIES_H_
#define UTILITIES_H_

#include <sys/select.h>
#include <semaphore.h>
#include <signal.h>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "RAVEN_defines.hpp"
#include "RAVEN_structs.hpp"
#include "serialPort.hpp"


//## Functions
void wrap(double* ang);

void unwrap(double* ang);

double wrap(double ang);

double unwrap(double ang);

double sgn(double x);

double saturate(double val, double vmin, double vmax);

double zeroBand(double val, double vmin, double vmax);

double distance2(double x, double y, double z, Waypoint wpt);

int pmin(double* in, int sz);

double distance2(double x1, double y1, double x2, double y2);

void enforceBounds(double &x, double &y, double &z);

unsigned char hibyte(short x);

unsigned char lobyte(short x);

void sendToGermanQuad(Controls* ctl, SerialPort* sp);

void sendToGermanQuad_PPM(Controls* ctl, SerialPort* sp);

void sendToBuddyQuad(unsigned char packetId, char * data, unsigned char len, SerialPort* sp);

#endif
