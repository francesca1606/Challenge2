#include "SparseMatrix.hpp"

using namespace algebra;


int main(){

    Matrix<int, StorageOrder::column_wise> S(2,4);
    S(1,1)=1;
    S(1,3)=5;
    S(1,4)=10;
    S(2,2)=1;
    S(2,3)=1;
    S(2,4)=4;
    S.print();
    S.compress();

    Matrix<float,StorageOrder::row_wise> M= readMatrixMarket<float,StorageOrder::row_wise>("Insp_131.mtx");
    M.compress();
    M.print();
    



    return 0;
};