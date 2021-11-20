#ifndef INCLUDED_SDSL_PEMB
#define INCLUDED_SDSL_PEMB

#include "sdsl_concepts.hpp"
#include "int_vector.hpp"
#include "rank_support_v.hpp"
#include "select_support_mcl.hpp"
#include "wt_helper.hpp"
#include "util.hpp"
#include "bp_support_sada.hpp"
#include <algorithm> // for std::swap
#include <stdexcept>
#include <vector>
#include <stack>
#include <utility>

#include "../complementary/Vertex.hpp"
#include "../complementary/Edge.hpp"
#include "../complementary/Tree.hpp"
#include "../complementary/Graph.hpp"

//! Namespace for the succinct data structure library.
namespace sdsl
{

//! A class that provides support for planar embeddings
/*! This data structure supports the following operations:
 *   - first
 *   - mate
 *   - next
 *   - vertex
 *   - degree
 *   - face
 *   - list_neighbors
 *  An opening parenthesis in the balanced parentheses sequence B and B_star is
 *  represented by a 1 in the bit_vector and a closing parenthesis by a 0.
 *
 *  \par References
 *      - Leo Ferres, José Fuentes Sepúlveda, Travis Gagie, Meng He and Gonzalo
 *        Navarro:
 *        Fast and Compact Planar Embeddings
 *        WADS 2017
 *      - Leo Ferres, José Fuentes Sepúlveda, Travis Gagie, Meng He and Gonzalo
 *        Navarro:
 *        Fast and Compact Planar Embeddings
 *        Computational Geometry: Theory and Applications
 *
 */

  template<class t_bitvector   = bit_vector,
	   class t_succ_tree    = bp_support_sada<>,
	   class t_rank        = typename t_bitvector::rank_1_type,
	   class t_select1     = typename t_bitvector::select_1_type,
	   class t_select0     = typename t_bitvector::select_0_type>
class pemb
{
    public:
        typedef int_vector<>::size_type              size_type;
        typedef int_vector<>::value_type             value_type;
        typedef random_access_const_iterator<pemb> const_iterator;
        typedef const_iterator                       iterator;
        typedef t_bitvector                          bit_vector_type;
        typedef t_rank                               rank_1_type;
        typedef t_select1                            select_1_type;
        typedef t_select0                            select_0_type;
        typedef t_succ_tree                          succ_tree;

protected:
        size_type         m_vertices  = 0;
        size_type         m_edges  = 0;
        bit_vector_type   m_A;
        rank_1_type       m_A_rank;
        select_1_type     m_A_select1;
        select_0_type     m_A_select0;
        bit_vector_type   m_B;
        bit_vector_type   m_B_star;
        succ_tree          m_B_st;
        succ_tree          m_B_star_st;

        void copy(const pemb& p) {
            m_vertices          = p.m_vertices;
            m_edges         = p.m_edges;
            m_A       = p.m_A;
            m_A_rank  = p.m_A_rank;
            m_A_select1     = p.m_A_select1;
            m_A_select0     = p.m_A_select0;
	    m_A_rank.set_vector(&m_A);
	    m_A_select1.set_vector(&m_A);
	    m_A_select0.set_vector(&m_A);
	    m_B = p.m_B;
	    m_B_star = p.m_B_star;
	    m_B_st = p.m_B_st;
	    m_B_star_st = p.m_B_star_st;
	    m_B_st.set_vector(m_B);
	    m_B_star_st.set_vector(m_B_star);
        }


    public:

        //! Default constructor
        pemb() {};

        pemb(Graph g) {
	  m_vertices = g.vertices();
	  m_edges = g.edges();

	  int *parent = new int[m_vertices];
	  for(unsigned int i=0; i<m_vertices; i++) parent[i]=-1;
	  unsigned int *count_edges =  new unsigned int[2*(m_vertices-1)]();
	  unsigned int *references =  new unsigned int[2*(m_vertices-1)]();
	  unsigned int init = 0;

	  Tree t = g.dfs_spanning_tree(init, parent, count_edges, references);
	  
	  bit_vector_type A_local(2*m_edges,0);
	  bit_vector_type B_local(2*m_vertices,0);
	  bit_vector_type B_star_local(2*m_edges-2*m_vertices+4,0);

	  unsigned int *marked_edges = new unsigned
	    int[2*m_edges-2*m_vertices+2]();	  
	  unsigned int idx = 0;
	  unsigned int ii = 0;
	  unsigned int pos = 0;

	  for(unsigned int i = 1; i <= 2*(m_vertices-1); i++) { 
	    Edge e = t.getEdge(idx);
	    Vertex n_src = t.getNode(e.getSrc());
	    Vertex n_tgt = t.getNode(e.getTgt());

	    // precompute forward and backward edges for B*
	    unsigned int ref = references[t.getEdgeCmp(idx)];
	    unsigned int limit = ref + count_edges[t.getEdgeCmp(idx)];
	    Edge ee = g.getEdge(ref);
	    if(limit <= g.getVertexLast(ee.getSrc()))
	      for(unsigned int j = ref+1; j <= limit; j++) {
		g.setEdgeSrc(j, pos);
	  	marked_edges[pos++] = j;
	      }
	    else {
	      for(unsigned int j = ref+1; j <= g.getVertexLast(ee.getSrc());
		  j++) {
		g.setEdgeSrc(j, pos);
	  	marked_edges[pos++] = j;
	      }
      
	      limit = g.getVertexFirst(ee.getSrc()) + limit -
		g.getVertexLast(ee.getSrc());
      
	      for(unsigned int j = g.getVertexFirst(ee.getSrc()); j < limit; j++) {
		g.setEdgeSrc(j, pos);
	  	marked_edges[pos++] = j;
	      }
	    }
        
	    A_local[ii] = 1;
	    ii += count_edges[t.getEdgeCmp(idx)]+1;

	    /* Forward edges. In the root, all the edges are forward edges */
	    /* e.src == init means the root
	       t->E[parent[e.tgt]].cmp != i means a forward edge */
	    if((e.getSrc() == init) | (parent[e.getSrc()] != idx)) {
	      B_local[i] = 1;

	      /* Leaf */
	      if(n_tgt.getFirst() == n_tgt.getLast())
	  	idx = e.getCmp();
	      /* Connect to the first child of the tgt node */
	      else {
	  	unsigned int succ = parent[e.getTgt()]+1;
	  	if(succ > t.getNodeLast(e.getTgt()))
	  	  succ = t.getNodeFirst(e.getTgt());
	  	idx = succ;
	      }
	    }
	    else {
	      /* Especial case for the last child of the root */
	      /* Assumption: N[init].first in an external edge and it is part of the
	  	 spanning tree */
	      if(e.getTgt() == init && n_tgt.getLast() == e.getCmp())
	  	break;
	      else {
	  	unsigned int last = parent[e.getTgt()] - 1;
	  	if(last < t.getNodeFirst(e.getTgt()))
	  	  last = t.getNodeLast(e.getTgt());
	
	  	/* Last child of t->N[i] */
	  	if(e.getCmp() == last)
	  	  idx = parent[e.getTgt()];
	  	/* Intermediate child */
	  	else {
	  	  unsigned int next = e.getCmp() + 1;
	  	  if(next > t.getNodeLast(e.getTgt()))
	  	    next = t.getNodeFirst(e.getTgt());
	  	  idx = next;	  
	  	}
	      }
	    }
	  }
	  B_local[0] = 1;
	  
	  unsigned int ul = 2*m_edges - 2*m_vertices + 2;
	  for(unsigned int i = 0; i < ul; i++) {
	    Edge e = g.getEdge(marked_edges[i]);
	    if(e.getSrc() <= g.getEdgeSrc(e.getCmp()))
	      B_star_local[i+1] = 1;
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

        //! Copy constructor
        pemb(const pemb& g) {
            copy(g);
        }

        //! Copy constructor
        pemb(pemb&& g) {
            *this = std::move(g);
        }

        //! Assignment operator
        pemb& operator=(const pemb g) {
            if (this != &g) {
                copy(g);
            }
            return *this;
        }

        //! Assignment move operator
        pemb& operator=(pemb&& g) {
            if (this != &g) {
                m_vertices          = g.m_vertices;
                m_edges         = g.m_edges;
		
		m_A             = std::move(g.m_A);
		m_A_rank        = std::move(g.m_A_rank);
		m_A_select1     = std::move(g.m_A_select1);
		m_A_select0     = std::move(g.m_A_select0);
		m_A_rank.set_vector(&m_A);
		m_A_select1.set_vector(&m_A);
		m_A_select0.set_vector(&m_A);

		m_B             = std::move(g.m_B);
		m_B_star        = std::move(g.m_B_star);

		m_B_st          = std::move(g.m_B_st);
		m_B_st.set_vector(&m_B);

		m_B_star_st          = std::move(g.m_B_star_st);
		m_B_star_st.set_vector(&m_B_star);
            }
            return *this;
        }

        //! Swap operator
        void swap(pemb& g) {
            if (this != &g) {
                std::swap(m_vertices, g.m_vertices);
                std::swap(m_edges,  g.m_edges);
		
		m_A.swap(g.m_A);
                util::swap_support(m_A_rank, g.m_A_rank, &m_A, &(g.m_A));
                util::swap_support(m_A_select1, g.m_A_select1, &m_A, &(g.m_A));
                util::swap_support(m_A_select0, g.m_A_select0, &m_A, &(g.m_A));

		m_B.swap(g.m_B);
                util::swap_support(m_B_st, g.m_B_st, &m_B, &(g.m_B));

		m_B_star.swap(g.m_B_star);
                util::swap_support(m_B_star_st, g.m_B_star_st, &m_B_star,
				   &(g.m_B_star));
            }
        }

        //! Returns the number of vertices of the graph
        size_type vertices()const {
            return m_vertices;
        }

        //! Returns the number of edges of the graph
        size_type edges()const {
            return m_edges;
        }
  
        //! Returns a const_iterator to the first vertex.
        const_iterator begin()const {
            return const_iterator(this, 0);
        }

        //! Returns a const_iterator to the element after the last vertex.
        const_iterator end()const {
            return const_iterator(this, vertices());
        }


        //! Serializes the data structure into the given ostream
        size_type serialize(std::ostream& out, structure_tree_node* v=nullptr, std::string name="")const {
	  structure_tree_node* child = structure_tree::add_child(v, name,
								 util::class_name(*this));

	  size_type written_bytes = 0;
	  written_bytes += write_member(m_vertices, out, child, "vertices");
	  written_bytes += write_member(m_edges, out, child, "edges");

	  written_bytes += m_A.serialize(out, child, "A");
	  written_bytes += m_A_rank.serialize(out, child, "A_rank");
	  written_bytes += m_A_select1.serialize(out, child, "A_select1");
	  written_bytes += m_A_select0.serialize(out, child, "A_select0");

	  written_bytes += m_B.serialize(out, child, "B");
	  written_bytes += m_B_st.serialize(out, child, "B_succ_tree");
	  
	  written_bytes += m_B_star.serialize(out, child, "B_star");
	  written_bytes += m_B_star_st.serialize(out, child, "B_star_succ_tree");
	  
	  structure_tree::add_size(child, written_bytes);
	  return written_bytes;
        }

        //! Loads the data structure from the given istream.
        void load(std::istream& in) {
            read_member(m_vertices, in);
            read_member(m_edges, in);
	    m_A.load(in);
	    m_A_rank.load(in, &m_A);
	    m_A_select1.load(in, &m_A);
	    m_A_select0.load(in, &m_A);

            m_B.load(in);
            m_B_st.load(in, &m_B);
	    
            m_B_star.load(in);
            m_B_star_st.load(in, &m_B_star);
        }

       /* Assuming indices start with 0 */
       size_type first(size_type v) {
	 if(v >= 0) {
	   size_type pos = m_B_st.select(v+1);
	   size_type edge = 0;
	   if(pos)
	     edge = m_A_select1(pos);
	   if(v == 0) // The root of the spanning tree
	     return edge;
	   else
	     return edge+1;
	 } else
	   return -1;
       }

      /* Assuming indices start with 0 */
      size_type mate(size_type i) {
	if(m_A[i] == 1) {
	  size_type pos_in_B = m_A_rank(i+1); // rank1

	  // Simulating the match operation
	  size_type match_in_B;
	  if(m_B[pos_in_B] == 1)
	    match_in_B = m_B_st.find_close(pos_in_B);
	  else 
	    match_in_B = m_B_st.find_open(pos_in_B);
	  return m_A_select1(match_in_B);
	}
	else
	  {
	    size_type pos_in_B_star = i + 1 - m_A_rank(i+1); // rank0

	    // Simulating the match operation
	    size_type match_in_B_star;
	    if(m_B_star[pos_in_B_star] == 1)
	      match_in_B_star = m_B_star_st.find_close(pos_in_B_star);
	    else 
	      match_in_B_star = m_B_star_st.find_open(pos_in_B_star);
	    
	    return m_A_select0(match_in_B_star);
	  }  
	return -1;
      }

      /* Assuming indices start with 0 */
      size_type next(size_type i) {
	if(i > m_A.size())
	  return -1;
	
	if(m_A[i] == 0) {
	  return i+1;
	}
	else {
	  size_type pos_in_B = m_A_rank(i+1); // rank1
	  if(m_B[pos_in_B] == 1) {
	    return mate(i) + 1;
	  }
	}
	return -1;
      }
  
      size_type degree(size_type v) {
	if(v >= m_vertices)
	  return 0;
	
	size_type dg = 0;
	size_type nxt = first(v);
	while(nxt < 2*m_edges) {
	  nxt = next(nxt);
	  dg++;
	}
	return dg;
      }

      size_type vertex(size_type e) {
      	size_type pos_in_A = m_A_rank(e+1); // rank1
      	if(m_A[e] == 1) {
      	  if(m_B[pos_in_A] == 0) {
      	    size_type match_pos;
      	    if(m_B[pos_in_A] == 1)
      	      match_pos = m_B_st.find_close(pos_in_A);
      	    else 
      	      match_pos = m_B_st.find_open(pos_in_A);
	    
      	    return m_B_st.rank(match_pos) - 1;
      	  }
      	  else {
	    size_type par = m_B_st.parent_t(pos_in_A);
      	    return m_B_st.rank(par) - 1;
      	  }
      	}
      	else {
      	  if(m_B[pos_in_A] == 1) {
      	    return m_B_st.rank(pos_in_A) - 1;
      	  }
      	  else {
      	    size_type match_pos;
      	    if(m_B[pos_in_A] == 1)
      	      match_pos = m_B_st.find_close(pos_in_A);
      	    else 
      	      match_pos = m_B_st.find_open(pos_in_A);
	    
	    size_type par = m_B_st.parent_t(match_pos);
      	    return m_B_st.rank(par) - 1;
      	  }
      	}
      }

      // Traversal of the neighbors of vertex v. If necessary, return all values
      // of variable x to obtain the neighbors in a appropriate format
      void list_neighbors(size_type v) {
	if(v >= m_vertices)
	  return;
	
	size_type nxt = first(v);
	while(nxt < 2*m_edges) {
	  if(nxt < 2*m_edges) {
	    size_type mt = mate(nxt);
	    size_type x = vertex(mt); // Print neighbor
	  }
	  nxt = next(nxt);	
	}
      }

      // Traversal of the face where the edge e belongs. If necessary, return
      // all values of variable curr_vertex to obtain the vertices of the face
      // in a appropriate format
      void face(size_type e) {
	if(e >= 2*m_edges)
	  return;
	char flag = 1;
	size_type nxt = e;
	size_type mt;
	size_type init_vertex = vertex(nxt);
	size_type curr_vertex = -1;

	while(curr_vertex != init_vertex || flag) {
	  if(nxt >= 2*m_edges) {
	    nxt = first(vertex(mt));
	  }
	  
	  flag = 0;
	  mt = mate(nxt);
	  curr_vertex = vertex(mt);
	  nxt = next(mt);
	}
      }
  };

}// end namespace sdsl
#endif
