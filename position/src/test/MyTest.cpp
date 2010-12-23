#include <stdio.h>
#include <signal.h>
#include <unistd.h>

#include <highgui.h>
#include <cv.h>

#include "./../findPosition.h"
#include "./../show.h"
#include "./../stopwatch.h"

CvCapture* src;
IplImage *img;


int main(int argc, char* argv[])
{
    cvNamedWindow( "video", CV_WINDOW_AUTOSIZE );
    char key = 'a';
    while( key != 'q' )
    {
	    startStopwatch();
	    src = cvCreateCameraCapture( 0 );
		img = cvQueryFrame( src );
		computeRobotPosition(img);
		stopStopwatch();
		printf("time to take a picture and compute position:%d\n",  getTimeInMillisecond());
		startStopwatch();
		drawPositonAndOrientation(img);
	    cvShowImage( "video", img );
	    stopStopwatch();
		printf("time to show a picture:%d\n",  getTimeInMillisecond());
		cvReleaseCapture(&src);
        
        key = cvWaitKey( 100 );
    }
    return 0;
}
