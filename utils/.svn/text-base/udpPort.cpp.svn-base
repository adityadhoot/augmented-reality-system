#include "udpPort.hpp"

int UDPPort::udpInit(bool bCast, char* addr, int inPort){
  
  broadcast = bCast;
  port      = inPort;

  // Receive port
  socketRx = socket(PF_INET, SOCK_DGRAM, 0);
  bzero(&addressRx, sizeof(addressRx));
  
  addressRx.sin_family      = AF_INET;
  addressRx.sin_port        = htons(port);
  addressRx.sin_addr.s_addr = INADDR_ANY;
  
  if( bind(socketRx, (struct sockaddr*)&addressRx, sizeof(addressRx)) != 0 ){ 
    printf("\tUDP Error :: Couldn't bind to Rx port %d\n",port);
  }
  
  //makes it a nonblocking socket
  //int x;
  //x=fcntl(socketRx,F_GETFL,0);
  //fcntl(socketRx,F_SETFL, x | O_NONBLOCK);
  
  //nvm let's keep it blocking - useful in the UDP datathread.

  printf("\tUDP listening on %s, port %d\n",addr,port);

  // Broadcast port
  if(broadcast){
    socketTx = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    bzero(&addressTx, sizeof(addressTx));

    if(socketTx < 0)
      printf("\tUDP Error :: Couldn't create socket\n");
    
    const int on = 1;
    if(setsockopt(socketTx, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on)) < 0)
      printf("\tUDP Error :: Couldn't set SO_BROADCAST\n");
    
    addressTx.sin_family = AF_INET;
    addressTx.sin_port   = htons(port);

    inet_aton(addr, &addressTx.sin_addr);
    printf("\tUDP broadcasting on %s, port %d\n",addr,port);
  }
  
  return(1);
}


int UDPPort::udpInit(char* theLocalIP, char* theMultiIP, int inPort){
  
  multicast = true;

  strcpy(localIP,theLocalIP);
  strcpy(multiIP,theMultiIP);

  port = inPort;

  // Setup multicast receive port
  socketMc = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if(socketMc < 0) {
    printf("\tUDP Error :: Couldn't create socket\n");
    return(-1);
  }

  bzero(&addressMc, sizeof(addressMc));  
  addressMc.sin_family      = AF_INET;
  addressMc.sin_port        = htons(port);
  addressMc.sin_addr.s_addr = INADDR_ANY;
  
  if( bind(socketMc, (struct sockaddr*)&addressMc, sizeof(addressMc)) != 0 ){ 
    printf("\tUDP Error :: Couldn't bind to port %d\n",port);
  }

  // Construct a multicast join request
  mc_req.imr_multiaddr.s_addr = inet_addr(multiIP);
  mc_req.imr_interface.s_addr = inet_addr(localIP);
  
  /* ADD MEMBERSHIP message via setsockopt */
  if( (setsockopt(socketMc, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*) &mc_req, sizeof(mc_req))) < 0) {
    printf("\tAdd to multicast failed\n");
  }	
  
  int reuse = 1;
  if( (setsockopt(socketMc, SOL_SOCKET,SO_REUSEADDR, (char*)&reuse,sizeof(int))) < 0){
    printf("\tCouldn't set reuse \n");
  }
  
  inet_aton(multiIP, &addressMc.sin_addr);

  printf("\tMulticast UDP :: localIP %s, multiIP %s, port %d\n",localIP,multiIP,port);
  
  return(1);
}


int UDPPort::udpSend(char* pkt){

  if( multicast )
    sendto(socketMc, pkt, strlen(pkt), 0, (struct sockaddr *)&addressMc, sizeof(addressMc));
  else
    sendto(socketTx, pkt, strlen(pkt), 0, (struct sockaddr *)&addressTx, sizeof(addressTx));

  return strlen(pkt);

}


/*int UDPPort::udpSend(Message* msg){
	
  char pkt[1028];
  bzero(pkt, sizeof(pkt));
  
  sprintf(pkt,"%d %d %d %f %f %f %f %f %f %f %f;",msg->src,msg->dest,msg->cmdID,
  	msg->data[0],msg->data[1],msg->data[2],msg->data[3],msg->data[4],msg->data[5],msg->data[6],msg->data[7]);
  
  return udpSend(pkt);

}*/


char* UDPPort::udpReceive(){

  bzero(buffer, sizeof(buffer));

  int addr_len;
  int bytes;

  if( multicast ){
    addr_len = sizeof(addressMc);    
    bytes = recvfrom(socketMc, buffer, sizeof(buffer), 0, (struct sockaddr*)&addressMc, (socklen_t*)&addr_len);
  }
  else{
    addr_len = sizeof(addressRx);    
    bytes = recvfrom(socketRx, buffer, sizeof(buffer), 0, (struct sockaddr*)&addressRx, (socklen_t*)&addr_len);
  }

  //printf("Buf = <%s>\n",buffer);
  return buffer;
}


void UDPPort::udpClose(){

  if( multicast ){

    close(socketMc);      

  }else{

    close(socketRx);
    
    if( broadcast )
      close(socketTx);    

  }

}
