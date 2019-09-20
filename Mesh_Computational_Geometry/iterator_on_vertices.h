#ifndef ITERATOR_ON_VERTICES_H
#define ITERATOR_ON_VERTICES_H

#include "mesh.h"

class Mesh;

class Iterator_on_vertices
{
    Mesh& mesh;
    index_vertex currentVertexIndex;

public:
    Iterator_on_vertices(Mesh & m): mesh(m), currentVertexIndex(-1){}
    Iterator_on_vertices(Mesh & m, int cFI): mesh(m), currentVertexIndex(cFI){}
    index_vertex operator*(){ return currentVertexIndex; }
    void operator++();
    bool operator!=(Iterator_on_vertices iov){
        if(currentVertexIndex != iov.currentVertexIndex) return true; else return false;
    }
    bool operator==(Iterator_on_vertices iov){
        if(currentVertexIndex != iov.currentVertexIndex) return false; else return true;
    }
};

#endif // ITERATOR_ON_VERTICES_H
