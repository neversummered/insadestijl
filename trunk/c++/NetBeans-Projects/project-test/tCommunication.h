/*
 * camera.h
 *
 *  Created on: 18 janv. 2011
 *      Author: P.E. Hladik
 *  Inspired of code from http://www.siteduzero.com/ by NoHar
 */

#ifndef RTCOMMUNICATION_H
#define RTCOMMUNICATION_H

#include <cstdlib>
#include <signal.h>
#include <unistd.h>
#include <sys/mman.h>

#include <native/task.h>
#include <native/timer.h>

#include "../../src/inc_project.h"

#include "global.h"
#include "rttools.h"

void communicationMoniteur(void *arg);
void sendMessageToMonitor(Message *msg);
void sendMessageToRobot(int status);

#endif
