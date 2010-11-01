#include "capturethread.h"
#include "imagebuffer.h"
#include <QDebug>
#include <QTime>

CaptureThread::CaptureThread(ImageBuffer* buffer) : QThread(), imageBuffer(buffer), captureActive(false), fps(0.0) {
    capture = cvCaptureFromCAM(501);
}

/* 
 * Start the thread
 */
void CaptureThread::run() {	
    QTime time;
    time.start();
    int numFrames = 0;
    while(true) {
        if (captureActive) {
            imageBuffer->addFrame(cvQueryFrame(capture));
            updateFPS(time.elapsed());
        }
    }
}

void CaptureThread::updateFPS(int time) {
    frameTimes.enqueue(time);
    if(frameTimes.size() > 15) {
        frameTimes.dequeue();
    }
    if(frameTimes.size() > 1) {
        fps = frameTimes.size()/((double)time-frameTimes.head())*1000.0;
    }
    else {
        fps = 0;
    }
}

/* 
 * Start the capture process
 */
bool CaptureThread::startCapture() {
    if(!captureActive) {
        if(!capture || !imageBuffer){
            qDebug() << "E: Capture not initialized or invalid buffer";
            return false;
        }
        qDebug() << "Starting to track";
        captureActive = true;
        captureWait.wakeAll();
        return true;
    }
    return false;
}

/* 
 * Stop the capture process
 */
void CaptureThread::stopCapture() {
    qDebug() << "Stop capture requested.";
    captureActive = false;
}
