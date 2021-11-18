//g++ -std=c++11 -O3 -DNDEBUG -I ~/include -Iinclude -L ~/lib  main.cpp -o main -lsdsl -ldivsufsort -ldivsufsort64 

#include <algorithm>

#include <vector>
#include <string>

#include <sdsl/bit_vectors.hpp>
#include <sdsl/pemb.hpp>
#include <complementary/Graph.hpp>
#include <complementary/utils.hpp>


#include <io_void.hpp>
#include <delfin.hpp>


using namespace sdsl;
using namespace std;

int main(int argc, char **argv) {
	// argv[1] is the path to a file with the planar embedding.
	// To check the input format, visit https://users.dcc.uchile.cl/~jfuentess/datasets/graphs.php  
	//read graph from file	
	Graph g = read_graph_from_file(argv[1]);
	//read nodes from file
	std::vector<double> points = read_nodes_from_file(string(argv[1]), g.vertices());
	//change indices of node according to the dfs in the spanning three of the graph

	g.tree_indices(points);



	//generate the graph using three bitvectors A, B, B_st
	pemb<> pe(g);

	cout << "Size in bytes: " << size_in_bytes(pe) << " B" << endl;
	cout << "Degree of vertex 3: " << pe.degree(3) << endl;
	cout << "Number of vertices: " << pe.vertices() << endl;
	cout << "Number of edges: " << pe.edges() << endl;

	cout<<"size A: "<<pe.size_bitvectorA()<<endl;
	cout<<"size B: "<<pe.size_bitvectorB()<<endl;
	cout<<"size B_st: "<<pe.size_bitvectorm_B_st()<<endl;

	//generate a list of faces using edges as index of generation of each face
	array<int, 3> face;
	array<int, 3> face_aux;
	bit_vector triangles(pe.size_bitvectorA(), true);
	for (int e = 0; e < pe.size_bitvectorA(); e++){
		if(!pe.is_interior_face(e))
			triangles[e] = false;
		if(triangles[e] == true){
			pe.get_face(e, face);
			for (int i = e + 1; i < pe.size_bitvectorA(); i++){
				if(pe.is_interior_face(i) && triangles[i] == true){
					pe.get_face(i, face_aux);
					if(i != e && triangles[i] == true){
						bool is_index_1 = face[0] == face_aux[0] || face[0] == face_aux[1] || face[0] == face_aux[2];
						bool is_index_2 = face[1] == face_aux[0] || face[1] == face_aux[1] || face[1] == face_aux[2];
						bool is_index_3 = face[2] == face_aux[0] || face[2] == face_aux[1] || face[2] == face_aux[2];
						if(is_index_1 && is_index_2 && is_index_3 ){
							triangles[i] = false;
						}
					}
				}	
			}
		}
	}

	cout<<"triangle bitvector: "<<triangles<<endl;
	for (int ptr = 0; ptr < pe.size_bitvectorA(); ptr++){
		if(triangles[ptr] == true){
			pe.face(ptr);
			cout<<endl;
		}
	}

	bit_vector max_edges(pe.size_bitvectorA(), false);
	bit_vector frontier_edges(pe.size_bitvectorA(), false);
	bit_vector terminal_edges(pe.size_bitvectorA(), false);
	bit_vector seeds(pe.size_bitvectorA(), false);

	//Label max edges
	for (int e = 0; e < triangles.size(); e++){
		if(triangles[e] == true){
			label_max_edge(e, pe, max_edges, points);
		}
	}
	cout<<"max_edges: "<<max_edges<<endl;

	//Label frontier edges
	for (int e = 0; e < pe.size_bitvectorA(); e++){
		frontier_edges[e] = is_frontier_edge(e, pe, max_edges, points);
		cout<<"frontier_edges["<<e<<"]: "<<frontier_edges[e]<<endl;
	}
	cout<<"frontier_edges: "<<frontier_edges<<endl;

	//Generate polygons
	
	for (int e = 0; e < triangles.size(); e++){
		if(triangles[e] == true){
			generate_polygon(e, pe, frontier_edges, triangles);
		}
	}



	
	return 0;
}
