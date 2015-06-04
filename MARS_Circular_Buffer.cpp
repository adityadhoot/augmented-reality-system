#include "MARS_Circular_Buffer.hpp"

MARS_Circular_Buffer::MARS_Circular_Buffer(int length, int packetlength){
	length--;
	Circular_BufferNode *cb = new Circular_BufferNode(packetlength);
	begin = cb;
	end = cb;
	Circular_BufferNode* cur = begin;
	while(length > 0){
		length--;
		cur->next = new Circular_BufferNode(packetlength);
		cur = cur->next;
	}
	cur->next = begin;
}

MARS_Circular_Buffer::~MARS_Circular_Buffer(){
	Circular_BufferNode *next, *todelete = begin->next;
	while(todelete != begin){
		next = todelete->next;
		delete todelete;
		todelete = next;
	}
	delete begin;
}

void MARS_Circular_Buffer::queue (char* packet){
	if (strlen(packet) > end->size){
		printf ("WARNING! MARS_Circular_Buffer::queue: Packet size too large for circular buffer.\n");
	}		
	bzero(end->data, end->size);
	strncpy(end->data, packet, end->size);
	if (end->next == begin){
		begin = begin->next;
		printf ("WARNING! MARS_Circular_Buffer::queue: End of Circular Buffer has been reached in datasource\n");
	}
	end= end->next;
}

void MARS_Circular_Buffer::dequeue (char* toFill, int len){
	if (strlen(begin->data) > len){
		printf ("WARNING! MARS_Circular_Buffer::dequeue: toFill smaller than size of packet from circular buffer\n");
	}
	if (begin != end){
		bzero(toFill, len);
		strncpy(toFill, begin->data, begin->size);
		begin = begin->next; 
	} else {
		bzero(toFill, len);
	}
}


