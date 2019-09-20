#ifndef ITERATOR_ON_FACES_H
#define ITERATOR_ON_FACES_H

#include "mesh.h"

class Mesh;

class Iterator_on_faces
{
    Mesh& mesh;
    index_triangle currentFaceIndex;

public:
    Iterator_on_faces(Mesh & m): mesh(m), currentFaceIndex(-1){}
    Iterator_on_faces(Mesh & m, int cFI): mesh(m), currentFaceIndex(cFI){}
    index_triangle operator*(){ return currentFaceIndex; }
    void operator++();
    bool operator!=(Iterator_on_faces iof){
        if(currentFaceIndex != iof.currentFaceIndex) return true; else return false;
    }
    bool operator==(Iterator_on_faces iof){
        if(currentFaceIndex != iof.currentFaceIndex) return false; else return true;
    }
};

#endif // ITERATOR_ON_FACES_H
