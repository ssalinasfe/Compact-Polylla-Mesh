//https://jerryyin.info/geometry-processing-algorithms/half-edge/
// https://doc.cgal.org/latest/Arrangement_on_surface_2/classCGAL_1_1Arrangement__2_1_1Halfedge.html

//Compress half-edge using pemb has graph
/*
Basic operations
    incident_face(e): return the face incident to e
    twin(e): return the twin halfedge of e
    next(e): return the next halfedge of e
    prev(e): return the previous halfedge of e
    origin(e): return the first vertex of halfedge e
    target(e): return the second vertex of halfedge e
Others
    CCW_edge_to_vertex(e): return the next CCW edge incident to v after e
    is_border(e): return true if e is a border edge
    is_interior(e): return true if e is an interior edge}
    edge_of_vertex(v): return A edge incident to v

TODO: 
    - Implementar función next, actualment next hace prev dos veces
    - Implementar función face con prev and last
    - Implementar CW función con next

BUGs:
    - La función prev da problemas con las últimas aristas, se retorna mate(i-1) cuando prev(i) retorna i
    - la función prev retorna mate(m_half_edges - 1) para prev(0) 
*/

#ifndef COMPRESSHALFEDGE_HPP
#define COMPRESSHALFEDGE_HPP


#include <sdsl/pemb.hpp>
#include <complementary/Graph.hpp>
#include <complementary/utils.hpp>


#include "triangulation.hpp"
using namespace sdsl;

class compactTriangulation: public pemb < >, public Mesh {

    public: typedef std::array < uint, 3 > triangle;
    std::vector < double > points; //nodes
    //sdsl::bit_vector triangles; //indices of edges to a unique triangle
    //sdsl::bit_vector triangles2; //indices of edges to a unique triangle
    int n_halfedges = 0; //number of halfedges
    int n_faces = 0; //number of faces
    int n_vertices = 0; //number of vertices

    double t_triangulation_generation = 0;

    private:
        /* methods used to for the construct */
        //Generate compress planar embedding graph
        void construct_pemb(Graph &g, int *dfs_order) {
            m_vertices = g.vertices();
            m_edges = g.edges();

            int * parent = new int[m_vertices];
            for (unsigned int i = 0; i < m_vertices; i++) parent[i] = -1;
            unsigned int * count_edges = new unsigned int[2 * (m_vertices - 1)]();
            unsigned int * references = new unsigned int[2 * (m_vertices - 1)]();
            unsigned int init = 0;

            Tree t = g.dfs_spanning_tree(init, parent, count_edges, references);

            t.get_DFS(parent, dfs_order);
            //for (size_t i = 0; i < m_vertices; i++)
            //{
            //	std::cout<<t.getNode(i).getFirst()<<" ";
            //}
            //std::cout<<std::endl;

            bit_vector_type A_local(2 * m_edges, 0);
            bit_vector_type B_local(2 * m_vertices, 0);
            bit_vector_type B_star_local(2 * m_edges - 2 * m_vertices + 4, 0);

            unsigned int * marked_edges = new unsigned int[2 * m_edges - 2 * m_vertices + 2]();
            unsigned int idx = 0;
            unsigned int ii = 0;
            unsigned int pos = 0;

            for (unsigned int i = 1; i <= 2 * (m_vertices - 1); i++) {
                Edge e = t.getEdge(idx);
                Vertex n_src = t.getNode(e.getSrc());
                Vertex n_tgt = t.getNode(e.getTgt());

                // precompute forward and backward edges for B*
                unsigned int ref = references[t.getEdgeCmp(idx)];
                unsigned int limit = ref + count_edges[t.getEdgeCmp(idx)];
                Edge ee = g.getEdge(ref);
                if (limit <= g.getVertexLast(ee.getSrc()))
                    for (unsigned int j = ref + 1; j <= limit; j++) {
                        g.setEdgeSrc(j, pos);
                        marked_edges[pos++] = j;
                    }
                else {
                    for (unsigned int j = ref + 1; j <= g.getVertexLast(ee.getSrc()); j++) {
                        g.setEdgeSrc(j, pos);
                        marked_edges[pos++] = j;
                    }

                    limit = g.getVertexFirst(ee.getSrc()) + limit -
                        g.getVertexLast(ee.getSrc());

                    for (unsigned int j = g.getVertexFirst(ee.getSrc()); j < limit; j++) {
                        g.setEdgeSrc(j, pos);
                        marked_edges[pos++] = j;
                    }
                }

                A_local[ii] = 1;
                ii += count_edges[t.getEdgeCmp(idx)] + 1;

                /*Forward edges. In the root, all the edges are forward edges */
                /*e.src == init means the root
                    t->E[parent[e.tgt]].cmp != i means a forward edge */
                if ((e.getSrc() == init) | (parent[e.getSrc()] != idx)) {
                    B_local[i] = 1;

                    /*Leaf */
                    if (n_tgt.getFirst() == n_tgt.getLast())
                        idx = e.getCmp();
                    /*Connect to the first child of the tgt node */
                    else {
                        unsigned int succ = parent[e.getTgt()] + 1;
                        if (succ > t.getNodeLast(e.getTgt()))
                            succ = t.getNodeFirst(e.getTgt());
                        idx = succ;
                    }
                } else {
                    /*Especial case for the last child of the root */
                    /*Assumption: N[init].first in an external edge and it is part of the
        spanning tree */
                    if (e.getTgt() == init && n_tgt.getLast() == e.getCmp())
                        break;
                    else {
                        unsigned int last = parent[e.getTgt()] - 1;
                        if (last < t.getNodeFirst(e.getTgt()))
                            last = t.getNodeLast(e.getTgt());

                        /*Last child of t->N[i] */
                        if (e.getCmp() == last)
                            idx = parent[e.getTgt()];
                        /*Intermediate child */
                        else {
                            unsigned int nxt = e.getCmp() + 1;
                            if (nxt > t.getNodeLast(e.getTgt()))
                                nxt = t.getNodeFirst(e.getTgt());
                            idx = nxt;
                        }
                    }
                }
            }
            B_local[0] = 1;

            unsigned int ul = 2 * m_edges - 2 * m_vertices + 2;
            for (unsigned int i = 0; i < ul; i++) {
                Edge e = g.getEdge(marked_edges[i]);
                if (e.getSrc() <= g.getEdgeSrc(e.getCmp()))
                    B_star_local[i + 1] = 1;
            }

            B_star_local[0] = 1;

            m_A.swap(A_local);
            m_B.swap(B_local);
            m_B_star.swap(B_star_local);

            util::init_support(m_A_rank, & m_A);
            util::init_support(m_A_select0, & m_A);
            util::init_support(m_A_select1, & m_A);

            succ_tree B_local_st( & m_B);
            succ_tree B_star_local_st( & m_B_star);

            m_B_st.swap(B_local_st);
            m_B_st.set_vector( & m_B);
            m_B_star_st.swap(B_star_local_st);
            m_B_star_st.set_vector( & m_B_star);
            
            delete[] marked_edges;
            delete[] parent;
            delete[] count_edges;
            delete[] references;
        }

    //Read node file in .node format and nodes in point vector
    void read_nodes_from_file(std::string name) {
        std::string line;
        std::ifstream nodefile(name);
        double a1, a2, a3, a4;

        //std::cout<<"Node file"<<std::endl;
        if (nodefile.is_open()) {
            nodefile >> n_vertices;
            //std::cout<<pnumber<<std::endl;
            points.resize(2 * n_vertices);
            std::getline(nodefile, line);
            int i = 0;
            while (nodefile >> a1 >> a2 >> a3 >> a4) {
                points[2 * i + 0] = a2;
                points[2 * i + 1] = a3;
                i++;
            }
        } else
            std::cout << "Unable to open node file";
        nodefile.close();
    }

    void change_index_vertices_using_dfs_tree(int n, int * dfs_order) {

        std::vector < double > aux(points.size());
        double x, y;
        for (int i = 0; i < n; i++) {
            aux[2 * i] = points[2 * dfs_order[i]];
            aux[2 * i + 1] = points[2 * dfs_order[i] + 1];
        }
        points = aux;
    }


    public:

    //default constructor
    compactTriangulation() {}


    //Constructor from file
    compactTriangulation(std::string node_file, std::string graph_file): pemb < > () {
        
        Graph g = read_graph_from_file(graph_file.c_str());
        //std::cout << "Graph done" << std::endl;
        read_nodes_from_file(node_file);
        //std::cout << "Read nodes done" << std::endl;

        auto t_start = std::chrono::high_resolution_clock::now();

        int * dfs_order = new int[g.vertices()];
        construct_pemb(g, dfs_order);
        change_index_vertices_using_dfs_tree(g.vertices(), dfs_order);
        //std::cout << "pemb done" << std::endl;
        //std::cout << "changing index done" << std::endl;
        n_halfedges = 2 * m_edges;
        n_vertices = m_vertices;
        auto t_end = std::chrono::high_resolution_clock::now();
        t_triangulation_generation = std::chrono::duration<double, std::milli>(t_end-t_start).count();    
        delete[] dfs_order;
    }



    double get_triangulation_generation_time() {
        return t_triangulation_generation;
    }

    long long get_size_nodes() {
        return sizeof(decltype(points.back())) * points.capacity();
    }

    long long get_size_pemb() {
        nullstream ns;
        return pemb::serialize(ns);
    }

    int halfEdges() {
        return n_halfedges;
    }

    int vertices() {
        return n_vertices;
    }

    int edges() {
        return m_edges;
    }

    int faces() {
        return pemb_faces();
    }


    double get_PointX(int i) {
        return points[2 * i];
    }

    double get_PointY(int i) {
        return points[2 * i + 1];
    }

    //Given a edge with vertex origin v, return the next coutnerclockwise edge of v
    //Input: e is the edge
    //Output: the next counterclockwise edge of v
    //Equivalent to pemb:next, but added a special case for border edges
    int CCW_edge_to_vertex(int e) {
        e = pemb::mate(e);
        size_type nxt = pemb::next(e);
        if (nxt >= n_halfedges) {
            nxt = pemb::first(pemb::vertex(e));
        }
        return pemb::mate(nxt);
    }

    //Given a edge with vertex origin v, return the prev clockwise edge of v with v as origin
    //Input: e is the edge
    //Output: the prev clockwise edge of v
    //This is a custom implementation of the function prev of pemb
    int CW_edge_to_vertex(int e) {
        e = pemb::mate(e);
        size_type prv = pemb_prev(e);
        if (prv >= n_halfedges) {
            size_type last_edge = pemb_last(pemb::vertex(e));
            //Especial case only detected in border edges
            if (last_edge == e) {
                prv = pemb::mate(e - 1);
            } else {
                return e - 1;
            }
        }
        return pemb::mate(prv);
    }

    //Calculates the next edge of the face incident to edge e
    //Input: e is the edge
    //Output: the next edge of the face incident to e
    int next(int e) {
        //e = pemb::mate(e);
        //size_type prv = CW_edge_to_vertex(e);
        //size_type mt = pemb::mate(prv);
        //return pemb::mate(CCW_edge_to_vertex(pemb::mate(CCW_edge_to_vertex(e))));
        return CW_edge_to_vertex(pemb::mate(e));
    }

    //Calculates the prev edge of the face incident to edge e
    //Input: e is the edge
    //Output: the prev edge of the face incident to e
    int prev(int e) {
        //e = pemb::mate(e);
        //size_type mt = pemb::mate(e);
        //size_type nxt = CCW_edge_to_vertex(mt);
        //return nxt;
        return pemb::mate(CCW_edge_to_vertex(e));
    }

    //Calculates the head vertex of the edge e
    //Input: e is the edge
    //Output: the head vertex v of the edge e
    //Equivalent to pemb::vertex(mate(e))
    int target(int e) {
        //if (e >= 2 * m_edges)
        //    return -1;
        //return pemb::vertex(e);
        return get_node(e);
    }
    //Calculates the tail vertex of the edge e
    //Input: e is the edge
    //Output: the tail vertex v of the edge e
    //Equivalent to pemb::vertex(e)
    int origin(int e) {
        //return pemb_last(e);
        return get_node(pemb::mate(e));
    }

    //Return the twin edge of the edge e
    //Input: e is the edge
    //Output: the twin edge of e
    //Equivalent to pemb::mate(e)
    int twin(int e) {
        return pemb::mate(e);
    }

    //return a edge associate to the node v
    //Input: v is the node
    //Output: the edge associate to the node v
    int edge_of_vertex(int v) {
        return pemb::mate(pemb::first(v));
    }
    
    //return the triangle incent to edge e
    triangle incident_face(uint e) {
        triangle face;
        uint nxt = e;
        uint init_vertex = origin(nxt);
        uint curr_vertex = -1;
        uint i = 0;
        while (i < 3) {
            curr_vertex = origin(nxt);
            face[i] = curr_vertex;
            i++;
            nxt = next(nxt);
        }
        return face;
    }

    bool is_border_face(int e) {
        return is_border(e);
    }

    bool is_interior_face(int e) {
        return !is_border(e);
    }


    //last(v): return i such that the last edge we process while visiting v is the ith we process during our traversal;
    int pemb_last(int v) {
        if (v >= 0) {
            size_type match_in_B;
            size_type pos_in_B = m_B_st.select(v + 1); // B.select0
            if (m_B[pos_in_B] == 1)
                match_in_B = m_B_st.find_close(pos_in_B);
            else
                match_in_B = m_B_st.find_open(pos_in_B);
            size_type pos = match_in_B;
            size_type edge = 0;
            if (pos)
                edge = m_A_select1(pos);
            if (v == 0) // The root of the spanning tree
                return n_halfedges - 1;
            else
                return edge; //border edge
        }
        return -1;
    }

    int pemb_prev(int i) {

        if (i < 1) { //if fist edge then return last edge as prev
            return mate(n_halfedges - 1);
        }
        size_type pos_in_B = m_A_rank(i - 1); // rank1
        if (m_A[i - 1] == 0) {
            return i - 1;
        } else if (m_A[i - 1] == 1 && m_B[pos_in_B] == 1) {
            return pemb::mate(i - 1);
        } else {
            return -1;
        }
    }



    /******************************/

    /*** Notation ***/

    // A[i]=1 means i-th half-edge belongs to the spanning tree of the primal
    // m_B[i] = 1 means the i-th parenthesis is open
    // m_B[i] = 0 means the i-th parenthesis is closed
    // m_B_star[i] = 1 means the i-th bracket is open
    // m_B_star[i] = 0 means the i-th bracket is closed

    // !Return the id of the origin node of the half-edge e
    int get_node(int e) {
        size_type pos_in_A = m_A_rank(e + 1); // rank1

        if (m_A[e] == 1) { // e is a half-edge of the primal spanning tree
            if (m_B[pos_in_A] == 0) { // e is a close parenthesis
                size_type match_pos;
                match_pos = m_B_st.find_open(pos_in_A);

                return m_B_st.rank(match_pos) - 1;
            } else { // e is an open parenthesis
                size_type par = m_B_st.parent_t(pos_in_A);
                return m_B_st.rank(par) - 1;
            }
        } else { // e is a half-edge of the dual spanning tree
            if (m_B[pos_in_A] == 1) { // The preceding parenthesis of e is an open parenthesis
                return m_B_st.rank(pos_in_A) - 1;
            } else {
                size_type match_pos;
                match_pos = m_B_st.find_open(pos_in_A);

                size_type par = m_B_st.parent_t(match_pos);
                return m_B_st.rank(par) - 1;
            }
        }
    }

    // !Return the id of the incident face to the half-edge e
    int get_face(int e) {
        size_type pos_in_A = e + 1 - m_A_rank(e + 1); // rank0

        if (m_A[e] == 0) { // e is a half-edge of the dual spanning tree
            if (m_B_star[pos_in_A] == 0) { // e is a close bracket
                size_type match_pos;
                match_pos = m_B_star_st.find_open(pos_in_A);

                return m_B_star_st.rank(match_pos) - 1;
            } else { // e is an open bracket
                size_type par = m_B_star_st.parent_t(pos_in_A);
                return m_B_star_st.rank(par) - 1;
            }
        } else { // e is a half-edge of the primal spanning tree
            if (m_B_star[pos_in_A] == 1) { // The preceding parenthesis of e is an open bracket
                return m_B_star_st.rank(pos_in_A) - 1;
            } else {
                size_type match_pos;
                match_pos = m_B_star_st.find_open(pos_in_A);

                size_type par = m_B_star_st.parent_t(match_pos);
                return m_B_star_st.rank(par) - 1;
            }
        }
    }

    bool is_border(int e) {
        size_type f = get_face(e);

        // The external face has id 0
        return f == 0;
    }

    //void get_Triangles_bitvector() {
    //    this -> triangles2 = sdsl::bit_vector(n_halfedges, false);
    //    for (size_type i = 0; i < pemb_faces(); i++)
    //        triangles2[first_dual(i)] = true;
    //}

    std::vector<int> get_Triangles(){
        return this->triangle_list();
    }

    std::vector<int> triangle_list() {
        std::vector<int> triangles(pemb_faces());
        for (size_type i = 0; i < pemb_faces(); i++)
            triangles.at(i) = first_dual(i);
        return triangles;
    }

    int degree(int v) {
        return pemb::degree(v);
    }

    double distance(int e) {
        int org = origin(e);
        int tgt = target(e);
        return pow(get_PointX(org) - get_PointX(tgt), 2) + pow(get_PointY(org) - get_PointY(tgt), 2);
        //return abs(get_PointX(org) - get_PointX(tgt)) + abs(get_PointY(org) - get_PointY(tgt));
    }

    int incident_halfedge(int f)
    {
        return first_dual(f);
    }
};

#endif