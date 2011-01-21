/* 
 * File:   main.cpp
 * Author: Piro
 *
 * Created on 18 janvier 2011, 21:22
 */

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include <string>

#include "/Users/Piro/Documents/Implementation/DeStijl/archi/video/myexception.h"
#include "/Users/Piro/Documents/Implementation/DeStijl/archi/video/image.h"
#include "/Users/Piro/Documents/Implementation/DeStijl/archi/video/jpegimage.h"
#include "/Users/Piro/Documents/Implementation/DeStijl/archi/video/camera.h"
#include "/Users/Piro/Documents/Implementation/DeStijl/archi/imageAnalyser/analyseimage.h"
#include "/Users/Piro/Documents/Implementation/DeStijl/archi/imageAnalyser/position.h"
#include "/Users/Piro/Documents/Implementation/DeStijl/archi/monitorCommunication/server.h"
//#include "/Users/Piro/Documents/Implementation/DeStijl/archi/video/utils.h"
using namespace std;

using namespace robotInsa;

long long debut_mesure;
long long fin_mesure;

void testVideo() {
    // On déclare notre pointeur sur SourceVideo
    SourceVideo *src;

    src = new Camera(CV_CAP_QT + 1);


    // Initialisation du flux vidéo
    try {
        src->open();
    }    catch (MyException &e) {
        // Si une exception se produit, on l'affiche et on quitte.
        std::cout << e.what() << std::endl;
        delete src;
    }

    // Si tout va bien, on affiche les informations du flux vidéo.
    std::cout << src->getInfos() << std::endl;

    cvNamedWindow("video", CV_WINDOW_AUTOSIZE);
    Image img;
    src->getFrame(img);
    JpegImage jImg(img);
    char key = 'a';

    // Début de la mesure du frame rate
    debut_mesure = img.getDate();

    while (key != 'q') {
        try {
            src->getFrame(img);
            fin_mesure = img.getDate();

        }        catch (MyException &e) {
            std::cout << "\n" << e.what() << std::endl;
            break;
        }

        jImg.setJpegImage(img);
        cvShowImage("video", img.ipl());


        key = cvWaitKey(10);

        // Affichage du frame rate
        std::cout << "\rFrame Rate : " << std::setw(5);
        std::cout << std::left << std::setprecision(4);
        std::cout << 1000.0 / (fin_mesure - debut_mesure) << " FPS" << std::flush;
        debut_mesure = fin_mesure;

    }

    std::cout << std::endl;
    cvDestroyWindow("video");
    delete src;

}

void testPosition() {
    IplImage * img;
    char *fileName = "/Users/Piro/Documents/Enseignements/robot_INSA/images_test/test_position/Photo3.jpg";

    IplImage *image = cvLoadImage(fileName);
    AnalyseImage a;
    Position pos = a.computeRobotPosition(image);
    std::cout << "x:" << pos.getX() << ", y:" << pos.getY() << ", o:"
            << pos.getOrientation() << std::endl;

    CvScalar color = CV_RGB(0, 255, 0);

    CvPoint p1 = cvPoint((int) pos.getX(), (int) pos.getY());
    CvPoint p2 = cvPoint(p1.x + (int) 30 * cos(pos.getOrientation()),
            p1.y + (int) 30 * sin(pos.getOrientation()));

    cvCircle(image, p1, 2, color, 5, CV_AA, 0);
    cvLine(image, p1, p2, color, 1, CV_AA, 0);

    cvNamedWindow("Position", CV_WINDOW_AUTOSIZE);

    cvShowImage("Position", image);
    cvWaitKey(0);

}

void testServeur() {
    Server s;
    s.openServer("9011");
    while (1) {
        Message msg = Message();
        msg.setInt(2);
        s.sendMessage(&msg);
        sleep(1);

        msg.setChar('a');
        s.sendMessage(&msg);
        sleep(1);

        msg.setString("coucou");
        s.sendMessage(&msg);
        sleep(1);

        s.receiveMessage(msg);
        cout << msg.getLenght() << endl;
    }
}

int main(int argc, char* argv[]) {
    //   testPosition();
    //   testVideo();
    testServeur();
    return 0;
}
