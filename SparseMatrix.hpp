#ifndef SPARSEMATRIX_HPP
#define SPARSEMATRIX_HPP

#include <vector>
#include <map>
#include <variant>


namespace algebra{


enum StorageOrder{
     row_wise, column_wise
};


template <class T, StorageOrder storage>
class Matrix{

using varVector = std::variant< std::vector<std::size_t>, std::vector<T> >;

public:

//6.constructor + resize
          Matrix(std::size_t r, std::size_t c );

//2.compress()
          void compress();

//3.uncompress()
          void uncompress();

//4. is_compressed
          bool is_compressed() const{
                return m_compressed;
          };

1.std::map<std::array<std::size t,2>,T>



  


5. call operator, const and non-const



   
7.friend *operator for multiplication with a vector

private:

    std::size_t m_rows, m_cols;
    bool m_compressed;

    std::map<std::array<std::size_t, 2>, T>  m_data_uncompressed;   
    
    std::vector<std::size_t> m_inner;
    std::vector<std::size_t> m_outer;
    std::vector<T> m_values;

};




};


#include "SparseMatrixImpl.hpp"




#endif /*SPARSEMATRIX_HPP*/