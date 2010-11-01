TEMPLATE = app
TARGET = headtracker
DEPENDPATH += .
CONFIG += console
RESOURCES += headtracker.qrc
win32 {
	LIBS += -L"C:\OpenCV\lib"
}
LIBS += -lcv -lcxcore -lhighgui

HEADERS += capturethread.h \
           filter.h \
           headstate.h \
           headtracker.h \
           imagebuffer.h \
           renderwidget.h \
           trackcontroller.h \
			processingthread.h \
    testwidget.h \
    methods.h
FORMS += headtracker.ui \
		settings.ui \
    testwidget.ui
SOURCES += capturethread.cpp \
           filter.cpp \
           headtracker.cpp \
           imagebuffer.cpp \
           main.cpp \
           renderwidget.cpp \
           trackcontroller.cpp \
			processingthread.cpp \
    testwidget.cpp \
    methods.cpp
