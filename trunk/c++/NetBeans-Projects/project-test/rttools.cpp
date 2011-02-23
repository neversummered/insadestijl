/* 
 * File:   main.cpp
 * Author: pehladik
 *
 * Created on 18 février 2011, 19:06
 */
#include "rttools.h"
#include "global.h"


void changeState(int event) {
    switch (event) {
        case ORDER_FIND_ARENA:
            state_camera = CAMERA_STATE_SEND_ARENA;
            break;
        case ORDER_ARENA_FAILED:
            state_camera = CAMERA_STATE_SEND_IMAGE;
            break;
        case ORDER_ARENA_IS_FOUND:
            state_camera = CAMERA_STATE_SAVE_ARENA;
            break;
        case ORDER_COMPUTE_CONTINUOUSLY_POSITION:
            state_camera = CAMERA_STATE_SEND_IMAGE_POSITION;
            break;
        case EVENT_ARENA_SAVED:
            state_camera = CAMERA_STATE_SEND_IMAGE;
            break;
        case EVENT_WAIT:
            state_camera = CAMERA_STATE_DO_NOTHING;
            break;
    }
}