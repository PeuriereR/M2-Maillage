#ifndef TRIANGLE
#define TRIANGLE

#include "vertex.h"

typedef int index_vertex;

class Triangle
{
    index_vertex _a;
    index_vertex _b;
    index_vertex _c;

    index_triangle _t1;
    index_triangle _t2;
    index_triangle _t3;

public:
    Triangle():_a(),_b(),_c() {}
    Triangle(index_vertex a_, index_vertex b_, index_vertex c_):_a(a_),_b(b_),_c(c_){}

    Triangle(index_vertex a_, index_vertex b_, index_vertex c_, index_triangle t1_, index_triangle t2_, index_triangle t3_):_a(a_),_b(b_),_c(c_), _t1(t1_),_t2(t2_),_t3(t3_) {}
    // get
    index_vertex a() const { return _a; }
    index_vertex b() const { return _b; }
    index_vertex c() const { return _c; }
    index_triangle t1() const { return _t1; }
    index_triangle t2() const { return _t2; }
    index_triangle t3() const { return _t3; }
    // set
    void t1(index_triangle i){ _t1=i; }
    void t2(index_triangle i){ _t2=i; }
    void t3(index_triangle i){ _t3=i; }
    void a(index_vertex i){ _a=i; }
    void b(index_vertex i){ _b=i; }
    void c(index_vertex i){ _c=i; }
};

#endif // TRIANGLE

