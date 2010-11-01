#include "methods.h"
#include <QDebug>

Methods::Methods()
{
}

void Methods::getObjectColor(int x, int y, IplImage *img, int *h, int *s, int *v){
    CvScalar pixel;
    IplImage *hsv;

    // Get the hsv image
    hsv = cvCloneImage(img);
    cvCvtColor(img, hsv, CV_BGR2HSV);

    int i, j;
    // Get the selected pixel
    *h = 0;
    *s = 0;
    *v = 0;
    // Mean on a square of 10px*10px
    for (i = 0; i < 10 ; i++) {
        for (j = 0; j< 10 ; j++) {
            pixel = cvGet2D(hsv, y+i, x+j);
            // Change the value of the tracked color with the color of the selected pixel
            *h += (int)pixel.val[0];
            *s += (int)pixel.val[1];
            *v += (int)pixel.val[2];
        }
    }
    *h = *h/100;
    *s = *s/100;
    *v = *v/100;

    // Release the memory of the hsv image
    cvReleaseImage(&hsv);
}

void Methods::binarization(IplImage* imgSrc, IplImage *imgDest, int h, int s, int v, int tolerance){
    IplImage *hsv;
    IplConvKernel *kernel;

    // Create the hsv image
    hsv = cvCloneImage(imgSrc);
    cvCvtColor(imgSrc, hsv, CV_BGR2HSV);

    // We create the mask
    cvInRangeS(hsv, cvScalar(h - tolerance -1, s - tolerance, 0), cvScalar(h + tolerance -1, s + tolerance, 255), imgDest);
    // Create kernels for the morphological operation
    kernel = cvCreateStructuringElementEx(5, 5, 2, 2, CV_SHAPE_ELLIPSE);

    // Morphological opening (inverse because we have white pixels on black background)
    cvDilate(imgDest, imgDest, kernel, 1);
    cvErode(imgDest, imgDest, kernel, 1);

    // We release the memory of kernels
    cvReleaseStructuringElement(&kernel);

    // We release the memory of the hsv image
    cvReleaseImage(&hsv);
}

CvRect Methods::computeROI(CvRect rect, int padding) {
    CvRect ROI = {0,0,0,0};
    ROI.x = rect.x + padding;
    ROI.y = rect.y + padding;
    ROI.width = rect.width - 2*padding;
    ROI.height = rect.height - 2*padding;

    return ROI;
}

void Methods::detectGoal(IplImage *img, CvRect ROI, int x, int y, CvBox2D *pBox, CvRect *pRect)
{
    int h, s, v;
    int tolerance = 10;

    getObjectColor(x, y, img, &h, &s, &v);
    qDebug() << "h:" << h << " s:" << s << " v:" << v;
    IplImage *imgDest ;
    imgDest = cvCreateImage(cvGetSize(img), img->depth, 1);

    cvSetImageROI(imgDest, ROI);
    binarization(img, imgDest, h, s, v, tolerance);

    showImage(imgDest, "bin");
    cvWaitKey(0);

    cvSetImageROI(imgDest, ROI);
    //CvPoint2D32f pt = cvPoint2D32f(x, y);
    //FindBox(imgDest, pBox, pRect);
    //findShapeAreaAroundPoint(imgDest, pt, pBox, pRect);
}

void Methods::detectArea(IplImage *img, CvPoint2D32f pt, CvBox2D *pBox, CvRect *pRect)
{
    // create memory storage that will contain all the dynamic data
    int flip=0;
    int seuil=200;

    //Initialization
    IplImage *img_nvg_tmp=cvCreateImage(cvGetSize(img), img->depth, 1);
    IplImage *img_bin_tmp=cvCloneImage(img_nvg_tmp);
    IplImage *tmp=cvCloneImage(img_bin_tmp);

    //Morphology Kernel definition
    IplConvKernel *kernel=cvCreateStructuringElementEx(3, 3, 1, 1,CV_SHAPE_RECT);

    //Control the orientation
    if(img->origin!=IPL_ORIGIN_TL){
        flip=CV_CVTIMG_FLIP;
    }

    cvConvertImage(img, img_nvg_tmp, flip);

    //Binarization
    cvThreshold(img_nvg_tmp, img_bin_tmp, seuil, 255, CV_THRESH_BINARY);
    //Morphology
    cvMorphologyEx(img_bin_tmp, img_bin_tmp, tmp, kernel, CV_MOP_OPEN);

    //Find the area shape
    findShapeAreaAroundPoint(img_bin_tmp, pt, pBox, pRect);

    //Release memory
    cvReleaseImage(&img_nvg_tmp);
    cvReleaseImage(&img_bin_tmp);
    cvReleaseImage(&tmp);
    cvReleaseStructuringElement(&kernel);
}

void Methods::findShapeAreaAroundPoint(IplImage *img_bin, CvPoint2D32f pt, CvBox2D *pBox, CvRect *pRect) {
    // create memory storage that will contain all the dynamic data
    CvMemStorage* storage = cvCreateMemStorage(0);
    CvSeq* contours;
    CvSeq *c;
    //CvPoint2D32f pt = cvPoint2D32f((double)img_bin->width/2, (double)img_bin->height/2);

    cvFindContours( img_bin, storage, &contours, sizeof(CvContour),CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0) );

    c = contours;
    int isInside = -1;

    while ((c != NULL)&&(isInside < 0)) {
        printf("isInside:%d\n", isInside);
        isInside =  cvPointPolygonTest(c, pt, 0);
        printf("isInside:%d\n", isInside);
        if (isInside < 0)
            c = c->h_next;
    }
    if (c!=NULL) {
        *pBox = cvMinAreaRect2(c);
        *pRect = cvBoundingRect(c, 1);
    }

    cvReleaseMemStorage(&storage);
}


void Methods::FindBox(IplImage *img_bin, CvBox2D *pBox, CvRect *pRect)
{
    // create memory storage that will contain all the dynamic data
    CvMemStorage* storage = cvCreateMemStorage(0);
    CvSeq* contours;

    cvFindContours( img_bin, storage, &contours, sizeof(CvContour),CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0) );
    if (contours) {
        *pBox = cvMinAreaRect2(contours);
        *pRect = cvBoundingRect(contours, 1);
    }

    cvReleaseMemStorage(&storage);
}

void Methods::drawBox(IplImage *img, CvBox2D box, CvRect rect)
{
    CvPoint2D32f box_vtx[4];
    CvPoint pt, pt0;
    int i;

    if (box.size.height != 0) {
        cvBoxPoints( box, box_vtx );	// Finds box vertices
        for( i = 0; i < 4; i++ )
        {
            pt0.x = cvRound(box_vtx[i].x);
            pt0.y = cvRound(box_vtx[i].y);
            pt.x = cvRound(box_vtx[(i+1)%4].x);
            pt.y = cvRound(box_vtx[(i+1)%4].y);
            cvLine(img, pt0, pt, CV_RGB(0, 255, 0), 5, CV_AA, 0);
        }
    }
    pt0.x = rect.x;
    pt0.y = rect.y;
    pt.x = rect.x + rect.width;
    pt.y = rect.y + rect.height;
    cvRectangle(img, pt0, pt, CV_RGB(0, 0, 255), 5, CV_AA, 0);
}

void Methods::showImage(IplImage *img_originale, char *name){
    cvNamedWindow(name, CV_WINDOW_AUTOSIZE);
    cvShowImage(name, img_originale);
}

void Methods::Print_Box(CvBox2D box) {
    printf("Box2D:{center: (%f,%f), size: (%f,%f), angle: %f\n",
           box.center.x, box.center.y, box.size.height, box.size.width, box.angle);
}

void Methods::Print_Rect(CvRect rect) {
    printf("Rect:{position: (%d,%d), size: (%d,%d)}\n", rect.x, rect.y, rect.height, rect.width);
}

void Methods::afficher_Point(IplImage *image, CvPoint p) {
    IplImage *temp=cvCloneImage(image);

    printf("x:%d, y:%d\n", p.x, p.y);
    cvCircle(temp, p, 5,CV_RGB(0, 255, 0), 5, CV_AA, 0);
    showImage(temp, "point");
}

void Methods::testMethods(){
    // Pb :Photo 4, 8, 13 (12 à supprimer)
    qDebug() << "debut" ;
    IplImage *normal = cvLoadImage("/Users/Piro/Desktop/Photo6.jpg");

    if (normal != NULL) {
        qDebug() << "Image chargée" ;
        CvBox2D box = {{0,0},{0,0},0};
        CvRect rect = {0,0,0,0};
        qDebug() << "debut traitement image" ;
        CvPoint2D32f pt = cvPoint2D32f((double)normal->width/2, (double)normal->height/2);
        detectArea(normal, pt, &box, &rect);
        qDebug() << "fin traitement image";
        qDebug() << "Box2D:{center: (" << box.center.x <<"," << box.center.y;
        qDebug() << "Rect:{position: ( " << rect.x << "," << rect.y << "), size: ("
                << rect.height << "," << rect.width << ")}";
        IplImage *imgDest = cvCloneImage(normal);
        drawBox(imgDest, box, rect);


        CvRect ROI;
        ROI = computeROI(rect, 10);
        cvCircle(imgDest, cvPoint(140, 220), 5, CV_RGB(0, 255, 0), 5, CV_AA, 0);
        showImage(imgDest, "BOX");
        cvWaitKey(0);

        detectGoal(normal, ROI, 140, 220, &box, &rect);
        qDebug() << "Box2D:{center: (" << box.center.x <<"," << box.center.y;
        qDebug() << "Rect:{position: ( " << rect.x << "," << rect.y << "), size: ("
                << rect.height << "," << rect.width << ")}";
    }
}
