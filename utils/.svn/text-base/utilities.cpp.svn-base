#include "utilities.hpp"


//## Enforces +pi <--> -pi
void wrap(double* ang)
{
    while( *ang > PI )
        *ang -= 2*PI;
    while( *ang <= -PI )
        *ang += 2*PI;
}

//## Enforces 0 <--> 2*pi
void unwrap(double* ang)
{
    while( *ang >= 2*PI )
        *ang -= 2*PI;
    while( *ang < 0 )
        *ang += 2*PI;
}

//## Enforces +pi <--> -pi
double wrap(double ang)
{
    while( ang > PI )
        ang -= 2*PI;
    while( ang <= -PI )
        ang += 2*PI;

    return ang;
}

//## Enforces 0 <--> 2*pi
double unwrap(double ang)
{
    while( ang >= 2*PI )
        ang -= 2*PI;
    while( ang < 0 )
        ang += 2*PI;

    return ang;
}

//## returns sign of input
double sgn(double val)
{
    if( val > 0 )
        return(1.0);
    else if( val < 0 )
        return(-1.0);
    else
        return(0.0);
}


//## saturates val at vmin and vmax
double saturate(double val, double vmin, double vmax)
{
    if( val > vmax )
        val = vmax;
    else if( val < vmin )
        val = vmin;

    return val;
}

//## zeros out any signal between vmin and vmax
double zeroBand(double val, double vmin, double vmax)
{
    if( val > vmin && val < vmax )
        val = 0;

    return val;
}

double distance2(double x, double y, double z, Waypoint wpt)
{

    return( (wpt.x-x)*(wpt.x-x) + (wpt.y-y)*(wpt.y-y) + (wpt.z-z)*(wpt.z-z) );

}

double distance2(double x1, double y1, double x2, double y2)
{
    //pt1 = [x,y]
    return sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
}

//Return the index of the row in the 1st column with the minimum value
int pmin(double* in, int sz)
{
    int index = 0;
    double min_value = in[0];
    for( int i=0; i<sz; i++ )
    {
        if(in[i]<min_value)
        {
            index = i;
            min_value = in[i];
        }
    }
    return index;
}


//## enforce RAVEN rooom bounds
void enforceBounds(double &x, double &y, double &z)
{

    if( x > X_MAX)
        x = X_MAX;
    else if( x < X_MIN)
        x = X_MIN;

    if( y > Y_MAX)
        y = Y_MAX;
    else if( y < Y_MIN)
        y = Y_MIN;

    if( z > Z_MAX)
        z = Z_MAX;
    else if( z < Z_MIN)
        z = Z_MIN;

}


//## Return the high byte of a short
unsigned char hibyte(short x)
{
    return (unsigned char)(x >> 8);
}


//## Return the lo byte of a short
unsigned char lobyte(short x)
{
    return (unsigned char)(x & 0xFF);
}


void sendToGermanQuad(Controls* ctl, SerialPort* sp)
{
    //Struct that must be populated and sent
    COMMANDDATA cmd;
    cmd.ident='C';

    //Note the arbitrary order of the channel array (thanks German dudes)
    //Put commands in 0-255 format:
    cmd.channels[0] = (unsigned char)(127+(int)(saturate(ctl->pitch,-1.0,1.0)*127)); //pitch
    cmd.channels[1] = (unsigned char)(127+(int)(saturate(-1.0*ctl->roll,-1.0,1.0)*127)); //roll
    cmd.channels[2] = (unsigned char)(127+(int)(saturate(ctl->throttle,-1.0,1.0)*127)); //thrust
    cmd.channels[3] = (unsigned char)(127+(int)(saturate(ctl->yaw,-1.0,1.0)*127)); // yaw

    //Control all channels
    cmd.channel_select = 0xFF;

    //Add up the check sum, (stuff) % 0x100
    cmd.chkSum = (cmd.channels[0]+cmd.channels[1]+cmd.channels[2]+cmd.channels[3]+cmd.channel_select) % 0x100;

    //Construct the packet
    unsigned char sendBuf[200];
    sendBuf[0]='U';
    sendBuf[1]='F';
    sendBuf[2]='O';
    memcpy(sendBuf+3,&cmd,sizeof(cmd));
    sp->spSend(sendBuf,3+sizeof(cmd));
}


void sendToGermanQuad_PPM(Controls* ctl, SerialPort* sp)
{
    // ppm generator expects 10 bytes:
    // [0xFF chksum roll_hi roll_lo pitch_hi pitch_lo ...
    //  throttle_hi throttle_lo yaw_hi yaw_lo]
    //
    // to generate the bytes, cmd signal (ie roll, pitch, ...)  should be
    // between 1000-2000 (representing microsecond pulse width in ppm train),
    // then we need to format this to be used directly by timer0 on the
    // xbee_to_ppm chip

    // shorts are 2 bytes - we'll use 'em
    short roll  = ctl->roll*500     + 1500; // (-1)<->(1)  |-->  (1000)<->(2000)
    short pitch = ctl->pitch*500    + 1500; // (-1)<->(1)  |-->  (1000)<->(2000)
    short coll  = ctl->throttle*500 + 1500; // (-1)<->(1)  |-->  (1000)<->(2000)
    short yaw   = ctl->yaw*500      + 1500; // (-1)<->(1)  |-->  (1000)<->(2000)

    // this is the part were we manipulate to fit the timer's needs
    roll  = 0xFFFF - roll  + 500;
    pitch = 0xFFFF - pitch + 500;
    coll  = 0xFFFF - coll  + 500;
    yaw   = 0xFFFF - yaw   + 500;

    // calc the checksum
    unsigned char chksum = 0;
    chksum += hibyte(roll);
    chksum += lobyte(roll);
    chksum += hibyte(pitch);
    chksum += lobyte(pitch);
    chksum += hibyte(coll);
    chksum += lobyte(coll);
    chksum += hibyte(yaw);
    chksum += lobyte(yaw);

    // Allocate the data packet
    unsigned char buf[10];

    buf[0] = 0xFF;
    buf[1] = chksum;
    buf[2] = hibyte(roll);
    buf[3] = lobyte(roll);
    buf[4] = hibyte(pitch);
    buf[5] = lobyte(pitch);
    buf[6] = hibyte(coll);
    buf[7] = lobyte(coll);
    buf[8] = hibyte(yaw);
    buf[9] = lobyte(yaw);

    // The data packet is now ready; send it!
    sp->spSend(buf,10);
}


void sendToBuddyQuad(unsigned char packetId, char * data, unsigned char len, SerialPort* sp)
{
 	// Length can't be 0xFF
	if(len==0xFF){
		printf("nanoPilot packet too long, can't send!\n");
		return;
	}


	// Start Checksum
	unsigned char chksum = 0;
	unsigned char sendBuff[256];
	int buffPtr = 0;

	// STX
	sendBuff[buffPtr] = 0xFF; // Start of packet
	buffPtr++;

	// Packet Id
	chksum += packetId;
	sendBuff[buffPtr] = packetId;
	buffPtr++;

	// Packet Data Length
	chksum += len;
	sendBuff[buffPtr] = len;
	buffPtr++;

	// Stuff Data
	for (int i = 0; i < len; i++) {
		// Send Byte
		if (data[i] == 0xFF) {
			data[i] = 0xFE;
		}
		chksum += data[i];
		sendBuff[buffPtr] = data[i];
		buffPtr++;
	}

	// Checksum Calculate and Send
	unsigned char chksumSend;
	chksumSend = chksum ^ 0xFF;
	chksumSend += 1;
	if ((chksumSend == 0xFF)) {
		if (len > 1) {
			if (data[1] == 0xFE) {
				data[1]--;
				chksum--;
			} else {
				data[1]++;
				chksum++;
			}
		} else {
			if (data[0] == 0xFE) {
				data[0]--;
				chksum--;
			} else {
				data[0]++;
				chksum++;
			}
		}
		chksumSend = chksum ^ 0xFF;
		chksumSend += 1;
	}
	sendBuff[buffPtr] = chksumSend;
	buffPtr++;

	// Send packet
	sp->spSend(sendBuff, buffPtr);
	//printf("sent: %x\n", sendBuff);
 
 
 /*
    // buddyquad expects the following:
    // [0xFF len chksum roll_hi roll_lo pitch_hi pitch_lo yaw_hi yaw_lo throttle_hi throttle_lo enabled]
    //

    // uint16 is 2 bytes
    uint16_t roll  = ctl->roll*5000     + 15000; // (-1)<->(1)  |-->  (10000)<->(20000)
    uint16_t pitch = ctl->pitch*5000    + 15000; // (-1)<->(1)  |-->  (10000)<->(20000)
    uint16_t yaw   = ctl->yaw*5000      + 15000; // (-1)<->(1)  |-->  (10000)<->(20000)
    uint16_t coll  = ctl->throttle*5000 + 15000; // (-1)<->(1)  |-->  (10000)<->(20000)
    
    if( enabled )
    	coll = (uint16_t)std::max((int)10001,(int)coll);
    else
        coll = 9999;
        

    // data length
    unsigned char len = 8;

    // calc the checksum
    unsigned char chksum = 0;
    chksum += len;
    chksum += hibyte(roll);
    chksum += lobyte(roll);
    chksum += hibyte(pitch);
    chksum += lobyte(pitch);
    chksum += hibyte(yaw);
    chksum += lobyte(yaw);
    chksum += hibyte(coll);
    chksum += lobyte(coll);

    // allocate the data packet
    unsigned char buf[11];
    buf[0]  = 0xFF;
    buf[1]  = len;
    buf[2]  = chksum;
    buf[3]  = hibyte(roll);
    buf[4]  = lobyte(roll);
    buf[5]  = hibyte(pitch);
    buf[6]  = lobyte(pitch);
    buf[7]  = hibyte(yaw);
    buf[8]  = lobyte(yaw);
    buf[9]  = hibyte(coll);
    buf[10] = lobyte(coll);

    // The data packet is now ready; send it!
    sp->spSend(buf,11);
*/
}





