/*
 *  analyseimage.h
 *  
 *
 *  Created by Pirem on 19/01/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef ORDREMOUVEMENT_H
#define ORDREMOUVEMENT_H

#include "../monitorCommunication/message.h"

#define DIRECTION_STOP            (0)
#define DIRECTION_AVANCE          (1)
#define DIRECTION_DROITE          (2)
#define DIRECTION_GAUCHE          (3)
#define DIRECTION_RECULE          (4)

namespace robotInsa
{
    class OrdreMouvement
    {
    public:
        OrdreMouvement() {
            direction = -1;
            speed = -1;
        }

        OrdreMouvement(Message msg) {
            direction = (int) *(msg.getData()+5);
            speed = (int) *(msg.getData()+6);
        }

        // Setter

        void setSpeed(int s){
            speed = s;
        }

        void setDirection(int d){
            direction = d;
        }

        // Getter

        int getSpeed(){
            return speed;
        }

        int getDirection(){
            return direction;
        }

    private:
        int speed;
        int direction;

    };
}

#endif