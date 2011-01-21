/*
 * utils.cpp
 *
 *  Created on: 20 janv. 2010
 *      Author: Piro
 */

#include "toolstime.h"


#include <time.h>
#include <sys/time.h>

namespace robotInsa
{
    long long getTimeMillis()
    {

        // On récupère l'heure dans une structure "timeval"
        timeval tv;
        gettimeofday( &tv, NULL );

        // On la convertit en millisecondes à partir des secondes et
        // des microsecondes
        long long l = tv.tv_usec / 1000;
        l += tv.tv_sec * 1000;


        return l;
    }
}
