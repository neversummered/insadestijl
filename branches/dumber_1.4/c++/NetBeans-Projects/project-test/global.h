/*
 * camera.h
 *
 *  Created on: 18 janv. 2011
 *      Author: P.E. Hladik
 *  Inspired of code from http://www.siteduzero.com/ by NoHar
 */

#ifndef RTGLOBAL_H
#define RTGLOBAL_H

#include <cstdlib>
#include <signal.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include <native/task.h>
#include <native/timer.h>
#include <native/queue.h>
#include <native/sem.h>
#include <native/mutex.h>


#include "../../src/inc_project.h"

#define CAMERA_STATE_SEND_IMAGE           (0)
#define CAMERA_STATE_SEND_IMAGE_POSITION  (1)
#define CAMERA_STATE_SEND_ARENA           (2)
#define CAMERA_STATE_SAVE_ARENA           (3)
#define CAMERA_STATE_DO_NOTHING           (4)

#define EVENT_WAIT                        (105)
#define EVENT_ARENA_SAVED                 (106)


using namespace std;
using namespace robotInsa;

extern RT_TASK demo_task;
extern RT_TASK tCommunication;
extern RT_TASK tControlRobot;
extern RT_TASK tControlBattery;
extern RT_TASK tCamera;

extern RT_QUEUE qOrdreMouvement;

extern RT_MUTEX mutexComRobot;
extern RT_MUTEX mutexComMonitor;


extern Server s;
extern CommunicationRobot comRobot;

extern int stateCommunicationMonitor;
extern int stateCommunicationRobot;
extern int state_camera;

#endif
