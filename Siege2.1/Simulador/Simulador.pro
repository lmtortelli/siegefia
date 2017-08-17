QT += core
QT -= gui

TARGET = Simulador
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

DEFINES += "NON_MATLAB_PARSING=1"
DEFINES += "MAX_EXT_API_CONNECTIONS=255"
DEFINES += "BUILD_SHARED_LIBS=OFF"

HEADERS +=  siege.h\
            tela.h\
            geral.h\
            comunicaTCP.h\
            comunicaUDP.h\
            remoteApi/extApi.h\
            remoteApi/extApiPlatform.h 

SOURCES += main.cpp\
           siege.cpp\
           tela.cpp\
           comunicaTCP.cpp\
           comunicaUDP.cpp\
           remoteApi/extApi.c\
           remoteApi/extApiPlatform.c

OTHER_FILES += conf

QMAKE_CXXFLAGS += -O3

