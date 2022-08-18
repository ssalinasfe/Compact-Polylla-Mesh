#include <algorithm>
#include <vector>
#include <string>

//For testing memory
#include <malloc_count-0.7.1/malloc_count.h>
#include <malloc_count-0.7.1/stack_count.h>

#include <polylla.hpp>
#include <HalfEdge_AoS.hpp>

int main(int argc, char **argv) {

	std::string node_file, ele_file, neigh_file, graph_file, output_file;
    node_file = std::string(argv[1]);
    ele_file = std::string(argv[2]);
    neigh_file = std::string(argv[3]);
	output_file = std::string(argv[4]);

	//Generate triangulation halfedge
	Triangulation *plannar =  new Triangulation(node_file, ele_file, neigh_file);
	plannar->print_pg(output_file + ".pg");
	std::cout<<"Pg file generated in "<<plannar->get_triangulation_generation_time()<<" seconds"<<std::endl;
	std::cout<<"Memory used: "<<(long long)malloc_count_peak()<<" bytes"<<std::endl;

	//Generate Polylla
	Polylla *mesh = new Polylla(plannar);
	mesh->print_time(output_file + ".json");
	printf("our peak memory allocation: %lld\n", (long long)malloc_count_peak());	
	delete mesh;
	malloc_count_reset_peak();

	//Generate compact triangulation
	compactTriangulation *compact = new compactTriangulation(node_file, output_file + ".pg");

	//Generate Compact Polylla
	Polylla *mesh_compact = new Polylla(compact);
	mesh_compact->print_time(output_file + ".json");
	printf("our peak memory allocation: %lld\n", (long long)malloc_count_peak());	
	delete mesh_compact;

	return 0;
}
