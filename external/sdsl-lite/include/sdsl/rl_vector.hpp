#ifndef INCLUDED_SDSL_RL_VECTOR
#define INCLUDED_SDSL_RL_VECTOR


#include "int_vector.hpp"
#include "iterators.hpp"
#include "rank_support_v.hpp"
#include <algorithm> // for std::swap

//! Namespace for the succinct data structure library.
namespace sdsl
{

  template<class t_bitvector   = bit_vector,
	   class t_rank        = typename t_bitvector::rank_1_type,
	   class t_sel        = typename t_bitvector::select_1_type>
class rl_vector
{
    public:

        typedef int_vector<>::size_type              size_type;
        typedef int_vector<>::value_type             value_type;
        typedef random_access_const_iterator<rl_vector> const_iterator;
        typedef const_iterator                       iterator;
        typedef t_bitvector                          bit_vector_type;
        typedef t_rank                               rank_1_type;
        typedef t_sel                                select_1_type;
        typedef int_alphabet_tag                     alphabet_category;
        typedef iv_tag                               index_category;

protected:


        size_type         m_size  = 0;
        size_type         m_runs = 0;
        int_vector<>      m_H;
        bit_vector_type   m_B;         
        rank_1_type       m_B_rank;
        select_1_type     m_B_sel;

        void copy(const rl_vector& rl) {
            m_size          = rl.m_size;
            m_runs          = rl.m_runs;
	    m_H             = rl.m_H;
	    m_B             = rl.m_B;
            m_B_rank        = rl.m_B_rank;
            m_B_sel         = rl.m_B_sel;
	    m_B_rank.set_vector(&m_B);
	    m_B_sel.set_vector(&m_B);
        }

    public:

        //! Default constructor
        rl_vector() {
        };

        template<uint8_t int_width = 32>
        rl_vector(int_vector<int_width>& buf) {
	  size_type n = buf.size();  // set n
	  m_size = n;
	  
	  if (n < m_size) {
	    throw std::logic_error("n="+util::to_string(n)+" < "+util::to_string(m_size)+"=m_size");
	    return;
	  }
	  
	  for (size_type i=1; i < m_size; ++i) {
	    if(buf[i] != buf[i-1])
	      m_runs++;
	  }
	  m_runs++;
	  int_vector<> H(m_runs, 0, buf.width());

	  bit_vector B_local(n,0);

	  for (size_type i=1, j=0; i < m_size; ++i) {
	    if(buf[i] != buf[i-1]) {
	      H[j] = buf[i-1];
	      B_local[i-1] = 1;
	      j++;
	    }
	  }
	  H[m_runs-1] = buf[n-1];
	  B_local[n-1] = 1;

	  bit_vector_type B(B_local);
	  m_B.swap(B);
	  m_H.swap(H);
	  util::init_support(m_B_rank, &m_B);
	  util::init_support(m_B_sel, &m_B);
        }
  
        //! Copy constructor
        rl_vector(const rl_vector& rl) {
            copy(rl);
        }

        //! Copy constructor
        rl_vector(rl_vector&& rl) {
            *this = std::move(rl);
        }

        //! Assignment operator
        rl_vector& operator=(const rl_vector rl) {
            if (this != &rl) {
                copy(rl);
            }
            return *this;
        }

        //! Assignment move operator
        rl_vector& operator=(rl_vector&& rl) {
            if (this != &rl) {
                m_size          = rl.m_size;
                m_runs          = rl.m_runs;
		m_H             = std::move(rl.m_H);
		m_B             = std::move(rl.m_B);
		m_B_rank        = std::move(rl.m_B_rank);
		m_B_sel        = std::move(rl.m_B_sel);
		m_B_sel.set_vector(&m_B);
		m_B_sel.set_vector(&m_B);
            }
            return *this;
        }

  
        //! Swap operator
        void swap(rl_vector& rl) {
            if (this != &rl) {
                std::swap(m_size, rl.m_size);
                std::swap(m_runs, rl.m_runs);
                m_H.swap(rl.m_H);
		m_B.swap(rl.m_B);
                util::swap_support(m_B_rank, rl.m_B_rank, &m_B, &(rl.m_B));
                util::swap_support(m_B_sel, rl.m_B_sel, &m_B, &(rl.m_B));
            }
        }

        //! Returns the size of the original vector.
        size_type size()const {
            return m_size;
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
	    value_type c = m_H[m_B_rank(i)];
	    return c;
        };

        /* Equivalent to the operator [], but setting the pointers pos_H and r */
        value_type get_value(size_type i, size_type* pos_H, long* r) {
            assert(i < size());
	    *pos_H = m_B_rank(i);
	    size_type next_one = m_B_sel(*pos_H+1);
	    *r = next_one - i;

	    value_type c = m_H[*pos_H];
	    return c;
        }

        /* Return the next value in the vector respect to the index stored in
	   pos_H and r */
        value_type get_next(size_type* pos_H, long* r) {
            assert(*pos_H < runs());

	    if(*r == 0) {
	      (*pos_H)++;
	      *r = m_B_sel(*pos_H+1) - m_B_sel(*pos_H) - 1;	      
	    }
	    else {
	      (*r)--;
	    }
	    
	    value_type c = m_H[*pos_H];
	    return c;
        }
  
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
	  written_bytes += write_member(m_runs, out, child, "runs");
	  written_bytes += m_H.serialize(out, child, "H");
	  written_bytes += m_B.serialize(out, child, "B");
	  written_bytes += m_B_rank.serialize(out, child, "B_rank");
	  written_bytes += m_B_sel.serialize(out, child, "B_sel");

	  return written_bytes;
        }

        //! Loads the data structure from the given istream.
        void load(std::istream& in) {
            read_member(m_size, in);
            read_member(m_runs, in);
	    m_H.load(in);
	    m_B.load(in);
	    m_B_rank.load(in, &m_B);
	    m_B_sel.load(in, &m_B);
        }

        void info() {
	  std::cout << "\tsize: " << m_size << std::endl;
	  std::cout << "\t# of runs: " << m_runs << std::endl;
	  std::cout << "\tTotal space (B): " << size_in_bytes(*this) << std::endl;
	}
};

}// end namespace sdsl
#endif
