#include "../libs/image/image.h"
#include "../libs/communication/message.h"
#include <stdio.h>
#include <time.h> 

#include <opencv/highgui.h>
#include <opencv/cv.h>

#define _DEBUG_

int port = 49150;

int main (int argc, char *argv[])
{

	MESSAGE msg = {0, 0, 0};

	if (argv[1] == 1 || argv[1] == NULL){
		Init_Webcam();
		Take_Picture_On_Webcam();
	}
	else if (argv[1] == 2)
	{
		Take_Picture_On_Webcam2();
	}
	else if (argv[1] == 3)
	{
		Load_Image("/home/pehladik/test2.jpg");
	}
	Convert_To_Jpeg();
	Convert_To_Msg(&msg);
	Print (&msg,50);

	return 0;
}


