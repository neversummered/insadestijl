/*
 * image.c
 *
 *  Created on: 1 nov. 2010
 *      Author: pehladik
 */

#include "image.h"

CvCapture *mCapture;		/* capture */
IplImage *frame;			/* current frame  */
CvMat *buf;					/* buffer for jpeg compression*/

void Init_Webcam()
{
	mCapture = cvCreateCameraCapture( -1 );
	if( !mCapture )
		printf( "Impossible d'ouvrir la camera" );
}

void Take_Picture_On_Webcam() {
	// On essaye de capturer une frame
	cvReleaseImage(&frame);
	frame = cvCloneImage(cvQueryFrame(mCapture));

#ifdef _DEBUG_
	printf("Image prise\n");
#endif
}

double Take_Picture_On_Webcam2() {
	clock_t start, finish;
	double duration;

	// Delay for a specified time.
	start = clock();

	mCapture = cvCreateCameraCapture( -1 );
	if( !mCapture )
		printf( "Impossible d'ouvrir la camera" );

	frame = cvCloneImage(cvQueryFrame(mCapture));
	cvReleaseCapture(&mCapture);

	finish = clock();
	duration = 1000*(double)(finish - start) / CLOCKS_PER_SEC;

#ifdef _DEBUG_
	printf( "%2.1f seconds\n", duration );
	printf("Image prise\n");
#endif

	return duration;
}

void Load_Image(const char * filename) {
	frame = cvLoadImage(filename,0);

#ifdef _DEBUG
	printf("Image %s chargee\n", filename);
#endif
}

void Convert_To_Jpeg() {
	int jpeg_params[] = { CV_IMWRITE_JPEG_QUALITY, 80, 0 };
	buf= cvEncodeImage(".jpeg", frame, jpeg_params);

#ifdef _DEBUG_
	printf("Conversion reussie\n");
	printf("taille de l'image %d\n", (buf->cols)*sizeof(uchar));
#endif
}

void Convert_To_Msg(MESSAGE *pMsg) {
	Set_Message (pMsg, 'V', (buf->cols)*sizeof(uchar), buf->data.ptr);
}

IplImage * get_Frame()
{
	return frame;
}

CvMat * get_Jpeg_Buffer()
{
	return buf;
}
