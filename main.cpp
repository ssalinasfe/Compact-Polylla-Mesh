//g++ -std=c++11 -O3 -DNDEBUG -I ~/include -Iinclude -L ~/lib  main.cpp -o main -lsdsl -ldivsufsort -ldivsufsort64 
//falla con 34
#include <algorithm>

#include <vector>
#include <string>



//#include <CompactPolylla.hpp>
//#include <Polylla_Aos.hpp>
//#include <CompactPolylla_no_matrix.hpp>
#include <polylla.hpp>


int main(int argc, char **argv) {

	std::string off_file = std::string(argv[1]);
	std::string output_off = std::string(argv[2]);
	Polylla mesh(off_file);
	mesh.print_OFF(output_off);
	std::cout<<"Size of uncompact mesh "<<sizeof(mesh)<<std::endl;
	//std::string node_file = std::string(argv[1]);
	//std::string graph_file = std::string(argv[2]);
	//std::string output_off = std::string(argv[3]);
	//std::string output_hedge_triangulation = std::string(argv[4]);
	//std::string output_hedge_polylla = std::string(argv[5]);
	//std::string output_new_node = std::string(argv[6]);
	//std::cout << "Read files: " << node_file<<" "<< graph_file<<" " << output_off<<" " << output_hedge_polylla<<" " << output_hedge_triangulation<<" " << output_new_node << std::endl;
	//Polylla mesh(node_file, graph_file);
	//std::cout<<"output off in "<<output_off<<std::endl;
	//mesh.print_OFF(output_off);
	////compressTriangulation(node_file, graph_file);
	//std::cout<<"output new nodes index in"<<output_new_node<<std::endl;
	//mesh.print_new_nodes(output_new_node);
	//std::cout<<"output hedge triangulation in "<<output_hedge_triangulation<<std::endl;
	//mesh.print_hedge_triangulation(output_hedge_triangulation);
	//std::cout<<"output hedge polylla in "<<output_hedge_polylla<<std::endl;
	//mesh.print_hedge(output_hedge_polylla);
	return 0;
}
