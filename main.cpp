#include "SparseMatrix.hpp"
#include "chrono.hpp"
#include <algorithm>
#include <complex>
#include <random>
#include <ranges>

using namespace algebra;


int main(){

    Timings::Chrono Time;

    SparseMatrix<double,StorageOrder::row_wise> M_rows= readMatrixMarket<double,StorageOrder::row_wise>("Insp_131.mtx");
    SparseMatrix<double,StorageOrder::column_wise> M_cols= readMatrixMarket<double,StorageOrder::column_wise>("Insp_131.mtx"); 
    std::vector<double> v(131,1);
    std::random_device rd;  
    std::mt19937 gen(rd()); 
    std::vector<double> randomVector(131);
    std::ranges::generate(randomVector, [&]() { return std::uniform_real_distribution<double>(0., 100.)(gen); });

    Time.start();
    std::vector<double> prod1=M_rows*randomVector;
    Time.stop();
    std::cout <<"\nProduct of uncompressed matrix (row_wise) with vector:        " << Time << std::endl;

    Time.start();
    std::vector<double> prod2=M_cols*randomVector;
    Time.stop();
    std::cout << "Product of uncompressed matrix (column_wise) with vector:     " << Time << std::endl;

    M_rows.compress();
    M_cols.compress();  

    Time.start();
    std::vector<double> prod3=M_rows*randomVector;
    Time.stop();
    std::cout << "Product of compressed matrix (row_wise) with vector:          " << Time << std::endl;

    Time.start();
    std::vector<double> prod4=M_cols*randomVector;
    Time.stop();
    std::cout << "Product of compressed matrix (column_wise) with vector:       " << Time << std::endl;

    if(prod1==prod2 && prod1==prod3 && prod1==prod4)
        std::cout << "All products are equal\n\n";


    //matrix with one column
    /*SparseMatrix<double, StorageOrder::row_wise> m(5,1);    
    m(0,0)=1;
    m(2,0)=3;
    m(3,0)=1;
    std::vector<double> v1(5,1);
    m.compress();
    std::vector<double> res= m*v1;
     for(auto &s:res){
        std::cout << s << " ";
    }
    std::cout << "\n\n";

    //complex numbers
    SparseMatrix<std::complex<double>, StorageOrder::row_wise> m(3, 3);
    m.compress();
    m(1,2)= {3,4};
    m(1,1)= {1,1};
    m(2,0)= {5,10};
    m.uncompress();
    std::vector<std::complex<double>> v1 = { {1, 2}, {3, 4}, {5, 6} };
    std::vector<std::complex<double>> result = m * v1;

    for(auto &s:result){
        std::cout << s << " ";
    }
    std::cout << "\n\n";*/

    return 0;
};