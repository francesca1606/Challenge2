#ifndef READMATRIXMARKET_HPP
#define READMATRIXMARKET_HPP

#include "SparseMatrix.hpp"
#include <iostream>
#include <fstream>
#include <limits>

namespace algebra{

// Method to read a matrix in Matrix Market format
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


