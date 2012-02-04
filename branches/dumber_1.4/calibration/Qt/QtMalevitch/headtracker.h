#ifndef HEAD_TRACKER_H
#define HEAD_TRACKER_H

#include "ui_headtracker.h"
#include "capturethread.h"
#include <QMainWindow>

class TrackController;
class RenderWidget;
class QToolBar;
class QDockWidget;
class TestWidget;

class HeadTracker : public QMainWindow {
Q_OBJECT
public:
    HeadTracker();
public slots:
        void stopTracking();
        void startTracking();
        void updateStats();
protected:
        void closeEvent(QCloseEvent*);
private:
    Ui::HeadTracker ui;
        TrackController* trackController;
        RenderWidget* renderWidget;
        QTimer* updateTimer;
        QDockWidget* settingsDock;
        TestWidget* testWidget;
};

#endif
