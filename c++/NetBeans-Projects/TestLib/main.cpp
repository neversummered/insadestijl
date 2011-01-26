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

#include "../../src/video/myexception.h"
#include "../../src/video/image.h"
#include "../../src/video/jpegimage.h"
#include "../../src/video/camera.h"
#include "../../src/imageAnalyser/analyseimage.h"
#include "../../src/data/position.h"
#include "../../src/monitorCommunication/server.h"
#include "../../src/data/ordremouvement.h"
#include "../../src/data/action.h"
#include "../../src/video/imageShop.h"

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
    } catch (MyException &e) {
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

        } catch (MyException &e) {
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

void testArena(){
    char *fileName = "/Users/Piro/Documents/Enseignements/robot_INSA/images_test/arene/Photo14.jpg";

    IplImage *image = cvLoadImage(fileName);
    AnalyseImage a;
    CvBox2D box = a.computeAreaPosition(image);
    drawBox(image, box);

    showImage(image, "Arena");


}

void testPosition() {
    IplImage * img;
    char *fileName = "/Users/Piro/Documents/Enseignements/robot_INSA/images_test/test_position/Photo1.jpg";

    IplImage *image = cvLoadImage(fileName);
    AnalyseImage a;
    Position pos = a.computeRobotPosition(image);
    std::cout << "x:" << pos.getX() << ", y:" << pos.getY() << ", o:"
            << pos.getOrientation() << std::endl;

    drawPosition(image, pos);

    showImage(image, "Position");

}

void testVideoSreveur() {
    // On déclare notre pointeur sur SourceVideo
    SourceVideo *src;

    src = new Camera(CV_CAP_QT + 1);


    // Initialisation du flux vidéo
    try {
        src->open();
    } catch (MyException &e) {
        // Si une exception se produit, on l'affiche et on quitte.
        std::cout << e.what() << std::endl;
        delete src;
    }

    // Si tout va bien, on affiche les informations du flux vidéo.
    std::cout << src->getInfos() << std::endl;

    Image img;
    JpegImage jImg(img);
    Message msg = Message();
    Position p = Position(1.0, 1.0, 4.0);

    Server s;
    s.openServer("9010");
    int broke = 1;
    while (broke >= 0) {
        try {
            src->getFrame(img);
            jImg.setJpegImage(img);
            msg.setJpegImage(&jImg);
            broke = s.sendMessage(&msg);
            cout << msg.getLenght() << endl;
            msg.setPosition(p);
            broke = s.sendMessage(&msg);
            cout << "postion sent:" << broke << endl;

        } catch (MyException &e) {
            std::cout << "\n" << e.what() << std::endl;
            break;
        }
    }
    s.closeServer();

    std::cout << std::endl;
    delete src;

}

void testReceptionOrdre() {
    Server s;
    s.openServer("9010");
    int broke = 1;

    Message msg = Message();

    while (broke > 0) {
        broke = s.receiveMessage(msg);
        //cout << "message reçu {type:" << msg.getType() << "}" << endl;

        if (msg.getType() == 'M') {
            OrdreMouvement mv = OrdreMouvement(msg);
            cout << "speed:" << mv.getSpeed()
                    << " direction:" << mv.getDirection() << endl;
        } else if (msg.getType() == 'A') {
            msg.print(10);
            Action action = Action(msg);
            cout << "action:" << action.getOrder() << endl;
        }
    }
    s.closeServer();

}

void testServeur() {
    Server s;
    s.openServer("9010");
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
     //  testPosition();
    //   testVideo();
    //testServeur();
    //testVideoSreveur();
    testReceptionOrdre();
    //testArena();
    return 0;
}