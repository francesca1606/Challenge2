#ifndef SPARSEMATRIX_HPP
#define SPARSEMATRIX_HPP
// clang-format off
/**
 * \file SparseMatrix.hpp
 * \brief Header file for the akgebra namespace and SparseMatrix class
 */

#include <vector>
#include <map>
#include <array>
#include <iostream>
//@note good doxygen comments
namespace algebra{


enum StorageOrder{
    row_wise, column_wise
};


/**
 * \brief Template struct for detecting S as row_wise or column_wise, default is true
*  \tparam S StorageOrder
*/
template <StorageOrder S>
struct IsRowWise : std::true_type {}; 

/**
 * \brief Specialization of template struct IsRowWise
 */
template <>
struct IsRowWise<column_wise> : std::false_type {}; 

/**
 * \brief Functor for operator < for std::array<std::size_t,2>
 * \tparam storage 
 */
template <StorageOrder storage>
struct  lessOperator{
    /**
     * \brief Operator () for comparing two std::array<std::size_t,2> objects
     * \param lhs The left-hand side object
     * \param rhs The right-hand side object
     * \return true if lhs is less than rhs, false otherwise
     */
    bool operator()(const std::array<std::size_t, 2>& lhs, const std::array<std::size_t, 2>& rhs) const {
        bool index;
        if constexpr (IsRowWise<storage>::value) 
            index=0;
        else
        //@note you can use tie to simplify the code:
        // return std::tie(lhs[1], lhs[0]) < std::tie(rhs[1], rhs[0]);
            index=1;   
        if (lhs[index] < rhs[index]) return true;
        if (lhs[index] > rhs[index]) return false;
        return lhs[!index] < rhs[!index];
    }
};

/**
 * \brief Class to store sparse matrices
 * \tparam T Type of the stored element 
 * \tparam storage Storage order
 */
template <class T, StorageOrder storage>
class SparseMatrix{

public:

    /**
     * \brief Constructor, uses the private resize(r,c) method
     * \param r Number of rows
     * \param c Number of columns
     */
    SparseMatrix(std::size_t r, std::size_t c ): m_compressed(0) { resize(r, c);}; 

    /**
     * \brief Compress SparseMatrix, fill m_inner, m_outer, m_values
     */
    void compress();

    /**
     * \brief Unompress SparseMatrix, fill map m_data_uncompressed
     */
    void uncompress();

    /**
     * \brief Check if the SparseMatrix is compressed
     * \return true if the SparseMatrix is compressed, false otherwise
     */
    bool is_compressed() const {return m_compressed;};

    /**
     * \brief Constant version of call operator 
     * \param r The row index
     * \param c The column index
     * \return The value at the specified position
     */
    T operator()(std::size_t r, std::size_t c) const;

    /**
     * \brief Non-constant version of call operator
     * \param r The row index
     * \param c The column index
     * \return Reference to the value at the specified position
     */
    T &operator()(std::size_t r, std::size_t c); 
    
    /**
     * \brief Overloading of the stream operator for SparseMatrix
     * \tparam U Type of elements stored inside SparseMatrix and std::vector 
     * \tparam s Storage order of SparseMatrix
     * \param str The output stream
     * \param m The SparseMatrix object
     * \return Reference to the output stream
     */
    template<class U, StorageOrder s>
    friend std::ostream & operator<<(std::ostream &str, const SparseMatrix<U,s> & m);  

    /**
     * \brief Method to resize the sparse matrix 
     * \param r_dir New number of rows
     * \param c_dir New number of columns
     * \note Used inside the constructor
     */
    void resize(std::size_t r_dir, std::size_t c_dir);

    /**
     * \brief Function that executes the product between a SparseMatrix and a vector, with compatible dimensions
     * \tparam U Type of elements stored inside SparseMatrix and std::vector 
     * \tparam s Storage order of SparseMatrix
     * \param m The SparseMatrix object
     * \param v The vector object
     * \return The product vector of elements of type U
     */
    template<class U, StorageOrder s>
    friend std::vector<U> operator*(const SparseMatrix<U,s> &m, const std::vector<U> &v);


    /**
     * \brief Function to read a matrix in a MatrixMarket format
     * \tparam U Type of stored elements
     * \tparam s Storage order
     * \param filename Name of file in which the matrix is written
     * \return Sparse matrix of elements of type U and storage order s
     */
    template<class U, StorageOrder s>
    friend SparseMatrix<U,s> readMatrixMarket(const std::string& filename);

private:

    std::size_t m_rows=0, m_cols=0;
    bool m_compressed;

    /**
     * \brief Map for uncompressed state of SparseMatrix that uses lessOperator for ordering
     */
    //@note the simplest way to account for column_wise and row_wise is to use a different comparison operator
    // for the map in the two cases. This way you avoid the complexity of having to exchange the row and column indexes,
    // which is error-prone and confusing.
    std::map<std::array<std::size_t, 2>, T, lessOperator<storage>>  m_data_uncompressed;   
    
    std::vector<std::size_t> m_inner;
    std::vector<std::size_t> m_outer;
    std::vector<T> m_values;   

    /**
     * \brief Private method to add a new element inside a compressed SparseMatrix
     * \param r The row index
     * \param c The column index
     * \return Reference to the new added element
     * \note Used in the non-const operator for the compressed case
     */
    T & insertElementCompressed(std::size_t r, std::size_t c);

};

/**
 * \brief Overloading of the stream operator for SparseMatrix
 * \tparam U Type of elements stored inside SparseMatrix and std::vector 
 * \tparam s Storage order of SparseMatrix
 * \param str The output stream
 * \param m The SparseMatrix object
 * \return Reference to the output stream
 */
template<class U, StorageOrder s>
std::ostream & operator<<(std::ostream &str, const SparseMatrix<U,s> & m);

/**
 * \brief Function that executes the product between a SparseMatrix and a vector, with compatible dimensions
 * \tparam U Type of elements stored inside SparseMatrix and std::vector 
 * \tparam s Storage order of SparseMatrix
 * \param m The SparseMatrix object
 * \param v The vector object
 * \return The product vector of elements of type U
 */
template<class U, StorageOrder s>
std::vector<U> operator*(const SparseMatrix<U,s> &m, const std::vector<U> &v);

/**
 * \brief Function to read a matrix in a MatrixMarket format
 * \tparam U Type of stored elements
 * \tparam s Storage order
 * \param filename Name of file in which the matrix is written
 * \return Sparse matrix of elements of type U and storage order s
 */
template<class U, StorageOrder s>
SparseMatrix<U,s> readMatrixMarket(const std::string& filename);


};


#include "SparseMatrixImpl.hpp"
#include "readMatrixMarket.hpp"




#endif /*SPARSEMATRIX_HPP*/