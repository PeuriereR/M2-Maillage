#ifndef CIRCULATOR_ON_FACES_H
#define CIRCULATOR_ON_FACES_H

#include "mesh.h"

class Mesh;

class Circulator_on_faces
{
    Mesh& mesh;
    index_vertex vertexIndex;
    index_triangle currentFaceIndex;

public:
    Circulator_on_faces(Mesh & m,index_vertex vi);

    index_triangle operator*(){ return currentFaceIndex; }
    void operator++();
    void operator--();
    bool operator!=(Circulator_on_faces cof){
        if(currentFaceIndex != cof.currentFaceIndex) return true; else return false;
    }
    bool operator==(Circulator_on_faces cof){
        if(currentFaceIndex != cof.currentFaceIndex) return false; else return true;
    }
};


#endif // CIRCULATOR_ON_FACES_H
