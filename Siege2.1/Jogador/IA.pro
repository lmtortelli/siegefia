TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle


QT += core
QT -= gui

SOURCES += main.cpp \
           tela.cpp \
           comunicaTCP.cpp \
           comunicaUDP.cpp \
           siege.cpp

HEADERS += geral.h \
           tela.h \
           comunicaTCP.h \
           comunicaUDP.h \
           siege.h

OTHER_FILES += conf

QMAKE_CXXFLAGS += -O3

DISTFILES += \
    conf_vermelho\
    conf_amarelo
