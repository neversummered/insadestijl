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
#include <opencv/cvtypes.h>

#include <string>

#include <pthread.h>

#include "../../src/video/myexception.h"
#include "../../src/video/Image.h"
#include "../../src/video/jpegimage.h"
#include "../../src/video/camera.h"
#include "../../src/imageAnalyser/analyseimage.h"
#include "../../src/data/position.h"
#include "../../src/monitorCommunication/server.h"
#include "../../src/data/ordremouvement.h"
#include "../../src/data/action.h"
#include "../../src/video/imageShop.h"
#include "../../src/data/arena.h"
#include "../../src/tools/cvrect32f.h"
#include "../../src/robotCommunication/communicationrobot.h"

//#include "/Users/Piro/Documents/Implementation/DeStijl/archi/video/utils.h"
using namespace std;

using namespace robotInsa;

long long debut_mesure;
long long fin_mesure;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
Action act = Action(0);

void testVideo() {
    // On déclare notre pointeur sur SourceVideo
    SourceVideo *src;

    src = new Camera(0);


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

void testArena() {
    char *fileName = "/Users/Piro/Documents/Enseignements/robot_INSA/images_test/arene/Photo14.jpg";

    IplImage *image = cvLoadImage(fileName);

    AnalyseImage a;
    Arena arene = a.computeAreaPosition(image);
    drawBox(image, arene.getBox());
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

void testPosition2() {
    // On déclare notre pointeur sur SourceVideo
    SourceVideo *src;

    src = new Camera(0);


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

    AnalyseImage a;
    Position pos = a.computeRobotPosition(img.ipl());
    std::cout << "x:" << pos.getX() << ", y:" << pos.getY() << ", o:"
            << pos.getOrientation() << std::endl;

    drawPosition(img.ipl(), pos);

    showImage(img.ipl(), "Position");
    delete src;
}

void testVideoSreveur() {


    while (1) {
        // On déclare notre pointeur sur SourceVideo
        SourceVideo *src;
        src = new Camera(0);
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
        long long time;

        while (broke >= 0) {
            try {
                time = getTimeMillis();
                src->getFrame(img);
                jImg.setJpegImage(img);
                msg.setJpegImage(&jImg);
                broke = s.sendMessage(&msg);
                cout << msg.getLenght() << " time:"
                        << img.getDate() - time << endl;
                sleep(1);
                /*msg.setPosition(p);
                broke = s.sendMessage(&msg);
                cout << "postion sent:" << broke << endl;*/

            } catch (MyException &e) {
                std::cout << "end\n" << e.what() << std::endl;
                //break;
            }
        }
        s.closeServer();

        std::cout << std::endl;
        delete src;
    }

}

void testVideoSreveur2() {

    while (1) {
        Image img;
        JpegImage jImg(img);
        Message msg = Message();
        Position p = Position(1.0, 1.0, 4.0);

        Server s;
        s.openServer("9010");
        int broke = 1;
        long long time;
        time = getTimeMillis();

        while (broke >= 0) {
            // On déclare notre pointeur sur SourceVideo
            SourceVideo *src;
            src = new Camera(0);
            // Initialisation du flux vidéo
            try {
                src->open();
            } catch (MyException &e) {
                // Si une exception se produit, on l'affiche et on quitte.
                std::cout << e.what() << std::endl;
                delete src;
            }

            try {
                src->getFrame(img);
                jImg.setJpegImage(img);
                msg.setJpegImage(&jImg);
                broke = s.sendMessage(&msg);
                cout << msg.getLenght() << " time:"
                        << img.getDate() - time << endl;
                time = img.getDate();
                //sleep(1);
                /*msg.setPosition(p);
                broke = s.sendMessage(&msg);
                cout << "postion sent:" << broke << endl;*/

            } catch (MyException &e) {
                std::cout << "end\n" << e.what() << std::endl;
                //break;
            }

            std::cout << std::endl;
            delete src;
        }
        s.closeServer();

    }

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

void* thr_f(void * param) {
    Server *s = (Server *) param;
    // On déclare notre pointeur sur SourceVideo
    SourceVideo *src;
    src = new Camera(0);
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

    int broke = 1;

    int status = 0;

    AnalyseImage a;
    Arena arene;
    Position pos;
    int dodo = 0;
    while (broke >= 0) {
        try {
            pthread_mutex_lock(&mutex);
            status = act.getOrder();
            pthread_mutex_unlock(&mutex);

            if (status == 0) {
                src->getFrame(img);
                jImg.setJpegImage(img);
                msg.setJpegImage(&jImg);
                broke = s->sendMessage(&msg);
                usleep(100000);
            } else if (status == 1) {
                src->getFrame(img);
                arene = a.computeAreaPosition(img);
                drawArena(img, arene);
                drawRec(img.ipl(), CvBoxtoCvRect(arene.getBox()));
                jImg.setJpegImage(img);
                msg.setJpegImage(&jImg);
                a.setArena(&arene);
                broke = s->sendMessage(&msg);
                act.setOrder(-1);
            } else if (status == 2) {

                cout << "image" << endl;
                src->getFrame(img);
                cout << "position" << endl;
                pos = a.computeRobotPosition(img);
                std::cout << "x:" << pos.getX() << ", y:" << pos.getY() << ", o:"
                        << pos.getOrientation() << std::endl;

                cout << "dessin" << endl;

                drawPosition(img.ipl(), pos);
                cout << "dessin2" << endl;
                jImg.setJpegImage(img);
                msg.setJpegImage(&jImg);
                broke = s->sendMessage(&msg);

                usleep(100000);
            }
            //cout << msg.getLenght() << endl;
            /*msg.setPosition(p);
            broke = s.sendMessage(&msg);
            cout << "postion sent:" << broke << endl;*/

        } catch (MyException &e) {
            std::cout << "\n" << e.what() << std::endl;
            break;
        }
    }
}

void testCalibration() {
    Server s;
    s.openServer("9010");
    int broke = 1;
    pthread_t thr1;


    pthread_create(&thr1, NULL, thr_f, &s);

    Message msg = Message();

    while (broke > 0) {
        broke = s.receiveMessage(msg);
        //cout << "message reçu {type:" << msg.getType() << "}" << endl;

        if (msg.getType() == 'M') {
            OrdreMouvement mv = OrdreMouvement(msg);
            cout << "speed:" << mv.getSpeed()
                    << " direction:" << mv.getDirection() << endl;
        } 
    }
    s.closeServer();
}

void testArenaPosition() {
    SourceVideo *src;
    src = new Camera(0);
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
    Position p = Position(1.0, 1.0, 4.0);
    AnalyseImage a;
    Arena arene;
    Position pos;
    /*Premiere image*/
    src->getFrame(img);
    arene = a.computeAreaPosition(img);
    drawArena(img, arene);
    drawRec(img.ipl(), CvBoxtoCvRect(arene.getBox()));
    showImage(img.ipl(), "arene");
    a.setArena(&arene);

    /* Deuxième image*/
    src->getFrame(img);
    pos = a.computeRobotPosition(img);
    drawPosition(img, pos);
    showImage(img.ipl(), "position");
}

void testSingleVideo() {

    // On déclare notre pointeur sur SourceVideo
    SourceVideo *src;
    src = new Camera(0);
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
    long long time;

    try {
        time = getTimeMillis();
        src->getFrame(img);
        jImg.setJpegImage(img);
        msg.setJpegImage(&jImg);
        cout << "size" << msg.getLenght() << endl;
        broke = s.sendMessage(&msg);
        cout << "Message envoye (size send:" << broke << endl;
        showImage(img.ipl(), "stop");
        /*msg.setPosition(p);
        broke = s.sendMessage(&msg);
        cout << "postion sent:" << broke << endl;*/

    } catch (MyException &e) {
        std::cout << "end\n" << e.what() << std::endl;
        //break;
    }

    s.closeServer();

    std::cout << std::endl;
    delete src;
}

void testCommRobot(){
    CommunicationRobot c;
    RobotStatus status;
    c.RobotOpenCom();
    //c.RobotStart();
    //c.RobotSetMotors(1,1);
    int vbat;
    c.RobotGetVBat(&vbat);
    cout << "Baterie:" << vbat << endl;
    int sensor;
    c.RobotGetSensor(&sensor);
    cout << "capteur:" << sensor << endl;
}

void testManualControl(){
    Server s;
    s.openServer("9010");
    int broke = 1;
    Message msg = Message();
    OrdreMouvement mv;

    CommunicationRobot c;
    c.RobotOpenCom();

    long long time;
    while (broke > 0) {
        broke = s.receiveMessage(msg);
        time = getTimeMillis();
        if (msg.getType() == 'M') {
            mv = OrdreMouvement(msg);
            cout << "Ordre{dir:" << mv.getDirection()
                    << ",speed:" << mv.getSpeed() << "}" << endl;
            switch (mv.getDirection()) {
                case DIRECTION_STOP:
                    c.RobotSetMotors(0,0);
                    break;
                case DIRECTION_AVANCE:
                    c.RobotSetMotors(1,1);
                    break;
                case DIRECTION_GAUCHE:
                    c.RobotSetMotors(-1,1);
                    break;
                case DIRECTION_DROITE:
                    c.RobotSetMotors(1,-1);
                    break;
                case DIRECTION_RECULE:
                    c.RobotSetMotors(-1,-1);
                    break;
                default:
                    c.RobotSetMotors(0,0);
            }
        }
    }

    s.closeServer();
}

/*void testManualControl(){
    Server s;
    s.openServer("9010");
    int broke = 1;
    long long time;

    Message msg = message();
    while (broke > 0) {
        broke = s.receiveMessage(msg);
        //cout << "message reçu {type:" << msg.getType() << "}" << endl;

        if (msg.getType() == 'M') {
            OrdreMouvement mv = OrdreMouvement(msg);
            cout << "speed:" << mv.getSpeed()
                    << " direction:" << mv.getDirection() << en

    s.closeServer();

    std::cout << std::endl;

}*/

int main(int argc, char* argv[]) {
    // testPosition2();
    //  testVideo();
    //testServeur();
    //testVideoSreveur();
    //testReceptionOrdre();
    //testArena();
    //testCalibration();
    //testArenaPosition();
    //testSingleVideo();
    //testCommRobot();

    testManualControl();
    return 0;
}