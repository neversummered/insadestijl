#include <time.h>

clock_t starTime;
clock_t endTime;


void startStopwatch(){
  starTime = clock ();
}

void stopStopwatch() {
    endTime = clock();
}

float getTimeInSecond(){
    return (float)(endTime-starTime) / CLOCKS_PER_SEC ;
}

int getTimeInMillisecond(){
    return (int) (1000*((float)(endTime-starTime) / CLOCKS_PER_SEC)) ;
}