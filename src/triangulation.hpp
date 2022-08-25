#include <vector>
#include <string>

#ifndef TRIANGULATION_HPP
#define TRIANGULATION_HPP

class Mesh {
  public:

    virtual int faces() = 0;
    virtual int halfEdges() = 0;
    virtual int vertices() = 0;

    virtual double get_triangulation_generation_time() = 0;
    virtual double distance(int e) = 0;
    //virtual int CCW_edge_to_vertex(int e) = 0;
    virtual int CW_edge_to_vertex(int e) = 0;
    virtual std::vector<int> get_Triangles() = 0;
    virtual double get_PointX(int i) = 0;
    virtual double get_PointY(int i) = 0;
    virtual int next(int e) = 0;
    virtual int origin(int e) = 0;
    virtual int target(int e) = 0;
    virtual int twin(int e) = 0;
    virtual int prev(int e) = 0;
    virtual int edge_of_vertex(int v) = 0;
    virtual bool is_border_face(int e)  = 0;
    virtual bool is_interior_face(int e) = 0;
    //virtual bool is_border_vertex(int v) = 0;
    virtual int degree(int v) = 0;
    
    virtual int incident_halfedge(int f)= 0;
    //Destructor
    virtual ~Mesh(){ };

};

#endif