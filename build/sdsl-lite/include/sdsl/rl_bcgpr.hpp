#ifndef INCLUDED_SDSL_RL_BCGPR
#define INCLUDED_SDSL_RL_BCGPR

/*
 * Implementation based on the paper "D. Belazzougui, F. Cunial, T. Gagie,
 * N. Prezza and M. Raffinot. Composite repetition-aware data structures"
 */

#include "sdsl_concepts.hpp"
#include "int_vector.hpp"
#include "rank_support_v.hpp"
#include "select_support_mcl.hpp"
#include "wt_helper.hpp"
#include "util.hpp"
#include <algorithm> // for std::swap
#include <stdexcept>
#include <vector>
#include <queue>
#include <utility>

//! Namespace for the succinct data structure library.
namespace sdsl
{

  template<class t_bitvector   = bit_vector,
	 class t_sparse_bitvector   = sd_vector<>,
         class t_rank        = typename t_bitvector::rank_1_type,
         class t_select      = typename t_bitvector::select_1_type,
         class t_sparse_rank        = typename t_sparse_bitvector::rank_1_type,
         class t_sparse_select      = typename t_sparse_bitvector::select_1_type>
class rl_bcgpr
{
    public:

        typedef int_vector<>::size_type              size_type;
        typedef int_vector<>::value_type             value_type;
        typedef random_access_const_iterator<rl_bcgpr> const_iterator;
        typedef const_iterator                       iterator;
        typedef t_bitvector                          bit_vector_type;
        typedef t_sparse_bitvector                   sparse_bit_vector_type;
        typedef t_rank                               rank_1_type;
        typedef t_select                             select_1_type;
        typedef t_sparse_rank                        sparse_rank_1_type;
        typedef t_sparse_select                      sparse_select_1_type;
        typedef wt_tag                               index_category;
        typedef int_alphabet_tag                     alphabet_category;

protected:


        size_type         m_size  = 0;
        size_type         m_sigma = 0;     //<- \f$ |\Sigma| \f$
        size_type         m_runs = 0;
        bit_vector_type   m_mapping;       // mapping symbols to consecutive
                                           // alphabet 
        rank_1_type       m_mapping_rank;  // rank support for the
                                           // mapping bit vector 
        int_vector<>      m_ids;      // Indexes of first position of each run (sorted by symbol)
        int_vector<>      m_sum;      // Number of previous occurrences of each symbol (sorted by symbol)
        int_vector<>      m_freq_run;  // First entry of each symbol in m_ids and m_sum
        int_vector<>      m_R;        // Short sequences, representing each run by one symbol
        sparse_bit_vector_type   m_B;         
        sparse_rank_1_type       m_B_rank;
        sparse_select_1_type     m_B_select1;

        void copy(const rl_bcgpr& rl) {
            m_size          = rl.m_size;
            m_sigma         = rl.m_sigma;
            m_runs          = rl.m_runs;
	    m_ids           = rl.m_ids;
	    m_sum           = rl.m_sum;
	    m_R             = rl.m_R;
	    m_B             = rl.m_B;
            m_B_rank        = rl.m_B_rank;
            m_B_select1        = rl.m_B_select1;
	    m_B_rank.set_vector(&m_B);
	    m_B_select1.set_vector(&m_B);
        }

    public:

        const size_type&       sigma = m_sigma;         //!< Effective alphabet size of the wavelet tree.

        //! Default constructor
        rl_bcgpr() {
        };

        //! Semi-external constructor
        /*! \param buf         File buffer of the int_vector for which the wt_int should be build.
         *  \param size        Size of the prefix of v, which should be indexed.
ermined automatically.
         *    \par Time complexity
         *
         *    \par Space complexity
         *
         */
        template<uint8_t int_width>
        rl_bcgpr(int_vector_buffer<int_width>& buf, size_type size) :
	  m_size(size){	  
	  if (0 == m_size)
	    return;
	  size_type n = buf.size();  // set n
	  
	  if (n < m_size) {
	    throw std::logic_error("n="+util::to_string(n)+" < "+util::to_string(m_size)+"=m_size");
	    return;
	  }

	  m_sigma = 0;
	  
	  value_type x = buf[0];  // variable for the biggest value in buf
	  for (size_type i=1; i < m_size; ++i) {
	    if (buf[i] > x )
		x = buf[i];
	    if(buf[i] != buf[i-1])
	      m_runs++;
	  }
	  x++;
	  m_runs++;
	  bit_vector_type mapping(x,0);
	  int_vector<> R(m_runs,0,buf.width());
	  bit_vector_type B_local(buf.size(),0);

	  mapping[buf[0]] = 1;
	  for (size_type i=1, j=0; i < m_size; ++i) {
	    mapping[buf[i]] = 1;
	    
	    if(buf[i] != buf[i-1]) {
	      R[j] = buf[i-1];
	      B_local[i-1] = 1;
	      j++;
	    }
	  }
	  R[m_runs-1] = buf[buf.size()-1];
	  B_local[buf.size()-1] = 1;

	  m_mapping.swap(mapping);
	  util::init_support(m_mapping_rank, &m_mapping);
	  m_sigma = m_mapping_rank(x);
	  int_vector<> freq_run(m_sigma,0); // Frequency of runs per symbol
	  int_vector<> ids(m_runs,0);
	  int_vector<> sum(m_runs,0);
	  int_vector<> acc(m_sigma,0);

	  for (size_type i=0; i < R.size(); ++i)
	    freq_run[m_mapping_rank(R[i])]++;	
	  
	  for (size_type i=0, tmp = 0, acc = 0; i < freq_run.size(); ++i) {
	    tmp = freq_run[i];
	    freq_run[i] = acc;
	    acc += tmp;
	  }

	  int_vector<> copy_freq(freq_run); // Copy of the frequency of runs per symbol

	  size_type map = m_mapping_rank(buf[0]);
	  size_type idx = copy_freq[map];
	  copy_freq[map]++;
	  ids[idx] = 0;
	  acc[map]++;
	  for (size_type i=1; i < m_size; ++i) {	    
	    map = m_mapping_rank(buf[i]);
	    if(buf[i] != buf[i-1]) {
	      idx = copy_freq[map];
	      copy_freq[map]++;
	      ids[idx] = i;
	      sum[idx] = acc[map];
	    }

	    acc[map]++;
	  }

	  sparse_bit_vector_type B(B_local);
	  util::init_support(m_B_rank, &B);
	  util::init_support(m_B_select1, &B);

	  m_B.swap(B);
	  m_ids.swap(ids);
	  m_sum.swap(sum);
	  m_freq_run.swap(freq_run);
	  m_R.swap(R);
        }

        //! Copy constructor
        rl_bcgpr(const rl_bcgpr& rl) {
            copy(rl);
        }

        //! Copy constructor
        rl_bcgpr(rl_bcgpr&& rl) {
            *this = std::move(rl);
        }

        //! Assignment operator
        rl_bcgpr& operator=(const rl_bcgpr rl) {
            if (this != &rl) {
                copy(rl);
            }
            return *this;
        }

        //! Assignment move operator
        rl_bcgpr& operator=(rl_bcgpr&& rl) {
            if (this != &rl) {
                m_size          = rl.m_size;
                m_sigma         = rl.m_sigma;
                m_runs          = rl.m_runs;
		m_mapping       = std::move(rl.m_mapping);
		m_mapping_rank  = std::move(rl.m_mapping_rank);
		m_ids           = std::move(rl.m_ids);
		m_sum           = std::move(rl.m_sum);
		m_freq_run      = std::move(rl.m_freq_run);
		m_R             = std::move(rl.m_R);

		m_B             = std::move(rl.m_B);
		m_B_rank        = std::move(rl.m_B_rank);
		m_B_select1     = std::move(rl.m_B_select1);
		m_B_rank.set_vector(&m_B);
		m_B_select1.set_vector(&m_B);
            }
            return *this;
        }

        //! Swap operator
        void swap(rl_bcgpr& rl) {
            if (this != &rl) {
                std::swap(m_size, rl.m_size);
                std::swap(m_sigma,  rl.m_sigma);
                std::swap(m_runs, rl.m_runs);
                m_mapping.swap(rl.m_mapping);
                util::swap_support(m_mapping_rank, rl.m_mapping_rank,
				   &m_mapping, &(rl.m_mapping));
                m_ids.swap(rl.m_ids);
                m_sum.swap(rl.m_sum);
                m_freq_run.swap(rl.m_freq_run);
                m_R.swap(rl.m_R);
		m_B.swap(rl.m_B);
                util::swap_support(m_B_rank, rl.m_B_rank, &m_B, &(rl.m_B));
                util::swap_support(m_B_select1, rl.m_B_select1, &m_B, &(rl.m_B));
            }
        }

        //! Returns the size of the original vector.
        size_type size()const {
            return m_size;
        }

        //! Returns the alphabet size.
        size_type alphabet()const
        {
            return m_sigma;
        }

        //! Returns whether the wavelet tree contains no data.
        bool empty()const {
            return m_size == 0;
        }

        //! Returns the number of runs in the sequence
        size_type runs()const {
            return m_runs;
        }

        //! Recovers the i-th symbol of the original vector.
        /*! \param i The index of the symbol in the original vector.
         *  \returns The i-th symbol of the original vector.
         *  \par Precondition
         *       \f$ i < size() \f$
         */
        value_type operator[](size_type i)const {
            assert(i < size());
	    value_type c = m_R[m_B_rank(i)];
	    return c;
        };

        //! Calculates how many symbols c are in the prefix [0..i-1] of the supported vector.
        /*!
         *  \param i The exclusive index of the prefix range [0..i-1], so \f$i\in[0..size()]\f$.
         *  \param c The symbol to count the occurrences in the prefix.
         *    \returns The number of occurrences of symbol c in the prefix [0..i-1] of the supported vector.
         *  \par Time complexity
         *  
         *  \par Precondition
         *  
         */
        size_type rank(size_type i, value_type c)const {
	  assert(i <= size());
	  
	  size_type map_sym = m_mapping_rank(c);
	  int32_t ll = m_freq_run[map_sym];
	  int32_t ul = (map_sym == m_sigma-1) ? m_ids.size()-1 : m_freq_run[map_sym+1]-1;	  
	  int32_t m;
	  
	  if(m_ids[ul] <= i)
	    ll = ul;
	  else
	    while((ul-ll) > 1) {
	      m = (ll+ul)/2;
	      if(m_ids[m] == i) {
		ll = m;
		break;
	      }
	      else if(m_ids[m] < i)
		ll = m;
	      else
		ul = m;
	    }

	  if(i < m_ids[ll])
	    return 0;
	  size_type rr = m_B_rank(m_ids[ll]);
	  size_type ss = m_B_select1(rr+1);
	  size_type out = m_sum[ll] + (std::min(ss,i)-m_ids[ll]);

	  if(i > ss)
	    return out+1;
	  return out;
	};


        //! Calculates the i-th occurrence of the symbol c in the supported vector.
        /*!
         *  \param i The i-th occurrence.
         *  \param c The symbol c.
         *  \par Time complexity
         *  
         *  \par Precondition
         *       \f$ 1 \leq i \leq rank(size(), c) \f$
         */
        size_type select(size_type i, value_type c)const {
	  assert(1 <= i and i <= rank(size(), c));
	  
	  size_type map_sym = m_mapping_rank(c);
	  int32_t ll = m_freq_run[map_sym];
	  int32_t ul = (map_sym == m_sigma-1) ? m_ids.size()-1 : m_freq_run[map_sym+1]-1;	  
	  int32_t m;
	  
	  if(m_sum[ul] <= i)
	    ll = ul;
	  else
	    while((ul-ll) > 1) {
	      m = (ll+ul)/2;
	      if(m_sum[m] == i) {
		ll = m;
		break;
	      }
	      else if(m_sum[m] < i)
		ll = m;
	      else
		ul = m;
	    }	  
	  
	  if(m_sum[ll]==i)
	    ll--;
	  
	  size_type id = m_ids[ll];
	  
	  return id + (i-m_sum[ll]-1);	  
        };

        //! Returns a const_iterator to the first element.
        const_iterator begin()const {
            return const_iterator(this, 0);
        }

        //! Returns a const_iterator to the element after the last element.
        const_iterator end()const {
            return const_iterator(this, size());
        }


        //! Serializes the data structure into the given ostream
        size_type serialize(std::ostream& out, structure_tree_node* v=nullptr, std::string name="")const {
	  structure_tree_node* child = structure_tree::add_child(v, name,
								 util::class_name(*this));

	  size_type written_bytes = 0;
	  written_bytes += write_member(m_size, out, child, "size");
	  written_bytes += write_member(m_sigma, out, child, "sigma");
	  written_bytes += write_member(m_runs, out, child, "runs");
	  written_bytes += m_mapping.serialize(out, child, "mapping");
	  written_bytes += m_mapping_rank.serialize(out, child, "mapping_rank");
	  written_bytes += m_ids.serialize(out, child, "ids");
	  written_bytes += m_sum.serialize(out, child, "sum");
	  written_bytes += m_freq_run.serialize(out, child, "freq_run");
	  written_bytes += m_R.serialize(out, child, "R");
	  written_bytes += m_B.serialize(out, child, "B");
	  written_bytes += m_B_rank.serialize(out, child, "B_rank");
	  written_bytes += m_B_select1.serialize(out, child, "B_select1");

	  return written_bytes;
        }

        //! Loads the data structure from the given istream.
        void load(std::istream& in) {
            read_member(m_size, in);
            read_member(m_sigma, in);
            read_member(m_runs, in);
	    m_mapping.load(in);
	    m_mapping_rank.load(in, &m_mapping);
	    m_ids.load(in);
	    m_sum.load(in);
	    m_freq_run.load(in);
	    m_R.load(in);
	    m_B.load(in);
	    m_B_rank.load(in, &m_B);
	    m_B_select1.load(in, &m_B);
        }

        void info() {
	  std::cout << "\tsize: " << m_size << std::endl;
	  std::cout << "\tsigma: " << m_sigma << std::endl;
	  std::cout << "\t# of runs: " << m_runs << std::endl;
	  std::cout << "\tTotal space (B): " << size_in_bytes(*this) << std::endl;
	}
};

}// end namespace sdsl
#endif
