#include <algorithm>

#include <vector>
#include <string>

#include <sdsl/bit_vectors.hpp>
#include <sdsl/pemb.hpp>
#include <complementary/Graph.hpp>
#include <complementary/utils.hpp>



class polylla
{
private:
    sdsl::pemb<> *pe;
    uint n_edges;
    uint n_vertices;
    

    std::vector<double> points;
    sdsl::bit_vector triangles;
    sdsl::bit_vector max_edges;
    sdsl::bit_vector frontier_edges;
    sdsl::bit_vector terminal_edges;
    sdsl::bit_vector seeds;
    

public:

    polylla(std::string node_file, std::string graph_file){
    
        Graph g = read_graph_from_file(graph_file.c_str());
        n_vertices = g.vertices();
        //read nodes from file
        read_nodes_from_file(node_file);

        //change indices of node according to the dfs in the spanning three of the graph
        change_index_vertices_using_dfs_tree(g);

        //generate the graph using three bitvectors A, B, B_st
        pe = new pemb<>(g);
        n_edges = pe->edges();

        triangles = sdsl::bit_vector(pe->size_bitvectorA(), true);
        max_edges = sdsl::bit_vector(pe->size_bitvectorA(), false);
        frontier_edges = sdsl::bit_vector(pe->size_bitvectorA(), false);
        terminal_edges = sdsl::bit_vector(pe->size_bitvectorA(), false);

        generate_list_of_faces();
        cout<<"triangle bitvector: "<<triangles<<endl;

        //Label max edges
        for (int e = 0; e < triangles.size(); e++)
            if(triangles[e] == true)
                label_max_edge(e);
        
        cout<<"max_edges: "<<max_edges<<endl;

        //Label frontier edges
        for (int e = 0; e < pe->size_bitvectorA(); e++){
            frontier_edges[e] = is_frontier_edge(e);
            //cout<<"frontier_edges["<<e<<"]: "<<frontier_edges[e]<<endl;
        }
        cout<<"frontier_edges: "<<frontier_edges<<endl;

        //Travel phase: Generate polygon mesh
        std::vector<uint> polygon;
        for (int e = 0; e < triangles.size(); e++){
            if(triangles[e] == true){
                polygon = generate_polygon_mesh(e);
                if(!has_BarrierEdgeTip(polygon))
                    cout<<"Simple Polygon: "<<polygon<<endl;
                else
                    cout<<"Non-simple Polygon: "<<polygon<<endl;
            }
        }
        
        //for(int e = 0; e < pe->size_bitvectorA(); e++){
        //    cout<<"edge "<<e<<" next: "<<pe->next(e)<<" prev: "<<pe->prev(e)<<endl;
        //}
        //for(int v = 0; v < pe->vertices(); v++){
        //    cout<<"vertex "<<v<<" first: "<<pe->first(v)<<" last: "<<pe->last(v)<<endl;
        //  }

    }

    ~polylla() {
        delete pe;
    }

private:

    void read_nodes_from_file(std::string name){
        std::string line;
        std::ifstream nodefile(name);
        double a1, a2, a3;
        int i = 0;
        points.resize(2*n_vertices);
        if (nodefile.is_open())
        {
            std::getline(nodefile, line); // skip the first line
            while (nodefile >> a1 >> a2 >> a3)
            {
                points[2*i + 0] = a2;
                points[2*i + 1] = a3;
                //std::cout<<points[2*i+0]<<" "<<points[2*i+1]<<std::endl;
                i++;
                
            }
        }
        else 
            std::cout << "Unable to open"<<name<<"file"<<std::endl; 

        nodefile.close();
    }

    void change_index_vertices_using_dfs_tree(Graph g){
        unsigned int n = g.vertices();
        unsigned int m = g.edges();
        std::vector<double> aux(points.size());
        
        char * visited = new char[n](); // TODO: Change to a boolean array
        unsigned int init = 0;
        stack < unsigned int > s;
        visited[init] = 1;
        s.push(init);
        unsigned int k = 1;
        double x,y;
        while (!s.empty()) {
            unsigned int curr = s.top();
            s.pop();
            for (unsigned int i = g.getVertex(curr).getFirst(); i <= g.getVertex(curr).getLast(); i++) {
                unsigned int tgt = g.getEdge(i).getTgt();
                if (visited[tgt] == 0) { // Not visited	
                    visited[tgt] = 1;
                    s.push(tgt);
                    std::cout << "moving vertex " << k  << " to pos "<< tgt << std::endl;
                    aux[2*k] = points[2*tgt];
                    aux[2*k+1] = points[2*tgt+1];
                    k++;
                }
            }
        }
        points = aux;
        delete [] visited;
    }


    double distance(double x1, double y1, double x2, double y2)
    {
        return sqrt(pow(x1-x2,2) + pow(y1-y2,2));
    }

    void label_max_edge(const uint e)
    {
        array<int, 3> face;
        pe->get_face(e, face);
        double dist0 = distance(points[2*face[0]], points[2*face[0]+1], points[2*face[1]], points[2*face[1]+1]);
        double dist1 = distance(points[2*face[1]], points[2*face[1]+1], points[2*face[2]], points[2*face[2]+1]);
        double dist2 = distance(points[2*face[0]], points[2*face[0]+1], points[2*face[2]], points[2*face[2]+1]);
        int max;
        if((dist0 >= dist1 && dist1 >= dist2) || (dist0 >= dist2 && dist2 >= dist1)){
            max = 0; //edge face[0]-face[1] is max
        }else if( (dist1 >= dist0 && dist0 >= dist2) || (dist1 >= dist2 && dist2 >= dist0)){
            max = 1; //edge face[1]-face[2] is max
        }else if( (dist2 >= dist1 && dist1 >= dist0) || (dist2 >= dist0 && dist0 >= dist1)){
            max = 2; //edge face[2]-face[0] is max
        }else{
            cout<<"ERROR: max edge not found"<<endl;
            exit(0);
        }
    
        if (e >= 2 *pe->edges())
            return;
        char flag = 1;
        uint nxt = e;
        uint mt;
        uint init_vertex = pe->vertex(nxt);
        uint curr_vertex = -1;
        int prev = -1;
        while (curr_vertex != init_vertex || flag)
        {
            flag = 0;
            mt = pe->mate(nxt);
            curr_vertex = pe->vertex(mt);
            nxt = pe->next(mt);
            if (nxt >= 2*pe->edges())
            {
                nxt = pe->first(pe->vertex(mt));
            }
            if(max == 0 && curr_vertex == face[0]){
                max_edges[nxt] = true;
            }else if(max == 1 && curr_vertex == face[1]){
                max_edges[nxt] = true;
            }else if(max == 2 && curr_vertex == face[2]){
                max_edges[nxt] = true;
            }        
        }
    }

    bool is_frontier_edge(const uint e)
    {
        uint mate = pe->mate(e);
        if(!pe->is_interior_face(e) || !pe->is_interior_face(pe->mate(e)) || !(max_edges[e] || max_edges[mate]) )
            return true;
        else
            return false;
    }
    
    uint search_frontier_edge(const uint e)
    {
        uint prev;
        uint nxt = e;
        //cout<<" searching for frontier edge "<<e<<endl;
        while(!frontier_edges[nxt])
        {
            prev = nxt;
            nxt = pe->next(nxt);
            if (nxt >= pe->size_bitvectorA())
            {
                nxt = pe->first(pe->vertex(prev));
            }
            triangles[nxt] = false;
        //  cout<<" next: "<<nxt<<endl;
        }
        //cout<<",found "<<nxt<<".";
        return nxt;
    }

    bool has_BarrierEdgeTip(vector<uint> polygon){
        uint length_poly = polygon.size();
        uint x, y, i;
        for (i = 0; i < length_poly; i++)
        {
            x = i % length_poly;
            y = (i+2) % length_poly;
            if (polygon[x] == polygon[y])
                return true;
        }
        return false;
    }   

    std::vector<uint> generate_polygon_mesh(const uint e)
    {   
        std::vector<uint> polygon;
        //triangles[e] = false;
        //std::vector<uint> polygon;
        uint e_init = search_frontier_edge(e);
        uint v_init = pe->vertex(e_init);
        polygon.push_back(v_init);
        uint e_curr = pe->succ(e_init);
        uint v_curr = pe->vertex(e_curr);
        triangles[e_curr] = false;
        //cout<<"Polygon "<<e<<":";
        while(e_curr != e_init && v_curr != v_init)
        {
            
            e_curr = search_frontier_edge(e_curr);
            triangles[e_curr] = false;
            v_curr = pe->vertex(e_curr);
            polygon.push_back(v_curr);
            //cout<<"e_curr: "<<e_curr<<", v_curr: "<<v_curr<<", e_succ: "<<pe->succ(e_curr)<<endl;
            e_curr = pe->succ(e_curr);
            triangles[e_curr] = false;
            v_curr = pe->vertex(e_curr);
        }
        
        //cout<<endl;
        return polygon;
    }

    void remove_barrierEdgeTip(const uint e){

    }

};

