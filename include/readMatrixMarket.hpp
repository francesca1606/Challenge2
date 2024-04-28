#ifndef READMATRIXMARKET_HPP
#define READMATRIXMARKET_HPP

#include "SparseMatrix.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

namespace algebra{

// Method to read a Matrix Market file
template<class U,StorageOrder s>
Matrix<U,s> readMatrixMarket(const std::string& filename) {
     std::ifstream file(filename);

     if (!file.is_open()) {
         std::cerr << "Failed to open file: " << filename << std::endl;
     }     

     std::string line;

     std::size_t rows, cols, nnz;
     file >> rows >> cols >> nnz;  

     Matrix<U,s> matrix(rows, cols); 
      
     for (std::size_t i = 0; i < nnz; ++i) {
         std::size_t row, col;
         float value;
         file >> row >> col >> value;
         matrix(row-1,col-1)= value;
     }

     return matrix;
};


}



#endif /*READMATRIXMARKET_HPP*/


