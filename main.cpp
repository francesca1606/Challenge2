#include "SparseMatrix.hpp"
#include "chrono.hpp"
#include <algorithm>

using namespace algebra;


int main(){

    /*Matrix<int, StorageOrder::column_wise> S(2,4);
    S(1,1)=1;
    S(1,3)=5;
    S(1,4)=10;
    S(2,2)=1;
    S(2,3)=1;
    S(2,4)=4;
    S.print();
    S.compress();*/

    Timings::Chrono Time;

    SparseMatrix<float,StorageOrder::row_wise> M_rows= readMatrixMarket<float,StorageOrder::row_wise>("Insp_131.mtx");
    SparseMatrix<float,StorageOrder::column_wise> M_cols= readMatrixMarket<float,StorageOrder::column_wise>("Insp_131.mtx");
    std::vector<float> v(131,1);

    std::cout << M_cols;

    Time.start();
    std::vector<float> prod1=M_rows*v;
    Time.stop();
    std::cout <<"\nProduct of uncompressed matrix (row_wise) with vector:        " << Time << std::endl;

    Time.start();
    std::vector<float> prod2=M_cols*v;
    Time.stop();
    std::cout << "Product of uncompressed matrix (column_wise) with vector:     " << Time << std::endl;

    M_rows.compress();
    M_cols.compress();

    Time.start();
    std::vector<float> prod3=M_rows*v;
    Time.stop();
    std::cout << "Product of compressed matrix (row_wise) with vector:          " << Time << std::endl;

    Time.start();
    std::vector<float> prod4=M_cols*v;
    Time.stop();
    std::cout << "Product of compressed matrix (column_wise) with vector:       " << Time << std::endl;
    
    std::cout << (prod1==prod2) << std::endl; 
    std::cout << (prod1==prod3) << std::endl;       
    std::cout << (prod1==prod4) << std::endl;

    //for (auto &s: prod1)
    //    std::cout << s << " ";
    std::cout << "\n\n";
    

    return 0;
};