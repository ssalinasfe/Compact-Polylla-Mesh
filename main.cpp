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

	//Generate pg
	Triangulation plannar(node_file, ele_file, neigh_file);
	plannar.print_pg(output_file + ".pg");
	std::cout<<"Pg file generated in "<<plannar.get_triangulation_generation_time()<<" seconds"<<std::endl;
	std::cout<<"Memory used: "<<(long long)malloc_count_peak()<<" bytes"<<std::endl;
	
//	if(argc  == 4){
//		node_file = std::string(argv[1]);
//		graph_file = std::string(argv[2]);
//		output_file = std::string(argv[3]);
//		Polylla mesh(node_file, graph_file);
//	}else if(argc == 5)
//    {
//        node_file = std::string(argv[1]);
//        ele_file = std::string(argv[2]);
//        neigh_file = std::string(argv[3]);
//        output_file = std::string(argv[4]);
//	}else{
//        std::cout<<"Usage: "<<argv[0]<<" <off file .off> <output name>"<<std::endl;
//        std::cout<<"Usage: "<<argv[0]<<" <node_file .node> <ele_file .ele> <neigh_file .neigh> <output name>"<<std::endl;
//        return 0;
//	}
//	
//	mesh.print_OFF(output + ".off");
//	mesh.print_time(output + ".json");
//	printf("our peak memory allocation: %lld\n",
//		(long long)malloc_count_peak());
	return 0;
}
