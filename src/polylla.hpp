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

        //Label max edges
        for (int e = 0; e < triangles.size(); e++){
            if(triangles[e] == true){
                max_edges[this->label_max_edge(e)] = true;   
            }
        }

        //cout<<"max_edges: "<<max_edges<<endl;


        //Label frontier edges
        for (int e = 0; e < tr->halfEdges(); e++){
            frontier_edges[e] = is_frontier_edge(e);
         //   cout<<"frontier_edges["<<e<<"]: "<<frontier_edges[e]<<endl;
        }
        //cout<<"frontier_edges: "<<frontier_edges<<endl;


        //Travel phase: Generate polygon mesh
        polygon poly;
        for (int e = 0; e < triangles.size(); e++){
            if(triangles[e] == true){
                poly = travel_triangles(e);
                triangles[e] = true;
                m_polygons++;
                if(!has_BarrierEdgeTip(poly)){
                    cout<<"Simple Polygon "<<poly.size()<<": "<<poly<<endl;
                    //triangles[e] = true;
                    //m_polygons++;
                }else{
                    cout<<"Non-simple Polygon: "<<poly<<endl;
                    //barrieredge_tip_reparation(e, poly);
                    //m_polygons = m_polygons + 2;
                }
            }
        }
        //cout<<"Triangles: "<<this->triangles<<endl;
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

    //Print off file
    void print_OFF(std::string filename){
        std::ofstream out(filename);
        polygon poly;
        out<<"{ appearance  {+edge +face linewidth 2} LIST\n";
        out<<"OFF"<<endl;
        //num_vertices num_polygons 0
        out<<tr->vertices()<<" "<<m_polygons<<" 0"<<endl;
        //print nodes
        for(int v = 0; v < tr->vertices(); v++)
            out<<tr->get_PointX(v)<<" "<<tr->get_PointY(v)<<" 0"<<endl; 
        //print polygons
        for (int e = 0; e < triangles.size(); e++){
            if(triangles[e] == true){
                poly = generate_polygon(e);
                out<<poly.size()<<" ";
                for(int i = 0; i < poly.size(); i++){
                    out<<poly[i]<<" ";
                }
                out<<endl; 
            }
        }
        out<<"}"<<endl;
        out.close();
    }

    polygon generate_polygon(int e){
        polygon poly;
        uint e_init = search_frontier_edge(e);
        uint v_init = tr->origin(e_init);
        poly.push_back(v_init);
        uint e_curr = tr->next(e_init);
        uint v_curr = tr->origin(e_curr);
        while(e_curr != e_init && v_curr != v_init)
        {   
            e_curr = search_frontier_edge(e_curr);
            v_curr = tr->origin(e_curr);
            poly.push_back(v_curr);
            e_curr = tr->next(e_curr);
            v_curr = tr->origin(e_curr);
        }
        return poly;
    }

private:

    double distance(double x1, double y1, double x2, double y2)
    {
        return sqrt(pow(x1-x2,2) + pow(y1-y2,2));
    }


    //Label max edges of all triangles in the triangulation
    //input: edge e indicent to a triangle t
    //output: position of edge e in max_edges[e] is labeled as true
    uint label_max_edge(const uint e)
    {
        
        triangle face;
        face = tr->incident_face(e);
        //cout<<endl<<"calculating max of edge "<<e<<endl;
        //cout<<" pont "<<face[0]<<": "<<tr->get_PointX(face[0])<<" "<<tr->get_PointY(face[0])<<endl;
        //cout<<" pont "<<face[1]<<": "<<tr->get_PointX(face[1])<<" "<<tr->get_PointY(face[1])<<endl;
        //cout<<" pont "<<face[2]<<": "<<tr->get_PointX(face[2])<<" "<<tr->get_PointY(face[2])<<endl;

        double dist0 = distance(tr->get_PointX(face[0]), tr->get_PointY(face[0]), tr->get_PointX(face[1]), tr->get_PointY(face[1]));
        double dist1 = distance(tr->get_PointX(face[1]), tr->get_PointY(face[1]), tr->get_PointX(face[2]), tr->get_PointY(face[2]));
        double dist2 = distance(tr->get_PointX(face[0]), tr->get_PointY(face[0]), tr->get_PointX(face[2]), tr->get_PointY(face[2]));
        //cout<<"dist0: "<<dist0<<" dist1: "<<dist1<<" dist2: "<<dist2<<" ";
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
        //cout<<"max: "<<max;
        uint init_vertex = tr->origin(e);
        uint curr_vertex = -1;
        uint nxt = e;
        //cout<<", face "<<face[0]<<" "<<face[1]<<" "<<face[2]<<", init_Vertex: "<<init_vertex<<endl;
        while (curr_vertex != init_vertex){
            nxt = tr->next(nxt);
            curr_vertex = tr->origin(nxt);
            //cout<<"curr_Vertex: "<<curr_vertex<<", next: "<<nxt<<endl;
            if(max == 0 && curr_vertex == face[0]){
              //  cout<<", edge "<<nxt<<endl;
                return nxt;
            }else if(max == 1 && curr_vertex == face[1]){
                //cout<<", edge "<<nxt<<endl;
                return nxt;
            }else if(max == 2 && curr_vertex == face[2]){
                //cout<<", edge "<<nxt<<endl;
                return nxt;
            }          
        }
        return nxt;
    }
    
    bool is_frontier_edge(const uint e)
    {
        uint twin = tr->twin(e);
        bool is_border_edge = tr->is_border_face(e) || tr->is_border_face(twin);
        bool is_not_max_edge = !(max_edges[e] || max_edges[twin]);
        //cout<<"edge: "<<e<<", twin: "<<twin<<", border_edge: "<<border_edge<<", max e: "<<max_edges[e]<<", max twin: "<<max_edges[twin]<<", max total: "<<!(max_edges[e] || max_edges[twin])<<" total: "<<(border_edge || !(max_edges[e] || max_edges[twin]))<<endl;
        if(is_border_edge || is_not_max_edge)
            return true;
        else
            return false;
    }

    //Travel in CCW order around the edges of vertex v from the edge e
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

    bool has_BarrierEdgeTip(vector<uint> poly){
        uint length_poly = poly.size();
        uint x, y, i;
        for (i = 0; i < length_poly; i++)
        {
            x = i % length_poly;
            y = (i+2) % length_poly;
            if (poly[x] == poly[y])
                return true;
        }
        return false;
    }   

    polygon travel_triangles(const uint e)
    {   
        triangles[e] = false;
        polygon poly;
        uint e_init = search_frontier_edge(e);
        triangles[e_init] = false;
        uint v_init = tr->origin(e_init);
        poly.push_back(v_init);
        uint e_curr = tr->next(e_init);
        uint v_curr = tr->origin(e_curr);
        triangles[e_curr] = false;
        cout<<"edge "<<e<<" e_init "<<e_init<<" v_init "<<v_init<<" e_curr "<<e_curr<<" v_curr "<<v_curr<<" logic: "<<(e_curr != e_init)<<" "<<(v_curr != v_init)<<" "<<(e_curr != e_init && v_curr != v_init)<<endl;
        //cout<<"Polygon "<<e<<":";
        while(e_curr != e_init && v_curr != v_init)
        {   
           // cout<<"enre"<<endl;
            e_curr = search_frontier_edge(e_curr);
            triangles[e_curr] = false;
            v_curr = tr->origin(e_curr);
            cout<<"e_curr: "<<e_curr<<", v_curr: "<<v_curr<<", e_succ: "<<tr->CCW_edge_to_vertex(e_curr)<<endl;
            poly.push_back(v_curr);
            e_curr = tr->next(e_curr);
            triangles[e_curr] = false;
            v_curr = tr->origin(e_curr);
        }
        
        return poly;
    }

    uint search_middle_edge(const uint v_bet)
    {
        uint frontieredge_with_bet = this->search_frontier_edge(tr->edge_of_vertex(v_bet));
        uint nxt = tr->CCW_edge_to_vertex(frontieredge_with_bet);
        uint adv = 1;
        while (nxt != frontieredge_with_bet)
        {
            nxt = tr->CCW_edge_to_vertex(nxt);
            adv++;
        }
        cout<<"frontieredge: "<<frontieredge_with_bet<<", nxtedge: "<<nxt<<", adv: "<<adv<<endl;
        if(adv%2 == 0){ //if the triangles surrounding the BET are even 
            adv = adv/2 - 1;
        }else{   
            //if the triangles surrounding the BET are odd, edges are even
            //Choose any edge of the triangle in the middle; prov is choose due to this always exists
            adv = adv/2;
        }   
        nxt = tr->CCW_edge_to_vertex(frontieredge_with_bet);
        adv--;
        while (adv != 0)
        {
            nxt = tr->CCW_edge_to_vertex(nxt);
            adv--;
        }
        cout<<"middle edge: "<<nxt<<", adv: "<<adv<<endl;
        return nxt;
    }

    void generate_repaired_polygon(const uint e, bit_vector &seed_list)
    {   
        uint e_init = e;
        while(!frontier_edges[e_init])
        {
            e_init = tr->CCW_edge_to_vertex(e_init);
            seed_list[e_init] = false;            
        }
        uint v_init = tr->origin(e_init);
        uint e_curr = tr->next(e_init);
        uint v_curr = tr->origin(e_curr);
        seed_list[e_curr] = false;
       // cout<<"edge "<<e<<" e_init "<<e_init<<" v_init "<<v_init<<" e_curr "<<e_curr<<" v_curr "<<v_curr<<" logic: "<<(e_curr != e_init)<<" "<<(v_curr != v_init)<<" "<<(e_curr != e_init && v_curr != v_init)<<endl;
        //cout<<"Polygon "<<e<<":";
        while(e_curr != e_init && v_curr != v_init)
        {   
           // cout<<"enre"<<endl;
            while(!frontier_edges[e_curr])
            {
                e_curr = tr->CCW_edge_to_vertex(e_curr);
                seed_list[e_curr] = false;
                
            }
            seed_list[e_curr] = false;
            v_curr = tr->origin(e_curr);
            e_curr = tr->next(e_curr);
            seed_list[e_curr] = false;
            v_curr = tr->origin(e_curr);
        }
    }

    void barrieredge_tip_reparation(const uint e, vector<uint> &poly)
    {
        //se elimina el generador del polígono
        this->triangles[e] = false;
        uint x, y, i;
        int v_bet;
        uint t1, t2;

        //list is initialize
        std::vector<uint> triangle_list;
        bit_vector seed_bet_mark(this->triangles.size(), false);

        for (i = 0; i < poly.size(); i++)
        {
            uint middle_edge, v_bet;
            for (i = 0; i < poly.size(); i++)
            {
                x = i;
                y = (i+2) % poly.size();
                if (poly[x] == poly[y]){
                    v_bet= poly[(i+1) % poly.size()];
                    middle_edge = search_middle_edge(v_bet);
                    cout<<"bet: "<<v_bet<<", edge_with_fe: "<<middle_edge<<endl;
                    t1 = middle_edge;
                    t2 = tr->twin(middle_edge);
                    cout<<"t1: "<<t1<<", t2: "<<t2<<endl;
                    //edge is labeled as frontier-edge
                    this->frontier_edges[t1] = true;
                    this->frontier_edges[t2] = true;
                    triangle_list.push_back(t1);
                    triangle_list.push_back(t2);
                    seed_bet_mark[t1] = true;
                    seed_bet_mark[t2] = true;
                }
            }
        }
        uint t_curr;
        while (!triangle_list.empty())
        {
            t_curr = triangle_list.back();
            triangle_list.pop_back();
            if(seed_bet_mark[t_curr]){
                seed_bet_mark[t_curr] = false;
                generate_repaired_polygon(t_curr, seed_bet_mark);
                this->triangles[t_curr] = true;
            }
        }
    }
};

