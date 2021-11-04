#ifndef VERTEX_COMP_HPP
#define VERTEX_COMP_HPP

// Used to represent graphs and trees
class Vertex {
  
private:
  unsigned int first; // Position of the first incident edge of a vertex in E
  unsigned int last; // Position of the last incident edge of a vertex in E

public:
  Vertex(){}
  
  ~Vertex(){}

  unsigned int getFirst() {
    return this->first;
  }

  unsigned int getLast() {
    return this->last;
  }

  void setFirst(unsigned int f) {
    this->first = f;
  }

  void setLast(unsigned int l) {
    this->last = l;
  }
};

#endif
