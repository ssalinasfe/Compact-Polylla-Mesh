/* Polygon mesh generator

//TODO: 
    - hacer la función distance parte de cada halfedge y cambiar el ciclo por 3 comparaciones.
    - iterador de polygono
    - Vector con los poligonos de malla
    - Método para imprimir SVG
    - Copy constructor
    - half-edge constructor
    

//POSIBLE BUG: el algoritmo no viaja por todos los halfedges dentro de un poligono, 
    //por lo que pueden haber semillas que no se borren y tener poligonos repetidos de output
*/

#ifndef POLYLLA_HPP
#define POLYLLA_HPP

#include <array>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <inversedHalfEdge.hpp>
//#include <compressHalfEdge.hpp>

#define print_e(eddddge) eddddge<<" ( "<<tr->origin(eddddge)<<" - "<<tr->target(eddddge)<<") "

class Polylla
{
private:
    typedef std::array<uint,3> triangle; //triangle
    typedef std::vector<uint> polygon; 
   // typedef std::vector<char> bit_vector; 
    compressTriangulation *tr;

    sdsl::bit_vector max_edges;
    sdsl::bit_vector frontier_edges;
    sdsl::bit_vector triangles;    
    sdsl::bit_vector seed_edges;
    sdsl::bit_vector mesh;
    uint m_polygons = 0;

public:

    Polylla(std::string node_file, std::string graph_file){
        
        
        this->tr = new compressTriangulation(node_file, graph_file);
        std::cout<<"Compress Triangulation Generated"<<std::endl;
        max_edges = bit_vector(tr->halfEdges(), false);
        frontier_edges = bit_vector(tr->halfEdges(), false);
        seed_edges = bit_vector(tr->halfEdges(), false);
        triangles = tr->get_Triangles();
        mesh = bit_vector(tr->halfEdges(), false);
        for (int e = 0; e < tr->halfEdges(); e++){
        //    std::cout<<"edge "<<e<<": next "<<tr->CCW_edge_to_vertex(e)<<", prev "<<tr->CW_edge_to_vertex(e)<<std::endl;
        //    std::cout<<"edge "<<e<<": next "<<tr->next(e)<<", prev "<<tr->pemb_prev(e)<<", first "<<tr->pemb_next(e)<<", last "<<tr->pemb_last(e)<<std::endl;
            //std::cout<<"edge "<<e<<": next "<<tr->pemb_next(e)<<", prev "<<tr->pemb_prev(e)<<std::endl;
            std::cout<<"edge "<<e<<": next "<<tr->next(e)<<", prev "<<tr->prev(e)<<", origin: "<<tr->origin(e)<<", target:"<<tr->target(e)<<", "<<tr->twin(e)<<", CW_edge: "<<tr->CW_edge_to_vertex(e)<<", CCW_edge: "<<tr->CCW_edge_to_vertex(e)<<", face: ";
            tr->face(e);
            std::cout<<std::endl;
            
        }

        std::cout<<"Labeling edges"<<std::endl;
        std::cout<<"Labeling max edges"<<std::endl;
        //Label max edges
        for (int e = 0; e < tr->halfEdges(); e++){
            if(triangles[e] == true){
                //std::cout<<"Labeling edge "<<e<<std::endl;
                max_edges[this->label_max_edge(e)] = true;   
            }
            
        }

        std::cout<<"Labeling frontier edges"<<std::endl;
        //Label frontier edges
        for (int e = 0; e < tr->halfEdges(); e++){
            //std::cout<<"Labeling edge "<<e<<std::endl;
            frontier_edges[e] = is_frontier_edge(e);
        }

        std::cout<<"Labeling seed edges"<<std::endl;
        //label seeds edges,
        for (int e = 0; e < tr->halfEdges(); e++){
            if(tr->is_interior_face(e)){
              //  std::cout<<"Labeling edge "<<e<<std::endl;
                seed_edges[e] = is_seed_edge(e);
            }
        }
        
        std::cout<<"Label done"<<std::endl;
        std::cout<<"Travel phase"<<std::endl;
        polygon poly;
        for (int e = 0; e < tr->halfEdges(); e++){
            //for each seed edge
            if(seed_edges[e] == true){
                poly = travel_triangles(e);
                this->mesh[e] = true;
                m_polygons++; 
                print_vector(poly);
            }
        }        


        //for (int e = 0; e < tr->halfEdges(); e++){
        //    std::cout<<"edge "<<e<<": max "<<max_edges[e]<<", frontier "<<frontier_edges[e]<<", seed "<<seed_edges[e]<<" , border "<<tr->is_border_face(e)<<std::endl;
        //}

        //Travel phase: Generate polygon mesh
        //polygon poly;
        //for (int e = 0; e < tr->halfEdges(); e++){
        //    //for each seed edge
        //    if(seed_edges[e] == true){
        //        poly = travel_triangles(e);
        //        if(!has_BarrierEdgeTip(poly)){ //If the polygon is a simple polygon then is part of the mesh
        //            this->seed_edges[e] = true;
        //            m_polygons++; 
        //        }else{ //Else, the polygon is send to reparation phase
        //            barrieredge_tip_reparation(e, poly);
        //        }         
        //    }
        //}


    }

    ~Polylla() {
        delete tr;
    }

    void print_new_nodes(std::string file_name){
        std::ofstream out(file_name);
        out<<tr->vertices()<<" 2 0 1"<<std::endl;
        for (int e = 0; e < tr->vertices(); e++){
            out<<e<<" "<<tr->get_PointX(e)<<" "<<tr->get_PointY(e)<<std::endl;
        }
        out.close();
    }

    void print_hedge_triangulation(std::string file_name){
        std::cout<<"Print halfedges"<<std::endl;
        std::ofstream file;
        file.open(file_name);
        file<<tr->halfEdges()<<std::endl;
        for(uint i = 0; i < tr->halfEdges(); i++){
                file<<tr->origin(i)<<" "<<tr->target(i)<<"\n";
        }
        file.close();
    }

    void print_hedge(std::string file_name){
        std::cout<<"Print halfedges"<<std::endl;
        std::ofstream file;
        file.open(file_name);
        uint n_frontier_edges = 0;
        for(uint i = 0; i < frontier_edges.size(); i++){
            if(frontier_edges[i] == true){
                n_frontier_edges++;
            }
        }
        file<<n_frontier_edges<<std::endl;
        for(uint i = 0; i < tr->halfEdges(); i++){
            if(frontier_edges[i] == true){
                file<<tr->origin(i)<<" "<<tr->target(i)<<"\n";
            }
        }
        file.close();
    }

    //function whose input is a vector and print the elements of the vector
    void print_vector(std::vector<uint> &v){
        std::cout<<v.size()<<" ";
        for (int i = 0; i < v.size(); i++){
            std::cout << v[i] << " ";
        }
        std::cout << std::endl;
    }

    //Print off file
    void print_OFF(std::string filename){
        std::ofstream out(filename);
        polygon poly;
        out<<"{ appearance  {+edge +face linewidth 2} LIST\n";
        out<<"OFF"<<std::endl;
        //num_vertices num_polygons 0
        out<<tr->vertices()<<" "<<m_polygons<<" 0"<<std::endl;
        //print nodes
        for(int v = 0; v < tr->vertices(); v++)
            out<<tr->get_PointX(v)<<" "<<tr->get_PointY(v)<<" 0"<<std::endl; 
        //print polygons
        for (int e = 0; e < tr->halfEdges(); e++){
            if(mesh[e] == true){
                poly = generate_polygon(e);
                std::reverse(poly.begin(), poly.end());
                out<<poly.size()<<" ";
                for(int i = 0; i < poly.size(); i++){
                    out<<poly[i]<<" ";
                }
                out<<std::endl; 
            }
        }
        out<<"}"<<std::endl;
        out.close();
    }

    //Return a polygon generated from a seed edge
    polygon generate_polygon(int e)
    {   
        polygon poly;
        //search next frontier-edge
        uint e_init = search_frontier_edge(e);
        uint v_init = tr->origin(e_init);
        uint e_curr = tr->next(e_init);
        uint v_curr = tr->origin(e_curr);
        poly.push_back(v_curr);
        while(e_curr != e_init && v_curr != v_init)
        {   
            e_curr = search_frontier_edge(e_curr);  
            //select triangle that contains v_curr as origin
            e_curr = tr->next(e_curr);
            v_curr = tr->origin(e_curr);
            poly.push_back(v_curr);
        }
        return poly;
    }


private:

    //Return true is the edge is terminal-edge or terminal border edge, 
    //but it only selects one halfedge as terminal-edge, the halfedge with lowest index is selected
    bool is_seed_edge(uint e){
        uint twin = tr->twin(e);

        bool is_terminal_edge = (tr->is_interior_face(twin) &&  (max_edges[e] && max_edges[twin]) );
        bool is_terminal_border_edge = (tr->is_border_face(twin) && max_edges[e]);

        if( (is_terminal_edge && e < twin ) || is_terminal_border_edge){
            return true;
        }
        return false;
    }

    //Return the lenght of a halfedge
    double distance(double x1, double y1, double x2, double y2)
    {
        return sqrt(pow(x1-x2,2) + pow(y1-y2,2));
    }


    //Label max edges of all triangles in the triangulation
    //input: edge e indicent to a triangle t
    //output: position of edge e in max_edges[e] is labeled as true
    uint label_max_edge(const uint e)
    {
        //Calculates the size of each edge of a triangle 
        triangle face = {tr->origin(e), tr->origin(tr->next(e)), tr->origin(tr->next(tr->next(e))) };
        std::cout<<e<<" "<<face[0]<<" "<<face[1]<<" "<<face[2]<<std::endl;
        //face = tr->incident_face(e);
        double dist0 = distance(tr->get_PointX(face[0]), tr->get_PointY(face[0]), tr->get_PointX(face[1]), tr->get_PointY(face[1]));
        double dist1 = distance(tr->get_PointX(face[1]), tr->get_PointY(face[1]), tr->get_PointX(face[2]), tr->get_PointY(face[2]));
        double dist2 = distance(tr->get_PointX(face[0]), tr->get_PointY(face[0]), tr->get_PointX(face[2]), tr->get_PointY(face[2]));
        int max;
        
        //Find the longest edge of the triangle
        if((dist0 >= dist1 && dist1 >= dist2) || (dist0 >= dist2 && dist2 >= dist1)){
            max = 0; //edge face[0]-face[1] is max
        }else if( (dist1 >= dist0 && dist0 >= dist2) || (dist1 >= dist2 && dist2 >= dist0)){
            max = 1; //edge face[1]-face[2] is max
        }else if( (dist2 >= dist1 && dist1 >= dist0) || (dist2 >= dist0 && dist0 >= dist1)){
            max = 2; //edge face[2]-face[0] is max
        }else{
            std::cout<<"ERROR: max edge not found"<<std::endl;
            exit(0);
        }
        std::cout<<max<<std::endl;
        uint init_vertex = tr->origin(e);
        uint curr_vertex = -1;
        uint nxt = e;
        // Return the index of the edge with the longest edge
        while (curr_vertex != init_vertex){
            nxt = tr->next(nxt);
            curr_vertex = tr->origin(nxt);
            if(max == 0 && curr_vertex == face[0]){
                return nxt;
            }else if(max == 1 && curr_vertex == face[1]){
                return nxt;
            }else if(max == 2 && curr_vertex == face[2]){
                return nxt;
            }          
        }
        return -1;
    }

 
    //Return true if the edge e is the lowest edge both triangles incident to e
    //in case of border edges, they are always labeled as frontier-edge
    bool is_frontier_edge(const uint e)
    {
        uint twin = tr->twin(e);
        bool is_border_edge = tr->is_border_face(e) || tr->is_border_face(twin);
        bool is_not_max_edge = !(max_edges[e] || max_edges[twin]);
        if(is_border_edge || is_not_max_edge)
            return true;
        else
            return false;
    }


    //Travel in CCW order around the edges of vertex v from the edge e looking for the next frontier edge
    uint search_frontier_edge(const uint e)
    {
        uint nxt = e;
        while(!frontier_edges[nxt])
        {
            nxt = tr->CW_edge_to_vertex(nxt);
            //std::cout<<"e_nxt: "<<nxt<<" ("<<tr->origin(nxt)<<"-"<<tr->target(nxt)<<"), e_succ: "<<tr->CW_edge_to_vertex(nxt)<<std::endl;
        }  
            return nxt;
    }


    //generate a polygon from a seed edge
    polygon travel_triangles(const std::size_t e)
    {   
        polygon poly;
        //search next frontier-edge
        std::size_t e_init = search_frontier_edge(e);
        std::size_t v_init = tr->origin(e_init);
        std::size_t e_curr = tr->next(e_init);
        std::size_t v_curr = tr->origin(e_curr);
        poly.push_back(v_curr);
        //travel inside frontier-edges of polygon
        while(e_curr != e_init && v_curr != v_init)
        {   
            e_curr = search_frontier_edge(e_curr);  
            //select edge that contains v_curr as origin
            e_curr = tr->next(e_curr);
            v_curr = tr->origin(e_curr);
            //v_curr is part of the polygon
            poly.push_back(v_curr);
        }
        return poly;
    }

    //return true if the polygon is not simple
    bool has_BarrierEdgeTip(std::vector<uint> poly){
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



    ////generate a polygon from a seed edge
    //polygon travel_triangles(const uint e)
    //{    
    //    polygon poly;
    //    //search next frontier-edge
    //    uint e_init = search_frontier_edge(e);
    //    uint v_init = tr->origin(e_init);
    //    uint e_curr = tr->next(e_init);
    //    uint v_curr = tr->origin(e_curr);
    //    poly.push_back(v_curr);
    //    //std::cout<<"edge "<<e<<"("<<tr->origin(e)<<"-"<<tr->target(e)<<"), "<<" e_init "<<e_init<<" v_init "<<v_init<<" e_curr "<<e_curr<<" ("<<tr->origin(e_curr)<<"-"<<tr->target(e_curr)<<") v_curr "<<v_curr<<std::endl;
    //    //cout<<"Polygon "<<e<<":";
    //    //travel inside frontier-edges of polygon
    //    while(e_curr != e_init && v_curr != v_init)
    //    {   
    //        e_curr = search_frontier_edge(e_curr); 
    //       // std::cout<<"e_curr: "<<e_curr<<" ("<<tr->origin(e_curr)<<"-"<<tr->target(e_curr)<<")"<<", v_curr: "<<v_curr<<", e_succ: "<<tr->CW_edge_to_vertex(e_curr)<<std::endl;
//
    //        //select edge that contains v_curr as origin
    //        e_curr = tr->next(e_curr);
    //        v_curr = tr->origin(e_curr);
    //        //v_curr is part of the polygon
    //        poly.push_back(v_curr);
    //    }
    //    return poly;
    //}

    uint search_middle_edge(const uint v_bet)
    {
        //select frontier-edge of barrier-edge tip
        uint frontieredge_with_bet = this->search_frontier_edge(tr->edge_of_vertex(v_bet));
        uint nxt = tr->CW_edge_to_vertex(frontieredge_with_bet);
        uint adv = 1; 
        //calculates the degree of v_bet
        while (nxt != frontieredge_with_bet)
        {
            nxt = tr->CW_edge_to_vertex(nxt);
            adv++;
        }
        adv--; //last edge visited is the same with the frontier-edge so it is not counted
        if(adv%2 == 0){ //if the triangles surrounding the BET are even 
            adv = adv/2 - 1;
        }else{   
            //if the triangles surrounding the BET are odd, edges are even
            //Choose any edge of the triangle in the middle; prov is choose due to this always exists
            adv = adv/2;
        }   
        //back to traversing the edges of v_bet until select the middle-edge
        nxt = tr->CW_edge_to_vertex(frontieredge_with_bet);
        //adv--;
        while (adv != 0)
        {
            nxt = tr->CW_edge_to_vertex(nxt);
            adv--;
        }
        return nxt;
    }

    void barrieredge_tip_reparation(const uint e, std::vector<uint> &poly)
    {
        //Generator of polygon is removed
        this->seed_edges[e] = false;
        uint x, y, i;
        uint t1, t2;
        uint middle_edge, v_bet;

        //list is initialize
        std::vector<uint> triangle_list;
        bit_vector seed_bet_mark(this->tr->halfEdges(), false);
        //look for barrier-edge tips
        for (i = 0; i < poly.size(); i++)
        {
            x = i;
            y = (i+2) % poly.size();
            if (poly[x] == poly[y]){
                //select edge with bet
                v_bet= poly[(i+1) % poly.size()];
                //middle edge that contains v_bet
                middle_edge = search_middle_edge(v_bet);
                t1 = middle_edge;
                t2 = tr->twin(middle_edge);
                //edges of middle-edge are labeled as frontier-edge
                this->frontier_edges[t1] = true;
                this->frontier_edges[t2] = true;
                //edges are use as seed edges and saves in a list
                triangle_list.push_back(t1);
                triangle_list.push_back(t2);

                seed_bet_mark[t1] = true;
                seed_bet_mark[t2] = true;
            }
        }
        uint t_curr;
        polygon poly_curr;
        //generate polygons from seeds, two seeds can generate the same polygon
        //so the bit_vector seed_bet_mark is used to label as false the edges that are already used
        while (!triangle_list.empty())
        {
            t_curr = triangle_list.back();
            triangle_list.pop_back();
            if(seed_bet_mark[t_curr]){
                seed_bet_mark[t_curr] = false;
                poly_curr = generate_repaired_polygon(t_curr, seed_bet_mark);
                this->seed_edges[t_curr] = true;   
                m_polygons++;
            }
        }
    }


    //Generate a polygon from a seed-edge and remove repeated seed from seed_list
    //POSIBLE BUG: el algoritmo no viaja por todos los halfedges dentro de un poligono, 
    //por lo que pueden haber semillas que no se borren y tener poligonos repetidos de output
    polygon generate_repaired_polygon(const uint e, bit_vector &seed_list)
    {   
        polygon poly;
        uint e_init = e;
        //search next frontier-edge
        while(!frontier_edges[e_init])
        {
            e_init = tr->CW_edge_to_vertex(e_init);
            seed_list[e_init] = false; 
        }        
        uint v_init = tr->origin(e_init);
        uint e_curr = tr->next(e_init);
        uint v_curr = tr->origin(e_curr);
        poly.push_back(v_curr);
        seed_list[e_curr] = false;
        while(e_curr != e_init && v_curr != v_init)
        {   
            while(!frontier_edges[e_curr])
            {
                e_curr = tr->CW_edge_to_vertex(e_curr);
                seed_list[e_curr] = false;
            } 
            seed_list[e_curr] = false;
            e_curr = tr->next(e_curr);
            v_curr = tr->origin(e_curr);
            poly.push_back(v_curr);
            seed_list[e_curr] = false;
        }
        return poly;
    }
};

#endif

