#ifndef SETTINGS_H
#define SETTINGS_H

#include "ui_settings.h"
#include "capturethread.h"

class SettingsWidget : public QWidget {
Q_OBJECT;
public:
	SettingsWidget(QWidget* parent = 0);
	CaptureThread::FrameSize getSelectedResolution();
public slots:
	void on640ResToggled(bool on);
	void on320ResToggled(bool on);
	void onVerticalFlipStateChanged(int state);
signals:
	void resolutionSelected(CaptureThread::FrameSize);
	void flipVerticalChanged(bool);
	
private:
	Ui::SettingsWidget ui;
};

#endif
