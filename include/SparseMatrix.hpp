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


/// @brief Template struct for detecting S as row_wise or column_wise, default is true
/// @tparam S StorageOrder
template <StorageOrder S>
struct IsRowWise : std::true_type {}; 

/// @brief Specialization of template struct IsRowWise
template <>
struct IsRowWise<column_wise> : std::false_type {}; 


/// @brief Functor for operator < for std::array<std::size_t,2>
/// @tparam storage 
template <StorageOrder storage>
struct  lessOperator{
    bool operator()(const std::array<std::size_t, 2>& lhs, const std::array<std::size_t, 2>& rhs) const {
        bool index;
        if constexpr (IsRowWise<storage>::value) 
            index=0;
        else
            index=1;   
        if (lhs[index] < rhs[index]) return true;
        if (lhs[index] > rhs[index]) return false;
        return lhs[!index] < rhs[!index];
    }
};


/// @brief Class to store sparse matrices
/// @tparam T Type of the stored element 
/// @tparam storage Storage order
template <class T, StorageOrder storage>
class SparseMatrix{

public:

    /// @brief Constructor, uses the private resize(r,c) method
    /// @param r Number of rows
    /// @param c Number of columns
    SparseMatrix(std::size_t r, std::size_t c ): m_compressed(0) { resize(r, c);}; 

    /// @brief Compress SparseMatrix, fill m_inner, m_outer, m_values
    void compress();

    /// @brief Unompress SparseMatrix, fill map m_data_uncompressed
    void uncompress();

    bool is_compressed() const{
        return m_compressed;
    };

    /// @brief Constant version of call operator 
    T operator()(std::size_t r, std::size_t c) const;

    /// @brief Non-constant version of call operator
    /// @return reference to element positioned at (r,c)
    T &operator()(std::size_t r, std::size_t c); 
    
    /// @brief Overloading of the stream operator for SparseMatrix
    template<class U, StorageOrder s>
    friend std::ostream & operator<<(std::ostream &str, const SparseMatrix<U,s> & m);  

    /// @brief Function that executes the product between a SparseMatrix and a vector, with compatible dimensions
    /// @tparam U Type of elements stored inside SparseMatrix and std::vector 
    /// @tparam s Storage order of SparseMatrix
    /// @param m SparseMatrix
    /// @param v vector
    /// @return product vector of elements of type U
    template<class U, StorageOrder s>
    friend std::vector<U> operator*(SparseMatrix<U,s> &m, std::vector<U> &v);

    /// @brief Function to read a matrix in a MatrixMarket format
    /// @tparam U Type of stored elements
    /// @tparam s Storage order
    /// @param filename Name of file in which the matrix is written
    /// @return Sparse matrix of elements of type U and storage order s
    template<class U, StorageOrder s>
    friend SparseMatrix<U,s> readMatrixMarket(const std::string& filename);

private:

    std::size_t m_rows=0, m_cols=0;
    bool m_compressed;

    /// @brief map for uncompressed state of SparseMatrix that uses lessOperator for ordering
    std::map<std::array<std::size_t, 2>, T, lessOperator<storage>>  m_data_uncompressed;   
    
    std::vector<std::size_t> m_inner;
    std::vector<std::size_t> m_outer;
    std::vector<T> m_values;

    /// @brief Private method to resize the sparse matrix 
    /// @param r_dir New numebr of rows
    /// @param c_dir New number of columns
    /// @note used inside the constructor
    void resize(std::size_t r_dir, std::size_t c_dir);   

    /// @brief Private method to add a new element inside a compressed SparseMatrix
    /// @param r row of the new element
    /// @param c column of the new element
    /// @return The reference to the new added element
    /// @note used in the non-const operator for the compressed case
    T & insertElementCompressed(std::size_t r, std::size_t c);

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