#include "circulator_on_vertices.h"


Circulator_on_vertices::Circulator_on_vertices(Mesh & m, index_vertex vi): mesh(m), vertexIndex(vi){
    currentFaceIndex = mesh.vertices[vertexIndex].incident();

    if(mesh.faces[currentFaceIndex].a() == vertexIndex){
        currentVertexIndex = mesh.faces[currentFaceIndex].b();
    }else if(mesh.faces[currentFaceIndex].b() == vertexIndex){
        currentVertexIndex = mesh.faces[currentFaceIndex].c();
    }else if(mesh.faces[currentFaceIndex].c() == vertexIndex){
        currentVertexIndex = mesh.faces[currentFaceIndex].a();
    }
}

void Circulator_on_vertices::operator ++(){
    if(mesh.faces[currentFaceIndex].a() == currentVertexIndex){
        currentFaceIndex = mesh.faces[currentFaceIndex].t1();
    }else if(mesh.faces[currentFaceIndex].b() == currentVertexIndex){
        currentFaceIndex = mesh.faces[currentFaceIndex].t2();
    }else{
        currentFaceIndex = mesh.faces[currentFaceIndex].t3();
    }

    if(mesh.faces[currentFaceIndex].a() == vertexIndex){
        currentVertexIndex = mesh.faces[currentFaceIndex].b();
    }else if(mesh.faces[currentFaceIndex].b() == vertexIndex){
        currentVertexIndex = mesh.faces[currentFaceIndex].c();
    }else if(mesh.faces[currentFaceIndex].c() == vertexIndex){
        currentVertexIndex = mesh.faces[currentFaceIndex].a();
    }
}
