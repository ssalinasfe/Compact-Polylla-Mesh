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

class compressTriangulation : public pemb<>
{

public:
    typedef std::array<uint,3> triangle; 
    std::vector<double> points; //nodes
    sdsl::bit_vector triangles; //indices of edges to a unique triangle
    size_type n_halfedges = 0; //number of halfedges
    size_type n_faces = 0; //number of faces
    size_type n_vertices = 0; //number of vertices


private:
    /* methods used to for the construct */
    //Generate compress planar embedding graph
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
        double a1, a2, a3, a4;
        
        //std::cout<<"Node file"<<std::endl;
        if (nodefile.is_open())
        {
            nodefile >> n_vertices;
            //std::cout<<pnumber<<std::endl;
            points.resize(2*n_vertices);
            std::getline(nodefile, line); 
            int i = 0;
            while (nodefile >> a1 >> a2 >> a3 >> a4)
            {
                points[2*i + 0] = a2;
                points[2*i + 1] = a3;
                i++;
            }
        }
        else 
            std::cout << "Unable to open node file"; 
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
                    //std::cout << "Pushing " << tgt << std::endl;
                    //std::cout << "moving vertex " << k  << " to pos "<< tgt << std::endl;
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
        for (int e = 0; e < this->n_halfedges; e++){
            if(this->is_border_face(e))
                this->triangles[e] = false;
            if(this->triangles[e] == true){
                face = this->incident_face(e);
                for (int i = e + 1; i < n_halfedges; i++){
                    if(this->is_interior_face(i) && this->triangles[i] == true){
                        face_aux = this->incident_face(i);
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

public:

    //default constructor
    compressTriangulation() {}

    //Constructor from file
    compressTriangulation(std::string node_file, std::string graph_file) : pemb<>() {
        
        Graph g = read_graph_from_file(graph_file.c_str());
        g.printgraph();
        std::cout << "Graph done" << std::endl;
        construct_pemb(g);
        std::cout << "pemb graph done" << std::endl;
        //read nodes from file
        read_nodes_from_file(node_file);
        std::cout << "reading nodes done" << std::endl;
        change_index_vertices_using_dfs_tree(g);
        std::cout << "changing index done" << std::endl;
        n_halfedges = 2*m_edges;
        n_vertices = m_vertices;
        std::cout<<"n_halfedges "<<n_halfedges<<std::endl;
        this->triangles = sdsl::bit_vector(n_halfedges, true);
        generate_list_of_triangles();
        for(int i = 0; i < n_halfedges; i++){
            if(this->triangles[i] == true){
                n_faces++;
            }
        }
        std::cout << "generating triangle list done" << std::endl;
        //cout<<"points:"<<endl;
        //for(int i = 0; i < m_vertices; i++){
        //    std::cout<<"point "<<i<<": "<<points[2*i+0]<<" "<<points[2*i+1]<<std::endl;
        //}
    }

    //copy constructor
    compressTriangulation(const compressTriangulation& ct) : pemb<>() {
        cout<<"called copy constructor"<<endl;
        m_vertices          = ct.m_vertices;
        m_edges         = ct.m_edges;
        m_A       = ct.m_A;
        m_A_rank  = ct.m_A_rank;
        m_A_select1     = ct.m_A_select1;
        m_A_select0     = ct.m_A_select0;
	    m_A_rank.set_vector(&m_A);
	    m_A_select1.set_vector(&m_A);
	    m_A_select0.set_vector(&m_A);
	    m_B = ct.m_B;
	    m_B_star = ct.m_B_star;
	    m_B_st = ct.m_B_st;
	    m_B_star_st = ct.m_B_star_st;
	    m_B_st.set_vector(&m_B);
	    m_B_star_st.set_vector(&m_B_star);   
        points = ct.points;
        triangles = ct.triangles;
        n_halfedges = ct.n_halfedges;
        
    }

    size_type halfEdges(){
        return n_halfedges;
    }

    size_type vertices(){
        return n_vertices;
    }

    size_type edges(){
        return m_edges;
    }

    size_type faces(){
        return n_faces;
    }

    bit_vector get_Triangles(){
        return triangles;
    }

    double get_PointX(size_type i){
        return points[2*i];
    }

    double get_PointY(size_type i){
        return points[2*i+1];
    }
    //Given a edge with vertex origin v, return the next coutnerclockwise edge of v
    //Input: e is the edge
    //Output: the next counterclockwise edge of v
    //Equivalent to pemb:next, but added a special case for border edges
    size_type CCW_edge_to_vertex(size_type e)
    {   
        e = pemb::mate(e);
        size_type prv = e;
        size_type nxt = pemb::next(e);
        if (nxt >= n_halfedges)
        {
            nxt = pemb::first(pemb::vertex(prv));
        }
        return pemb::mate(nxt);
    } 

    //Given a edge with vertex origin v, return the prev clockwise edge of v with v as origin
    //Input: e is the edge
    //Output: the prev clockwise edge of v
    //This is a custom implementation of the function prev of pemb
    size_type CW_edge_to_vertex(size_type e)
    {
        e = pemb::mate(e);
        size_type prev = pemb_prev(e);
        if (prev >= n_halfedges)
        {
            size_type last_edge = pemb_last(pemb::vertex(e));
            if(last_edge == e) //Especial case only detected in border edges
                prev = pemb::mate(e - 1);
            else
                prev = last_edge;
        }
        return  pemb::mate(prev);
    }



    //Calculates the next edge of the face incident to edge e
    //Input: e is the edge
    //Output: the next edge of the face incident to e
    size_type next(size_type e){
        //e = pemb::mate(e);
        //size_type prv = CW_edge_to_vertex(e);
        //size_type mt = pemb::mate(prv);
        //return pemb::mate(CCW_edge_to_vertex(pemb::mate(CCW_edge_to_vertex(e))));
        return CW_edge_to_vertex(pemb::mate(e));
    }

    //Calculates the prev edge of the face incident to edge e
    //Input: e is the edge
    //Output: the prev edge of the face incident to e
    size_type prev(size_type e){
        //e = pemb::mate(e);
        //size_type mt = pemb::mate(e);
        //size_type nxt = CCW_edge_to_vertex(mt);
        //return nxt;
        size_type nxt = pemb::mate(CCW_edge_to_vertex(e));
    }

    //Calculates the head vertex of the edge e
    //Input: e is the edge
    //Output: the head vertex v of the edge e
    //Equivalent to pemb::vertex(mate(e))
    size_type target(size_type e)
    {
        if (e >= 2 *m_edges)
            return -1;
        return pemb::vertex(e);
    }
    //Calculates the tail vertex of the edge e
    //Input: e is the edge
    //Output: the tail vertex v of the edge e
    //Equivalent to pemb::vertex(e)
    size_type origin(size_type e)

    {
        size_type nxt = e;
        size_type mt = pemb::mate(nxt);
        size_type curr_vertex = pemb::vertex(mt);
        return curr_vertex;
    }

    //Return the twin edge of the edge e
    //Input: e is the edge
    //Output: the twin edge of e
    //Equivalent to pemb::mate(e)
    size_type twin(size_type e)
    {
        return pemb::mate(e);
    }


    //return a edge associate to the node v
    //Input: v is the node
    //Output: the edge associate to the node v
    size_type edge_of_vertex(size_type v)
    {
        return pemb::first(v);
    }
    //return the triangle incent to edge e
    triangle incident_face(uint e)
    {   
        triangle face;  
        uint nxt = e;
        uint init_vertex = origin(nxt);
        uint curr_vertex = -1;
        uint i = 0;
        while ( i < 3 ){
            curr_vertex = origin(nxt);
            face[i] = curr_vertex;
            i++;
            nxt = next(nxt);            
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
        size_type init_vertex = pemb::vertex(nxt);
        size_type curr_vertex = -1;
        size_type i = 0;
        while (curr_vertex != init_vertex || flag)
        {
            if (nxt >= 2 *m_edges)
            {
                nxt = pemb::first(pemb::vertex(mt));
            }

            flag = 0;
            mt = pemb::mate(nxt);
            curr_vertex = pemb::vertex(mt);
            i++;
            nxt = pemb::next(mt);
           // cout<<"edge "<<e<<" curr_vertex "<<curr_vertex<<" i "<<i<<endl;
            
            if (i > 4)
                return true;
        }
        return false;
    }

    // Input: edge e of compressTriangulation
    // Output: true if the edge is an interior face a
    //         false otherwise
     bool is_interior_face(size_type e)
    {
       return !this->is_border_face(e);
    }    


    //last(v): return i such that the last edge we process while visiting v is the ith we process during our traversal;
    size_type pemb_last(size_type v)
    {
        if(v >= 0){
            size_type match_in_B;
            size_type pos_in_B = m_B_st.select(v + 1);	// B.select0
            if (m_B[pos_in_B] == 1)
                match_in_B = m_B_st.find_close(pos_in_B);
            else
                match_in_B = m_B_st.find_open(pos_in_B);

            size_type pos = match_in_B;
            size_type edge = 0;
            if (pos)
                edge = m_A_select1(pos);
            if (v == 0)	// The root of the spanning tree
                return n_halfedges - 1;
            else
                return edge ; //border edge
        }
        return -1;
    }

    size_type pemb_prev(size_type i)
    {
        if(i < 1){ //if fist edge then return last edge as prev
            return mate(n_halfedges - 1);
        }
        size_type pos_in_B = m_A_rank(i - 1);	// rank1
        if (m_A[i-1] == 0){
            //std::cout<<" (case 1) ";
            return i - 1;
        }else if(m_A[i-1] == 1 && m_B[pos_in_B]  == 1){
            //std::cout<<" (case 2) ";
            return pemb::mate(i - 1);
        }else{
            return -1;
        }
    }

    /*
    //Given a edge with vertex origin v, return the prev clockwise edge of v with v as origin
    //Input: e is the edge
    //Output: the prev clockwise edge of v
    //This is a custom implementation of the function prev of pemb
    size_type CW_edge_to_vertex(size_type i)
    {
        if(i < 1){ //if fist edge then return last edge as prev
            return mate(n_halfedges - 1);
        }
        size_type pos_in_B = m_A_rank(i - 1);	// rank1
        if (m_A[i-1] == 0){
            //std::cout<<" (case 1) ";
            return i - 1;
        }else if(m_A[i-1] == 1 && m_B[pos_in_B]  == 1){
            //std::cout<<" (case 2) ";
            return pemb::mate(i - 1);
        }else{
            //std::cout<<" (case 3 vertex "<<pemb::vertex(i)<<" last "<<pemb_last(pemb::vertex(i))<<") ";
            size_type last_edge = pemb_last(pemb::vertex(i));
            if(last_edge == i) //Especial case only detected in border edges
                return mate(i - 1);
            else
                return last_edge;
        }
    }

    

    size_type pemb_prev(size_type i)
    {
        if(i < 1){ //if fist edge then return last edge as prev
            return mate(n_halfedges - 1);
        }
        size_type pos_in_B = m_A_rank(i - 1);	// rank1
        if (m_A[i-1] == 0){
            //std::cout<<" (case 1) ";
            return i - 1;
        }else if(m_A[i-1] == 1 && m_B[pos_in_B]  == 1){
            //std::cout<<" (case 2) ";
            return pemb::mate(i - 1);
        }else{
            //std::cout<<" (case 3 vertex "<<pemb::vertex(i)<<" last "<<pemb_last(pemb::vertex(i))<<") ";
            size_type last_edge = pemb_last(pemb::vertex(i));
            if(last_edge == i) //Especial case only detected in border edges
                return mate(i - 1);
            else
                return last_edge;
        }
    }
    //Given a edge with vertex origin v, return the next coutnerclockwise edge of v
    //Input: e is the edge
    //Output: the next counterclockwise edge of v
    size_type CCW_edge_to_vertex(size_type e)
    {
        size_type prev = e;
        size_type nxt = pemb::next(e);
        if (nxt >= n_halfedges)
        {
            nxt = pemb::first(pemb::vertex(prev));
        }
        return nxt;
    } 
    */

    ~compressTriangulation() {};
};

#endif

