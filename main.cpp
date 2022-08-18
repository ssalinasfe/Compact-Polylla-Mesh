#include <algorithm>
#include <vector>
#include <string>

//For testing memory
#include <malloc_count-0.7.1/malloc_count.h>
#include <malloc_count-0.7.1/stack_count.h>
//#include <malloc_count-0.7.1/memprofile.h>


#include <polylla.hpp>
#include <HalfEdge_AoS.hpp>

int main(int argc, char **argv){
	//Declaration of variables
	std::string node_file, ele_file, neigh_file, graph_file, output_file;
	long long mem_triangulation, mem_polylla, mem_compact_triangulation, mem_compact_polylla;

	//Read input names
    node_file = std::string(argv[1]);
    ele_file = std::string(argv[2]);
    neigh_file = std::string(argv[3]);
	output_file = std::string(argv[4]);

	{
		//Generate triangulation halfedge
		Triangulation *plannar =  new Triangulation(node_file, ele_file, neigh_file);
		plannar->print_pg(output_file + ".pg");
		std::cout<<"Triangulation generated in "<<plannar->get_triangulation_generation_time()<<" seconds"<<std::endl;
		std::cout<<"Memory used to generate triangulation "<<(long long)malloc_count_peak()<<" bytes"<<std::endl;
		std::cout<<"Memory of triangulation "<<(long long)malloc_count_current()<<" bytes"<<std::endl;	mem_compact_triangulation = (long long) malloc_count_peak();
		mem_triangulation = (long long) malloc_count_peak();

		//reset memory counter
		malloc_count_reset_peak();

		//Generate Polylla
		Polylla *mesh = new Polylla(plannar);
		mem_polylla = (long long) malloc_count_peak();
		//mesh->print_time(output_file + "_AoS.json");

		std::cout<<"Memory used to generate Polylla "<<(long long)malloc_count_peak()<<" bytes"<<std::endl;
		std::cout<<"Memory of Polylla "<<(long long)malloc_count_current()<<" bytes"<<std::endl;	
		delete mesh;
	}

	//reset memoty
	malloc_count_reset_peak();
	

	//Generate compact triangulation
	compactTriangulation *compact = new compactTriangulation(node_file, output_file + ".pg");
	std::cout<<"Compact Triangulation generated in "<<compact->get_triangulation_generation_time()<<" seconds"<<std::endl;
	std::cout<<"Memory used to generate compact mesh "<<(long long)malloc_count_peak()<<" bytes"<<std::endl;
	std::cout<<"Memory of compact mesh "<<(long long)malloc_count_current()<<" bytes"<<std::endl;	mem_compact_triangulation = (long long) malloc_count_peak();

	//reset memory counter
	malloc_count_reset_peak();

	//Generate Compact Polylla
	Polylla *mesh_compact = new Polylla(compact);
	mem_compact_polylla = (long long) malloc_count_peak();
	//mesh_compact->print_time(output_file + "_compact.json");
	std::cout<<"Memory used to generate compact Polylla: "<<(long long)malloc_count_peak()<<std::endl;	
	std::cout<<"Memory of compact Polylla "<<(long long)malloc_count_current()<<" bytes"<<std::endl;	
	delete mesh_compact;
	


	return 0;
}
