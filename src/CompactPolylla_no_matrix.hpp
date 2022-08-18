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
#include <numeric>
#include <chrono>

#include <CompactHalfEdge.hpp>

#define print_e(eddddge) eddddge<<" ( "<<tr->origin(eddddge)<<" - "<<tr->target(eddddge)<<") "

class Polylla
{
private:
    typedef std::array<std::size_t,3> triangle; //triangle
    typedef std::vector<std::size_t> polygon; 
   // typedef std::vector<char> bit_vector; 
    compressTriangulation *tr;

    sdsl::bit_vector max_edges;
    sdsl::bit_vector frontier_edges;
    sdsl::bit_vector triangles;    
    sdsl::bit_vector seed_edges;
    sdsl::bit_vector mesh;
    std::size_t m_polygons = 0;

public:

    Polylla(std::string node_file, std::string graph_file){
        
        
        auto t_start = std::chrono::high_resolution_clock::now();
        this->tr = new compressTriangulation(node_file, graph_file);
        auto t_end = std::chrono::high_resolution_clock::now();
        double elapsed_time_ms = std::chrono::duration<double, std::milli>(t_end-t_start).count();
        std::cout<<"Compress Triangulation Generated in "<<elapsed_time_ms<< " ms"<<std::endl;
        
        max_edges = bit_vector(tr->halfEdges(), false);
        frontier_edges = bit_vector(tr->halfEdges(), false);
        seed_edges = bit_vector(tr->halfEdges(), false);
        triangles = tr->get_Triangles();
        mesh = bit_vector(tr->halfEdges(), false);
        
        ////Print info
        //for (int e = 0; e < tr->halfEdges(); e++){
        //    std::cout<<"edge "<<e<<": next "<<tr->next(e)<<", prev "<<tr->prev(e)<<", origin: "<<tr->origin(e)<<", target:"<<tr->target(e)<<", "<<tr->twin(e)<<", CW_edge: "<<tr->CW_edge_to_vertex(e)<<", CCW_edge: "<<tr->CCW_edge_to_vertex(e)<<", (new) is_border: "<<tr->is_border(e)<<", is_border_face: "<<tr->is_border_face(e)<<", (new) get_face(e): "<<tr->get_face(e)<<", (new) get_node:"<<tr->get_node(e)<<std::endl;
        //}

        std::cout<<"Labeling edges"<<std::endl;
        
        t_start = std::chrono::high_resolution_clock::now();
        //Label max edges
        for (int e = 0; e < tr->halfEdges(); e++){
            if(triangles[e] == true){
                max_edges[this->label_max_edge(e)] = true;   
            }
            
        }
        t_end = std::chrono::high_resolution_clock::now();
        elapsed_time_ms = std::chrono::duration<double, std::milli>(t_end-t_start).count();
        std::cout<<"Labered max edges in "<<elapsed_time_ms<<" ms"<<std::endl;

        
        //Label frontier edges
        t_start = std::chrono::high_resolution_clock::now();
        for (int e = 0; e < tr->halfEdges(); e++){
            //std::cout<<"Labeling edge "<<e<<std::endl;
            frontier_edges[e] = is_frontier_edge(e);
        }
        t_end = std::chrono::high_resolution_clock::now();
        elapsed_time_ms = std::chrono::duration<double, std::milli>(t_end-t_start).count();
        std::cout<<"Labeled frontier edges in "<<elapsed_time_ms<<" ms"<<std::endl;

        
        //label seeds edges,
        t_start = std::chrono::high_resolution_clock::now();
        for (int e = 0; e < tr->halfEdges(); e++){
            if(tr->is_interior_face(e)){
              //  std::cout<<"Labeling edge "<<e<<std::endl;
                seed_edges[e] = is_seed_edge(e);
            }
        }
        t_end = std::chrono::high_resolution_clock::now();
        elapsed_time_ms = std::chrono::duration<double, std::milli>(t_end-t_start).count();
        std::cout<<"Labeled seed edges in "<<elapsed_time_ms<<" ms"<<std::endl;
        
        polygon poly;
        int mesh_edge;
        t_start = std::chrono::high_resolution_clock::now();
        for (int e = 0; e < tr->halfEdges(); e++){
            //for each seed edge
            if(seed_edges[e] == true){
                poly = travel_triangles(e);
                if(!has_BarrierEdgeTip(poly)){ //If the polygon is a simple polygon then is part of the mesh
                    mesh_edge = search_frontier_edge(e); // One of the frontier-edges is choosen as seed edge
                    this->mesh[mesh_edge] = true;
                    m_polygons++; 
                }else{ //Else, the polygon is send to reparation phase
                    barrieredge_tip_reparation(e, poly);
                }      
            }
        }        
        t_end = std::chrono::high_resolution_clock::now();
        elapsed_time_ms = std::chrono::duration<double, std::milli>(t_end-t_start).count();
        std::cout<<"Polygons generated in "<<elapsed_time_ms<<" ms"<<std::endl;
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
        for(std::size_t i = 0; i < tr->halfEdges(); i++){
                file<<tr->origin(i)<<" "<<tr->target(i)<<"\n";
        }
        file.close();
    }

    void print_hedge(std::string file_name){
        std::cout<<"Print halfedges"<<std::endl;
        std::ofstream file;
        file.open(file_name);
        std::size_t n_frontier_edges = 0;
        for(std::size_t i = 0; i < frontier_edges.size(); i++){
            if(frontier_edges[i] == true){
                n_frontier_edges++;
            }
        }
        file<<n_frontier_edges<<std::endl;
        for(std::size_t i = 0; i < tr->halfEdges(); i++){
            if(frontier_edges[i] == true){
                file<<tr->origin(i)<<" "<<tr->target(i)<<"\n";
            }
        }
        file.close();
    }

    //function whose input is a vector and print the elements of the vector
    void print_vector(std::vector<std::size_t> &v){
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
        std::size_t e_init = search_frontier_edge(e);
        std::size_t v_init = tr->origin(e_init);
        std::size_t e_curr = tr->next(e_init);
        std::size_t v_curr = tr->origin(e_curr);
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
    bool is_seed_edge(std::size_t e){
        std::size_t twin = tr->twin(e);

        bool is_terminal_edge = (tr->is_interior_face(twin) &&  (max_edges[e] && max_edges[twin]) );
        bool is_terminal_border_edge = (tr->is_border_face(twin) && max_edges[e]);

        if( (is_terminal_edge && e < twin ) || is_terminal_border_edge){
            return true;
        }
        return false;
    }

    //Label max edges of all triangles in the triangulation
    //input: edge e indicent to a triangle t
    //output: position of edge e in max_edges[e] is labeled as true
    std::size_t label_max_edge(const std::size_t e)
    {
        //Calculates the size of each edge of a triangle 
        double dist0 = tr->distance(e);
        double dist1 = tr->distance(tr->next(e));
        double dist2 = tr->distance(tr->next(tr->next(e)));
        //Find the longest edge of the triangle
        if(std::max({dist0, dist1, dist2}) == dist0)
            return e;
        else if(std::max({dist0, dist1, dist2}) == dist1)
            return tr->next(e);
        else
            return tr->next(tr->next(e));
    }

 
    //Return true if the edge e is the lowest edge both triangles incident to e
    //in case of border edges, they are always labeled as frontier-edge
    bool is_frontier_edge(const std::size_t e)
    {
        std::size_t twin = tr->twin(e);
        bool is_border_edge = tr->is_border_face(e) || tr->is_border_face(twin);
        bool is_not_max_edge = !(max_edges[e] || max_edges[twin]);
        if(is_border_edge || is_not_max_edge)
            return true;
        else
            return false;
    }

    //Travel in CCW order around the edges of vertex v from the edge e looking for the next frontier edge
    std::size_t search_frontier_edge(const std::size_t e)
    {
        std::size_t nxt = e;
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
    bool has_BarrierEdgeTip(polygon poly){
        int length_poly = poly.size();
        int x, y, i;
        for (i = 0; i < length_poly; i++)
        {
            x = i % length_poly;
            y = (i+2) % length_poly;
            if (poly[x] == poly[y])
                return true;
        }
        return false;
    } 
  
    //Given a barrier-edge tip v, return the middle edge incident to v
    //The function first calculate the degree of v - 1 and then divide it by 2, after travel to until the middle-edge
    //input: vertex v
    //output: edge incident to v
    int calculate_middle_edge(const int v){
        int frontieredge_with_bet = this->search_frontier_edge(tr->edge_of_vertex(v));
        int internal_edges =tr->degree(v) - 1; //internal-edges incident to v
        int adv = (internal_edges%2 == 0) ? internal_edges/2 - 1 : internal_edges/2 ;
        int nxt = tr->CW_edge_to_vertex(frontieredge_with_bet);
        //back to traversing the edges of v_bet until select the middle-edge
        while (adv != 0)
        {
            nxt = tr->CW_edge_to_vertex(nxt);
            adv--;
        }
        return nxt;
    }

    //Given a seed edge e and a polygon poly generated by e, split the polygon until remove al barrier-edge tips
    //input: seed edge e, polygon poly
    //output: polygon without barrier-edge tips
    void barrieredge_tip_reparation(const int e, const polygon poly)
    {
        int x, y, i;
        int t1, t2;
        int middle_edge, v_bet;

        //list is initialize
        std::vector<int> triangle_list;
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
                middle_edge = calculate_middle_edge(v_bet);
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
        int t_curr;
        polygon poly_curr;
        //generate polygons from seeds,
        //two seeds can generate the same polygon
        //so the bit_vector seed_bet_mark is used to label as false the edges that are already used
        while (!triangle_list.empty())
        {
            t_curr = triangle_list.back();
            triangle_list.pop_back();
            if(seed_bet_mark[t_curr]){
                poly_curr = generate_repaired_polygon(t_curr, seed_bet_mark);
                seed_bet_mark[t_curr] = false;
                //Store the polygon in the as part of the mesh
                this->mesh[t_curr] = true;
                m_polygons++; 
            }
        }
    }


    //Generate a polygon from a seed-edge and remove repeated seed from seed_list
    //POSIBLE BUG: el algoritmo no viaja por todos los halfedges dentro de un poligono, 
    //por lo que pueden haber semillas que no se borren y tener poligonos repetidos de output
    polygon generate_repaired_polygon(const int e, bit_vector &seed_list)
    {   
        polygon poly;
        int e_init = e;
        //search next frontier-edge
        while(!frontier_edges[e_init])
        {
            e_init = tr->CW_edge_to_vertex(e_init);
            seed_list[e_init] = false; 
            //seed_list[tr->twin(e_init)] = false;
        }        
        int v_init = tr->origin(e_init);
        int e_curr = tr->next(e_init);
        int v_curr = tr->origin(e_curr);
        poly.push_back(v_curr);
        seed_list[e_curr] = false;
        //seed_list[tr->twin(e_curr)] = false;
        while(e_curr != e_init && v_curr != v_init)
        {   
            while(!frontier_edges[e_curr])
            {
                e_curr = tr->CW_edge_to_vertex(e_curr);
                seed_list[e_curr] = false;
          //      seed_list[tr->twin(e_curr)] = false;
            } 
            seed_list[e_curr] = false;
            //seed_list[tr->twin(e_curr)] = false;
            e_curr = tr->next(e_curr);
            v_curr = tr->origin(e_curr);
            poly.push_back(v_curr);
            seed_list[e_curr] = false;
            //seed_list[tr->twin(e_curr)] = false;
        }
        return poly;
    }
};

#endif

