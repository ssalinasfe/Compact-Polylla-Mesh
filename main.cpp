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

	std::string node_file = std::string(argv[1]);
	std::string graph_file = std::string(argv[2]);
	std::string output = std::string(argv[3]);
	Polylla mesh(node_file, graph_file);
	mesh.print_OFF(output + ".off");
	mesh.print_time(output + ".json");

	return 0;
}
