/*
 * utils.cpp
 *
 *  Created on: 20 janv. 2010
 *      Author: Piro
 */

#include "toolsconvert.h"


#include <time.h>
#include <sys/time.h>

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
    
}
