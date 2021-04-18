QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  tst_modeltest.cpp \
    ../../source/warehouseSim/model/model.cpp \
    ../../source/warehouseSim/model/pod.cpp \
    ../../source/warehouseSim/model/robot.cpp \
    ../../source/warehouseSim/model/wtile.cpp

HEADERS += \
    ../../source/warehouseSim/model/model.h \
    ../../source/warehouseSim/model/pod.h \
    ../../source/warehouseSim/model/robot.h \
    ../../source/warehouseSim/model/wtile.h
