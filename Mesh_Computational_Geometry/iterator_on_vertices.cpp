#include "iterator_on_vertices.h"

void Iterator_on_vertices::operator++(){
    currentVertexIndex++;
    if((unsigned)currentVertexIndex == mesh.vertices.size()) currentVertexIndex = -1;
}
