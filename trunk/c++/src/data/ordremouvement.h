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

namespace robotInsa
{
    class OrdreMouvement
    {
    public:
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