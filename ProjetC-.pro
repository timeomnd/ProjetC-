######################################################################
# Automatically generated by qmake (3.1) Tue Apr 15 18:41:11 2025
######################################################################

TEMPLATE = app
TARGET = ProjetC-
INCLUDEPATH += .

# You can make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# Please consult the documentation of the deprecated API in order to know
# how to port your code away from it.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Input
HEADERS += Boss.hpp Item.hpp MainWindow.hpp Map.hpp MyScene.hpp Player.hpp
SOURCES += Boss.cpp \
           main.cpp \
           MainWindow.cpp \
           MyScene.cpp \
           Player.cpp \
           qrc_ressource.cpp
    
QT += core widgets
QT += multimedia
