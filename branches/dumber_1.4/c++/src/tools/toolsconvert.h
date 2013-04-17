/*
 * utils.h
 *
 *  Created on: 20 janv. 2010
 *      Author: Piro
 */

#ifndef TOOLSCONVERT_H
#define TOOLSCONVERT_H

#include <opencv/cxtypes.h>

namespace robotInsa {
    
    void int2ByteArray(int i, char* b);

    int ByteArray2int(char *b);

    CvRect CvBoxtoCvRect(CvBox2D box);
}

#endif