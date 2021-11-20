#include <compresshalfedge.hpp>

class Polylla
{
private:
    compressTriangulation *tr;
    sdsl::bit_vector max_edges;
    sdsl::bit_vector frontier_edges;
    sdsl::bit_vector terminal_edges;
    sdsl::bit_vector triangles;    

public:

    Polylla(std::string node_file, std::string graph_file){
    
        this->tr = new compressTriangulation(node_file, graph_file);
        
        max_edges = sdsl::bit_vector(tr->halfEdges(), false);
        frontier_edges = sdsl::bit_vector(tr->halfEdges(), false);
        terminal_edges = sdsl::bit_vector(tr->halfEdges(), false);
        triangles = tr->get_Triangles();

        cout<<"Triangles: "<<triangles.size()<<endl;

        //Label max edges
        for (int e = 0; e < triangles.size(); e++)
            if(triangles[e] == true){
                this->label_max_edge(e);
              //  cout<<"Max edge: "<<e<<endl;
            }

        cout<<"max_edges: "<<max_edges<<endl;


        //Label frontier edges
        for (int e = 0; e < tr->halfEdges(); e++){
            frontier_edges[e] = is_frontier_edge(e);
        //    cout<<"frontier_edges["<<e<<"]: "<<frontier_edges[e]<<endl;
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
        
        //for(int e = 0; e < tr->halfEdges(); e++){
        //    cout<<"edge "<<e<<" next: "<<pe->next(e)<<" prev: "<<pe->prev(e)<<endl;
        //}
        //for(int v = 0; v < pe->vertices(); v++){
        //    cout<<"vertex "<<v<<" first: "<<pe->first(v)<<" last: "<<pe->last(v)<<endl;
        //  }

    }

    ~Polylla() {
        delete tr;
    }

private:

    double distance(double x1, double y1, double x2, double y2)
    {
        return sqrt(pow(x1-x2,2) + pow(y1-y2,2));
    }


    //Label max edges of all triangles in the triangulation
    //input: edge e indicent to a triangle t
    //output: position of edge e in max_edges[e] is labeled as true
    void label_max_edge(const uint e)
    {
        array<int, 3> face;
        face = tr->incident_face(e);
        double dist0 = distance(tr->get_PointX(face[0]), tr->get_PointY(face[0]), tr->get_PointX(face[1]), tr->get_PointY(face[1]));
        double dist1 = distance(tr->get_PointX(face[1]), tr->get_PointY(face[1]), tr->get_PointX(face[2]), tr->get_PointY(face[2]));
        double dist2 = distance(tr->get_PointX(face[0]), tr->get_PointY(face[0]), tr->get_PointX(face[2]), tr->get_PointY(face[2]));
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
    
        uint init_vertex = tr->origin(e);
        uint curr_vertex = -1;
        uint nxt = e;
        while (curr_vertex != init_vertex){
            if(max == 0 && curr_vertex == face[0]){
                max_edges[nxt] = true;
            }else if(max == 1 && curr_vertex == face[1]){
                max_edges[nxt] = true;
            }else if(max == 2 && curr_vertex == face[2]){
                max_edges[nxt] = true;
            }          
            nxt = tr->next(nxt);
            curr_vertex = tr->origin(nxt);
        }
    }
    
    bool is_frontier_edge(const uint e)
    {
        uint twin = tr->twin(e);
        if(tr->is_border_face(e) || tr->is_border_face(twin) || !(max_edges[e] || max_edges[twin]) )
            return true;
        else
            return false;
    }

    uint search_frontier_edge(const uint e)
    {
        uint nxt = e;
        //cout<<"searching for frontier edge "<<e<<endl;
        while(!frontier_edges[nxt])
        {
            nxt = tr->CCW_edge_to_vertex(nxt);
            triangles[nxt] = false;
            
        }
        //cout<<"frontier edge founded in edge "<<nxt<<endl;
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
        uint e_init = search_frontier_edge(e);
        uint v_init = tr->origin(e_init);
        polygon.push_back(v_init);
        uint e_curr = tr->next(e_init);
        uint v_curr = tr->origin(e_curr);
        triangles[e_curr] = false;
       // cout<<"edge "<<e<<" e_init "<<e_init<<" v_init "<<v_init<<" e_curr "<<e_curr<<" v_curr "<<v_curr<<" logic: "<<(e_curr != e_init)<<" "<<(v_curr != v_init)<<" "<<(e_curr != e_init && v_curr != v_init)<<endl;
        //cout<<"Polygon "<<e<<":";
        while(e_curr != e_init && v_curr != v_init)
        {   
           // cout<<"enre"<<endl;
            e_curr = search_frontier_edge(e_curr);
            triangles[e_curr] = false;
            v_curr = tr->origin(e_curr);
         //   cout<<"e_curr: "<<e_curr<<", v_curr: "<<v_curr<<", e_succ: "<<tr->CCW_edge_to_vertex(e_curr)<<endl;
            polygon.push_back(v_curr);
            e_curr = tr->next(e_curr);
            triangles[e_curr] = false;
            v_curr = tr->origin(e_curr);
        }
        //cout<<endl;
        return polygon;
    }
/*
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
            if (nxt >= tr->halfEdges())
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
    */

};

