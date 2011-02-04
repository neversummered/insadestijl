/*
 *  analyseimage.h
 *  
 *
 *  Created by Pirem on 19/01/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef ARENA_H
#define ARENA_H


namespace robotInsa
{
    class Arena
    {
    public:
        Arena(float x = 0.0, float y = 0.0, float height = 0.0,
                float width = 0.0, float angle = 0.0) {
            box.angle = angle;
            box.center.x = x;
            box.center.x = y;
            box.size.height = height;
            box.size.width = width;
        }

        Arena(CvBox2D b) {
            box = b;
        }

        // Setter

        void setArena(float x = 0.0, float y = 0.0, float height = 0.0,
                float width = 0.0, float angle = 0.0) {
            box.angle = angle;
            box.center.x = x;
            box.center.x = y;
            box.size.height = height;
            box.size.width = width;
        }
        // Getter

        float getX()
        {
            return box.center.x;
        }

        float getY()
        {
            return box.center.y;
        }

        float getAngle()
        {
            return box.angle;
        }

        float getHeight()
        {
            return box.size.height;
        }

        float getWidth()
        {
            return box.size.width;
        }

        CvBox2D getBox(){
            return box;
        }

    private:
        CvBox2D box;

    };
}

#endif