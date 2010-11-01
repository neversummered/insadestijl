#include "settings.h"
#include <QDebug>

SettingsWidget::SettingsWidget(QWidget* parent) : QWidget(parent) {
	ui.setupUi(this);
	setMinimumWidth(150);
	
	connect(ui.res640Radio, SIGNAL(toggled(bool)), this, SLOT(on640ResToggled(bool)));
	connect(ui.res320Radio, SIGNAL(toggled(bool)), this, SLOT(on320ResToggled(bool)));
	connect(ui.flipVertical, SIGNAL(stateChanged(int)), this, SLOT(onVerticalFlipStateChanged(int)));
}

void SettingsWidget::onVerticalFlipStateChanged(int state) {
	emit(flipVerticalChanged(state == Qt::Checked));
}

void SettingsWidget::on640ResToggled(bool on) {
	qDebug() << "640 is" << on;
	if(on) {
		emit(resolutionSelected(CaptureThread::Size640));
	}
}

void SettingsWidget::on320ResToggled(bool on) {
	qDebug() << "320 is" << on;
	if(on) {
		emit(resolutionSelected(CaptureThread::Size320));
	}
}

CaptureThread::FrameSize SettingsWidget::getSelectedResolution() {
	if(ui.res640Radio->isChecked()) {
		return CaptureThread::Size640;
	}
	return CaptureThread::Size320;
}
