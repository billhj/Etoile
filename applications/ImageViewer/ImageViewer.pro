#-------------------------------------------------
#
# Project created by QtCreator 2011-10-30T19:48:33
#
#-------------------------------------------------

QT       += core gui

TARGET = ImageViewer
TEMPLATE = app


SOURCES += main.cpp \
    ImageViewer.cpp \
    ../Common/IO/TGAImageLoader.cpp \
    ../Common/IO/ReadImage.cpp \
    ../Common/IO/QTImageLoader.cpp \
    ../Common/IO/PFMImageLoader.cpp \
    ../Common/IO/Image.cpp \
    ../Common/IO/HelpFunctions.cpp \
    ../Common/IO/File.cpp

HEADERS  += \
    ImageViewer.h \
    ../Common/IO/TGAImageLoader.h \
    ../Common/IO/ReadImage.h \
    ../Common/IO/QTImageLoader.h \
    ../Common/IO/PFMImageLoader.h \
    ../Common/IO/ImageLoader.h \
    ../Common/IO/Image.h \
    ../Common/IO/HelpFunctions.h \
    ../Common/IO/File.h
