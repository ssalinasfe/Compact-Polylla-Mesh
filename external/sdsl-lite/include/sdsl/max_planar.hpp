#ifndef INCLUDED_SDSL_MAX_PLANAR_G
#define INCLUDED_SDSL_MAX_PLANAR_G

#include "int_vector.hpp"
#include "rrr_vector.hpp"
#include "util.hpp"

#include <stack>
#include <map>
#include <set>
#include <utility>
#include <stdexcept>

namespace sdsl
{
  template<class t_parseq     = rrr_vector<>,
	   class t_rank_parseq       = typename t_parseq::rank_1_type,
	   class t_select_parseq       = typename t_parseq::select_1_type>
class max_planar_rmmt
{
    public:
        typedef bit_vector::size_type                size_type;
        typedef t_parseq                             parseq_type;
        typedef t_rank_parseq                        rank_1_parseq_type;
        typedef t_select_parseq                      select_1_parseq_type;
    private:
        size_type         m_size; // length of m_S
        size_type         m_vertices  = 0;
        size_type         m_edges  = 0;
        int32_t           m_height  = 0; // height of the rmMt
        uint32_t          m_num_internal  = 0; // height of the rmMt
        uint32_t          m_num_leaves  = 0; // Leaves of the rmMt
        uint32_t          m_bs = 128;
  
        parseq_type       m_S;
        rank_1_parseq_type m_S_rank;
        select_1_parseq_type m_S_select;

        parseq_type       m_S2;
        rank_1_parseq_type m_S2_rank;

        // Range min-max tree of left net excess
        int_vector<> leaves_min;
        int_vector<> leaves_max;
        int_vector<> internal_min;
        int_vector<> internal_max;

        // Range min-max tree of empty excess
        int_vector<> leaves_min_bre;
        int_vector<> leaves_max_bre;
        int_vector<> internal_min_bre;
        int_vector<> internal_max_bre;
        int_vector<> leaves_min_bre_empty;
        int_vector<> internal_min_bre_empty;
  
  public:

        max_planar_rmmt() {}
  
        //! Constructor
        explicit
        max_planar_rmmt(Graph g, int32_t bs) {
	  m_vertices = g.vertices();
	  m_edges = g.edges();
	  m_bs = bs;

	  if(m_vertices <= 0) {
	    cerr << "The input graph has not vertices" << endl;
	    exit(EXIT_FAILURE);
	  }
	  if(m_S.block_size != m_S2.block_size) {
	    cerr << "m_S and m_S2 have different block size (" << m_S.block_size
	    << " != " << m_S2.block_size << ")" << endl;
	    exit(EXIT_FAILURE);
	  }
	  if((m_bs % m_S.block_size) != 0) {
	    cerr << "The block size of Max Planar (" << m_bs << ") is not a "
	    "multiple of the block size of RRR bitvectors (" << m_S.block_size
		 << "). Try a block size of " << ceil(m_bs/m_S.block_size)*m_S.block_size<< endl;
	    exit(EXIT_FAILURE);
	  }

	  int_vector<> ps_tree = g.ps_tree_encoding();
	  bit_vector local_S;
	  bit_vector local_S2;

	  m_size = ps_tree.size();

	  local_S.resize(m_size);
	  local_S2.resize(m_size);
	  
	  util::set_to_value(local_S, 0);
	  util::set_to_value(local_S2, 0);
	  for(size_type i=0; i < m_size; i++)
	    if(ps_tree[i] == 1) local_S[i] = 1; // Opening parenthesis
	    else if(ps_tree[i] == 0) local_S2[i] = 1; // Closing parenthesis

	  parseq_type succ_S(local_S);
	  parseq_type succ_S2(local_S2);

	  m_S.swap(succ_S);
	  m_S2.swap(succ_S2);
	  util::init_support(m_S_rank, &m_S);
	  util::init_support(m_S_select, &m_S);
	  util::init_support(m_S2_rank, &m_S2);

	  m_num_leaves = ceil((double)m_size/m_bs);

	  leaves_min.width(32);
	  leaves_max.width(32);
	  leaves_min_bre.width(32);
	  leaves_max_bre.width(32);
	  leaves_min_bre_empty.width(32);

	  leaves_min.resize(m_num_leaves);
	  leaves_max.resize(m_num_leaves);

	  leaves_min_bre.resize(m_num_leaves);
	  leaves_max_bre.resize(m_num_leaves);
	  leaves_min_bre_empty.resize(m_num_leaves);

	  uint32_t bre = 2; // By definition of the excess

	  uint32_t min_bre = bre;
	  uint32_t max_bre = bre;
	  uint32_t min_bre_empty = bre; // minimum value for empty-entries
	  char flag_empty = 0; // Flag used to check if a block contains an empty symbol
	  
	  uint32_t selected_leaf = (m_size-5)/m_bs;
	  uint32_t ul = selected_leaf*m_bs;

	  // Omit the last three zeros
	  for(uint32_t j=ul, k=m_size-5; j <= m_size-5; j++, k--) {
	    if(ps_tree[k] == 2) {
	      bre++; // Empty
	      flag_empty = 1;
	      if(bre < min_bre_empty)
		min_bre_empty = bre;
	    }
	    else bre--; // Opening or closing parenthesis
	    
	    if(bre < min_bre)
	      min_bre = bre;
	    else if(bre > max_bre)
	      max_bre = bre;
	  }
	  
	  leaves_min_bre[selected_leaf] = min_bre;
	  leaves_max_bre[selected_leaf] = max_bre;
	  if(flag_empty)
	    leaves_min_bre_empty[selected_leaf] = min_bre_empty;
	  else // The current block does not contain empty symbols
	    leaves_min_bre_empty[selected_leaf] = max_bre + 1;

	  ul = selected_leaf*m_bs-1;

	  for(int32_t i=selected_leaf-1, k=ul; i >= 0 ; i--) {
	    uint32_t min_bre = 0;
	    uint32_t max_bre = 0;
	    uint32_t min_bre_empty = 0;
	    flag_empty = 0;
	    for(uint32_t j=0; j < m_bs && k >= 1; j++, k--) {
	      if(ps_tree[k] == 2) { // Empty
		bre++;
		flag_empty = 1;
		if(!min_bre_empty)
		  min_bre_empty = bre;
		else if(bre < min_bre_empty)
		  min_bre_empty = bre;
	      }
	      else bre--; // Opening or closing parenthesis

	      if(!j) {
		min_bre = bre;
		max_bre = bre;
	      }
	      else if(bre < min_bre)
		min_bre = bre;
	      else if(bre > max_bre)
		max_bre = bre;		  
	    }

	    leaves_min_bre[i] = min_bre;
	    leaves_max_bre[i] = max_bre;
	    if(flag_empty)
	      leaves_min_bre_empty[i] = min_bre_empty;
	    else // The current block does not contain empty symbols
	      leaves_min_bre_empty[i] = max_bre + 1;
	  }
	  
	  uint32_t nle = 0;
	  for(uint32_t i=0, k=0; i < m_num_leaves; i++) {
	    uint32_t min_nle = 0;
	    uint32_t max_nle = 0;
	    for(uint32_t j=0; j < m_bs && k < m_size; j++, k++) {
	      if(ps_tree[k] == 1) nle += 3;
	      else nle -= 1; // Zero or empty

	      if(!j) {
		min_nle = nle;
		max_nle = nle;
	      }
	      else if(nle < min_nle)
		min_nle = nle;
	      else if(nle > max_nle)
		max_nle = nle;		  
	    }

	    leaves_min[i] = min_nle;
	    leaves_max[i] = max_nle;	    
	  }
	  
	  
	  m_height = ceil(log(m_num_leaves)/log(2));
	  m_num_internal = (pow(2,m_height)-1); // Number of internal nodes;
	  
	  internal_min.width(32);
	  internal_max.width(32);
	  internal_min_bre.width(32);
	  internal_max_bre.width(32);
	  internal_min_bre_empty.width(32);

	  internal_min.resize(m_num_internal);
	  internal_max.resize(m_num_internal);

	  internal_min_bre.resize(m_num_internal);
	  internal_max_bre.resize(m_num_internal);
	  internal_min_bre_empty.resize(m_num_internal);

	  util::set_to_value(internal_min, 0);
	  util::set_to_value(internal_max, 0);
	  
	  // Number of nodes in the top level of the rmMt (in the last height-t levels)
	  uint32_t top_nodes = pow(2, m_height-1)-1;

	  // Filling the last level of the rmMt
	  for(uint32_t i=0; i < m_num_leaves; i+=2) {
	    uint32_t min = leaves_min[i];
	    uint32_t max = leaves_max[i];
	    uint32_t min_bre = leaves_min_bre[i];
	    uint32_t max_bre = leaves_max_bre[i];
	    uint32_t min_bre_empty = leaves_min_bre_empty[i];

	    if(i+1 < m_num_leaves) {
	      min = (leaves_min[i+1] < min)? leaves_min[i+1] : min;
	      max = (leaves_max[i+1] > max)? leaves_max[i+1] : max;
	      
	      min_bre = (leaves_min_bre[i+1] < min_bre)? leaves_min_bre[i+1] : min_bre;
	      max_bre = (leaves_max_bre[i+1] > max_bre)? leaves_max_bre[i+1] : max_bre;
	      min_bre_empty = (leaves_min_bre_empty[i+1] < min_bre_empty)?
		leaves_min_bre_empty[i+1] : min_bre_empty;
	    }

	    uint32_t internal = i/2;

	    internal_min[top_nodes + internal] = min;
	    internal_max[top_nodes + internal] = max;

	    internal_min_bre[top_nodes + internal] = min_bre;
	    internal_max_bre[top_nodes + internal] = max_bre;
	    internal_min_bre_empty[top_nodes + internal] = min_bre_empty;
	  }

	  // Filling the rest of the rmMt
  	  for(int lvl=m_height-2; lvl >= 0 ; lvl--){
	    
	    int32_t num_curr_nodes = pow(2, lvl);
	    int32_t node = 0;
    
	    for(node = 0; node < num_curr_nodes; node++) {
	      int32_t pos = (pow(2,lvl)-1) + node; // Position in the final array of 'node'
	      
	      internal_min[pos] = std::min(internal_min[2*pos+1], internal_min[2*(pos+1)]);
	      internal_max[pos] = std::max(internal_max[2*pos+1], internal_max[2*(pos+1)]);

	      internal_min_bre[pos] = std::min(internal_min_bre[2*pos+1], internal_min_bre[2*(pos+1)]);
	      internal_max_bre[pos] = std::max(internal_max_bre[2*pos+1], internal_max_bre[2*(pos+1)]);
	      internal_min_bre_empty[pos] = std::min(internal_min_bre_empty[2*pos+1],
						     internal_min_bre_empty[2*(pos+1)]);
	    }
	  }

	  util::bit_compress(internal_min);
	  util::bit_compress(internal_max);
	  util::bit_compress(leaves_min);
	  util::bit_compress(leaves_max);
	  util::bit_compress(internal_min_bre);
	  util::bit_compress(internal_max_bre);
	  util::bit_compress(leaves_min_bre);
	  util::bit_compress(leaves_max_bre);
	  util::bit_compress(internal_min_bre_empty);
	  util::bit_compress(leaves_min_bre_empty);
        }

        //! Copy constructor
        max_planar_rmmt(const max_planar_rmmt& mp) {
            copy(mp);
        }

        //! Move constructor
        max_planar_rmmt(max_planar_rmmt&& mp) {
            *this = std::move(mp);
        }

        //! Assignment operator
        max_planar_rmmt& operator=(const max_planar_rmmt& mp) {
            if (this != &mp) {
                copy(mp);
            }
            return *this;
        }

        size_type vertices() const {
            return m_vertices;
        }

        size_type edges() const {
            return m_edges;
        }

        size_type size() const {
            return m_size;
        }  

        // Return the index in S of the vertex i
        int64_t vertex_pos(uint64_t i) const {
	  return m_S_select(i+1);
	}

        // Return id of the visited vertex when traversing S[i]
        int64_t vertex_id(uint64_t i) const {
	  if(m_S[i]) return m_S_rank(i);
	  else if(m_S2[i]) return m_S_rank(find_open(i));
	  else return -1; // TODO
	}


        // Return the position of the first child of vertex i in the PS-tree
        // Note: the index i corresponds to the position of i in the string S
        int64_t first_child(uint64_t i)const {
	  assert(i < m_size);

	  if(!m_S[i]) return -1;

	  if(m_S[i+1]) return i+1;
	  else if(m_S[i+2]) return i+2;
	  else if(m_S[i+3]) return i+3;
	  else return i; // vertex i is a leaf	  
	}


        // Return the position of the next sibling of vertex i
        // Note: the index i corresponds to the position of i in the string S
        int64_t next_sibling(uint64_t i)const {
	  assert(i < m_size);

	  if(!m_S[i]) return -1;

	  size_type match = (size_type)find_close(i);
	  int64_t par = (size_type)parent(i);

	  // the root has no siblings
	  if(par == -1) return i;
	  
	  size_type match_par = (size_type)find_close(par);

	  if(match + 3 >= match_par)
	    return i; // vertex i is the last sibling

	  if(match+1 < m_size && m_S[match+1]) return match+1;
	  else if(match+2 < m_size && m_S[match+2]) return match+2;
	  else if(match+3 < m_size && m_S[match+3]) return match+3;
	  else return i; // vertex i is the last sibling
	}
  
  

        int64_t find_close(uint64_t i)const {
	  assert(i < m_size);
	  if (!m_S[i]) // if there is a closing parenthesis or an empty symbol at index i, then return i
	    return i;

	  if(i < 3)  return m_size-i-1;

	  uint32_t ones_i = m_S_rank(i+1); // Number of 1-bits up to i (included)
	  uint32_t zeros_i = (i+1) - ones_i;
	  uint32_t nle = 3*ones_i - zeros_i; // net left excess of S[i]
	  uint32_t target = nle-3; // Target value
	  
	  return fwd_search(i, nle, target);
	}


        int64_t find_first_empty(uint64_t i)const {
	  assert(i < m_size);
	  if (!m_S[i]) // if there is a closing parenthesis or an empty symbol at index i, then return i
	    return i;

	  if(i < 2)  return -1;
	  if(i == 2) return m_size - 4;
	  
	  uint32_t ones_i = m_S_rank(i+1); // Number of 1-bits up to i (included)
	  uint32_t zeros_i = (i+1) - ones_i;
	  uint32_t nle = 3*ones_i - zeros_i; // net left excess of S[i]
	  uint32_t target = nle-1; // Target value
	  
	  return fwd_search(i, nle, target);
	}

        int64_t find_second_empty(uint64_t i)const {
	  assert(i < m_size);
	  if (!m_S[i]) // if there is a closing parenthesis or an empty symbol at index i, then return i
	    return i;
	  
	  if(i < 3)  return -1;
	  
	  uint32_t ones_i = m_S_rank(i+1); // Number of 1-bits up to i (included)
	  uint32_t zeros_i = (i+1) - ones_i;
	  uint32_t nle = 3*ones_i - zeros_i; // net left excess of S[i]
	  uint32_t target = nle-2; // Target value
	  
	  return fwd_search(i, nle, target);
	}


        int64_t find_open(uint64_t i)const {
	  assert(i < m_size);
	  if (m_S[i]) // if there is an opening parenthesis at index i, then return i
	    return i;

	  if(i > m_size-4) return m_size - i - 1;

	  uint32_t ones_i = m_S_rank(i+1); // Number of 1-bits up to i (included)
	  uint32_t zeros_i = (i+1) - ones_i;
	  uint32_t nle = 3*ones_i - zeros_i; // net left excess of S[i]
	  uint32_t target = nle; // Target value

	  return bwd_search(i, nle, target);
	}


        int64_t find_open_empty(uint64_t i)const {
	  if (i > m_size || m_S[i]) // if there is an opening parenthesis at index i, then return i
	    return i;

	  // Check if S[i] is an empty symbol
	  if(m_S2[i] == 1)
	    return i;

	  // The expanded formula of bre is
	  // bre(i) = Nx - N1 - N0 + n1(i) + n0(i) - nx(i) + 4
	  // Where:
	  //   - Nx: Total amount of empty symbols (2n-5)
	  //   - N1: Total amount of 1 symbols (n)
	  //   - N0: Total amount of 0 symbols (n)
	  //   - nx(i): Number of empty symbols up to i (excluded)
	  //   - n1(i): Number of 1 symbols up to i (excluded)
	  //   - n0(i): Number of 0 symbols up to i (excluded)
	  //   - the last term '+4' is necessary to force the last empty symbol
	  //     to have bre=2
	  uint32_t ones_i = m_S_rank(i); // Number of 1-bits up to i (excluded)
	  uint32_t zeros_i = m_S2_rank(i); // Number of 0-bits up to i (excluded)
	  uint32_t bre = 2*ones_i + 2*zeros_i - i - 1; // New net right excess
	                                               // of S[i] to find the
	                                               // matching position of an empty 
	  uint32_t target = bre - 2; // Target value

	  int64_t bwd = bwd_search_empty(i, bre, target);
	  
	  if(m_S[bwd])
	    return parent(bwd);

	  return find_open(bwd);
	}

        // Find the rightmost empty symbol depending on the position i
        // The symbol at position i must be 1 or 0
        int64_t find_rightmost_dependency(uint64_t i)const {
	  assert(i < m_size);
	  if (!m_S[i] && !m_S2[i])
	    return -1;

	  // By proposition 5
	  if (!m_S[i+1] && !m_S2[i+1])
	    return i;

	  // by definition
	  if(i == 0)  return i;
	  if(i == 1)  return m_size - 4;
	  if(i > m_size-4) return i;
	  
	  // To see the details of the formula, go to the function find_open_empty()
	  uint32_t ones_i = m_S_rank(i); // Number of 1-bits up to i (excluded)
	  uint32_t zeros_i = m_S2_rank(i); // Number of 0-bits up to i (excluded)
	  uint32_t bre = 2*ones_i + 2*zeros_i - i - 1; // New net right excess
	                                               // of S[i] to find the
	                                               // matching position of an empty 
	  uint32_t target = bre + 1; // Target value

	  return fwd_search_empty_bre(i, bre, target);
	}

        // Find the position of the previous empty symbol depending on the same
        // vertex than the empty symbol at position i
        // The symbol at position i must be an empty symbol
        int64_t find_prev_empty(uint64_t i)const {
	  assert(i < m_size);
	  if (m_S[i] || m_S2[i])
	    return -1;

	  // To see the details of the formula, go to the function find_open_empty()
	  uint32_t ones_i = m_S_rank(i); // Number of 1-bits up to i (excluded)
	  uint32_t zeros_i = m_S2_rank(i); // Number of 0-bits up to i (excluded)
	  uint32_t bre = 2*ones_i + 2*zeros_i - i - 1; // New net right excess
	                                               // of S[i] to find the
	                                               // matching position of an empty 
	  uint32_t target = bre; // Target value

	  return bwd_search_empty_bre(i, bre, target);
	}
  

        // Return the parent of vertex i in the PS-tree
        // Find the rightmost' j<i, such that nle(j) <= nle(i)-4. Then, the
        // answer is j+1
        // Note: the index i corresponds to the position of i in the string S
        int64_t parent(uint64_t i)const {
	  assert(i < m_size);
	  if(i == 0) return -1;

	  uint32_t ones_i = m_S_rank(i+1); // Number of 1-bits up to i (included)
	  uint32_t zeros_i = (i+1) - ones_i;
	  uint32_t nle = 3*ones_i - zeros_i; // net left excess of S[i]
	  uint32_t target = nle-4; // Target value
	  
	  return bwd_search(i, nle, target);
	}
  
        int64_t fwd_search(uint64_t i, uint32_t nle, uint32_t target)const {
	  
	  int32_t block = i / m_bs;
	  uint64_t end = (block+1)*m_bs;
	  // Assumption m_bs is multiple of rrr_bs
	  int32_t rrr_bs = m_S.block_size;
	  int32_t r = m_bs/rrr_bs;
	  uint64_t mask = (uint64_t)1;
	  
	  // 1: Find the answer in the same block
	  // TODO: Improve the performance of the case 1 replace the access
	  // m_S[i] by bit operations
	  uint32_t excess = nle;
	  for (uint64_t j=i+1; j < end; ++j) {
	    if (m_S[j]) excess += 3;
	    else excess--;
	    
	    if (excess == target)
	      return j;
	  }

	  // 2: Find the answer in the right sibling
	  if(block%2 == 0) { // The current block has a right sibling
	    // The answer is in the right sibling of the current node
	    if(leaves_min[block+1] <= target && target <= leaves_max[block+1]) { 

	      // The excess value after point 1 is used here
	      for(int32_t ri=0, id=end; ri < r; ri++, id+=rrr_bs) {
		uint16_t bits1 = m_S.get_class(id);
		uint16_t bits0 = rrr_bs - bits1;
		int32_t min_e = excess - bits0;
		if(min_e <= (int32_t)target) {
		  uint64_t val = m_S.get_int(id, rrr_bs);
		  int64_t ul = id+rrr_bs;
		  for (int64_t j=id; j < ul; ++j) {
		    if ((val & mask)) excess += 3;
		    else excess--;
		    
		    if (excess == target)
		      return j;
		    val >>= 1;
		  }
		}
		else
		  excess += (3*bits1 - bits0);
	      }
	    }
	  }

	  // 3: It is necessary up and then down in the rmMt
	  uint32_t top_nodes = pow(2, m_height-1)-1;

	  uint32_t node = top_nodes + block/2; // Initial node
	  // Go up the tree
	  while (node != 0) { // while node is not the root
	    if (node%2) { // if the node is a left child
	      node++; // choose right sibling
	      
	      if (internal_min[node] <= target && target <= internal_max[node])
		break;
	    }
	    node = (node-1)/2; // choose parent
	  }

	  // Go down the tree
	  if (node != 0) { // found solution for the query
	    node = 2*node + 1; // choose left child
	    while (node < m_num_internal) { // while node is not a leaf
	      if (!(internal_min[node] <= target && target <= internal_max[node])) {
		node++; // choose right child == right sibling of the left child
		if(internal_min[node] > target || target > internal_max[node]) {
		  return -1;
		}
	      }
	      node = 2*node + 1; // choose left child
	    }
	  }
	  else
	    return -1;

	  // Check the leaves
	  int32_t leaf = node - m_num_internal; // left leaf of the node
	  if (!(leaves_min[leaf] <= target && target <= leaves_max[leaf])) {
	    leaf++; // The answer is in the right sibling of the leaf
	  }

	  // The answer is in the right sibling of the current node
	  if(leaves_min[leaf] <= target && target <= leaves_max[leaf]) {

	    uint32_t ones_i = m_S_rank(leaf*m_bs); // Number of 1-bits up to i (included)
	    uint32_t zeros_i = leaf*m_bs - ones_i;
	    excess = 3*ones_i - zeros_i; // net left excess of S[i]
	    
	    for(int32_t ri=0, id=leaf*m_bs; ri < r; ri++, id+=rrr_bs) {
	      uint16_t bits1 = m_S.get_class(id);
	      uint16_t bits0 = rrr_bs - bits1;
	      int32_t min_e = excess - bits0;
	      if(min_e <= (int32_t)target) {
		uint64_t val = m_S.get_int(id, rrr_bs);
		int64_t ul = id+rrr_bs;
		for (int64_t j=id; j < ul; ++j) {
		  if ((val & mask)) excess += 3;
		  else excess--;
		  
		  if (excess == target)
		    return j;
		  val >>= 1;
		}
	      }
		else
		  excess += (3*bits1 - bits0);
	    }
	  }
	  return -1;
	}

        int64_t bwd_search(uint64_t i, uint32_t nle, uint32_t target)const {
	  
	  int32_t block = i / m_bs;
	  int64_t init = block*m_bs;
	  // Assumption m_bs is multiple of rrr_bs
	  int32_t rrr_bs = m_S.block_size;
	  int32_t r = m_bs/rrr_bs;
	  uint64_t mask = ((uint64_t)1 << (rrr_bs-1));

	  // 1: Find the answer in the same block
	  // TODO: Improve the performance of the case 1 replace the access
	  // m_S[i] by bit operations
	  uint32_t excess = nle;

	  for (int64_t j=i; j >= init; --j) {
	    if (m_S[j]) excess -= 3;
	    else excess++;
	    
	    if (excess <= target)
	      return j;
	  }

	  // 2: Find the answer in the left sibling
	  if(block%2 == 1) { // The current block has a right sibling

	    // The answer is in the right sibling of the current node
	    if(leaves_min[block-1] <= target) { 
	      // The excess value after point 1 is used here
	      for(int32_t ri=0, id=init-1; ri < r; ri++, id-=rrr_bs) {
		uint16_t bits1 = m_S.get_class(id);
		uint16_t bits0 = rrr_bs - bits1;
		int32_t min_e = excess - 3*bits1;
		if(min_e <= (int32_t)target) {
		  int64_t ll = id-rrr_bs;
		  uint64_t val = m_S.get_int(ll+1, rrr_bs);
		  for (int64_t j=id; j > ll; --j) {
		    if ((val & mask)) excess -= 3;
		    else excess++;
		    
		    if (excess <= target)
		      return j;
		    val <<= 1;
		  }
		}
		else
		  excess += (bits0 - 3*bits1);
	      }
	    }
	  }

	  // 3: It is necessary up and then down in the rmMt
	  uint32_t top_nodes = pow(2, m_height-1)-1;

	  uint32_t node = top_nodes + block/2; // Initial node
	  // Go up the tree
	  while (node != 0) { // while node is not the root
	    if (!(node%2)) { // if the node is a right child
	      node--; // choose left sibling
	      
	      if (internal_min[node] <= target)
	  	break;
	    }
	    node = (node-1)/2; // choose parent
	  }

	  // Go down the tree
	  if (node != 0) { // found solution for the query
	    node = 2*node + 2; // choose right child
	    while (node < m_num_internal) { // while node is not a leaf
	      if (!(internal_min[node] <= target)) {
	  	node--; // choose left child == left sibling of the right child
	  	if(internal_min[node] > target) {
	  	  return -1;
	  	}
	      }
	      node = 2*node + 2; // choose right child
	    }
	  }
	  else
	    return -1;

	  // Check the leaves
	  int32_t leaf = node - m_num_internal; // Leaf associated to the node
	  if (!(leaves_min[leaf] <= target)) {
	    leaf--; // The answer is in the right sibling of the leaf
	  }

	  // The answer is in the right sibling of the current node
	  if(leaves_min[leaf] <= target) {

	    uint32_t ones_i = m_S_rank((leaf+1)*m_bs); // Number of 1-bits up to i (included)
	    uint32_t zeros_i = (leaf+1)*m_bs - ones_i;
	    excess = 3*ones_i - zeros_i; // net left excess of S[i]

	    if (excess <= target)
	  	return (leaf+1)*m_bs;
	    
	    // The excess value after point 1 is used here
	    for(int32_t ri=0, id=(leaf+1)*m_bs-1; ri < r; ri++, id-=rrr_bs) {
	      uint16_t bits1 = m_S.get_class(id);
	      uint16_t bits0 = rrr_bs - bits1;
	      int32_t min_e = excess - 3*bits1;
	      if(min_e <= (int32_t)target) {
		int64_t ll = id-rrr_bs;
		uint64_t val = m_S.get_int(ll+1, rrr_bs);
		for (int64_t j=id; j > ll; --j) {
		  if ((val & mask)) excess -= 3;
		  else excess++;
		  
		  if (excess <= target)
		    return j;
		  val <<= 1;
		}
	      }
	      else
		excess += (bits0 - 3*bits1);
	    }	    
	  }
	  return -1;
	}
  
        int64_t bwd_search_empty(uint64_t i, uint32_t bre, uint32_t target)const {
	  
	  int32_t block = i / m_bs;
	  int64_t init = block*m_bs;
	  // Assumption 1: m_bs is multiple of rrr_bs
	  // Assumption 1: The block size of m_S and m_S2 are equal
	  int32_t rrr_bs = m_S.block_size;
	  int32_t r = m_bs/rrr_bs;
	  uint64_t mask = ((uint64_t)1 << (rrr_bs-1));

	  // 1: Find the answer in the same block
	  // TODO: Improve the performance of the case 1 replace the accesses
	  // m_S[i] and m_S2[i] by bit operations
	  uint32_t excess = bre;

	  for (int64_t j=i-1; j >= init; --j) {
	    if (m_S[j] || m_S2[j]) excess--; // 1 or 0 symbol
	    else excess++; // empty symbol
	    
	    if (excess == target)
	      return j;
	  }

	  // 2: Find the answer in the left sibling
	  if(block%2 == 1) { // The current block has a right sibling

	    // The answer is in the right sibling of the current node
	    if(leaves_min_bre[block-1] <= target && target <= leaves_max_bre[block-1]) { 
	      
	      // The excess value after point 1 is used here
	      for(int32_t ri=0, id=init-1; ri < r; ri++, id-=rrr_bs) {
		uint16_t bits1 = m_S.get_class(id) + m_S2.get_class(id); // 1 or 0 symbol
		uint16_t bits0 = rrr_bs - bits1;
		int32_t min_e = excess - bits1;
		if(min_e <= (int32_t)target) {
		  int64_t ll = id-rrr_bs;
		  uint64_t val1 = m_S.get_int(ll+1, rrr_bs);
		  uint64_t val2 = m_S2.get_int(ll+1, rrr_bs);
		  for (int64_t j=id; j > ll; --j) {
		    if ((val1 & mask) || (val2 & mask)) excess--; // 1 or 0 symbol
		    else excess++; // empty symbol
		    
		    if (excess == target)
		      return j;

		    val1 <<= 1;
		    val2 <<= 1;
		  }
		}
		else
		  excess += (bits0 - bits1);
	      }
	    }
	  }

	  // 3: It is necessary up and then down in the rmMt
	  uint32_t top_nodes = pow(2, m_height-1)-1;

	  uint32_t node = top_nodes + block/2; // Initial node
	  // Go up the tree
	  while (node != 0) { // while node is not the root
	    if (!(node%2)) { // if the node is a right child
	      node--; // choose left sibling
	      
	      if (internal_min_bre[node] <= target && target <= internal_max_bre[node])
	  	break;
	    }
	    node = (node-1)/2; // choose parent
	  }

	  // Go down the tree
	  if (node != 0) { // found solution for the query
	    node = 2*node + 2; // choose right child
	    while (node < m_num_internal) { // while node is not a leaf
	      if (!(internal_min_bre[node] <= target && target <= internal_max_bre[node])) {
	  	node--; // choose left child == left sibling of the right child
	  	if(internal_min_bre[node] > target || target > internal_max_bre[node]) {
	  	  return -1;
	  	}
	      }
	      node = 2*node + 2; // choose right child
	    }
	  }
	  else
	    return -1;

	  // Check the leaves
	  int32_t leaf = node - m_num_internal; // Leaf associated to the node
	  if (!(leaves_min_bre[leaf] <= target && target <= leaves_max_bre[leaf])) {
	    leaf--; // The answer is in the right sibling of the leaf
	  }

	  // The answer is in the right sibling of the current node
	  if(leaves_min_bre[leaf] <= target && target <= leaves_max_bre[leaf]) {
	    uint32_t ones_i = m_S_rank((leaf+1)*m_bs); // Number of 1-bits up to i (excluded)
	    uint32_t zeros_i = m_S2_rank((leaf+1)*m_bs); // Number of 0-bits up to i (excluded)
	    uint32_t excess = 2*ones_i + 2*zeros_i - (leaf+1)*m_bs - 1; // New net right excess
	    
	    if (excess == target)
	  	return (leaf+1)*m_bs;
	    
	    // The excess value after point 1 is used here
	    for(int32_t ri=0, id=(leaf+1)*m_bs-1; ri < r; ri++, id-=rrr_bs) {
	      uint16_t bits1 = m_S.get_class(id) + m_S2.get_class(id); // 1 or 0 symbol
	      uint16_t bits0 = rrr_bs - bits1;
	      int32_t min_e = excess - bits1;
	      if(min_e <= (int32_t)target) {
		int64_t ll = id-rrr_bs;
		uint64_t val1 = m_S.get_int(ll+1, rrr_bs);
		uint64_t val2 = m_S2.get_int(ll+1, rrr_bs);
		for (int64_t j=id; j > ll; --j) {
		  if ((val1 & mask) || (val2 & mask)) excess--; // 1 or 0 symbol
		  else excess++; // empty symbol
		  
		  if (excess == target)
		    return j;

		  val1 <<= 1;
		  val2 <<= 1;
		}
	      }
	      else
		excess += (bits0 - bits1);
	    }
	  }
	  return -1;
	}


        // Backward search for the bre excess. It only consider positions with
        // empty symbols
        int64_t bwd_search_empty_bre(uint64_t i, uint32_t bre, uint32_t target)const {
	  
	  int32_t block = i / m_bs;
	  int64_t init = block*m_bs;
	  // Assumption 1: m_bs is multiple of rrr_bs
	  // Assumption 1: The block size of m_S and m_S2 are equal
	  int32_t rrr_bs = m_S.block_size;
	  int32_t r = m_bs/rrr_bs;
	  uint64_t mask = ((uint64_t)1 << (rrr_bs-1));

	  // 1: Find the answer in the same block
	  // TODO: Improve the performance of the case 1 replace the accesses
	  // m_S[i] and m_S2[i] by bit operations
	  uint32_t excess = bre;

	  for (int64_t j=i-1; j >= init; --j) {
	    if (m_S[j] || m_S2[j]) {
	      excess--; // 1 or 0 symbol
	    }
	    else {
	      excess++; // empty symbol
	      
	      if (excess == target)
		return j;
	    }
	  }

	  // 2: Find the answer in the left sibling
	  if(block%2 == 1) { // The current block has a left sibling
	    // The answer is in the left sibling of the current node
	    if(leaves_min_bre_empty[block-1] <= target && target <= leaves_max_bre[block-1]) { 

	      // The excess value after point 1 is used here
	      for(int32_t ri=0, id=init-1; ri < r; ri++, id-=rrr_bs) {
		uint16_t bits1 = m_S.get_class(id) + m_S2.get_class(id); // 1 or 0 symbols
		uint16_t bits0 = rrr_bs - bits1; // empty symbols
		int32_t min_e = excess - bits1;
		if(min_e <= (int32_t)target) {
		  int64_t ll = id-rrr_bs;
		  uint64_t val1 = m_S.get_int(ll+1, rrr_bs);
		  uint64_t val2 = m_S2.get_int(ll+1, rrr_bs);
		  for (int64_t j=id; j > ll; --j) {
		    if ((val1 & mask) || (val2 & mask)) excess--; // 1 or 0 symbol
		    else {
		      excess++; // empty symbol
		      
		      if (excess == target)
		    	return j;
		    }
		    
		    val1 <<= 1;
		    val2 <<= 1;
		  }
		}
		else
		  excess += (bits0 - bits1);
	      }
	    }
	  }

	  // 3: It is necessary up and then down in the rmMt
	  uint32_t top_nodes = pow(2, m_height-1)-1;

	  uint32_t node = top_nodes + block/2; // Initial node
	  
	  // Go up the tree
	  while (node != 0) { // while node is not the root
	    if (!(node%2)) { // if the node is a right child
	      node--; // choose left sibling
	      
	      if (internal_min_bre_empty[node] <= target && target <= internal_max_bre[node])
	  	break;
	    }
	    node = (node-1)/2; // choose parent
	  }

	  // Go down the tree
	  if (node != 0) { // found solution for the query
	    node = 2*node + 2; // choose right child
	    while (node < m_num_internal) { // while node is not a leaf
	      if (!(internal_min_bre_empty[node] <= target && target <= internal_max_bre[node])) {
	  	node--; // choose left child == left sibling of the right child
	  	if(internal_min_bre_empty[node] > target || target > internal_max_bre[node])
	  	  return -1;	  	
	      }
	      node = 2*node + 2; // choose right child
	    }
	  }
	  else
	    return -1;
	  
	  // Check the leaves
	  int32_t leaf = node - m_num_internal; // Leaf associated to the node
	  if (!(leaves_min_bre_empty[leaf] <= target && target <= leaves_max_bre[leaf]))
	    leaf--; // The answer is in the right sibling of the leaf

	  // The answer is in the right sibling of the current node
	  if(leaves_min_bre_empty[leaf] <= target && target <= leaves_max_bre[leaf]) {
	    uint32_t lim = (leaf+1)*m_bs;
	    uint32_t ones_i = m_S_rank(lim); // Number of 1-bits up to i (excluded)
	    uint32_t zeros_i = m_S2_rank(lim); // Number of 0-bits up to i (excluded)
	    uint32_t excess = 2*ones_i + 2*zeros_i - lim - 1; // New net right excess

	    if (excess == target && lim != i)
	      if (!m_S[lim] && !m_S2[lim]) // Empty
	  	return lim;
	    
	    // The excess value after point 1 is used here
	    for(int32_t ri=0, id=lim-1; ri < r; ri++, id-=rrr_bs) {
	      uint16_t bits1 = m_S.get_class(id) + m_S2.get_class(id); // 1 or 0 symbol
	      uint16_t bits0 = rrr_bs - bits1;
	      int32_t min_e = excess - bits1;
	      if(min_e <= (int32_t)target) {
		int64_t ll = id-rrr_bs;
		uint64_t val1 = m_S.get_int(ll+1, rrr_bs);
		uint64_t val2 = m_S2.get_int(ll+1, rrr_bs);
		for (int64_t j=id; j > ll; --j) {
		  if ((val1 & mask) || (val2 & mask)) excess--; // 1 or 0 symbol
		  else {
		    excess++; // empty symbol
		    
		    if (excess == target)
		      return j;
		  }
		  
		  val1 <<= 1;
		  val2 <<= 1;
		}
	      }
	      else
		excess += (bits0 - bits1);
	    }
	  }
	  return -1;
	}


        // Forward search for the bre excess. It only consider positions with
        // empty symbols
        int64_t fwd_search_empty_bre(uint64_t i, uint32_t bre, uint32_t target)const {
	  
	  int32_t block = i / m_bs;
	  uint64_t end = (block+1)*m_bs;
	  // Assumption 1: m_bs is multiple of rrr_bs
	  // Assumption 1: The block size of m_S and m_S2 are equal
	  int32_t rrr_bs = m_S.block_size;
	  int32_t r = m_bs/rrr_bs;
	  uint64_t mask = (uint64_t)1;

	  // 1: Find the answer in the same block
	  // TODO: Improve the performance of the case 1 replace the accesses
	  // m_S[i] and m_S2[i] by bit operations
	  uint32_t excess = bre;
	  for (uint64_t j=i; j < end; ++j) {
	    if (m_S[j] || m_S2[j]) excess++; // 1 or 0 symbol
	    else {
	      excess--; // empty symbol
	      
	      // Notice that the answer ocurrs in a segment with two consecutive
	      // empty symbols
	      if (excess == target)
		if(!m_S[j+1] && !m_S2[j+1])
		  return j;
	    }
	  }

	  // 2: Find the answer in the right sibling
	  if(block%2 == 0) { // The current block has a right sibling
	    // The answer is in the right sibling of the current node
	    if(leaves_min_bre_empty[block+1] <= target && target <=
	       leaves_max_bre[block+1]) { 

	      // The excess value after point 1 is used here
	      for(int32_t ri=0, id=end; ri < r; ri++, id+=rrr_bs) {
		uint16_t bits1 = m_S.get_class(id) + m_S2.get_class(id); // 1 or 0 symbols
		uint16_t bits0 = rrr_bs - bits1; // empty symbols
		int32_t max_e = excess + bits1;
		if(max_e >= (int32_t)target) {
		  int64_t ul = id+rrr_bs;
		  uint64_t val1 = m_S.get_int(id, rrr_bs);
		  uint64_t val2 = m_S2.get_int(id, rrr_bs);
		  for (int64_t j=id; j < ul; ++j) {
		    if ((val1 & mask) || (val2 & mask)) excess++; // 1 or 0 symbol
		    else {
		      excess--; // empty symbol
		      
		      // Notice that the answer ocurrs in a segment with two consecutive
		      // empty symbols
		      if (excess == target)
			if(!m_S[j+1] && !m_S2[j+1]) // TODO: Improve it
			  return j;
		    }
		    val1 >>= 1;
		    val2 >>= 1;
		  }
		}
		else
		  excess += (bits1 - bits0);
	      }
	    }
	  }

	  // 3: It is necessary up and then down in the rmMt
	  uint32_t top_nodes = pow(2, m_height-1)-1;

	  uint32_t node = top_nodes + block/2; // Initial node
	  // Go up the tree
	  while (node != 0) { // while node is not the root
	    if (node%2) { // if the node is a left child
	      node++; // choose right sibling
	      
	      if (internal_min_bre_empty[node] <= target && target <=
		  internal_max_bre[node])
		break;
	    }
	    node = (node-1)/2; // choose parent
	  }

	  // Go down the tree
	  if (node != 0) { // found solution for the query
	    node = 2*node + 1; // choose left child
	    while (node < m_num_internal) { // while node is not a leaf
	      if (!(internal_min_bre_empty[node] <= target && target <= internal_max_bre[node])) {
		node++; // choose right child == right sibling of the left child
		if(internal_min_bre_empty[node] > target || target > internal_max_bre[node])
		  return -1;
	      }
	      node = 2*node + 1; // choose left child
	    }
	  }
	  else
	    return -1;

	  // Check the leaves
	  int32_t leaf = node - m_num_internal; // left leaf of the node
	  if (!(leaves_min_bre_empty[leaf] <= target && target <= leaves_max_bre[leaf])) {
	    leaf++; // The answer is in the right sibling of the leaf
	  }

	  // The answer is in the right sibling of the current node
	  if(leaves_min_bre_empty[leaf] <= target && target <=
	  leaves_max_bre[leaf])  {

	    uint32_t ones_i = m_S_rank(leaf*m_bs); // Number of 1-bits up to i (excluded)
	    uint32_t zeros_i = m_S2_rank(leaf*m_bs); // Number of 0-bits up to i (excluded)
	    uint32_t excess = 2*ones_i + 2*zeros_i - leaf*m_bs - 1; // New net right excess

	    // The excess value after point 1 is used here
	    for(int32_t ri=0, id=leaf*m_bs; ri < r; ri++, id+=rrr_bs) {
	      uint16_t bits1 = m_S.get_class(id) + m_S2.get_class(id); // 1 or 0 symbols
	      uint16_t bits0 = rrr_bs - bits1; // empty symbols
	      int32_t max_e = excess + bits1;
	      if(max_e >= (int32_t)target) {
		int64_t ul = id+rrr_bs;
		uint64_t val1 = m_S.get_int(id, rrr_bs);
		uint64_t val2 = m_S2.get_int(id, rrr_bs);
		for (int64_t j=id; j < ul; ++j) {
		  if ((val1 & mask) || (val2 & mask)) excess++; // 1 or 0 symbol
		  else {
		    excess--; // empty symbol
		    
		    // Notice that the answer ocurrs in a segment with two consecutive
		    // empty symbols
		    if (excess == target)
		      if(!m_S[j+1] && !m_S2[j+1]) // TODO: Improve it
			return j;
		  }
		  val1 >>= 1;
		  val2 >>= 1;
		}
	      }
	      else
		excess += (bits1 - bits0);
	    }	    
	  }

	  return -1;
	}

        void list_neighbors(uint64_t v) {
	  int64_t pos = vertex_pos(v);
	  int64_t match_pos = find_close(pos); // Matching closing-parenthesis
	  bool first_empty = 1; // 1 = still not returned
	  bool second_empty = 1;
	  
	  int64_t par = parent(pos);
	  if(par != -1)
	    vertex_id(par); // Print neighbor (parent)
	  
	  int64_t first_empty_pos = find_first_empty(pos);
	  int64_t second_empty_pos = find_second_empty(pos);
	  if(first_empty_pos == -1) first_empty = 0;
	  if(second_empty_pos == -1) second_empty = 0;

	  int64_t child = first_child(pos);
	  // Check all the children of v in the PS-tree
	  while(child != pos) {
	    // If the first or second empty of v occurs before the current
	    // child, then return it
	    if(second_empty && child > second_empty_pos) {
	      second_empty = 0;
	      vertex_id(find_open_empty(second_empty_pos)); // Print neighbor
	    }
	    else if(first_empty && child > first_empty_pos) {
	      first_empty = 0;
	      vertex_id(find_open_empty(first_empty_pos)); // Print neighbor
	    }
	    
	    pos = child;

	    // Find the empties associated to the current child
	    int64_t dep = find_rightmost_dependency(child);
	    while(dep > child) {
	      vertex_id(find_open(dep)); // Print neighbor
	      dep = find_prev_empty(dep);
	    }

	    // After returning their associated empties, return the current
	    // child
	    vertex_id(child); // Print neighbor
	    child = next_sibling(pos);
	  }

	  // If the first or second empty of v has not been returned, then they
	  // must be returned before analysing the last dependencies
	  if(first_empty)
	    vertex_id(find_open_empty(first_empty_pos)); // Print neighbor
	  if(second_empty)
	    vertex_id(find_open_empty(second_empty_pos)); // Print neighbor

	  // Check if the closing parenthesis of v has associated empties
	  int64_t dep = find_rightmost_dependency(match_pos);
	  while(dep > match_pos) {
	    vertex_id(find_open(dep)); // Print neighbor
	    dep = find_prev_empty(dep);
	    if(dep == -1)
	      return;
	    else if(dep == find_prev_empty(dep))
	      return;
	  }
	}  

        // Note: i and j are indexes of vertices. They are not positions in the
        // string S
        bool are_neighbors(int64_t i, int64_t j) {
	  // Assumption: i < j
	  if(i > j) {
	    int64_t tmp = i;
	    i = j;
	    j = tmp;
	  }

	  int64_t pos_i = vertex_pos(i);
	  int64_t pos_j = vertex_pos(j);

	  // Check if i is parent of j
	  if(pos_i == parent(pos_j)) return true;

	  // Check if the first stem of i connects both vertices
	  int64_t empty_pos = find_first_empty(pos_i);
	  int64_t match_pos = find_open_empty(empty_pos);

	  if(match_pos == pos_j) return true;

	  // Check if the second stem of i connects both vertices
	  empty_pos = find_second_empty(pos_i);
	  match_pos = find_open_empty(empty_pos);

	  if(match_pos == pos_j) return true;

	  // Check if the first stem of j connects both vertices
	  empty_pos = find_first_empty(pos_j);
	  match_pos = find_open_empty(empty_pos);

	  if(match_pos == pos_i) return true;

	  // Check if the second stem of j connects both vertices
	  empty_pos = find_second_empty(pos_j);
	  match_pos = find_open_empty(empty_pos);

	  if(match_pos == pos_i) return true;
	  
	  return false;
	}  
  
        //! Serializes the max_planar_rmmt to a stream.
        /*!
         * \param out The outstream to which the data structure is written.
         * \return The number of bytes written to out.
         */
        size_type serialize(std::ostream& out, structure_tree_node* v=nullptr, std::string name="")const {
            structure_tree_node* child = structure_tree::add_child(v, name, util::class_name(*this));
            size_type written_bytes = 0;

	    written_bytes += write_member(m_size, out, child, "size");
	    written_bytes += write_member(m_vertices, out, child, "vertices");
	    written_bytes += write_member(m_edges, out, child, "edges");
	    written_bytes += write_member(m_height, out, child, "height_rmmt");
	    written_bytes += write_member(m_num_internal, out, child, "internal_nodes_rmmt");
	    written_bytes += write_member(m_num_leaves, out, child, "leaves_rmmt");
	    written_bytes += write_member(m_bs, out, child, "block_size");
	    
	    written_bytes += m_S.serialize(out, child, "S");
	    written_bytes += m_S_rank.serialize(out, child, "S_rank");
	    written_bytes += m_S_select.serialize(out, child, "S_select");

	    written_bytes += m_S2.serialize(out, child, "S2");
            written_bytes += m_S2_rank.serialize(out, child, "S2_rank");

            written_bytes += leaves_min.serialize(out, child, "min_leaves_rmmt");
            written_bytes += leaves_max.serialize(out, child, "max_leaves_rmmt");
            written_bytes += internal_min.serialize(out, child, "min_internal_rmmt");
            written_bytes += internal_max.serialize(out, child, "max_internal_rmmt");

            written_bytes += leaves_min_bre.serialize(out, child, "min_leaves_bre_rmmt");
            written_bytes += leaves_max_bre.serialize(out, child, "max_leaves_bre_rmmt");
            written_bytes += internal_min_bre.serialize(out, child, "min_internal_bre_rmmt");
            written_bytes += internal_max_bre.serialize(out, child, "max_internal_bre_rmmt");

            written_bytes += leaves_min_bre_empty.serialize(out, child, "min_leaves_bre_empty_rmmt");
            written_bytes += internal_min_bre_empty.serialize(out, child, "min_internal_bre_empty_rmmt");
	    
            structure_tree::add_size(child, written_bytes);
            return written_bytes;
        }

        void load(std::istream& in) {
            read_member(m_vertices, in);
	    read_member(m_size, in);
	    read_member(m_vertices, in);
	    read_member(m_edges, in);
	    read_member(m_height, in);
	    read_member(m_num_internal, in);
	    read_member(m_num_leaves, in);
	    read_member(m_bs, in);

	    m_S.load(in);
	    m_S_rank.load(in, &m_S);
	    m_S_select.load(in, &m_S);

	    m_S2.load(in);
	    m_S2_rank.load(in, &m_S2);
	    
            leaves_min.load(in);
            leaves_max.load(in);
            internal_min.load(in);
            internal_max.load(in);

            leaves_min_bre.load(in);
            leaves_max_bre.load(in);
            internal_min_bre.load(in);
            internal_max_bre.load(in);

            leaves_min_bre_empty.load(in);
            internal_min_bre_empty.load(in);
        }

        int32_t degree(uint64_t v) {
	  int64_t pos = vertex_pos(v);
	  int64_t match_pos = find_close(pos); // Matching closing-parenthesis
	  bool first_empty = 1; // 1 = still not returned
	  bool second_empty = 1;
	  int32_t deg = 0;
	  int64_t par = parent(pos);
	  if(par != -1)
	    deg++;
	  
	  int64_t first_empty_pos = find_first_empty(pos);
	  int64_t second_empty_pos = find_second_empty(pos);
	  if(first_empty_pos == -1) first_empty = 0;
	  if(second_empty_pos == -1) second_empty = 0;

	  int64_t child = first_child(pos);
	  // Check all the children of v in the PS-tree
	  while(child != pos) {
	    // If the first or second empty of v occurs before the current
	    // child, then return it
	    if(second_empty && child > second_empty_pos) {
	      second_empty = 0;
	      deg++;
	    }
	    else if(first_empty && child > first_empty_pos) {
	      first_empty = 0;
	      deg++;
	    }
	    
	    pos = child;

	    // Find the empties associated to the current child
	    int64_t dep = find_rightmost_dependency(child);
	    while(dep > child) {
	      deg++;
	      dep = find_prev_empty(dep);
	    }

	    // After returning their associated empties, return the current
	    // child
	    deg++;
	    child = next_sibling(pos);
	  }

	  // If the first or second empty of v has not been returned, then they
	  // must be returned before analysing the last dependencies
	  if(first_empty)
	    deg++;
	  if(second_empty)
	    deg++;

	  // Check if the closing parenthesis of v has associated empties
	  int64_t dep = find_rightmost_dependency(match_pos);
	  while(dep > match_pos) {
	    deg++;
	    dep = find_prev_empty(dep);
	    if(dep == -1)
	      return deg;
	    else if(dep == find_prev_empty(dep))
	      return deg;
	  }
	  return deg;
	}

        // Return the i-th neighbor (ccw) of v (i >= 0)
        int32_t select_neighbor(uint64_t v, int32_t i) {
	  int64_t pos = vertex_pos(v);
	  int64_t match_pos = find_close(pos); // Matching closing-parenthesis
	  bool first_empty = 1; // 1 = still not returned
	  bool second_empty = 1;
	  int32_t deg = 0;
	  int64_t par = parent(pos);
	  if(par != -1) {
	    if(deg == i)
	      return vertex_id(par); // Print neighbor (parent)
	    deg++;
	  }
	  
	  int64_t first_empty_pos = find_first_empty(pos);
	  int64_t second_empty_pos = find_second_empty(pos);
	  if(first_empty_pos == -1) first_empty = 0;
	  if(second_empty_pos == -1) second_empty = 0;

	  int64_t child = first_child(pos);
	  // Check all the children of v in the PS-tree
	  while(child != pos) {
	    // If the first or second empty of v occurs before the current
	    // child, then return it
	    if(second_empty && child > second_empty_pos) {
	      second_empty = 0;
	      if(deg == i)
		return vertex_id(find_open_empty(second_empty_pos)); // Print neighbor
	      deg++;
	    }
	    else if(first_empty && child > first_empty_pos) {
	      first_empty = 0;
	      if(deg == i)
		return vertex_id(find_open_empty(first_empty_pos)); // Print neighbor
	      deg++;
	    }
	    
	    pos = child;

	    // Find the empties associated to the current child
	    int64_t dep = find_rightmost_dependency(child);
	    while(dep > child) {
	      if(deg == i)
		return vertex_id(find_open(dep)); // Print neighbor
	      deg++;
	      dep = find_prev_empty(dep);
	    }

	    // After returning their associated empties, return the current
	    // child
	    if(deg == i)
	      return vertex_id(child); // Print neighbor
	    deg++;
	    child = next_sibling(pos);
	  }

	  // If the first or second empty of v has not been returned, then they
	  // must be returned before analysing the last dependencies
	  if(first_empty) {
	    if(deg == i)
	      return vertex_id(find_open_empty(first_empty_pos)); // Print neighbor
	    deg++;
	  }
	  if(second_empty) {
	    if(deg == i)
	      return vertex_id(find_open_empty(second_empty_pos)); // Print neighbor
	    deg++;
	  }

	  // Check if the closing parenthesis of v has associated empties
	  int64_t dep = find_rightmost_dependency(match_pos);
	  while(dep > match_pos) {
	    if(deg == i)
	      return vertex_id(find_open(dep)); // Print neighbor
	    deg++;
	    dep = find_prev_empty(dep);
	    if(dep == -1)
	      return -1;
	    if(dep == find_prev_empty(dep))
	      return -1;
	  }
	  return -1;
	}  
};

}// end namespace

#endif
