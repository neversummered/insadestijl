/*
 *  analyseimage.h
 *  
 *
 *  Created by Pirem on 19/01/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef POSITION_H
#define POSITION_H


namespace robotInsa
{
    class Position
    {
    public:
        Position(float x1 = 0.0, float y1 = 0.0, float o1 = 0.0) : x(x1), y(y1), orientation(o1) {
        }

        // Setter

        void setPosition(float x1, float y1, float o1){
            x = x1;
            y = y1;
            orientation = o1;
        }
        // Getter

        float getX()
        {
            return x;
        }

        float getY()
        {
            return y;
        }

        float getOrientation()
        {
            return orientation;
        }

    private:
        float x; /* x-coordinate, usually zero-based */
        float y; /* y-coordinate, usually zero-based */
        float orientation;

    };
}

#endif