#include "iterator_on_faces.h"

void Iterator_on_faces::operator++(){
    currentFaceIndex++;
    if((unsigned)currentFaceIndex == mesh.faces.size()) currentFaceIndex = -1;
}
