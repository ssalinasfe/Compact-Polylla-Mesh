
#include <algorithm>
#include <vector>
#include <string>

#include <sdsl/bit_vectors.hpp>
#include <sdsl/pemb.hpp>

#include <math.h>


//function calculate the distance between two point
double distance(double x1, double y1, double x2, double y2)
{
    return sqrt(pow(x1-x2,2) + pow(y1-y2,2));
}

//Calculates the maximun edges of faces of grape using the coords of points
//stores the result in edges_to_label
void label_max_edge(const uint e, sdsl::pemb<> &pe, bit_vector &edges_to_label, vector<double> &points)
{
    array<int, 3> face;
    pe.get_face(e, face);
    cout<<"edge: "<<e<<" face: "<<face[0]<<" "<<face[1]<<" "<<face[2]<<" ";
    double dist0 = distance(points[2*face[0]], points[2*face[0]+1], points[2*face[1]], points[2*face[1]+1]);
    double dist1 = distance(points[2*face[1]], points[2*face[1]+1], points[2*face[2]], points[2*face[2]+1]);
    double dist2 = distance(points[2*face[0]], points[2*face[0]+1], points[2*face[2]], points[2*face[2]+1]);
    cout<<"("<<dist0<<" "<<dist1<<" "<<dist2<<") ";
    int max;
    if((dist0 >= dist1 && dist1 >= dist2) || (dist0 >= dist2 && dist2 >= dist1)){
        max = 0; //edge face[0]-face[1] is max
        cout<<"edge "<<face[0]<<"-"<<face[1]<<" is max (dist0)";
    }else if( (dist1 >= dist0 && dist0 >= dist2) || (dist1 >= dist2 && dist2 >= dist0)){
        max = 1; //edge face[1]-face[2] is max
        cout<<"edge "<<face[1]<<"-"<<face[2]<<" is max (dist1)";
    }else if( (dist2 >= dist1 && dist1 >= dist0) || (dist2 >= dist0 && dist0 >= dist1)){
        max = 2; //edge face[2]-face[0] is max
        cout<<"edge "<<face[2]<<"-"<<face[0]<<" is max (dist2)";
    }else{
        cout<<"ERROR: max edge not found"<<endl;
        exit(0);
    }
 
    if (e >= 2 *pe.edges())
        return;
    char flag = 1;
    uint nxt = e;
    uint mt;
    uint init_vertex = pe.vertex(nxt);
    uint curr_vertex = -1;
    int prev = -1;
    while (curr_vertex != init_vertex || flag)
    {
        flag = 0;
        mt = pe.mate(nxt);
        curr_vertex = pe.vertex(mt);
        nxt = pe.next(mt);
        if (nxt >= 2 *pe.edges())
        {
            nxt = pe.first(pe.vertex(mt));
        }

        if(max == 0 && curr_vertex == face[0]){
            edges_to_label[nxt] = true;
            cout<<" next: "<<nxt<<" curr_vertex "<<curr_vertex<< endl;
        }else if(max == 1 && curr_vertex == face[1]){
            edges_to_label[nxt] = true;
            cout<<" next: "<<nxt<<" curr_vertex "<<curr_vertex<< endl;
        }else if(max == 2 && curr_vertex == face[2]){
            edges_to_label[nxt] = true;
            cout<<" next: "<<nxt<<" curr_vertex "<<curr_vertex<< endl;
            }
        

        
    }
}

bool is_frontier_edge(const uint e, sdsl::pemb<> &pe, bit_vector &max_edges, vector<double> &points)
{
    uint mate = pe.mate(e);
    //cout<<"testing "<<e<<" "<<mate<<( (pe.is_interior_face(e) || !(max_edges[e] || max_edges[mate]) ) ? " fe" : "" )<<   endl;

    if(!pe.is_interior_face(e) || !(max_edges[e] || max_edges[mate]) )
        return true;
    else
        return false;
}

//000010001010001010001010010001100011000000