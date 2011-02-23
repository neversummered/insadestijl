/* 
 * File:   main.cpp
 * Author: pehladik
 *
 * Created on 18 février 2011, 19:06
 */

#include "global.h"

using namespace std;
using namespace robotInsa;

RT_TASK demo_task;
RT_TASK tCommunication;
RT_TASK tControlRobot;
RT_TASK tControlBattery;
RT_TASK tCamera;

RT_QUEUE qOrdreMouvement;

RT_MUTEX mutexComRobot;
RT_MUTEX mutexComMonitor;;

Server s;
CommunicationRobot comRobot;

int stateCommunicationMonitor;
int stateCommunicationRobot;
int state_camera;