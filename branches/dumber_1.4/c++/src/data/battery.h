/*
 *  analyseimage.h
 *  
 *
 *  Created by Pirem on 19/01/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef BATTERY_H
#define BATTERY_H

namespace robotInsa
{
    class Battery
    {
    public:
        Battery() {
            level = 0;
        }

        Battery(int l) {
            level = l;
        }

        // Setter

        void setLevel(int l){
            level = l;
        }

        // Getter

        int getLevel(){
            return level;
        }
        
    private:
        int level;
    };
}

#endif