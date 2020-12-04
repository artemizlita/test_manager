#-------------------------------------------------
#
# Project created by QtCreator 2019-07-27T19:36:36
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test_manager_ver3
TEMPLATE = app


SOURCES += main.cpp\
        main_window.cpp \
    edit_window.cpp \
    result_window.cpp \
    load_window.cpp \
    options_window.cpp

HEADERS  += main_window.h \
    edit_window.h \
    result_window.h \
    load_window.h \
    options_window.h

FORMS    += main_window.ui \
    edit_window.ui \
    result_window.ui \
    load_window.ui \
    options_window.ui
