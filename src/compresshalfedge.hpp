//https://jerryyin.info/geometry-processing-algorithms/half-edge/

#include <algorithm>

#include <vector>
#include <string>

#include <sdsl/bit_vectors.hpp>
#include <sdsl/pemb.hpp>
#include <complementary/Graph.hpp>
#include <complementary/utils.hpp>

class Triangulation : public pemb<>
{

private:
    typedef std::array<int,3> triangle;
    std::vector<double> points; //vertices position
    sdsl::bit_vector triangles; //indices of edges to a unique triangle
    size_type m_total_edges = 0; //indices of edges to a unique edge


public:


    Triangulation(std::string node_file, std::string graph_file) : pemb<>() {

        Graph g = read_graph_from_file(graph_file.c_str());
        construct_pemb(g);
        //read nodes from file
        read_nodes_from_file(node_file);
        cout<<"vertices "<<m_vertices<<" edges "<<m_edges<<std::endl;
        generate_list_of_triangles();

        cout<<*triangles.begin()<<" "<<*(triangles.begin()+1)<<" "<<*(triangles.begin()+2)<<std::endl;
        cout<<*begin()<<std::endl;
    }

    void construct_pemb(Graph g)
    {
        m_vertices = g.vertices();
        m_edges = g.edges();

        int *parent = new int[m_vertices];
        for (unsigned int i = 0; i < m_vertices; i++) parent[i] = -1;
        unsigned int *count_edges = new unsigned int[2 *(m_vertices - 1)]();
        unsigned int *references = new unsigned int[2 *(m_vertices - 1)]();
        unsigned int init = 0;

        Tree t = g.dfs_spanning_tree(init, parent, count_edges, references);

        //for (size_t i = 0; i < m_vertices; i++)
        //{
        //	std::cout<<t.getNode(i).getFirst()<<" ";
        //}
        //std::cout<<std::endl;

        bit_vector_type A_local(2 *m_edges, 0);
        bit_vector_type B_local(2 *m_vertices, 0);
        bit_vector_type B_star_local(2 *m_edges - 2 *m_vertices + 4, 0);

        unsigned int *marked_edges = new unsigned
        int[2 *m_edges - 2 *m_vertices + 2]();
        unsigned int idx = 0;
        unsigned int ii = 0;
        unsigned int pos = 0;

        for (unsigned int i = 1; i <= 2 *(m_vertices - 1); i++)
        {
            Edge e = t.getEdge(idx);
            Vertex n_src = t.getNode(e.getSrc());
            Vertex n_tgt = t.getNode(e.getTgt());

            // precompute forward and backward edges for B*
            unsigned int ref = references[t.getEdgeCmp(idx)];
            unsigned int limit = ref + count_edges[t.getEdgeCmp(idx)];
            Edge ee = g.getEdge(ref);
            if (limit <= g.getVertexLast(ee.getSrc()))
                for (unsigned int j = ref + 1; j <= limit; j++)
                {
                    g.setEdgeSrc(j, pos);
                    marked_edges[pos++] = j;
                }
            else
            {
                for (unsigned int j = ref + 1; j <= g.getVertexLast(ee.getSrc()); j++)
                {
                    g.setEdgeSrc(j, pos);
                    marked_edges[pos++] = j;
                }

                limit = g.getVertexFirst(ee.getSrc()) + limit -
                    g.getVertexLast(ee.getSrc());

                for (unsigned int j = g.getVertexFirst(ee.getSrc()); j < limit; j++)
                {
                    g.setEdgeSrc(j, pos);
                    marked_edges[pos++] = j;
                }
            }

            A_local[ii] = 1;
            ii += count_edges[t.getEdgeCmp(idx)] + 1;

            /*Forward edges. In the root, all the edges are forward edges */
            /*e.src == init means the root
                t->E[parent[e.tgt]].cmp != i means a forward edge */
            if ((e.getSrc() == init) | (parent[e.getSrc()] != idx))
            {
                B_local[i] = 1;

                /*Leaf */
                if (n_tgt.getFirst() == n_tgt.getLast())
                    idx = e.getCmp();
                /*Connect to the first child of the tgt node */
                else
                {
                    unsigned int succ = parent[e.getTgt()] + 1;
                    if (succ > t.getNodeLast(e.getTgt()))
                        succ = t.getNodeFirst(e.getTgt());
                    idx = succ;
                }
            }
            else
            { 			/*Especial case for the last child of the root */
                /*Assumption: N[init].first in an external edge and it is part of the
    spanning tree */
                if (e.getTgt() == init && n_tgt.getLast() == e.getCmp())
                    break;
                else
                {
                    unsigned int last = parent[e.getTgt()] - 1;
                    if (last < t.getNodeFirst(e.getTgt()))
                        last = t.getNodeLast(e.getTgt());

                    /*Last child of t->N[i] */
                    if (e.getCmp() == last)
                        idx = parent[e.getTgt()];
                    /*Intermediate child */
                    else
                    {
                        unsigned int nxt = e.getCmp() + 1;
                        if (nxt > t.getNodeLast(e.getTgt()))
                            nxt = t.getNodeFirst(e.getTgt());
                        idx = nxt;
                    }
              }
            }
        }
        B_local[0] = 1;

        unsigned int ul = 2 *m_edges - 2 *m_vertices + 2;
        for (unsigned int i = 0; i < ul; i++)
        {
            Edge e = g.getEdge(marked_edges[i]);
            if (e.getSrc() <= g.getEdgeSrc(e.getCmp()))
                B_star_local[i + 1] = 1;
        }

        B_star_local[0] = 1;

        delete[] marked_edges;

        m_A.swap(A_local);
        m_B.swap(B_local);
        m_B_star.swap(B_star_local);

        util::init_support(m_A_rank, &m_A);
        util::init_support(m_A_select0, &m_A);
        util::init_support(m_A_select1, &m_A);

        succ_tree B_local_st(&m_B);
        succ_tree B_star_local_st(&m_B_star);

        m_B_st.swap(B_local_st);
        m_B_st.set_vector(&m_B);
        m_B_star_st.swap(B_star_local_st);
        m_B_star_st.set_vector(&m_B_star);
    }

    //Read node file in .node format and nodes in point vector
    void read_nodes_from_file(std::string name){
        std::string line;
        std::ifstream nodefile(name);
        double a1, a2, a3;
        int i = 0;
        points.resize(2*m_vertices);
        if (nodefile.is_open())
        {
            std::getline(nodefile, line); // skip the first line
            while (nodefile >> a1 >> a2 >> a3)
            {
                points[2*i + 0] = a2;
                points[2*i + 1] = a3;
                std::cout<<points[2*i+0]<<" "<<points[2*i+1]<<std::endl;
                i++;
                
            }
        }
        else 
            std::cout << "Unable to open"<<name<<"file"<<std::endl; 

        nodefile.close();
    }

    //Change the position of the nodes in points vector to the position of the nodes in the tree
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
                   // std::cout << "moving vertex " << k  << " to pos "<< tgt << std::endl;
                    aux[2*k] = points[2*tgt];
                    aux[2*k+1] = points[2*tgt+1];
                    k++;
                }
            }
        }
        points = aux;
        delete [] visited;
    }

    //Generate list of uniques faces
    //The output is a bitvector triangles that containts the edges that generate the triangles
    //each triangle is generate one time
    void generate_list_of_triangles(){
        triangle face;
        triangle face_aux;
        for (int e = 0; e < this->m_total_edges; e++){
            if(!this->is_interior_face(e))
                this->triangles[e] = false;
            if(this->triangles[e] == true){
                face = this->incident_face(e);
                for (int i = e + 1; i < m_total_edges; i++){
                    if(this->is_interior_face(i) && this->triangles[i] == true){
                        face_aux = this->get_triangle(i);
                        if(i != e && this->triangles[i] == true){
                            bool is_index_1 = face[0] == face_aux[0] || face[0] == face_aux[1] || face[0] == face_aux[2];
                            bool is_index_2 = face[1] == face_aux[0] || face[1] == face_aux[1] || face[1] == face_aux[2];
                            bool is_index_3 = face[2] == face_aux[0] || face[2] == face_aux[1] || face[2] == face_aux[2];
                            if(is_index_1 && is_index_2 && is_index_3 ){                
                                this->triangles[i] = false;
                            }
                        }
                    }	
                }
            }
        }
    }

    //Return triangle of the face incident to edge e
    //Input: e is the edge
    //output: array with the vertices of the triangle
    triangle incident_face(size_type e)
    {   
        triangle face;
        char flag = 1;
        size_type nxt = e;
        size_type mt;
        size_type init_vertex = vertex(nxt);
        size_type curr_vertex = -1;
        size_type i = 0;
        while (curr_vertex != init_vertex || flag)
        {
            if (nxt >= 2 *m_edges)
            {
                nxt = first(vertex(mt));
            }

            flag = 0;
            mt = mate(nxt);
            curr_vertex = vertex(mt);
            try {
                face[i] = curr_vertex;
            }
            catch (int e) {
                std::cout << "Error in get_triangle of edge "<<e<< std::endl;
            }
            i++;
            nxt = next(mt);
        }
        return face;
    }

    //Input: edge e
    //Output: true if is the face of e is border face
    //        false otherwise
    bool is_border_face(int e){
        char flag = 1; 
        size_type nxt = e;
        size_type mt;
        size_type init_vertex = vertex(nxt);
        size_type curr_vertex = -1;
        size_type i = 0;
        while (curr_vertex != init_vertex || flag)
        {
            if (nxt >= 2 *m_edges)
            {
                nxt = first(vertex(mt));
            }

            flag = 0;
            mt = mate(nxt);
            curr_vertex = vertex(mt);
            i++;
            nxt = pemb::next(mt);
            if (i > 3)
                return true;
        }
        if(i<3){
            std::cout<<"Graph is not a triangulation";
            exit(0);
        }
        return i == 3;
    }

    // Input: edge e of triangulation
    // Output: true if the edge is an interior face a
    //         false otherwise
     bool is_interior_face(size_type e)
    {
       return !this->is_border_face(e);
    }    

    //Calculates the next edge of the face incident to edge e
    //Input: e is the edge
    //Output: the next edge of the face incident to e
    size_type next(size_type e){
        size_type mt = mate(e);
        size_type nxt = pemb::next(mt);
        while(nxt >= m_total_edges)
            nxt = first(vertex(mt));
        return nxt;
    }


    //Calculates the tail vertex of the edge e
    //Input: e is the edge
    //Output: the tail vertex v of the edge e
    size_type origin(size_type e)
    {
        if (e >= 2 *m_edges)
            return -1;
        return vertex(e);
    }


    //Calculates the head vertex of the edge e
    //Input: e is the edge
    //Output: the head vertex v of the edge e
    size_type target(size_type e)
    {
        size_type nxt = e;
        size_type init_vertex = vertex(nxt);
        size_type mt = mate(nxt);
        size_type curr_vertex = vertex(mt);
        return curr_vertex;
    }

    //Return the twin edge of the edge e
    //Input: e is the edge
    //Output: the twin edge of e
    size_type twin(size_type e)
    {
        return mate(e);
    }

    

    /*Assuming indices start with 0 
    size_type prev(size_type i)
    {
        if (i > 1 && m_A[i-1] == 0)
            return i - 1;
        else if(i > 1 && m_A[i-1] == 1 && m_B[m_A_rank(i-1) == 1])
            return mate(i - 1);
        else
            return -1;
    }
    */

    ~Triangulation() {};
};