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


#define ORDER_FIND_ARENA                    (1)
#define ORDER_ARENA_FAILED                  (2)
#define ORDER_ARENA_IS_FOUND                (3)
#define ORDER_COMPUTE_CONTINUOUSLY_POSITION (4)
#define ORDER_CONNECT_ROBOT                 (10)

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