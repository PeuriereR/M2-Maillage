#include "gldisplaywidget.h"
#ifdef __APPLE__
#include <glu.h>
#else
#include <GL/glu.h>
#endif

#include <iostream>
#include "QDebug"
#include <QVector2D>

GLDisplayWidget::GLDisplayWidget(QWidget *parent) : QGLWidget(parent)
{
    // Update the scene
    connect( &_timer, SIGNAL(timeout()), this, SLOT(updateGL()));
    _timer.start(16);
    _X = 0;
    _Y = 0;
    _Z = 0;
    test=0;
    display = false;
    onBox2D = true;
    loadedBox2File = false;
    addedConvexHull = false;
    split = false;
    incrLawson = false;
}

void GLDisplayWidget::initializeGL()
{
    // background color
    glClearColor(0.2, 0.2, 0.2, 1);

    // Shader
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);

    draw(BOX2D);
}

void GLDisplayWidget::paintGL(){

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Center the camera
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0,0,5,  0,0,-50,   0,1,0);

    // Translation
    glTranslated(_X, _Y, _Z);

    // Rotation
    glRotatef(_angle, 1.0f, 1.0f, 0.0f);

    // Color for your mesh
    glColor3f(0, 1 ,0);

    // example with a tetraedre
    if(display)
        _mesh.drawMeshWireFrame();
    else
        _mesh.drawMesh();

}

void GLDisplayWidget::resizeGL(int width, int height){

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)width/(GLfloat)height, 0.1f, 100.0f);

    updateGL();
}

// - - - - - - - - - - - - Mouse Management  - - - - - - - - - - - - - - - -
// When you click, the position of your mouse is saved
void GLDisplayWidget::mousePressEvent(QMouseEvent *event)
{
    if( event != NULL ){
        _lastPosMouse = event->pos();
        selectPositionAndColorOn3DModel();
    }
}

// Mouse movement management
void GLDisplayWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - _lastPosMouse.x();
    // int dy = event->y() - lastPosMouse.y();

    if( event != NULL )
    {
        _angle += dx;
        _lastPosMouse = event->pos();

        updateGL();
    }
}

// Mouse Management for the zoom
void GLDisplayWidget::wheelEvent(QWheelEvent *event) {
    QPoint numDegrees = event->angleDelta();
    double stepZoom = 0.25;
    if (!numDegrees.isNull())
    {
        _Z = (numDegrees.x() > 0 || numDegrees.y() > 0) ? _Z + stepZoom : _Z - stepZoom;
    }
}


void GLDisplayWidget::draw(int figure){
    if(figure==TETRAEDRA){
        onBox2D = false;
        _mesh.tetraedra();
    }
    else if(figure==PYRAMID){
        onBox2D = false;
        _mesh.pyramid();
    }
    else if(figure==QUEEN){
        onBox2D = false;
        _mesh = Mesh("../Object/queen.off");
    }
    else if(figure==BOX2D){
        onBox2D = true;
        addedConvexHull = false;
        loadedBox2File = false;
        split = false;
        incrLawson = false;
        qDebug() << "Split off";
        qDebug() << "Lawson Incremental off";
        _mesh = Mesh(QVector2D(-1,-1),QVector2D(-1,1),QVector2D(1,1),QVector2D(1,-1));
    }
}

void GLDisplayWidget::switchDisplay(){
    display = !display;
}

void GLDisplayWidget::launchCirculator(){
    _mesh.testCirculator();
}

void GLDisplayWidget::delauniser(){
    if(onBox2D){
        _mesh.lawson();
    }else{
        qDebug() << "Impossible de rendre un maillage autre que Box2D Delaunay";
    }
}

void GLDisplayWidget::selectPositionAndColorOn3DModel() {
    if(onBox2D && split){
        int xMouse = _lastPosMouse.x();
        int yMouse = height() - 1 - _lastPosMouse.y();

        float OnObjectZ;

        unsigned char color[4];
        color[3] = 0;
        glReadPixels(xMouse, yMouse, 1, 1, GL_RGB,GL_UNSIGNED_BYTE, &color[0]);

        if(color[0] != 0 || color[1] != 0 || color[2] != 0)
        {
            glReadPixels(xMouse,yMouse,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&OnObjectZ);
            QVector3D posClickSelected = unproject(xMouse,yMouse,OnObjectZ); // Pos of click
            posClickSelected.setZ(0);
//            qDebug() << posClickSelected;;
//            qDebug() << color[0]<< ", " << color[1] << ", "<< color[2]; // Color of face

            if (incrLawson)
                _mesh.incrementalLawson(QVector2D(posClickSelected));
            else{
                for (int var = 0; var < _mesh.faces.size(); ++var) {
                    if( _mesh.inTriangle(QVector2D(posClickSelected),_mesh.faces[var]) ){
                        _mesh.splitTriangle(var,QVector2D(posClickSelected));
                        return;
                    }
                }
            }
        }
    }
}

// UnProjection: **Model** => GL_MODELVIEW_MATRIX => **View** => GL_PROJECTION_MATRIX ==> **Projection**
// Return the position on the face
QVector3D GLDisplayWidget::unproject(int X, int Y, float Depth)
{
    std::vector<GLdouble> ModelView(16),Projection(16);
    glGetDoublev(GL_MODELVIEW_MATRIX,&ModelView[0]);
    glGetDoublev(GL_PROJECTION_MATRIX,&Projection[0]);
    std::vector<GLint> Viewport(4);
    glGetIntegerv(GL_VIEWPORT,&Viewport[0]);
    GLdouble x,y,z;
    gluUnProject(X,Y,Depth,&ModelView[0],&Projection[0],&Viewport[0],&x,&y,&z);
    return QVector3D(x,y,z);
}

void GLDisplayWidget::addConvexHull(){
    if(onBox2D){
        if(addedConvexHull){
            qDebug() << "Impossible d'etendre deux fois l'enveloppe convexe";
            return;
        }
        _mesh.insertionOutsideConvexHull(QVector2D(-0.8,-2));
        _mesh.insertionOutsideConvexHull(QVector2D(0,-1.7));
        _mesh.insertionOutsideConvexHull(QVector2D(-2,0));
        _mesh.insertionOutsideConvexHull(QVector2D(-1.7,0.7));
        _mesh.insertionOutsideConvexHull(QVector2D(-1.5,-1.5));
        _mesh.insertionOutsideConvexHull(QVector2D(-1.8,-0.8));
        addedConvexHull = true;
    }else{
        qDebug() << "Impossible d'étendre l'enveloppe convexe d'un autre mesh que BOX2D";
    }
}

void GLDisplayWidget::loadBox2File(){
    if(onBox2D){
        if(loadedBox2File){
            qDebug() << "Impossible de charger 2 fois les memes points";
            return;
        }
        _mesh.insertFromFile2D("../Object/test.xy",incrLawson);
        loadedBox2File = true;
    }else{
        qDebug() << "Impossible de rajouter des points dans un autre mesh que BOX2D";
    }
}

void GLDisplayWidget::switchSplit(){
    if(onBox2D){
        split = !split;
        if (split)
            qDebug() << "Split on: Attention, cliquer en dehors des triangles cassera le maillage";
        else
            qDebug() << "Split off";
    }else{
        qDebug() << "Impossible de split les triangles dans un autre mesh que BOX2D";
    }
}
void GLDisplayWidget::switchIncrLawson(){
    if(onBox2D){
        incrLawson = !incrLawson;
        if (incrLawson)
            qDebug() << "Lawson Incremental on (utilisable sur split et load .xy file)";
        else
            qDebug() << "Lawson Incremental off";
    }else{
        qDebug() << "Impossible d'utiliser lawson sur un mesh autre que BOX2D";
    }
}
