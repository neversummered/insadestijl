/* 
 * File:   main.cpp
 * Author: pehladik
 *
 * Created on 18 février 2011, 19:06
 */
#include <xenomai/native/task.h>

#include "tCommunication.h"

#define ERROR_TOLERANCE (2)

void communicationMoniteur(void *arg) {

    Message msg = Message();

    printf("Task tCommunication ready\n");
    fflush(stdout);
    while (true) {
        s.openServer("9010");
        stateCommunicationMonitor = 1;
        state_camera = CAMERA_STATE_SEND_IMAGE;
        while (stateCommunicationMonitor > 0) {
            stateCommunicationMonitor = s.receiveMessage(msg);
            printf("Communication status Monitor%d\n", stateCommunicationMonitor);
            if (msg.getType() == 'A') {
                Action action = Action(msg);
                if (action.getOrder() == ORDER_CONNECT_ROBOT) {
                    printf("ordre connexion robot\n");
                    fflush(stdout);
                    
                    rt_mutex_acquire(&mutexComRobot, TM_INFINITE);
                    sendMessageToRobot(comRobot.RobotOpenCom());
                    rt_mutex_release(&mutexComRobot);

                    printf("State comm robot:%d after openComm\n", stateCommunicationRobot);
                    msg.setInt(stateCommunicationRobot);
                    sendMessageToMonitor(&msg);
                } else {
                    changeState(action.getOrder());
                }
            } else if (msg.getType() == 'M') {
                OrdreMouvement mv = OrdreMouvement(msg);
                printf("ordre mouvement robot\n");
                int res = rt_queue_write(&qOrdreMouvement, &mv, sizeof (OrdreMouvement), Q_NORMAL);
                //printf("ecriture queue:%d\n", res);
            }
        }
        changeState(CAMERA_STATE_DO_NOTHING);
        sendMessageToRobot(comRobot.RobotCloseCom());
        stateCommunicationRobot = STATUS_ERR_UNKNOWN;

        s.closeServer();
    }
}

void sendMessageToMonitor(Message *pMsg) {
    if (s.isActive()) {
        printf("send message to monitor %c\n", pMsg->getType());
        rt_mutex_acquire(&mutexComMonitor, TM_INFINITE);
        stateCommunicationMonitor = s.sendMessage(pMsg);
        printf("State communication monitor %d\n", stateCommunicationMonitor);
        rt_mutex_release(&mutexComMonitor);
    }
}

void sendMessageToRobot(int status) {
    static int errorCommunicationRobot = 0;
    Message msg = Message();

    if (status != STATUS_OK) {
        errorCommunicationRobot++;
    } else {
        errorCommunicationRobot = 0;
        stateCommunicationRobot = status;
    }

    if (errorCommunicationRobot >= ERROR_TOLERANCE) {
        stateCommunicationRobot = status;
        printf("robot error:%d\n", stateCommunicationRobot);
        msg.setInt(stateCommunicationRobot);
        sendMessageToMonitor(&msg);
    }

    stateCommunicationRobot = status;
}
