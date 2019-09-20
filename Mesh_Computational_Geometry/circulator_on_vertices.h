#ifndef CIRCULATOR_ON_VERTICES_H
#define CIRCULATOR_ON_VERTICES_H

#include "mesh.h"

class Mesh;

class Circulator_on_vertices
{
    // we could use a circulator_on_faces
    Mesh& mesh;
    index_vertex vertexIndex;
    index_vertex currentVertexIndex; // not necessary
    index_triangle currentFaceIndex;

public:
    Circulator_on_vertices(Mesh & m,index_vertex vi);

    index_vertex operator*(){ return currentVertexIndex; }
    void operator++();
    bool operator!=(Circulator_on_vertices cov){
        if(currentVertexIndex != cov.currentVertexIndex) return true; else return false;
    }
    bool operator==(Circulator_on_vertices cov){
        if(currentVertexIndex != cov.currentVertexIndex) return false; else return true;
    }
};


#endif // CIRCULATOR_ON_VERTICES_H
