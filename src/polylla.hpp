#include <compresshalfedge.hpp>

class Polylla
{
private:
    typedef std::array<uint,3> triangle; //triangle
    typedef std::vector<uint> polygon; 
    compressTriangulation *tr;
    sdsl::bit_vector max_edges;
    sdsl::bit_vector frontier_edges;
    sdsl::bit_vector terminal_edges;
    sdsl::bit_vector triangles;    
    uint m_polygons = 0;

public:

    Polylla(std::string node_file, std::string graph_file){
    
        this->tr = new compressTriangulation(node_file, graph_file);
        
        max_edges = sdsl::bit_vector(tr->halfEdges(), false);
        frontier_edges = sdsl::bit_vector(tr->halfEdges(), false);
        terminal_edges = sdsl::bit_vector(tr->halfEdges(), false);
        triangles = tr->get_Triangles();

        cout<<"Triangles: "<<triangles.size()<<endl;

       triangle face;
       for (int e = 0; e < tr->halfEdges(); e++){
           if(tr->is_interior_face(e)){
               face = tr->incident_face(e);
               cout<<"face "<<e<<": "<<face[0]<<" "<<face[1]<<" "<<face[2]<<endl;
           }
        //   cout<<"frontier_edges["<<e<<"]: "<<frontier_edges[e]<<endl;
       }
        
        //for(int v = 0; v < tr->halfEdges();v++ ){
        //    std::cout<<"edge "<<v<<" prev "<<tr->pemb_prev(v)<<", next "<<tr->pemb_next(v)<<std::endl;
        //}
        
        for (int e = 0; e < tr->halfEdges(); e++){
        //    std::cout<<"edge "<<e<<": next "<<tr->next(e)<<", prev "<<tr->pemb_prev(e)<<", first "<<tr->pemb_next(e)<<", last "<<tr->pemb_last(e)<<std::endl;
            //std::cout<<"edge "<<e<<": next "<<tr->pemb_next(e)<<", prev "<<tr->pemb_prev(e)<<std::endl;
            std::cout<<"edge "<<e<<": next "<<tr->next(e)<<", prev "<<tr->prev(e)<<", origin: "<<tr->origin(e)<<", target:"<<tr->target(e)<<", "<<tr->twin(e)<<", CW_edge: "<<tr->CW_edge_to_vertex(e)<<", CCW_edge: "<<tr->CCW_edge_to_vertex(e)<<std::endl;
            
        }

    }

    ~Polylla() {
        delete tr;
    }

};

