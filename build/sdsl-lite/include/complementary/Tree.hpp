#ifndef TREE_COMP_HPP
#define TREE_COMP_HPP

#include <iostream>
#include <stack>

#include "Vertex.hpp"
#include "Edge.hpp"

using namespace std;

class Tree {
private:
  Vertex *V;      // Array of nodes of the tree
  Edge *E;        // Array of edges of the tree. It is the concatenation of the
	          // adjacency lists of all nodes 
  unsigned int n; // Number of nodes in the tree
                  // The number of edges is n-1
public:
  Tree() {
    this->n = 0;
  }
  
  Tree(unsigned int n) {
    this->n = n;
    this->V = new Vertex[n];
    this->E = new Edge[2*(n-1)];
  }
  
  unsigned int nodes() {
    return n;
  }

  unsigned int edges() {
    return n-1;
  }

  // E[i].src = src and E[i].tgt = tgt
  void setEdge(int i, unsigned int src, unsigned int tgt) {
    this->E[i].setSrc(src);
    this->E[i].setTgt(tgt);
    this->E[i].setCmp(-1);
  }

  // V[i].first = first and V[i].last = last
  void setNode(int i, unsigned int first, unsigned int last) {
    this->V[i].setFirst(first);
    this->V[i].setLast(last);
  }

  // V[i].first = first
  void setNodeFirst(int i, unsigned int first) {
    this->V[i].setFirst(first);
  }

  // V[i].last = last
  void setNodeLast(int i, unsigned int last) {
    this->V[i].setLast(last);
  }

  void setEdgeCmp(int i, int c) {
    this->E[i].setCmp(c);
  }

  unsigned int getEdgeSrc(int i) {
    return this->E[i].getSrc();
  }

  unsigned int getEdgeTgt(int i) {
    return this->E[i].getTgt();
  }

  unsigned int getEdgeCmp(int i) {
    return this->E[i].getCmp();
  }

  unsigned int getNodeFirst(int i) {
    return this->V[i].getFirst();
  }

  unsigned int getNodeLast(int i) {
    return this->V[i].getLast();
  }

  Vertex getNode(int i) {
    return this->V[i];
  }

  Edge getEdge(int i) {
    return this->E[i];
  }

  int tree_cycle() {
    unsigned int n = this->nodes();
    stack <unsigned int> s;

    char *visited = new char[n](); // TODO: Change data type to bool
    char *edges = new char[2*(n-1)]();
    int par, curr = -1;
    unsigned int edge;
    int first = 1;

    while(!s.empty() || first) {
      if(first) { // Root
	par = -1;
	curr = 0;
	first = 0;
      }
      else {
	edge = s.top();s.pop();
	par = this->E[edge].getSrc(); // parent
	curr = this->E[edge].getTgt(); // current
      }
    
      visited[curr] = 1;

      for(unsigned int i = this->V[curr].getFirst(); i <=
	    this->V[curr].getLast(); i++) {
	edges[i] = 1;
	if(!visited[this->E[i].getTgt()])
	  s.push(i);      
	else if(par != (int)this->E[i].getTgt() && par != -1) {
	  cout << "\tThe tree has cycles!" << endl;
	  return 1;
	}
      }
    }

    unsigned int unvisited = 0;
    for(unsigned int i = 0; i < n; i++)
      if(visited[i] == 0) { // There are unvisited nodes
	unvisited++;
      }
    
    if(unvisited > 0) {
      cout << "\tNo connected tree (" << unvisited << " unvisited nodes)" << endl;
      return 1;
    }

    unvisited = 0;
    for(unsigned int i = 0; i < 2*(n-1); i++)
      if(edges[i] == 0) { // There are unvisited edges
	unvisited++;
      }
    if(unvisited > 0) {
      cout << "\tNo connected graph (" << unvisited << " unvisited edges)" << endl;
      return 1;
    }
    
    return 0;
  }
  
};

#endif
