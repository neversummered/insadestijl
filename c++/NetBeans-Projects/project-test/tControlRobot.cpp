/* 
 * File:   main.cpp
 * Author: pehladik
 *
 * Created on 18 février 2011, 19:06
 */
#include "tControlRobot.h"
#include "tCommunication.h"

void controlRobot(void *arg) {
    OrdreMouvement mv = OrdreMouvement();
    Message msg = Message();
    int status;

    printf("Task tControl ready\n");

    while (1) {
        printf("Attente message\n");
        status = rt_queue_read(&qOrdreMouvement, &mv, sizeof (mv), TM_INFINITE);

        if (status > 0) {
            printf("Reception queue Mvt: %d\n", mv.getDirection());

            rt_mutex_acquire(&mutexComRobot, TM_INFINITE);

            switch (mv.getDirection()) {
                case DIRECTION_STOP:
                    sendMessageToRobot(comRobot.RobotSetMotors(0, 0));
                    break;
                case DIRECTION_AVANCE:
                    sendMessageToRobot(comRobot.RobotSetMotors(1, 1));
                    break;
                case DIRECTION_GAUCHE:
                    sendMessageToRobot(comRobot.RobotSetMotors(-1, 1));
                    break;
                case DIRECTION_DROITE:
                    sendMessageToRobot(comRobot.RobotSetMotors(1, -1));
                    break;
                case DIRECTION_RECULE:
                    sendMessageToRobot(comRobot.RobotSetMotors(-1, -1));
                    break;
                default:
                    sendMessageToRobot(comRobot.RobotSetMotors(0, 0));
            }
            rt_mutex_release(&mutexComRobot);
        }
    }
}

void batteryControl(void *args) {
    int vbat;
    Message msg;
    Battery b = Battery();
    RTIME now, previous;

    rt_task_set_periodic(NULL, TM_NOW, 250000000);
    previous = rt_timer_read();

    printf("tBatteyr ready\n");

    while (true) {
        rt_task_wait_period(NULL);
        
        /*POUR TESTER LE TEMPS
        now = rt_timer_read();
        printf("Time since last turn: %ld.%06ld ms\n",
                (long) (now - previous) / 1000000,
                (long) (now - previous) % 1000000);
        previous = now;
        FIN TEST*/

        printf("BATTERIE TEST\n");
        if (stateCommunicationRobot == STATUS_OK) {
            printf("Battery control start\n");
            rt_mutex_acquire(&mutexComRobot, TM_INFINITE);
            sendMessageToRobot(comRobot.RobotGetVBat(&vbat));
            rt_mutex_release(&mutexComRobot);

            if (stateCommunicationRobot == STATUS_OK) {
                b.setLevel(vbat);
                msg.setBattery(b);
                sendMessageToMonitor(&msg);
            }
        }
    }
}