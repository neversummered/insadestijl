#include "testwidget.h"
#include "ui_testwidget.h"
#include <QDebug>
#include <QState>
#include <QPoint>

TestWidget::TestWidget(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::TestWidget)
{
    ui->setupUi(this);
    //connect(ui->pBtStop, SIGNAL(released()), this, SLOT(onStop()));

    QState *qSoff = new QState();
    machine.addState(qSoff);
    qSoff->setObjectName("qSoff");
    connect(qSoff, SIGNAL(entered()), this, SLOT(off()));

    QState *qSwaitWebcamPositioning = new QState();
    machine.addState(qSwaitWebcamPositioning);
    qSwaitWebcamPositioning->setObjectName("qSwaitWebcamPositioning");
    connect(qSwaitWebcamPositioning, SIGNAL(entered()), this, SLOT(waitWebcamPositioning()));

    QState *qSbeginAreaDetection = new QState();
    machine.addState(qSbeginAreaDetection);
    qSbeginAreaDetection->setObjectName("qSbeginAreaDetection");
    connect(qSbeginAreaDetection, SIGNAL(entered()), this, SLOT(beginAreaDetection()));

    QState *qSisAreaDetected = new QState();
    machine.addState(qSisAreaDetected);
    qSisAreaDetected->setObjectName("qSisAreaDetected");
    connect(qSisAreaDetected, SIGNAL(entered()), this, SLOT(isAreaDetected()));

    QState *qSbeginGreenGoalDetection = new QState();
    machine.addState(qSbeginGreenGoalDetection);
    qSbeginGreenGoalDetection->setObjectName("qSbeginGreenGoalDetection");
    connect(qSbeginGreenGoalDetection, SIGNAL(entered()), this, SLOT(beginGreenGoalDetection()));

    QState *qSisGreenGoalDetected = new QState();
    machine.addState(qSisGreenGoalDetected);
    qSisGreenGoalDetected->setObjectName("qSisGreenGoalDetected");
    connect(qSisGreenGoalDetected, SIGNAL(entered()), this, SLOT(isGreenAreaDetected()));

    QState *qSbeginRedGoalDetection = new QState();
    machine.addState(qSbeginRedGoalDetection);
    qSbeginRedGoalDetection->setObjectName("qSbeginRedGoalDetection");
    connect(qSbeginRedGoalDetection, SIGNAL(entered()), this, SLOT(beginRedGoalDetection()));

    QState *qSisRedDetected = new QState();
    machine.addState(qSisRedDetected);
    qSisRedDetected->setObjectName("qSisRedDetected");
    connect(qSisRedDetected, SIGNAL(entered()), this, SLOT(isRedAreaDetected()));

    QState *qSbeginTestRobotPositioning = new QState();
    machine.addState(qSbeginTestRobotPositioning);
    qSbeginTestRobotPositioning->setObjectName("qSbeginTestRobotPositioning");
    connect(qSbeginTestRobotPositioning, SIGNAL(entered()), this, SLOT(beginTestRobotPositioning()));

    QState *qStestRobotPositioning = new QState();
    machine.addState(qStestRobotPositioning);
    qStestRobotPositioning->setObjectName("qStestRobotPositioning");
    connect(qStestRobotPositioning, SIGNAL(entered()), this, SLOT(testRobotPositioning()));
    connect(qStestRobotPositioning, SIGNAL(finished()), this, SLOT(quitRobotPositioning()));

    //Transitions
    qSoff->addTransition(ui->pBtConnecter, SIGNAL(released()), qSwaitWebcamPositioning);

    qSwaitWebcamPositioning->addTransition(ui->pBtSuivant, SIGNAL(released()), qSbeginAreaDetection);
    qSwaitWebcamPositioning->addTransition(ui->pBtStop, SIGNAL(released()), qSoff);

    qSbeginAreaDetection->addTransition(this, SIGNAL(clickOnArea()), qSisAreaDetected);
    qSbeginAreaDetection->addTransition(ui->pBtPrevious, SIGNAL(released()), qSwaitWebcamPositioning);
    qSbeginAreaDetection->addTransition(ui->pBtStop, SIGNAL(released()), qSoff);

    qSisAreaDetected->addTransition(this, SIGNAL(clickOnArea()), qSisAreaDetected);
    qSisAreaDetected->addTransition(ui->pBtPrevious, SIGNAL(released()), qSwaitWebcamPositioning);
    qSisAreaDetected->addTransition(ui->pBtSuivant, SIGNAL(released()), qSbeginGreenGoalDetection);
    qSisAreaDetected->addTransition(ui->pBtStop, SIGNAL(released()), qSoff);

    qSbeginGreenGoalDetection->addTransition(this, SIGNAL(clickOnArea()), qSisGreenGoalDetected);
    qSbeginGreenGoalDetection->addTransition(ui->pBtPrevious, SIGNAL(released()), qSbeginAreaDetection);
    qSbeginGreenGoalDetection->addTransition(ui->pBtStop, SIGNAL(released()), qSoff);

    qSisGreenGoalDetected->addTransition(this, SIGNAL(clickOnArea()), qSisGreenGoalDetected);
    qSisGreenGoalDetected->addTransition(ui->pBtPrevious, SIGNAL(released()), qSbeginAreaDetection);
    qSisGreenGoalDetected->addTransition(ui->pBtSuivant, SIGNAL(released()), qSbeginRedGoalDetection);
    qSisGreenGoalDetected->addTransition(ui->pBtStop, SIGNAL(released()), qSoff);

    qSbeginRedGoalDetection->addTransition(this, SIGNAL(clickOnArea()), qSisRedDetected);
    qSbeginRedGoalDetection->addTransition(ui->pBtPrevious, SIGNAL(released()), qSbeginGreenGoalDetection);
    qSbeginRedGoalDetection->addTransition(ui->pBtStop, SIGNAL(released()), qSoff);

    qSisRedDetected->addTransition(this, SIGNAL(clickOnArea()), qSisRedDetected);
    qSisRedDetected->addTransition(ui->pBtPrevious, SIGNAL(released()), qSbeginGreenGoalDetection);
    qSisRedDetected->addTransition(ui->pBtSuivant, SIGNAL(released()), qSbeginTestRobotPositioning);
    qSisRedDetected->addTransition(ui->pBtStop, SIGNAL(released()), qSoff);

    qSbeginTestRobotPositioning->addTransition(ui->pBtPrevious, SIGNAL(released()), qSbeginRedGoalDetection);
    qSbeginTestRobotPositioning->addTransition(ui->pBtSuivant, SIGNAL(released()), qStestRobotPositioning);
    qSbeginTestRobotPositioning->addTransition(ui->pBtStop, SIGNAL(released()), qSoff);

    qStestRobotPositioning->addTransition(ui->pBtPrevious, SIGNAL(released()), qSbeginRedGoalDetection);
    qStestRobotPositioning->addTransition(ui->pBtStop, SIGNAL(released()), qSoff);

    /*QState *off = new QState();
    machine.addState(off);
    off->setObjectName("off");
    connect(off, SIGNAL(entered()), this, SLOT(onOff()));

    QState *positionCam = new QState();
    machine.addState(positionCam);
    positionCam->setObjectName("positionner webcam");
    connect(positionCam, SIGNAL(entered()), this, SLOT(onPosition()));

    QState *detecterTerrain = new QState();
    machine.addState(detecterTerrain);
    detecterTerrain->setObjectName("detecter terrain");
    connect(detecterTerrain, SIGNAL(entered()), this, SLOT(onDetecterTerrain()));

    QState *detecterButVert = new QState();
    machine.addState(detecterButVert);
    detecterButVert->setObjectName("detecter but vert");
    connect(detecterButVert, SIGNAL(entered()), this, SLOT(onDetecterButVert()));

    QState *detecterButRouge = new QState();
    machine.addState(detecterButRouge);
    detecterButRouge->setObjectName("detecter but rouge");
    connect(detecterButRouge, SIGNAL(entered()), this, SLOT(onDetecterButRouge()));

    QState *testerRobot = new QState();
    machine.addState(testerRobot);
    testerRobot->setObjectName("tester robot");
    connect(testerRobot, SIGNAL(entered()), this, SLOT(onTesterRobot()));*/

    /*off->addTransition(ui->pBtConnecter, SIGNAL(released()), positionCam);
    positionCam->addTransition(ui->pBtSuivant, SIGNAL(released()),detecterTerrain);
    positionCam->addTransition(ui->pBtStop, SIGNAL(released()), off);
    detecterTerrain->addTransition(ui->pBtSuivant, SIGNAL(released()),detecterButVert);
    detecterTerrain->addTransition(ui->pBtStop, SIGNAL(released()), off);
    detecterButVert->addTransition(ui->pBtSuivant, SIGNAL(released()),detecterButRouge);
    detecterButVert->addTransition(ui->pBtStop, SIGNAL(released()), off);
    detecterButRouge->addTransition(ui->pBtSuivant, SIGNAL(released()),testerRobot);
    detecterButRouge->addTransition(ui->pBtStop, SIGNAL(released()), off);
    testerRobot->addTransition(ui->pBtSuivant, SIGNAL(released()),off);
    testerRobot->addTransition(ui->pBtStop, SIGNAL(released()), off);*/

    machine.setInitialState(qSoff);
    machine.start();

}

TestWidget::~TestWidget()
{
    delete ui;
}

void TestWidget::off(){
    ui->lTitre->setText("Step 0 : Start webcam");
    ui->textEdit->setText("To begin the calibration process, click on the button <strong>Connect Webcam</strong>.");
    qDebug() << "State:off";
    emit(stop());
    qDebug() << "stop webcam";

}

void TestWidget::waitWebcamPositioning(){
    ui->lTitre->setText("Step 1 : Position webcam");
    ui->textEdit->setText("Place the webcam above the area so as to fully cover. When done, click <strong>Next</srtong>.");
    qDebug() << "State:waitWebcamPositioning";
    emit(connectionDemander());
    qDebug() << "start webcam";
}

void TestWidget::beginAreaDetection(){
    ui->lTitre->setText("Step 2 : Detection of the area");
    ui->textEdit->setText("Point the area with the mouse and click it.");
    qDebug() << "State:beginAreaDetection";

}

void TestWidget::isAreaDetected(){
    qDebug() << "State:isAreaDetected";
    ui->textEdit->setText("If the area is detected, click <strong>Next</strong>, else point and click again the area.");
}

void TestWidget::beginGreenGoalDetection(){
    ui->lTitre->setText("Step 3 : Detection of the green goal");
    ui->textEdit->setText("Point the <strong>green</strong> goal with the mouse and click it.");
    qDebug() << "State:beginGreenGoalDetection";

}

void TestWidget::isGreenAreaDetected(){
    qDebug() << "State:isGreenAreaDetected";
    ui->textEdit->setText("If the green goal is detected, click <strong>Next</strong>, else point and click again the area.");

}

void TestWidget::beginRedGoalDetection(){
    ui->lTitre->setText("Step 4 : Detection of the red goal");
    ui->textEdit->setText("Point the <strong>red</strong> goal with the mouse and click it.");
    qDebug() << "State:beginRedGoalDetection";

}

void TestWidget::isRedAreaDetected(){
    qDebug() << "State:isRedAreaDetected";
    ui->textEdit->setText("If the red goal is detected, click <strong>Next</strong>, else point and click again the area.");

}

void TestWidget::beginTestRobotPositioning(){
    ui->lTitre->setText("Step 5 : Test the positioning of the robot");
    ui->textEdit->setText("Place the robot in the area and click <strong>Next</strong>");
    qDebug() << "State:beginTestRobotPositioning";

}

void TestWidget::testRobotPositioning(){
    ui->pBtSuivant->setText("Finish");
    qDebug() << "State:testRobotPositioning";
    ui->textEdit->setText("If the robot is detected, click <strong>Finish</strong>... if not call for help !");
}

void TestWidget::quitRobotPositioning(){
    ui->pBtSuivant->setText("Next");
}

void TestWidget::onClickOnArea(QPoint point) {
    emit(clickOnArea());
    qDebug() << "TextWidget says yes to point x:" << point.x() << " y:" << point.y();

}
