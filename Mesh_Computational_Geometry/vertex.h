#ifndef VERTEX_H
#define VERTEX_H

typedef int index_triangle;

class Vertex
{
    // Class to be enriched, otherwise it would have been simply named as 3DPoint!!
    double _x;
    double _y;
    double _z;

    index_triangle _incident;

public:
    Vertex():_x(),_y(),_z() {}
    Vertex(float x_, float y_, float z_):_x(x_),_y(y_),_z(z_) {}
    Vertex(float x_, float y_, float z_, index_triangle incident_):_x(x_),_y(y_),_z(z_),_incident(incident_) {}
    // get
    double x() const { return _x; }
    double y() const { return _y; }
    double z() const { return _z; }
    //set
    void incident(index_triangle i) { _incident = i; }
    index_triangle incident() const { return _incident; }
    void x(double v){ _x=v; }
    void y(double v){ _y=v; }
    void z(double v){ _z=v; }
};

#endif // VERTEX_H
