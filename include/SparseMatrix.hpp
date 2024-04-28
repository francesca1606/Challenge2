#ifndef SPARSEMATRIX_HPP
#define SPARSEMATRIX_HPP

#include <vector>
#include <map>
#include <variant>   //cancella
#include <array>
#include <iostream>

namespace algebra{


enum StorageOrder{
     row_wise, column_wise
};


template <StorageOrder S>
struct IsRowWise : std::false_type {}; //to help detect if it's row_wise or column_wise

template <>
struct IsRowWise<row_wise> : std::true_type {};  //specialization    , should I need inline???


template <StorageOrder storage>
struct  lessOperator{
    bool operator()(const std::array<std::size_t, 2>& lhs, const std::array<std::size_t, 2>& rhs) const {
        bool index;
        if constexpr (storage == StorageOrder::row_wise) 
            index=0;
        else
            index=1;
            
        // Compare based on the first element (row)
        if (lhs[index] < rhs[index]) return true;
        if (lhs[index] > rhs[index]) return false;
            // If rows are equal, compare based on the second element (column)
        return lhs[!index] < rhs[!index];
    }
};


template <class T, StorageOrder storage>
class SparseMatrix{

public:

    SparseMatrix(std::size_t r, std::size_t c ): m_compressed(0) { resize(r, c);};

    void resize(std::size_t r_dir, std::size_t c_dir);   //PRIVATO????  

    void compress();

    void uncompress();

    bool is_compressed() const{
        return m_compressed;
    };

    const T &operator()(std::size_t r, std::size_t c) const;
    T &operator()(std::size_t r, std::size_t c); 
    
    template<class U, StorageOrder s>
    friend std::ostream & operator<<(std::ostream &str, const SparseMatrix<U,s> & m);  

    template<class U, StorageOrder s>
    friend std::vector<U> operator*(SparseMatrix<U,s> &m, std::vector<U> &v);

    template<class U, StorageOrder s>
    friend SparseMatrix<U,s> readMatrixMarket(const std::string& filename);



private:

    std::size_t m_rows=0, m_cols=0;
    bool m_compressed;

    std::map<std::array<std::size_t, 2>, T, lessOperator<storage>>  m_data_uncompressed;   
    
    std::vector<std::size_t> m_inner;
    std::vector<std::size_t> m_outer;
    std::vector<T> m_values;

    T &insertElementCompressed(std::size_t r, std::size_t c);

};

template<class U, StorageOrder s>
std::ostream & operator<<(std::ostream &str, const SparseMatrix<U,s> & m);

template<class U, StorageOrder s>
std::vector<U> operator*(SparseMatrix<U,s> &m, std::vector<U> &v);

template<class U, StorageOrder s>
SparseMatrix<U,s> readMatrixMarket(const std::string& filename);


};


#include "SparseMatrixImpl.hpp"
#include "readMatrixMarket.hpp"




#endif /*SPARSEMATRIX_HPP*/