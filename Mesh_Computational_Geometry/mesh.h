#ifndef MESH_H
#define MESH_H

#include <QGLWidget>
#include "triangle.h" // includes vertex and index_vertex index_triangle
#include "iterator_on_faces.h"
#include "iterator_on_vertices.h"
#include "circulator_on_faces.h"
#include "circulator_on_vertices.h"

class Iterator_on_faces;
class Iterator_on_vertices;
class Circulator_on_faces;
class Circulator_on_vertices;


//** TO MODIFY
class Mesh
{
public:
    Mesh();
    Mesh(const char* file_name);
    Mesh(QVector2D a, QVector2D b, QVector2D c, QVector2D d);

    // The two following functions could be displaced into a module OpenGLDisplayMesh that would include Mesh
    void drawMesh();
    void tetraedra();
    void pyramid();
    void drawMeshWireFrame();
    void testCirculator();
    bool inTriangle(QVector2D v, Triangle t);
    void splitTriangle(index_triangle ti, QVector2D v);

    Iterator_on_faces faces_begin();
    Iterator_on_vertices vertex_begin();

    Iterator_on_faces faces_past_the_end();
    Iterator_on_vertices vertex_past_the_end();

    Circulator_on_faces incident_faces(index_vertex v);
    Circulator_on_vertices incident_vertices(index_vertex v);

    //    void flip(index_triangle t1, index_triangle t2); //one or the other
    void flip(index_vertex v1,index_triangle t1);
    void flip(index_vertex v1, index_vertex v2, index_triangle t1, index_triangle t2);

    void insertionOutsideConvexHull(QVector2D v);

    bool isLocallyDelaunay(index_triangle t, index_vertex v);

    index_vertex getOpposite(index_triangle t1, index_triangle t2);
    index_vertex getCommonIndex(index_triangle t1, index_triangle t2);
    void lawson();
    void incrementalLawson(QVector2D v);

    void insertFromFile2D(const char * filename, bool incrLawson);
    QVector<Vertex> vertices;
    QVector<Triangle> faces;
};

#endif // MESH_H
