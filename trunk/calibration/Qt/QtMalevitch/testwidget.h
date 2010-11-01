#ifndef TESTWIDGET_H
#define TESTWIDGET_H

#include <QWidget>
#include <QStateMachine>

namespace Ui {
    class TestWidget;
}

class TestWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TestWidget(QWidget *parent = 0);
    ~TestWidget();
public slots:
    void off();
    void waitWebcamPositioning();
    void beginAreaDetection();
    void isAreaDetected();
    void beginGreenGoalDetection();
    void isGreenAreaDetected();
    void beginRedGoalDetection();
    void isRedAreaDetected();
    void beginTestRobotPositioning();
    void testRobotPositioning();
    void quitRobotPositioning();
    void onClickOnArea(QPoint);
signals:
    void connectionDemander();
    void stop();
    void next();
    void clickOnArea();

private:
    Ui::TestWidget *ui;
    QStateMachine machine;
};

#endif // TESTWIDGET_H
