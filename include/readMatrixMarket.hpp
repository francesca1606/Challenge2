#ifndef READMATRIXMARKET_HPP
#define READMATRIXMARKET_HPP

/**
 * \file readMatrixMarket.hpp
 * \brief Header file for the readMatrixMarket function
 */

#include "SparseMatrix.hpp"
#include <iostream>
#include <fstream>
#include <limits>

namespace algebra{

/**
 * \brief Method to read a matrix in Matrix Market format
 * \tparam U Type of the stored element
 * \tparam s Storage order
 * \param filename The name of the file to read
 * \return The matrix read from the file
 */
template<class U,StorageOrder s>
SparseMatrix<U,s> readMatrixMarket(const std::string& filename) {
     std::ifstream file(filename);

     if (!file.is_open()) {
         std::cerr << "Failed to open file: " << filename << std::endl;
     }     

     file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

     //number of rows, columns and non-zero elements
     std::size_t rows, cols, nnz;
     file >> rows >> cols >> nnz;  

     SparseMatrix<U,s> matrix(rows, cols); 
      
     //fill matrix
     for (std::size_t i = 0; i < nnz; ++i) {
         std::size_t row, col;
         U value;
         file >> row >> col >> value;
         matrix(row-1,col-1)= value;
     }

     return matrix;
};


}



#endif /*READMATRIXMARKET_HPP*/


