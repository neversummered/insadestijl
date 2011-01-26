/*
 *  analyseimage.h
 *  
 *
 *  Created by Pirem on 19/01/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef ACTION_H
#define ACTION_H

#include "../monitorCommunication/message.h"
#include "../tools/toolsconvert.h"

namespace robotInsa
{
    class Action
    {
    public:
        Action(int o = -1) : order(o) {
        }

        Action(Message msg) {
            msg.print(100);
            order = ByteArray2int(msg.getData()+5);
            printf("o:%d\n", order);
        }

        // Setter

        void setOrder(int o){
            order = o;
        }
        // Getter

        int getOrder()
        {
            return order;
        }

    private:
        int order;

    };
}

#endif