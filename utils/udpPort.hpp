#ifndef UDPPORT_H_
#define UDPPORT_H_

#include <iostream>
#include <string>
#include <termios.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "RAVEN_structs.hpp"

class UDPPort{

  bool broadcast;
  bool multicast;

  int  port;

  char localIP[128];
  char multiIP[128];

  int  socketTx;
  int  socketRx;
  int  socketMc;

  struct sockaddr_in addressTx;
  struct sockaddr_in addressRx;
  struct sockaddr_in addressMc;

  char buffer[2048];

  ip_mreq mc_req;

public:

  int   udpInit(bool bcast, char* udpIP, int portnum);
  int   udpInit(char* udpIP, char* multicastIP, int portnum);
  int   udpSend(char* pkt);
  //int   udpSend(Message* msg);
  char* udpReceive();
  void  udpClose();
  int getSocketRX(){
  	return socketRx;
  }

};

#endif
