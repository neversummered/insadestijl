/* 
 * File:   main.cpp
 * Author: pehladik
 *
 * Created on 18 février 2011, 19:06
 */

#include <xenomai/native/task.h>

#include "global.h"
#include "tCommunication.h"
#include "tControlRobot.h"
#include "tCamera.h"



void catch_signal(int sig) {
}

int main(int argc, char* argv[]) {

    printf("#################################\n");
    printf("# CONTROLEUR DU ROBOT           #\n");
    printf("#################################\n");

    signal(SIGTERM, catch_signal);
    signal(SIGINT, catch_signal);

    /* Avoids memory swapping for this program */
    mlockall(MCL_CURRENT | MCL_FUTURE);

    stateCommunicationRobot = STATUS_ERR_UNKNOWN;
    stateCommunicationMonitor = 0;
    state_camera = CAMERA_STATE_DO_NOTHING;

    rt_mutex_create(&mutexComRobot, "MutexComRobot");
    rt_mutex_create(&mutexComMonitor, "MutexComMonitor");

    int status = rt_queue_create(&qOrdreMouvement, "Queue mvt", sizeof (OrdreMouvement), 2, Q_FIFO);
   
    //Exemple : rt_task_create(&demo_task, "trivial", 0, 99, 0);
    rt_task_create(&tCommunication, "tCommMonitor", 0, 99, 0);
    rt_task_create(&tControlRobot, "tControlRobot", 0, 98, 0);
    rt_task_create(&tControlBattery, "tControlBattery", 0, 97, 0);
    rt_task_create(&tCamera, "tCamera", 0, 90, 0);

    //Exemple : rt_task_start(&demo_task, &demo, NULL);
    rt_task_start(&tControlRobot, &controlRobot, NULL);
    rt_task_start(&tCommunication, &communicationMoniteur, NULL);
    rt_task_start(&tControlBattery, &batteryControl, NULL);
    rt_task_start(&tCamera, &camera, NULL);
    
    pause();

    rt_task_delete(&tCommunication);
    rt_task_delete(&tControlRobot);
    rt_task_delete(&tControlBattery);
}


