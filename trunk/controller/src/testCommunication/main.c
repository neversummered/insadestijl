#include "../libs/communication/communication.h"
#include "../libs/communication/message.h"
#include <stdio.h>
#include <time.h> 

#include <opencv/highgui.h>
#include <opencv/cv.h>

#define _DEBUG_

int port = 49150;

int main (int argc, char *argv[])
{
	int n;

	MESSAGE msg = {0, 0, 0};
	int connectSocket;
	connectSocket = Open_Server(port);

	Send_Message(connectSocket, msg.pPacket, msg.len);
	sleep(5);

	Put_Int(&msg, 675755);
	Print (&msg,50);
	Send_Message(connectSocket, msg.pPacket, msg.len);

	Put_Char(&msg, 'c');
	Print (&msg,50);
	Send_Message(connectSocket, msg.pPacket, msg.len);

	Put_String(&msg, "ceci n'est pas une chaîne de caractère");
	Print (&msg,50);
	Send_Message(connectSocket, msg.pPacket, msg.len);

	printf("Taper sur un touche pour quitter");
  	scanf("%d",&n);

	Close_Server(connectSocket);

	return 0;
}


