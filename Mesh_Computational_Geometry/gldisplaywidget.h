#ifndef GLDISPLAYWIDGET_H
#define GLDISPLAYWIDGET_H

#include <QGLWidget>
#include <QtWidgets>
#include <QTimer>
#include "mesh.h"

#define TETRAEDRA 1
#define PYRAMID 2
#define QUEEN 3
#define BOX2D 4

class GLDisplayWidget : public QGLWidget
{
public:
    int test;
    explicit GLDisplayWidget(QWidget *parent = 0);

    void initializeGL();
    void paintGL(); // Display Gl
    void resizeGL(int width, int height);
    void selectPositionAndColorOn3DModel();
    QVector3D unproject(int X, int Y, float Depth);

    void draw(int figure);
    void switchDisplay();
    void launchCirculator();
    void delauniser();
    void addConvexHull();
    void loadBox2File();
    void switchSplit();
    void switchIncrLawson();

protected:
    // Mouse Management
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

private:

    QTimer _timer; // To update the scene
    float _X, _Y ,_Z; // Translation
    float _angle; // Rotation
    bool display;
    bool onBox2D;
    bool addedConvexHull;
    bool loadedBox2File;
    bool split;
    bool incrLawson;

    QPoint _lastPosMouse; // To keep the last position of the mouse

    Mesh _mesh; // The object to be displayed
};

#endif // GLDISPLAYWIDGET_H
