//g++ -std=c++11 -O3 -DNDEBUG -I ~/include -Iinclude -L ~/lib  main.cpp -o main -lsdsl -ldivsufsort -ldivsufsort64 

#include <algorithm>

#include <vector>
#include <string>

#include <sdsl/pemb.hpp>
#include <complementary/Graph.hpp>
#include <complementary/utils.hpp>


//#include <compressHalfEdge.hpp>
#include <polylla.hpp>

//#include <compresshalfedge.hpp>
//#include <io_void.hpp>
//#include <delfin.hpp>
//

using namespace sdsl;
using namespace std;

int main(int argc, char **argv) {

	std::string node_file = std::string(argv[1]);
	std::string graph_file = std::string(argv[2]);
	std::string output_off = std::string(argv[3]);
	std::string output_hedge_triangulation = std::string(argv[4]);
	std::string output_hedge_polylla = std::string(argv[5]);
	std::string output_new_node = std::string(argv[6]);
	std::cout << "node_file: " << node_file<< graph_file << output_off << output_hedge_polylla << output_hedge_triangulation << output_new_node << std::endl;
	Polylla mesh(node_file, graph_file);
	cout<<"output off in "<<output_off<<endl;
	mesh.print_OFF(output_off);
	//compressTriangulation(node_file, graph_file);
	cout<<"output new nodes index in"<<output_new_node<<endl;
	mesh.print_new_nodes(output_new_node);
	cout<<"output hedge triangulation in "<<output_hedge_triangulation<<endl;
	mesh.print_hedge_triangulation(output_hedge_triangulation);
	cout<<"output hedge polylla in "<<output_hedge_polylla<<endl;
	mesh.print_hedge(output_hedge_polylla);
	return 0;
}
