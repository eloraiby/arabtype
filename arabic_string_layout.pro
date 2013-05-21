#-------------------------------------------------
#
# Project created by QtCreator 2012-09-21T20:12:51
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = arabic_string_layout
TEMPLATE = app

QMAKE_CFLAGS_DEBUG += -std=c99
QMAKE_CFLAGS_RELEASE += -std=c99

CONFIG += link_pkgconfig

PKGCONFIG	+= freetype2

SOURCES += main.cpp\
	mainwindow.cpp \
    utf8.c \
    arabtype.c

HEADERS  += mainwindow.h \
    utf8.h \
    arabtype.h

FORMS    += mainwindow.ui
