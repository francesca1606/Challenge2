#include "SparseMatrix.hpp"


namespace algebra{

template <class T, StorageOrder storage>
Matrix<T, storage>::Matrix(std::size_t r, std::size_t c ): m_rows(i), m_cols(j), m_compressed(0) {
          //        RESIZE METHOD    ......;
          // .......

         
        
 
};

template <class T, StorageOrder storage>
void Matrix<T, storage>::compress(){
    if(!m_compressed){

          if (storage== StorageOrder::row_wise)
                    m_inner.resize(m_rows +1);
          else
                    m_inner.resize(m_cols +1);
          
          m_inner[0]=0;
          m_outer.resize(m_compressed.size());
          m_values.resize(m_compressed.size());

          int which_row=-1;
          int i=0;

          for(auto it= m_data_uncompressed.begin(); it!= m_data_uncompressed.end(); ++it ){   //sto pensando row-wise   DA RIVEDERE PER COLONNE

                    if(it->first[0]==which_row)
                              m_inner[which_row]++;   //devo controllare ordinamento ----> scrivere ordinamento per pair, che cambia a seconda di row-wise e column-wise
                    else
                              which_row++;

                    m_col[i]= it->first[1];

                    m_values[i]= it->second;

                    ++i;
          }

          m_compressed=1;

          m_data_uncompressed.clear();

    }

}


template <class T, StorageOrder storage>
void Matrix<T, storage>::uncompress(){
    if(m_compressed){

          int count_per_row =0;
          int i_value=0;
          //int j_index=0;

          for(int i=1; i< m_inner.size(); ++i){  //m_inner            PENSATO ROW_WISE, ADATTARE A COLONNA
                    
                    if(m_inner[i]!= i_value){

                              count_per_row = m_inner[i] - i_value;

                              for(int j=i_value; j < i_value + count_per_row; ++j){    //m_outer, m_values
                                        std::array<std::size_t,2> f={i-1, m_outer[j]};
                                        m_data_uncompressed.insert(std::make_pair(f,m_values[j]));
                              }

                              i_value= m_inner[i];
                              //j_index+= count_per_row;
                    }
                    
          }

          m_compressed=0;

          m_inner.clear();
          m_outer.clear();
          m_values.clear();

    }
}


};