#include "mesh.h"
#include <QDebug>
#include <iostream>

#include <QFile>
#include <QTextStream>
#include <QIODevice>
#include <QMessageBox>

#include <QVector3D>
#include <QVector2D>
#include <QQueue>

Mesh::Mesh(){}

Mesh::Mesh(QVector2D a, QVector2D b, QVector2D c, QVector2D d){
    /*
     *   b_______c
     *   |       |
     *   |       |
     *   a_______d
     *
     */
    // reverse pyramid

    vertices.clear();
    faces.clear();

    vertices.push_back(Vertex(a.x(),a.y(),0,   0)); //0
    vertices.push_back(Vertex(b.x(),b.y(),0,   0)); //1
    vertices.push_back(Vertex(c.x(),c.y(),0,   0)); //2
    vertices.push_back(Vertex(d.x(),d.y(),0,   1)); //3
    vertices.push_back(Vertex(0.,0.,-10,       5)); //4


    faces.push_back(Triangle(1,0,2, 1,3,2)); //0
    faces.push_back(Triangle(3,2,0, 0,5,4)); //1
    faces.push_back(Triangle(1,4,0, 5,0,3)); //2
    faces.push_back(Triangle(2,4,1, 2,0,4)); //3
    faces.push_back(Triangle(3,4,2, 3,1,5)); //4
    faces.push_back(Triangle(0,4,3, 4,1,2)); //5

}

Mesh::Mesh(const char* file_name){
    vertices.clear();
    faces.clear();

    QFile file(file_name);
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "Impossible d'ouvrir le fichier .off", file.errorString());
    }
    try {
        QTextStream in(&file);

        QStringList fields = in.readLine().split(" ");
        int verticeNumber = fields.at(0).toInt();
        int faceNumber = fields.at(1).toInt();
        // useless last number

        double x,y,z;
        for (int vertexNumber = 0; vertexNumber < verticeNumber; ++vertexNumber) {
            fields = in.readLine().split(" ");
            x = fields.at(0).toDouble();
            y = fields.at(1).toDouble();
            z = fields.at(2).toDouble();
            vertices.push_back(Vertex(x,y,z));
        }

        std::map<std::pair<int,int>,int> consMap;

        int v1,v2,v3;
        for (int triangleNumber = 0; triangleNumber < faceNumber; ++triangleNumber) {
            // first member useless as we only use triangles as faces here > always 3 vertices

            fields = in.readLine().split(" ");
            v1 = fields.at(1).toInt();
            v2 = fields.at(2).toInt();
            v3 = fields.at(3).toInt();

            faces.push_back(Triangle(v1,v2,v3)); // adding the triangle

            consMap[std::pair<int,int>(v1,v2)] = triangleNumber; // adding edges
            consMap[std::pair<int,int>(v2,v3)] = triangleNumber;
            consMap[std::pair<int,int>(v3,v1)] = triangleNumber;

            //checking adjacent triangles
            int adjacentTriangle;
            if (consMap.find(std::pair<int,int>(v2,v1)) != consMap.end()){
                adjacentTriangle = consMap.at(std::pair<int,int>(v2,v1));
                if(faces[adjacentTriangle].a()==v2){
                    faces[adjacentTriangle].t3(triangleNumber); // 1/3 adjacent triangle
                    vertices[faces[adjacentTriangle].c()].incident(adjacentTriangle); // opposite triangle

                    faces[triangleNumber].t3(adjacentTriangle);
                    vertices[faces[triangleNumber].c()].incident(triangleNumber);
                }else if(faces[adjacentTriangle].b()==v2){
                    faces[adjacentTriangle].t1(triangleNumber);
                    vertices[faces[adjacentTriangle].a()].incident(adjacentTriangle);

                    faces[triangleNumber].t3(adjacentTriangle);
                    vertices[faces[triangleNumber].c()].incident(triangleNumber);
                }else if(faces[adjacentTriangle].c()==v2){
                    faces[adjacentTriangle].t2(triangleNumber);
                    vertices[faces[adjacentTriangle].b()].incident(adjacentTriangle);

                    faces[triangleNumber].t3(adjacentTriangle);
                    vertices[faces[triangleNumber].c()].incident(triangleNumber);
                }
                consMap.erase(std::pair<int,int>(v2,v1));// drop the useless keys
                consMap.erase(std::pair<int,int>(v1,v2));
            }
            if (consMap.find(std::pair<int,int>(v3,v2))!= consMap.end()){
                adjacentTriangle = consMap.at(std::pair<int,int>(v3,v2));
                if(faces[adjacentTriangle].a()==v3){
                    faces[adjacentTriangle].t3(triangleNumber); // 1/3 adjacent triangle
                    vertices[faces[adjacentTriangle].c()].incident(adjacentTriangle); // opposite triangle

                    faces[triangleNumber].t1(adjacentTriangle);
                    vertices[faces[triangleNumber].a()].incident(triangleNumber);
                }else if(faces[adjacentTriangle].b()==v3){
                    faces[adjacentTriangle].t1(triangleNumber);
                    vertices[faces[adjacentTriangle].a()].incident(adjacentTriangle);

                    faces[triangleNumber].t1(adjacentTriangle);
                    vertices[faces[triangleNumber].a()].incident(triangleNumber);
                }else if(faces[adjacentTriangle].c()==v3){
                    faces[adjacentTriangle].t2(triangleNumber);
                    vertices[faces[adjacentTriangle].b()].incident(adjacentTriangle);

                    faces[triangleNumber].t1(adjacentTriangle);
                    vertices[faces[triangleNumber].a()].incident(triangleNumber);
                }
                consMap.erase(std::pair<int,int>(v3,v2));// drop the useless keys
                consMap.erase(std::pair<int,int>(v2,v3));
            }
            if (consMap.find(std::pair<int,int>(v1,v3)) != consMap.end()){
                adjacentTriangle = consMap.at(std::pair<int,int>(v1,v3));
                if(faces[adjacentTriangle].a()==v1){
                    faces[adjacentTriangle].t3(triangleNumber); // 1/3 adjacent triangle
                    vertices[faces[adjacentTriangle].c()].incident(adjacentTriangle); // opposite triangle

                    faces[triangleNumber].t2(adjacentTriangle);
                    vertices[faces[triangleNumber].b()].incident(triangleNumber);
                }else if(faces[adjacentTriangle].b()==v1){
                    faces[adjacentTriangle].t1(triangleNumber);
                    vertices[faces[adjacentTriangle].a()].incident(adjacentTriangle);

                    faces[triangleNumber].t2(adjacentTriangle);
                    vertices[faces[triangleNumber].b()].incident(triangleNumber);
                }else if(faces[adjacentTriangle].c()==v1){
                    faces[adjacentTriangle].t2(triangleNumber);
                    vertices[faces[adjacentTriangle].b()].incident(adjacentTriangle);

                    faces[triangleNumber].t2(adjacentTriangle);
                    vertices[faces[triangleNumber].b()].incident(triangleNumber);
                }
                consMap.erase(std::pair<int,int>(v3,v1));// drop the useless keys
                consMap.erase(std::pair<int,int>(v1,v3));
            }
        }
        // useless final value

        file.close();
    } catch (...) {
        qDebug() << "Erreur dans la construction du fichier off";
    }
}

// The following functions could be displaced into a module OpenGLDisplayMesh that would include Mesh
// Draw a vertex
void glVertexDraw(Vertex vertex) {
    glVertex3f(vertex.x(), vertex.y(), vertex.z());
}

//Example with a tetraedra

void Mesh::tetraedra(){
    vertices.clear();
    faces.clear();

    vertices.push_back(Vertex(-0.5,-0.5,-0.5,  2)); //0
    vertices.push_back(Vertex(0.5,-0.5,-0.5,   1)); // 1
    vertices.push_back(Vertex(0,0.5,-0.5,      1)); // 2
    vertices.push_back(Vertex(0,-0.5,0.5,      2)); // 3

    // sens trigo ok
    faces.push_back(Triangle(0,2,1,   1,3,2));
    faces.push_back(Triangle(1,2,3,   2,3,0));
    faces.push_back(Triangle(2,0,3,   3,1,0));
    faces.push_back(Triangle(0,1,3,   1,2,0));
}
void Mesh::pyramid(){
    vertices.clear();
    faces.clear();

    vertices.push_back(Vertex(-0.5,-0.5,-0.5,   0)); //0
    vertices.push_back(Vertex(-0.5,0.5,-0.5,    0)); //1
    vertices.push_back(Vertex(0.5,0.5,-0.5,     0)); //2
    vertices.push_back(Vertex(0.5,-0.5,-0.5,    1)); //3
    vertices.push_back(Vertex(0.,0.,0.5,        5)); //4


    faces.push_back(Triangle(1,2,0, 1,2,3)); //0
    faces.push_back(Triangle(3,0,2, 0,4,5)); //1
    faces.push_back(Triangle(1,0,4, 5,3,0)); //2
    faces.push_back(Triangle(2,1,4, 2,4,0)); //3
    faces.push_back(Triangle(3,2,4, 3,5,1)); //4
    faces.push_back(Triangle(0,3,4, 4,2,1)); //5
}
void Mesh::drawMesh() {

    for (int var = 0; var < faces.size(); ++var) {
        glColor3d(((double)var)/faces.size(),((double)faces.size()-var)/faces.size(),1-((double)var)/faces.size());
        glBegin(GL_TRIANGLES);
        glVertexDraw(vertices[faces[var].a()]);
        glVertexDraw(vertices[faces[var].b()]);
        glVertexDraw(vertices[faces[var].c()]);
        glEnd();
    }


}

void Mesh::testCirculator(){
    for( Iterator_on_vertices its = vertex_begin();
         its!=vertex_past_the_end();
         ++its){
        std::cout << "Vertex number: "<<*its << std::endl;
        Circulator_on_faces cf = incident_faces(*its);
        Circulator_on_faces cfbegin = incident_faces(*its) ;
        std::cout << "Initial face: "<<*cfbegin<< std::endl;
        int cmpt=1 ;
        for (++cf; cf!=cfbegin; ++cf){
            std::cout << "     Face number: "<<*cf<< std::endl;
            cmpt++ ;
        }
        std::cout << "valence of the vertex "<< cmpt << std::endl;

    }
    std::cout << " fini " << std::endl;
}

//Example with a wireframe tedraedra
void Mesh::drawMeshWireFrame() {

    for (int var = 0; var < faces.size(); ++var) {
        //        if (var < 1) glColor3d(1,0,0);
        //        else if (var < 2) glColor3d(0,1,0);
        //        else if (var < 3) glColor3d(0,0,1);
        //        else if (var < 4) glColor3d(.5,0,.5);
        //        else if (var < 5) glColor3d(0,.5,.5);
        //        else if (var < 6) glColor3d(.5,.5,0);
        //        else glColor3d(1,1,0);

        glColor3d((double)var/faces.size(),1-((double)var/faces.size()),(double)var/faces.size()/2);
        glBegin(GL_LINE_LOOP);
        glVertexDraw(vertices[faces[var].a()]);
        glVertexDraw(vertices[faces[var].b()]);
        glVertexDraw(vertices[faces[var].c()]);
        //        glVertexDraw(vertices[faces[var].a()]);
        glEnd();
    }


}

Iterator_on_faces Mesh::faces_begin(){
    return Iterator_on_faces(*this,0);
}

Iterator_on_vertices Mesh::vertex_begin(){
    return Iterator_on_vertices(*this,0);
}

Circulator_on_faces Mesh::incident_faces(index_vertex vi){
    return Circulator_on_faces(*this,vi);
}

Circulator_on_vertices Mesh::incident_vertices(index_vertex vi){
    return Circulator_on_vertices(*this,vi);
}

Iterator_on_faces Mesh::faces_past_the_end(){
    return Iterator_on_faces(*this,-1);
}

Iterator_on_vertices Mesh::vertex_past_the_end(){
    return Iterator_on_vertices(*this,-1);
}

bool Mesh::inTriangle(QVector2D v, Triangle t){

    Vertex a = vertices[t.a()];
    Vertex b = vertices[t.b()];
    Vertex c = vertices[t.c()];

    QVector2D ab(b.x()-a.x(),b.y()-a.y());
    QVector2D bc(c.x()-b.x(),c.y()-b.y());
    QVector2D ca(a.x()-c.x(),a.y()-c.y());
    ab.normalize();
    bc.normalize();
    ca.normalize();

    QVector2D av(v.x()-a.x(),v.y()-a.y());
    QVector2D bv(v.x()-b.x(),v.y()-b.y());
    QVector2D cv(v.x()-c.x(),v.y()-c.y());
    av.normalize();
    bv.normalize();
    cv.normalize();

    bool b1,b2,b3;

    // if one vertex is the infinite vertex, we just want to check one edge
    // for the insertion outside the convex hull
    if(a.z() < 0) // infinite a
        return (cv.x()*bc.y() - bc.x()*cv.y() <= 0);
    else if(b.z() <0) // infinite b
        return (av.x()*ca.y() - ca.x()*av.y() <= 0);
    else if(c.z() <0) // infinite c
        return (bv.x()*ab.y() - ab.x()*bv.y() <= 0);

    b1 = (bv.x()*ab.y() - ab.x()*bv.y() <= 0);
    b2 = (cv.x()*bc.y() - bc.x()*cv.y() <= 0);
    b3 = (av.x()*ca.y() - ca.x()*av.y() <= 0);

    if ( b1 && b2 && b3 )
        return true;
    return false;
}

void Mesh::splitTriangle(index_triangle ti, QVector2D v){

    /*
     *
     *  Triangle ti split:
     *        a
     *       /|\
     *      / | \
     *     /  |  \
     *    / ti|f+2\
     *   /   / \   \
     *  / / f+1   \ \
     * //___________\\
     *b               c
     *
     * f = faces.size()-1;
     *
     *
     */


    Triangle t = faces[ti];
    index_triangle t1 = t.t1();
    index_triangle t2 = t.t2();
    index_triangle t3 = t.t3();

    Vertex a = vertices[t.a()];
    Vertex b = vertices[t.b()];
    Vertex c = vertices[t.c()];

    //new data
    Vertex central = Vertex(v.x(),v.y(),0,ti);


    Triangle tn2(vertices.size(),t.b(),t.c(),t1,faces.size()+1,ti);
    Triangle tn3(t.a(),vertices.size(),t.c(),faces.size(),t2,ti);

    //correcting adjacents and opposites
    //t1
    vertices[faces[ti].c()].incident(t1);
    vertices[faces[ti].a()].incident(t2);
    vertices[faces[ti].b()].incident(t3);

    //vertex displacement
    faces[ti].c(vertices.size());
    //neighbors central
    faces[ti].t1(faces.size());
    faces[ti].t2(faces.size()+1);

    //neighbors t1
    if(faces[t1].a()==t.b()){
        faces[t1].t2(faces.size());
    }else if(faces[t1].b() == t.b()){
        faces[t1].t3(faces.size());
    }else{
        faces[t1].t1(faces.size());
    }

    //neighbors t2
    if(faces[t2].a()==t.a()){
        faces[t2].t3(faces.size()+1);
    }else if(faces[t2].b() == t.a()){
        faces[t2].t1(faces.size()+1);
    }else{
        faces[t2].t2(faces.size()+1);
    }

    //adding
    vertices.push_back(central);
    faces.push_back(tn2);
    faces.push_back(tn3);
}

void Mesh::flip(index_vertex v1, index_vertex v2, index_triangle t1, index_triangle t2){

    /*                      v2
     *       /\            /|\
     *      /  \          / | \
     *     / t1 \        /  |  \
     *    v1_____v2  => / t1|t2 \
     *     \     /      \   |   /
     *      \ t2/        \  |  /
     *       \ /          \ | /
     *                      v1
     */

    index_vertex opposite_t1, opposite_t2;
    if(faces[t1].a() == v1)
        opposite_t1 = faces[t1].c();
    else if(faces[t1].b() == v1)
        opposite_t1 = faces[t1].a();
    else
        opposite_t1 = faces[t1].b();

    if(faces[t2].a() == v1)
        opposite_t2 = faces[t2].b();
    else if(faces[t2].b() == v1)
        opposite_t2 = faces[t2].c();
    else
        opposite_t2 = faces[t2].a();


    //voisin bas gauche
    index_triangle bas_gauche;
    if(faces[t2].a() == v1)
        bas_gauche = faces[t2].t3();
    else if(faces[t2].b() == v1)
        bas_gauche = faces[t2].t1();
    else
        bas_gauche = faces[t2].t2();

    index_triangle haut_droite;
    if(faces[t1].a() == v1)
        haut_droite = faces[t1].t1();
    else if(faces[t1].b() == v1)
        haut_droite = faces[t1].t2();
    else
        haut_droite = faces[t1].t3();

    //FACE GAUCHE
    if(faces[t1].a() == v1){ // 3 sommets 3 voisins
        //3 sommets
        faces[t1].b(opposite_t2);
        //3 voisins
        faces[t1].t1(t2);
        faces[t1].t3(bas_gauche);
    }else if(faces[t1].b() == v1){
        //3 sommets
        faces[t1].c(opposite_t2);
        //3 voisins
        faces[t1].t2(t2);
        faces[t1].t1(bas_gauche);
    }else{
        //3 sommets
        faces[t1].a(opposite_t2);
        //3 voisins
        faces[t1].t3(t2);
        faces[t1].t2(bas_gauche);
    }

    //FACE DROITE
    if(faces[t2].a() == v1){
        //3 sommets
        faces[t2].a(opposite_t1);
        //3 voisins
        faces[t2].t2(haut_droite);
        faces[t2].t3(t1);
    }else if(faces[t2].b() == v1){
        //3 sommets
        faces[t2].b(opposite_t1);
        //3 voisins
        faces[t2].t3(haut_droite);
        faces[t2].t1(t1);
    }else{
        //3 sommets
        faces[t2].c(opposite_t1);
        //3 voisins
        faces[t2].t1(haut_droite);
        faces[t2].t2(t1);
    }

    //FACE BAS GAUCHE
    if(faces[bas_gauche].a() == v1){
        faces[bas_gauche].t2(t1);
    }else if(faces[bas_gauche].b() == v1){
        faces[bas_gauche].t3(t1);
    }else{
        faces[bas_gauche].t1(t1);
    }

    //FACE HAUT DROITE
    if(faces[haut_droite].a() == v2){
        faces[haut_droite].t2(t2);
    }else if(faces[haut_droite].b() == v2){
        faces[haut_droite].t3(t2);
    }else{
        faces[haut_droite].t1(t2);
    }

    // Incidents
    vertices[faces[t1].a()].incident(t1);
    vertices[faces[t1].b()].incident(t1);
    vertices[faces[t1].c()].incident(t1);
    vertices[v2].incident(t2);
}

//// if prefered to the other index_vertex-index_triangle
//void Mesh::flip(index_triangle t1, index_triangle t2){
//    index_triangle t1_a,t1_b,t1_c,t2_a,t2_b,t2_c;
//    t1_a = faces[t1].a();
//    t1_b = faces[t1].b();
//    t1_c = faces[t1].c();
//    t2_a = faces[t2].a();
//    t2_b = faces[t2].b();
//    t2_c = faces[t2].c();

//    // if oriented
//    if((t1_a == t2_a) || (t1_a == t2_b) || (t1_a == t2_c)){
//        // common t1_a
//        if((t1_b == t2_a) || (t1_b == t2_b) || (t1_b == t2_c)){
//            //common t1_b
//            flip(t1_a,t1_b,t1,t2);
//        }else{
//            flip(t1_c,t1_a,t1,t2);
//        }
//    }else if((t1_b == t2_a) || (t1_b == t2_b) || (t1_b == t2_c)){
//        // common t1_b
//        if((t1_c == t2_a) || (t1_c == t2_b) || (t1_c == t2_c)){
//            flip(t1_b,t1_c,t1,t2);
//        }else{
//            flip(t1_a,t1_b,t1,t2);
//        }
//    }else{
//        // common t1_c
//        if((t1_a == t2_a) || (t1_a == t2_b) || (t1_a == t2_c)){
//            flip(t1_c,t1_a,t1,t2);
//        }else{
//            flip(t1_b,t1_c,t1,t2);
//        }
//    }

//}

void Mesh::flip(index_vertex v1,index_triangle t1){
    index_triangle t2;
    index_vertex v2;
    if(v1 == faces[t1].a()){
        t2 = faces[t1].t3();
        v2 = faces[t1].b();
    }
    else if(v1 == faces[t1].b()){
        t2 = faces[t1].t1();
        v2 = faces[t1].c();
    }
    else{
        t2 = faces[t1].t2();
        v2 = faces[t1].a();
    }

    flip(v1,v2,t1,t2);
}

void Mesh::insertionOutsideConvexHull(QVector2D v){

    index_vertex infinite;
    // find infinite vertex
    for (int var = 0; var < vertices.size(); ++var) {
        if(vertices[var].z()<0){
            infinite = var;
            break;
        }
    }

    Circulator_on_faces circu_on_infinite = incident_faces(infinite);
    index_vertex a;

    /*       a
     *      /\     .V     X infinite
     *     /  \
     *    b___c
     */

    int goBack = -1; // operator --
    int shiftLeft = 0;
    bool firstFlip = true;



    while(shiftLeft != -1){
        if(inTriangle(v,faces[*circu_on_infinite])){

            // Get the first vertex
            if(faces[*circu_on_infinite].a() == infinite)
                a = faces[*circu_on_infinite].b();
            else if(faces[*circu_on_infinite].b() == infinite)
                a = faces[*circu_on_infinite].c();
            else
                a = faces[*circu_on_infinite].a();

            // Save initial face pos
            if(shiftLeft==0){
                goBack = faces.size();
            }

            //split
            if(firstFlip){
                splitTriangle(*circu_on_infinite,v); // en fonction de v
                vertices[vertices.size()-1].z(0);
                vertices[vertices.size()-1].x(v.x());
                vertices[vertices.size()-1].y(v.y());
                firstFlip = false;
            }else{
                // then flip to the left
                flip(infinite,*circu_on_infinite);
            }
            ++circu_on_infinite;
            shiftLeft++;
        }else{
            // stop conditions
            if(!firstFlip)
                shiftLeft = -1;
            else{
                ++circu_on_infinite;
                shiftLeft++;
            }
        }
    }


    if (goBack != -1){
        // potential missed edges on the right of the initial face
        Circulator_on_faces circu_on_infinite_less = incident_faces(infinite);
        while(*circu_on_infinite_less != goBack)
            --circu_on_infinite_less;
        --circu_on_infinite_less;
        // getting back to the right face -> depends on split code
        while(inTriangle(v,faces[*circu_on_infinite_less])){
            // Get first vertex
            if(faces[*circu_on_infinite].a() == infinite)
                a = faces[*circu_on_infinite_less].c();
            else if(faces[*circu_on_infinite_less].b() == infinite)
                a = faces[*circu_on_infinite_less].a();
            else
                a = faces[*circu_on_infinite_less].b();

            flip(a,*circu_on_infinite_less);
            --circu_on_infinite_less;
        }
    }
}

bool Mesh::isLocallyDelaunay(index_triangle t, index_vertex v){
    Vertex p = vertices[faces[t].a()];
    Vertex q = vertices[faces[t].b()];
    Vertex r = vertices[faces[t].c()];

    Vertex s = vertices[v];

    float a,b,c,d,e,f,g,h,i;
    a = q.x() - p.x();
    b = r.x() - p.x();
    c = s.x() - p.x();
    d = q.y() - p.y();
    e = r.y() - p.y();
    f = s.y() - p.y();
    g = a*a + d*d;
    h = b*b + e*e;
    i = c*c + f*f;

    //sign is s in the circumscribed circle pqr
    float sign = a*e*i + b*f*g + c*d*h - c*e*g - b*d*i - a*f*h;

    return (sign>=0);
}

index_vertex Mesh::getOpposite(index_triangle t1, index_triangle t2){
    // returns the opposite vertex of t1;
    if(faces[t2].a() == faces[t1].a() || faces[t2].a() == faces[t1].b() || faces[t2].a() == faces[t1].c()){
        if(faces[t2].b() == faces[t1].a() || faces[t2].b() == faces[t1].b() || faces[t2].b() == faces[t1].c()){
            return faces[t2].c();
        }else{
            return faces[t2].b();
        }

    }else{
        return faces[t2].a();
    }
}

index_vertex Mesh::getCommonIndex(index_triangle t1, index_triangle t2){
    // returns the lower left common vertex between upper t1 and lower t2
    if(faces[t1].a() == faces[t2].a() || faces[t1].a() == faces[t2].b() || faces[t1].a() == faces[t2].c()){
        if(faces[t1].b() == faces[t2].a() || faces[t1].b() == faces[t2].b() || faces[t1].b() == faces[t2].c()){
            return faces[t1].a();
        }else{
            return faces[t1].c();
        }
    }else{
        return faces[t1].b();
    }
}

void Mesh::lawson(){

    QQueue<QPair<index_triangle,index_triangle> > lawsonQueue;

    // init
    for (int var = 0; var < faces.size(); ++var) {
        // Testing edges, pushing the wrong ones
        // We can push 2 times the same edge > checked later

        // Getting rid of the infinite faces
        if ( vertices[faces[var].a()].z()<-1  || vertices[faces[var].b()].z()<-1 || vertices[faces[var].c()].z()<-1 )
            continue;

        if ( vertices[getOpposite(var,faces[var].t1())].z()>-1 )
            if ( !isLocallyDelaunay(var,getOpposite(var,faces[var].t1())) )
                lawsonQueue.append(QPair<index_triangle,index_triangle>(var,faces[var].t1()));

        if ( vertices[getOpposite(var,faces[var].t2())].z()>-1 )
            if ( !isLocallyDelaunay(var,getOpposite(var,faces[var].t2())) )
                lawsonQueue.append(QPair<index_triangle,index_triangle>(var,faces[var].t2()));

        if ( vertices[getOpposite(var,faces[var].t3())].z()>-1 )
            if ( !isLocallyDelaunay(var,getOpposite(var,faces[var].t3())) )
                lawsonQueue.append(QPair<index_triangle,index_triangle>(var,faces[var].t3()));


    }

    QPair<index_triangle,index_triangle> edge;
    index_vertex commonVertex;
//    std::cout << "lawson size queue " << lawsonQueue.size() << std::endl;


    while(!lawsonQueue.empty()){
        //Getting an edge
        edge = lawsonQueue.first();
        lawsonQueue.pop_front();

        // 1st check: are the 2 triangles still adjacent ? (In case of a previous flip)
        // If not, already in the queue in the right form thanks to the 4 new edges/step
        if ( faces[edge.first].t1() != edge.second && faces[edge.first].t2() != edge.second && faces[edge.first].t3() != edge.second)
            continue;
        // 2nd check: is the edge still non Locally Delaunay
        if ( !isLocallyDelaunay( edge.first, getOpposite(edge.first,edge.second)) ){

            commonVertex = getCommonIndex(edge.first, edge.second);
            flip( commonVertex , edge.first );

            // Testing the new 4 edges (6 tests : 4 success)

            // left triangle
            if(faces[edge.first].t1() !=  edge.second){
                if ( vertices[getOpposite(edge.first,faces[edge.first].t1())].z()>-1 ){
                    lawsonQueue.append(QPair<index_triangle,index_triangle>(edge.first,faces[edge.first].t1()));
                }
            }
            if(faces[edge.first].t2() !=  edge.second){
                if ( vertices[getOpposite(edge.first,faces[edge.first].t2())].z()>-1 ){
                    lawsonQueue.append(QPair<index_triangle,index_triangle>(edge.first,faces[edge.first].t2()));
                }
            }
            if(faces[edge.first].t3() !=  edge.second){
                if ( vertices[getOpposite(edge.first,faces[edge.first].t3())].z()>-1 ){
                    lawsonQueue.append(QPair<index_triangle,index_triangle>(edge.first,faces[edge.first].t3()));
                }
            }
            // right triangle
            if(faces[edge.second].t1() !=  edge.first){
                if ( vertices[getOpposite(edge.second,faces[edge.second].t1())].z()>-1 ){
                    lawsonQueue.append(QPair<index_triangle,index_triangle>(edge.second,faces[edge.second].t1()));
                }
            }
            if(faces[edge.second].t2() !=  edge.first){
                if ( vertices[getOpposite(edge.second,faces[edge.second].t2())].z()>-1 ){
                    lawsonQueue.append(QPair<index_triangle,index_triangle>(edge.second,faces[edge.second].t2()));
                }
            }
            if(faces[edge.second].t3() !=  edge.first){
                if ( vertices[getOpposite(edge.second,faces[edge.second].t3())].z()>-1 ){
                    lawsonQueue.append(QPair<index_triangle,index_triangle>(edge.second,faces[edge.second].t3()));
                }
            }
        }
    }
    qDebug() << "Lawson done";
}

void Mesh::incrementalLawson(QVector2D v){ // Only differs on the init of the queue and adding new edges

    QQueue<QPair<index_triangle,index_triangle> > lawsonQueue;
    QQueue<index_triangle> checkedTriangles;

    // v is the new vertex, first add the 3 edges
    index_triangle oldt1,oldt2,oldt3;

    /* Reminder: SPLIT: triangle var
     *     a
     *    / \      ->   var to the left  size-2 to the right
     *  b/___\c                size-1 bottom
     *
     */

    // find the correct triangle
    for (int var = 0; var < faces.size(); ++var) {
        if (inTriangle(v,faces[var])){

            // check the 3 edges
            oldt1 = faces[var].t1();
            oldt2 = faces[var].t2();
            oldt3 = faces[var].t3();

            splitTriangle(var,v);

            if ( vertices[getOpposite(var,oldt3)].z()>-1 ){
                lawsonQueue.append(QPair<index_triangle,index_triangle>(var,oldt3));
            }

            if ( vertices[getOpposite(faces.size()-2,oldt1)].z()>-1 ){
                lawsonQueue.append(QPair<index_triangle,index_triangle>(faces.size()-2,oldt1));
            }

            if ( vertices[getOpposite(faces.size()-1,oldt2)].z()>-1 ){
                lawsonQueue.append(QPair<index_triangle,index_triangle>(faces.size()-1,oldt2));
            }

            checkedTriangles.append(var);
            checkedTriangles.append(faces.size()-1);
            checkedTriangles.append(faces.size()-2);

            break;
        }
    }

    QPair<index_triangle,index_triangle> edge;
    index_vertex commonVertex;

    while(!lawsonQueue.empty()){

        edge = lawsonQueue.first();
        lawsonQueue.pop_front();

        // Same as regular lawson
        if ( faces[edge.first].t1() != edge.second && faces[edge.first].t2() != edge.second && faces[edge.first].t3() != edge.second)
            continue;
        if ( !isLocallyDelaunay( edge.first, getOpposite(edge.first,edge.second)) ){
            commonVertex = getCommonIndex(edge.first, edge.second);

            flip( commonVertex , edge.first );

            // Adding new checked Triangles
            checkedTriangles.append(edge.first);
            checkedTriangles.append(edge.second);

            // Adding the 2 new exterior edges: 6 tests: 2 success (checked list)

            if (!checkedTriangles.contains(faces[edge.first].t1())){
                if ( vertices[getOpposite(edge.first,faces[edge.first].t1())].z()>-1 ){
                    lawsonQueue.append(QPair<index_triangle,index_triangle>(edge.first,faces[edge.first].t1()));
                }
            }
            if (!checkedTriangles.contains(faces[edge.first].t2())){
                if ( vertices[getOpposite(edge.first,faces[edge.first].t2())].z()>-1 ){
                    lawsonQueue.append(QPair<index_triangle,index_triangle>(edge.first,faces[edge.first].t2()));
                }
            }
            if (!checkedTriangles.contains(faces[edge.first].t3())){
                if ( vertices[getOpposite(edge.first,faces[edge.first].t3())].z()>-1 ){
                    lawsonQueue.append(QPair<index_triangle,index_triangle>(edge.first,faces[edge.first].t3()));
                }
            }

            //right
            if (!checkedTriangles.contains(faces[edge.second].t1())){
                if ( vertices[getOpposite(edge.second,faces[edge.second].t1())].z()>-1 ){
                    lawsonQueue.append(QPair<index_triangle,index_triangle>(edge.second,faces[edge.second].t1()));
                }
            }
            if (!checkedTriangles.contains(faces[edge.second].t2())){
                if ( vertices[getOpposite(edge.second,faces[edge.second].t2())].z()>-1 ){
                    lawsonQueue.append(QPair<index_triangle,index_triangle>(edge.second,faces[edge.second].t2()));
                }
            }
            if (!checkedTriangles.contains(faces[edge.second].t3())){
                if ( vertices[getOpposite(edge.second,faces[edge.second].t3())].z()>-1 ){
                    lawsonQueue.append(QPair<index_triangle,index_triangle>(edge.second,faces[edge.second].t3()));
                }
            }

        }
    }
}

void Mesh::insertFromFile2D(const char * file_name, bool incrLawson){

    QFile file(file_name);
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "Impossible d'ouvrir le fichier .xyz", file.errorString());
    }
    try {
        QTextStream in(&file);

        double x,y,z;
        while(!in.atEnd()) {
            QStringList fields = in.readLine().split(" ");

            x = fields.at(0).toDouble();
            // En 2D
            y = fields.at(1).toDouble();
            //            z = fields.at(2).toDouble();
            for (int var = 0; var < faces.size(); ++var) {
                if (inTriangle(QVector2D(x,y),faces[var]) ){
                    if(incrLawson)
                        incrementalLawson(QVector2D(x,y));
                    else
                        splitTriangle(var,QVector2D(x,y));
                    var = faces.size();
                }
            }
        }

        file.close();
    } catch (...) {
        qDebug() << "Erreur dans la construction du fichier off";
    }
}
