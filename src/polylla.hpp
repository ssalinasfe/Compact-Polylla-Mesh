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
//#include <compressHalfEdge.hpp>
//#include <compressHalfEdgeJose.hpp>
#include <inversedHalfEdge.hpp>
#include <chrono>


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
        
        //Print info
        for (int e = 0; e < tr->halfEdges(); e++){
            std::cout<<"edge "<<e<<": next "<<tr->next(e)<<", prev "<<tr->prev(e)<<", origin: "<<tr->origin(e)<<", target:"<<tr->target(e)<<", "<<tr->twin(e)<<", CW_edge: "<<tr->CW_edge_to_vertex(e)<<", CCW_edge: "<<tr->CCW_edge_to_vertex(e)<<", (new) is_border: "<<tr->is_border(e)<<", is_border_face: "<<tr->is_border_face(e)<<", (new) get_face(e): "<<tr->get_face(e)<<", (new) get_node:"<<tr->get_node(e)<<std::endl;
        }

        

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
        
        //std::cout<<"Label done"<<std::endl;
        //std::cout<<"Travel phase"<<std::endl;
        polygon poly;
        t_start = std::chrono::high_resolution_clock::now();
        for (int e = 0; e < tr->halfEdges(); e++){
            //for each seed edge
            if(seed_edges[e] == true){
                poly = travel_triangles(e);
                this->mesh[e] = true;
                m_polygons++; 
       //         print_vector(poly);
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

    //Return the lenght of a halfedge
    double distance(double x1, double y1, double x2, double y2)
    {
        return pow(x1-x2,2) + pow(y1-y2,2);
    }


    //Label max edges of all triangles in the triangulation
    //input: edge e indicent to a triangle t
    //output: position of edge e in max_edges[e] is labeled as true
    std::size_t label_max_edge(const std::size_t e)
    {
        
        //Get vertices of face incident to e
        triangle face = {tr->origin(e),  tr->origin(tr->next(e)),tr->origin(tr->next(tr->next(e))) };

        //Calculates the size of each edge of a triangle 
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
        std::size_t init_vertex = tr->origin(e);
        std::size_t curr_vertex = -1;
        std::size_t nxt = e;
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
};

#endif

