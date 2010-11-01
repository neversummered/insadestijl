#ifndef HEAD_TRACKER_H
#define HEAD_TRACKER_H

#include "ui_headtracker.h"
#include "capturethread.h"
#include <QMainWindow>

class TrackController;
class RenderWidget;
class QToolBar;
class QDockWidget;
class SettingsWidget;
    
class Malevitch : public QMainWindow {
Q_OBJECT
public:
    Malevitch();
public slots:
	void onFlipVerticalChanged(bool flip);
	void onResolutionSelected(CaptureThread::FrameSize newSize);
	void startTracking();
	void stopTracking();
	void startRecording();
	void updateStats();   
protected:
	void closeEvent(QCloseEvent*);
private:
    Ui::HeadTracker ui;
	TrackController* trackController;
	RenderWidget* renderWidget;
	QTimer* updateTimer;
	QToolBar* cameraToolbar;
	QDockWidget* settingsDock;
	SettingsWidget* settingsWidget;
};

#endif
