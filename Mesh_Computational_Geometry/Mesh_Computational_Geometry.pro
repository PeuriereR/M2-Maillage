#-------------------------------------------------
#
# Project created by QtCreator 2018-08-28T10:55:17
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Mesh_Computational_Geometry
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    gldisplaywidget.cpp \
    mesh.cpp \
    iterator_on_faces.cpp \
    iterator_on_vertices.cpp \
    circulator_on_faces.cpp \
    circulator_on_vertices.cpp

HEADERS  += mainwindow.h \
    gldisplaywidget.h \
    mesh.h \
    vertex.h \
    triangle.h \
    iterator_on_faces.h \
    iterator_on_vertices.h \
    circulator_on_faces.h \
    circulator_on_vertices.h

FORMS    += mainwindow.ui

#---- Comment the following line on MacOS
LIBS = -lglut -lGLU
