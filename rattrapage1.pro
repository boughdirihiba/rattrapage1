QT += core widgets sql charts printsupport network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = rattrapage1
TEMPLATE = app
SOURCES += main.cpp \
           mainwindow.cpp \
           connexion.cpp \
           stagiaire.cpp
HEADERS += mainwindow.h \
           connexion.h \
           stagiaire.h
FORMS += mainwindow.ui

RESOURCES +=

