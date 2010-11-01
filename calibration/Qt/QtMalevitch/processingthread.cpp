#include "processingthread.h"
#include "headstate.h"
#include "filter.h"
#include "imagebuffer.h"
#include <QDebug>

ProcessingThread::ProcessingThread(ImageBuffer* buffer) : QThread(), imageBuffer(buffer), rootFilter(0), flipVertical(false) {
    currState = new HeadState();
}

void ProcessingThread::run() {
    while(true) {
        IplImage* currentFrame = imageBuffer->getFrame();
        if(currentFrame) {
            currState->frame = currentFrame;

            if(rootFilter) {
                rootFilter->processPoint(currState);
            }
            cvReleaseImage(&currentFrame);
        }
    }
}


