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

     // Assuming the matrix is in CSR or CSC format based on the storage template parameter
     // Read the matrix dimensions
     std::size_t rows, cols, nnz;
     file >> rows >> cols >> nnz;  

     // Assuming the matrix is stored in CSR format for simplicity
     // Adjust the implementation for CSC format if necessary
     Matrix<U,s> matrix(rows, cols); 
     std::cout<<matrix.m_rows << matrix.m_cols <<std::endl;  
     for (std::size_t i = 0; i < nnz; ++i) {
         std::size_t row, col;
         float value;
         file >> row >> col >> value;
         matrix(row,col)= value;
     }

     return matrix;
};


}


