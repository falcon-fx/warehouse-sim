QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  tst_modeltest.cpp \
    ../source/model/model.cpp \
    ../source/model/pod.cpp \
    ../source/model/robot.cpp \
    ../source/model/wtile.cpp

HEADERS += \
    ../source/model/model.h \
    ../source/model/pod.h \
    ../source/model/robot.h \
    ../source/model/wtile.h
