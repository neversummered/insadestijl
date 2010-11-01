#include "headtracker.h"
#include "trackcontroller.h"
#include "renderwidget.h"
#include "testwidget.h"
#include "processingthread.h"
#include <QTimer>
#include <QToolBar>
#include <QDockWidget>
#include <QHBoxLayout>

HeadTracker::HeadTracker() : QMainWindow(0) {
    ui.setupUi(this);
    renderWidget = new RenderWidget(this);
    trackController = new TrackController();
    trackController->setRootFilter(renderWidget);
    setCentralWidget(renderWidget);

    settingsDock = new QDockWidget("Settings", this);
    settingsDock->setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea);
    testWidget = new TestWidget();
    settingsDock->setWidget(testWidget);
    addDockWidget(Qt::LeftDockWidgetArea, settingsDock);
    connect(testWidget, SIGNAL(connectionDemander()), this, SLOT(startTracking()));
    connect(testWidget, SIGNAL(stop()), this, SLOT(stopTracking()));
    connect(renderWidget, SIGNAL(clickOnArea(QPoint)), testWidget, SLOT(onClickOnArea(QPoint)));

    updateTimer = new QTimer(this);
    connect(updateTimer, SIGNAL(timeout()), this, SLOT(updateStats()));
    updateTimer->start(1000);
}

void HeadTracker::closeEvent(QCloseEvent*) {
    if(trackController->isTracking()) {
        trackController->stopTracking();
    }
}

void HeadTracker::startTracking() {
    trackController->startTracking();
}

void HeadTracker::stopTracking() {
    trackController->stopTracking();
}


void HeadTracker::updateStats() {
    statusBar()->showMessage(QString("FPS: ")+QString::number(trackController->getFPS(), 'f', 1));
}
