/* 
 * File:   main.cpp
 * Author: pehladik
 *
 * Created on 18 février 2011, 19:06
 */

#include "tCamera.h"
#include "tCommunication.h"

void camera(void *arg) {
    SourceVideo *src;
    src = new Camera(0);
    // Initialisation du flux vidéo
    try {
        src->open();
    } catch (MyException &e) {
        // Si une exception se produit, on l'affiche et on quitte.
        //std::cout << e.what() << std::endl;
        delete src;
    }

    // Si tout va bien, on affiche les informations du flux vidéo.
    //std::cout << src->getInfos() << std::endl;
    Image img;
    JpegImage jImg(img);
    Message msg = Message();
    Position p = Position(1.0, 1.0, 4.0);

    AnalyseImage a;
    Arena arene;
    Position pos;
    rt_task_set_periodic(NULL, TM_NOW, 500000000);

    while (1) {
        rt_task_wait_period(NULL);
        printf("CAMERA\n");
        if (stateCommunicationMonitor > 0) {
            printf("TAKE PICTURE, camera state:%d\n", state_camera);
            try {
                switch (state_camera) {
                    case CAMERA_STATE_SEND_IMAGE:
                        printf("Image\n");
                        src->getFrame(img);
                        jImg.setJpegImage(img);
                        msg.setJpegImage(&jImg);
                        sendMessageToMonitor(&msg);
                        break;
                    case CAMERA_STATE_SEND_IMAGE_POSITION:
                        printf("Image+Position\n");
                        src->getFrame(img);
                        pos = a.computeRobotPosition(img);
                        drawPosition(img.ipl(), pos);
                        jImg.setJpegImage(img);
                        msg.setJpegImage(&jImg);
                        sendMessageToMonitor(&msg);
                        break;
                    case CAMERA_STATE_SEND_ARENA:
                        printf("Arene\n");
                        a.setArena(NULL);
                        src->getFrame(img);
                        arene = a.computeAreaPosition(img);
                        drawArena(img, arene);
                        drawRec(img.ipl(), CvBoxtoCvRect(arene.getBox()));
                        jImg.setJpegImage(img);
                        msg.setJpegImage(&jImg);
                        changeState(EVENT_WAIT);
                        sendMessageToMonitor(&msg);
                        break;
                    case CAMERA_STATE_SAVE_ARENA:
                        printf("Save arene\n");
                        a.setArena(&arene);
                        changeState(EVENT_ARENA_SAVED);
                        break;
                    case CAMERA_STATE_DO_NOTHING:
                        printf("Do nothing\n");
                        break;
                }
            } catch (MyException &e) {
                //std::cout << "\n" << e.what() << std::endl;
                break;
            }
        }
    }
}