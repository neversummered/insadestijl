#include "headtracker.h"
#include "trackcontroller.h"
#include "renderwidget.h"
#include "settings.h"
#include "processingthread.h"
#include <QTimer>
#include <QToolBar>
#include <QDockWidget>

HeadTracker::HeadTracker() : QMainWindow(0) {
    ui.setupUi(this);
	renderWidget = new RenderWidget(this);
	trackController = new TrackController();
	trackController->setRootFilter(renderWidget);
	setCentralWidget(renderWidget);

	// create the toolbar
	cameraToolbar = new QToolBar("Camera Toolbar", this);
	cameraToolbar->setIconSize(QSize(32,32));
	cameraToolbar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
	cameraToolbar->addAction(ui.actionStart);
	cameraToolbar->addAction(ui.actionStop);
	cameraToolbar->addAction(ui.actionRecord);
	addToolBar(Qt::LeftToolBarArea, cameraToolbar);
	
        // Initialization of setting (unused in this application)
        settingsWidget = new SettingsWidget();

	updateTimer = new QTimer(this);
	connect(updateTimer, SIGNAL(timeout()), this, SLOT(updateStats()));
	updateTimer->start(1000);
	
	connect(ui.actionQuit, SIGNAL(triggered()), this, SLOT(close()));
	connect(ui.actionStart, SIGNAL(triggered()), this, SLOT(startTracking()));
	connect(ui.actionStop, SIGNAL(triggered()), this, SLOT(stopTracking()));
	connect(ui.actionRecord, SIGNAL(triggered()), this, SLOT(startRecording()));
}

void HeadTracker::closeEvent(QCloseEvent*) {
	if(trackController->isTracking()) {
		trackController->stopTracking();
	}
}

// flip the image vertically
void HeadTracker::onFlipVerticalChanged(bool flip) {
	trackController->getProcessingThread()->setFlipVertical(flip);
}

// resolution has been changed from the settings
void HeadTracker::onResolutionSelected(CaptureThread::FrameSize newSize) {
	if(trackController->isTracking()) {
		trackController->stopTracking();
		trackController->setFrameSize(newSize);
		trackController->startTracking();
	}
	else {
		trackController->setFrameSize(newSize);
	}
}

void HeadTracker::startRecording() {
	
}

void HeadTracker::startTracking() {
	trackController->setFrameSize(settingsWidget->getSelectedResolution());
	trackController->startTracking();
	ui.actionStart->setEnabled(false);
	ui.actionStop->setEnabled(true);
}

void HeadTracker::stopTracking() {
	trackController->stopTracking();
	ui.actionStart->setEnabled(true);
	ui.actionStop->setEnabled(false);
}
	

void HeadTracker::updateStats() {
	statusBar()->showMessage(QString("FPS: ")+QString::number(trackController->getFPS(), 'f', 1));
}
