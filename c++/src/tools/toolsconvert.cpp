/*
 * utils.cpp
 *
 *  Created on: 20 janv. 2010
 *      Author: Piro
 */

#include "toolsconvert.h"


#include <time.h>
#include <sys/time.h>

#include <math.h>
#include <opencv/highgui.h>
#include <opencv/cv.h>

namespace robotInsa
{
    void int2ByteArray(int i, char* b) {
        *b = i >> 24;
        *(b + 1) = i >> 16;
        *(b + 2) = i >> 8;
        *(b + 3) = i;
    }

    int ByteArray2int(char *b){
        int i;
        i = (int)(*(b) << 24)
                + (int)(*(b+1) << 16)
                +  (int)(*(b+2) << 8)
                +  (int)(*(b+3));
        return i;
    }

    CvRect CvBoxtoCvRect(CvBox2D box) {
        CvRect rec;
        CvPoint2D32f box_vtx[4];
        cvBoxPoints(box, box_vtx);

        rec.x = box_vtx[3].x;
        rec.y = box_vtx[0].y;
        rec.height = abs(box_vtx[0].y -box_vtx[2].y);
        rec.width = abs(box_vtx[1].x -box_vtx[3].x);
                

        return rec;
    }
    
}
