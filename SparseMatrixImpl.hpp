#include "SparseMatrix.hpp"


namespace algebra{


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
                m_inner[which_row]++;   //devo controllare ordinamento ----> ma come faccio a passare storage????? (fatto in utils)
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


template<class T, StorageOrder storage>
void Matrix<T,storage>::resize(std::size_t r_dir, std::size_t c_dir){
    
    //cambia per row-wise e cols-wise

    if(m_compressed){  //ROW-WISE

        if(r_dir >= m_rows && c_dir>=m_cols ){  //row-wise, don't change outer, values cause I add zeros
            m_inner.resize(r_dir+1);
            int nnz= m_inner(m_rows);
            for (std::size_t i= m_rows +1; i < r_dir+1; ++i)
                m_inner[i]= nnz;   
        }

        if(r_dir <m_rows && c_dir>=m_cols){
            int diff= m_inner[m_rows] - m_inner[r_dir];
            m_inner.resize(m_dir+1);
            m_outer.resize(diff);
            m_values.resize(diff);

        }

        if(r_dir >= m_rows && c_dir<m_cols){
            //int diff= m_cols - c_dir;
            //m_inner rimane la stessa
            int count=0;
            std::vector<std::size_t> sub_outer(m_outer.size());
            std::vector<std::size_t> sub_values(m_outer.size());
            for(int i=0; i<m_outer.size() ; ++i)
                if(m_outer[i]>= c_dir){   
                   sub_outer[i]=m_outer[i];  
                   sub_values[i]=m_values[i];
                   count++;
                }
            std::swap(sub_outer,m_outer);
            std::swap(sub_values, m_values);
            m_outer.resize(count);
            m_values.resize(count);
        }

        if(r_dir < m_rows && c_dir<m_cols){
            int count=0;
            std::vector<std::size_t> sub_outer(m_outer.size());
            std::vector<std::size_t> sub_values(m_outer.size());
            std::vector<std::size_t> sub_inner(r_dir +1);
            int row_count=0;
            for(int i=0; i<= r_dir; ++i){
                sub_inner[i]=row_count;
                for(int j= m_inner[i]; j< m_inner[i+1]; ++j)  //????
                    if(m_outer[j]< c_dir){   
                      sub_outer[j]=m_outer[j];  
                      sub_values[j]=m_values[j];
                      count++;
                      row_count++;
                    }
            }
            std::swap(sub_outer,m_outer);
            std::swap(sub_values, m_values);
            std::swap(sub_inner, m_inner);
            m_outer.resize(count);
            m_values.resize(count);

        }
           
    }

    else{
        if(r_dir<m_rows || c_dir<m_cols){
            auto it=m_data_uncompressed.begin();
            while(it!=m_data_uncompressed.end()){
                if(it->first[0]>r_dir)
                   it=m_data_uncompressed.erase(it);    
                else if(it->first[1]>c_dir)
                   it=m_data_uncompressed.erase(it);
                else ++it;
            }
        }
    }

    m_rows=r_dir;
    m_cols=c_dir;

}




};