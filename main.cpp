//g++ -std=c++11 -O3 -DNDEBUG -I ~/include -Iinclude -L ~/lib  main.cpp -o main -lsdsl -ldivsufsort -ldivsufsort64 

#include <vector>
#include <string>

#include <sdsl/pemb.hpp>
#include <complementary/Graph.hpp>
#include <complementary/utils.hpp>


#include <io_void.hpp>

using namespace sdsl;
using namespace std;

int main(int argc, char **argv) {
  // argv[1] is the path to a file with the planar embedding.
  // To check the input format, visit https://users.dcc.uchile.cl/~jfuentess/datasets/graphs.php  

  Graph g = read_graph_from_file(argv[1]);
  vector<double> points = read_nodes_from_file(string(argv[1]), g.vertices());

  for (int ptr = 0; ptr < g.vertices(); ptr++)
  {
    cout<<"vertex "<<ptr<<": ";
    std::cout<<points[2*ptr+0]<<" "<<points[2*ptr+1];
    std::cout<<std::endl;
  }
  cout<<endl;
  

  g.tree_indices(points);

  pemb<> pe(g);

  cout << "Size in bytes: " << size_in_bytes(pe) << " B" << endl;
  cout << "Degree of vertex 3: " << pe.degree(3) << endl;
  cout << "Number of vertices: " << pe.vertices() << endl;
  cout << "Number of edges: " << pe.edges() << endl;

  for (int ptr = 0; ptr < pe.vertices(); ptr++)
  {
    cout<<"vertex "<<ptr<<": ";
    std::cout<<points[2*ptr+0]<<" "<<points[2*ptr+1];
    std::cout<<std::endl;
  }
  cout<<endl;
  
  for (int ptr = 0; ptr < pe.vertices(); ptr++)
  {
    cout<<"Degree vertex "<<ptr<<": ";
    cout<<pe.degree(ptr); 
    std::cout<<std::endl;
  }
  cout<<endl;

  
  for (int ptr = 0; ptr < pe.edges(); ptr++)
  {
    cout<<"Face edge "<<ptr<<": ";
    pe.face(ptr);
    std::cout<<std::endl;
  }
  cout<<endl;

  for (int ptr = 0; ptr < pe.vertices(); ptr++)
  {
    cout<<"Neigh vertex "<<ptr<<": ";
    pe.list_neighbors(ptr);
    std::cout<<std::endl;
  }
  cout<<endl;

  return 0;
}
