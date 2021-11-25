//g++ -std=c++11 -O3 -DNDEBUG -I ~/include -Iinclude -L ~/lib  main.cpp -o main -lsdsl -ldivsufsort -ldivsufsort64 

#include <algorithm>

#include <vector>
#include <string>

#include <sdsl/pemb.hpp>
#include <complementary/Graph.hpp>
#include <complementary/utils.hpp>


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

	Polylla mesh(node_file, graph_file);
	//std::string output = node_file.substr(0, node_file.find(".")) + ".off";
	//cout<<"output in "<<output<<endl;
	//mesh.print_OFF(output);
	//compressTriangulation(node_file, graph_file);

	return 0;
}
