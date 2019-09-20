#include "circulator_on_faces.h"


Circulator_on_faces::Circulator_on_faces(Mesh & m,index_vertex vi): mesh(m), vertexIndex(vi){
    currentFaceIndex = mesh.vertices[vertexIndex].incident();
}

void Circulator_on_faces::operator++(){
    if(mesh.faces[currentFaceIndex].a() == vertexIndex){
        currentFaceIndex = mesh.faces[currentFaceIndex].t2();
    }else if(mesh.faces[currentFaceIndex].b() == vertexIndex){
        currentFaceIndex = mesh.faces[currentFaceIndex].t3();
    }else{
        currentFaceIndex = mesh.faces[currentFaceIndex].t1();
    }
}
void Circulator_on_faces::operator--(){
    if(mesh.faces[currentFaceIndex].a() == vertexIndex){
        currentFaceIndex = mesh.faces[currentFaceIndex].t3();
    }else if(mesh.faces[currentFaceIndex].b() == vertexIndex){
        currentFaceIndex = mesh.faces[currentFaceIndex].t1();
    }else{
        currentFaceIndex = mesh.faces[currentFaceIndex].t2();
    }
}
