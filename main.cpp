#include "SparseMatrix.hpp"

using namespace algebra;


int main(){

    Matrix<int, StorageOrder::column_wise>  m(7,4);
    m(3,1)=1;
    m(1,3)=6;
    m(5,2)=10;
    
    m.print();
    m.compress();

    m(5,1)=4;
    m.print();
    m.uncompress();
    m.print();



    return 0;
};