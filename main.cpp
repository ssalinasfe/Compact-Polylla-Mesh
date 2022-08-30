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
	long long mem_gen_triangulation, mem_gen_polylla, mem_gen_compact_triangulation, mem_gen_compact_polylla;
	long long mem_pemb, mem_nodes;
	long long mem_initial;
	long num_vertices, num_triangles, num_halfedges;

	//Read input names
    node_file = std::string(argv[1]);
    ele_file = std::string(argv[2]);
    neigh_file = std::string(argv[3]);
	output_file = std::string(argv[4]);

	std::cout<<"---------------------------------"<<std::endl;
	std::cout<<"Testing Polylla meshes with files: "<<node_file<<" "<<ele_file<<" "<<neigh_file<<std::endl;
	std::cout<<"Current memory peak: "<<malloc_count_peak()<<std::endl;
	std::cout<<"Current memory: "<<malloc_count_current()<<std::endl;
	std::cout<<"---------------------------------"<<std::endl<<std::endl;
	mem_initial = malloc_count_current();
	{
		//Generate triangulation halfedge
		Triangulation *plannar =  new Triangulation(node_file, ele_file, neigh_file);
		plannar->print_pg(output_file + ".pg");
		std::cout<<"Triangulation generated in "<<plannar->get_triangulation_generation_time()<<" seconds"<<std::endl;
		std::cout<<"Memory used to generate triangulation "<<(long long)malloc_count_peak()<<" bytes"<<std::endl;
		std::cout<<"Memory of triangulation "<<(long long)malloc_count_current()<<" bytes"<<std::endl;	

		mem_triangulation = (long long) malloc_count_current();
		mem_gen_triangulation = (long long) malloc_count_peak();
		std::cout<<"---------------------------------"<<std::endl;
		malloc_count_reset_peak();

		num_vertices = plannar->vertices();
		num_triangles = plannar->faces();
		num_halfedges = plannar->halfEdges();

		//Generate Polylla
		Polylla *mesh = new Polylla(plannar);
		mem_polylla = (long long) malloc_count_current();
		mem_gen_polylla = (long long) malloc_count_peak();
		
		mesh->print_OFF(output_file + ".off");

		std::cout<<"Memory used to generate Polylla "<<(long long)malloc_count_peak()<<" bytes"<<std::endl;
		std::cout<<"Memory of Polylla "<<(long long)malloc_count_current()<<" bytes"<<std::endl;	
		std::cout<<"---------------------------------"<<std::endl;
		mesh->print_time(output_file + "_time.json");
		std::cout<<"---------------------------------"<<std::endl;
		//mesh->print_OFF(output_file + ".off");
		delete mesh;
	}
	
	//reset memoty
	malloc_count_reset_peak();
	std::cout<<"Memory after reset "<<(long long)malloc_count_peak()<<" bytes"<<std::endl;
	std::cout<<"---------------------------------"<<std::endl;
	
	//Generate compact triangulation
	compactTriangulation *compact = new compactTriangulation(node_file, output_file + ".pg");
	mem_compact_triangulation = (long long) malloc_count_current();
	mem_gen_compact_triangulation = (long long) malloc_count_peak();
	mem_pemb = compact->get_size_pemb();
	mem_nodes = compact->get_size_nodes();

	std::cout<<"---------------------------------"<<std::endl;
	std::cout<<"Compact Triangulation generated in "<<compact->get_triangulation_generation_time()<<" seconds"<<std::endl;
	std::cout<<"Memory used to generate compact mesh "<<(long long)malloc_count_peak()<<" bytes"<<std::endl;
	std::cout<<"Memory of compact mesh "<<(long long)malloc_count_current()<<" bytes"<<std::endl;	
	std::cout<<"Memory of pemb serialize "<<compact->get_size_pemb()<<" bytes"<<std::endl;	
	std::cout<<"Memory of nodes of compact mesh "<<compact->get_size_nodes()<<" bytes"<<std::endl;	
	std::cout<<"---------------------------------"<<std::endl;
	//reset memory counter
	malloc_count_reset_peak();

	//Generate Compact Polylla
	Polylla *mesh_compact = new Polylla(compact);
	mem_compact_polylla = (long long) malloc_count_current();
	mem_gen_compact_polylla = (long long) malloc_count_peak();
	
	std::cout<<"Memory used to generate compact Polylla: "<<(long long)malloc_count_peak()<<std::endl;	
	std::cout<<"Memory of compact Polylla "<<(long long)malloc_count_current()<<" bytes"<<std::endl;	
	std::cout<<"---------------------------------"<<std::endl;
	mesh_compact->print_time(output_file + "_time_compact.json");
	std::cout<<"---------------------------------"<<std::endl;
	mesh_compact->print_OFF(output_file + ".off");
	delete mesh_compact;

	std::ofstream file;
    file.open(output_file + "_mem.json");
	file<<"{"<<std::endl;
	file<<"\"num_vertices\": "<<num_vertices<<","<<std::endl;
	file<<"\"num_triangles\": "<<num_triangles<<","<<std::endl;
	file<<"\"num_halfedges\": "<<num_halfedges<<","<<std::endl;
	file<<"\"mem_initial\": "<<mem_initial<<","<<std::endl;
	file<<"\"mem_triangulation\": "<<mem_triangulation<<","<<std::endl;
	file<<"\"mem_polylla\": "<<mem_polylla<<","<<std::endl;
	file<<"\"mem_compact_triangulation\": "<<mem_compact_triangulation<<","<<std::endl;
	file<<"\"mem_compact_polylla\": "<<mem_compact_polylla<<","<<std::endl;
	file<<"\"mem_gen_triangulation\": "<<mem_gen_triangulation<<","<<std::endl;
	file<<"\"mem_gen_polylla\": "<<mem_gen_polylla<<","<<std::endl;
	file<<"\"mem_gen_compact_triangulation\": "<<mem_gen_compact_triangulation<<","<<std::endl;
	file<<"\"mem_gen_compact_polylla\": "<<mem_gen_compact_polylla<<","<<std::endl;
	file<<"\"mem_pemb\": "<<mem_pemb<<","<<std::endl;
	file<<"\"mem_nodes\": "<<mem_nodes<<std::endl;
	file<<"}";
	file.close();


	return 0;
}
