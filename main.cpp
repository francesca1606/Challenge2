#include "SparseMatrix.hpp"

using namespace algebra;


int main(){

    Matrix<int, StorageOrder::row_wise>  m(7,4);
    m(3,1)=1;
    m(1,3)=6;
    m(5,2)=10;
    
    m.print();
    m.compress();

    m(5,1)=4;
    m.print();
    m.uncompress();
    m.print();

    Matrix<int, StorageOrder::column_wise> S(2,4);
    S(0,0)=1;
    S(0,2)=5;
    S(0,3)=10;
    S(1,1)=1;
    S(1,2)=1;
    S(1,3)=4;
   
    S.compress();
    S.print();
    std::vector<int> v={1,2,3,4};

    std::vector<int> res=S*v;

    for(std::size_t i=0; i<res.size();++i){
        std::cout << res[i] << "  " ;
    }
    std::cout << "\n";



    return 0;
};