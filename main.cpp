//g++ -std=c++11 -O3 -DNDEBUG -I ~/include -Iinclude -L ~/lib  main.cpp -o main -lsdsl -ldivsufsort -ldivsufsort64 

#include <algorithm>

#include <vector>
#include <string>

//#include <polylla.hpp>
#include <compresshalfedge.hpp>
//#include <io_void.hpp>
//#include <delfin.hpp>
//

using namespace sdsl;
using namespace std;

int main(int argc, char **argv) {

	std::string node_file = std::string(argv[1]);
	std::string graph_file = std::string(argv[2]);

//	polylla p(node_file, graph_file);

	Triangulation(node_file, graph_file);

	return 0;
}
