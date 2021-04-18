QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  tst_modeltest.cpp \
    ../../source/warehouseSim/model.cpp \
    ../../source/warehouseSim/pod.cpp \
    ../../source/warehouseSim/robot.cpp \
    ../../source/warehouseSim/wtile.cpp

HEADERS += \
    ../../source/warehouseSim/model.h \
    ../../source/warehouseSim/pod.h \
    ../../source/warehouseSim/robot.h \
    ../../source/warehouseSim/wtile.h
