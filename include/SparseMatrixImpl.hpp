#ifndef SPARSEMATRIXIMPL_HPP
#define SPARSEMATRIXIMPL_HPP

/**
 * \file SparseMatrixImpl.hpp
 * \brief Implementation file for the SparseMatrix class
 */


#include "SparseMatrix.hpp"
#include <iostream>


namespace algebra{

/**
 * @brief Compresses the sparse matrix.
 * 
 * This function compresses the sparse matrix using either the CSR (Compressed Sparse Row) or CSC (Compressed Sparse Column) format.
 * It initializes the compressed data structures and populates them with the non-zero elements of the uncompressed data.
 * After compression, the matrix is marked as compressed and the uncompressed data is cleared.
 * 
 * @tparam T The type of the matrix elements.
 * @tparam storage The storage order of the matrix (row-wise or column-wise).
 */
template <class T, StorageOrder storage>
void SparseMatrix<T, storage>::compress(){
    if (!m_compressed) {
        
        bool key_index;
        //initialize the compressed data
        if (IsRowWise<storage>::value){ //CSR
            m_inner.resize(m_rows +1);
            key_index=0;
        }
        else{ //CSC
            m_inner.resize(m_cols +1);
            key_index=1;
        }
        m_inner[0]=0; //first element is always 0
        m_outer.resize(m_data_uncompressed.size());
        m_values.resize(m_data_uncompressed.size());

        // index that follows progression in m_inner, based on key[key_index]
        std::size_t idx_inner=0;
        //ndex that follows progression in m_outer
        std::size_t idx_outer=0;
        //number of non-zero elements
        int nnz=0;
   
        // following lessOperator ordering
        for (const auto& [key, value] : m_data_uncompressed) { 
            
            nnz++;   

            //before changing row/column
            if(key[key_index]!=idx_inner){
                //in between precedent row/column (idx_inner) and key[key_index], set all 
                //in between elements of m_inner to number of non-zero elements reached
                for(std::size_t j=idx_inner +1 ; j<= key[key_index]; ++j)     
                    m_inner[j]=m_inner[idx_inner+1];
                //update idx_inner and m_inner[idx_inner+1]
                idx_inner=key[key_index];
                m_inner[idx_inner +1]+=nnz;
            }
            else{ //increase number of elements of (idx_inner+1) row/column
                m_inner[idx_inner+1]++;
            }
            
            m_outer[idx_outer]= key[!key_index];  

            m_values[idx_outer]= value;

            ++idx_outer;
        }

        //fill reamining m_inner
        for(std::size_t i=idx_inner+1; i<m_inner.size(); ++i)
            m_inner[i]=nnz;

        //mark the matrix as compressed
        m_compressed = true;

        //clear the uncompressed data
        m_data_uncompressed.clear();
    }  
};

/**
 * @brief Uncompresses the sparse matrix.
 * 
 * This function uncompresses the sparse matrix by converting it from the CSR or CSC format to the uncompressed format.
 * It initializes the uncompressed data structures and populates them with the non-zero elements of the compressed data.
 * After uncompression, the matrix is marked as uncompressed and the compressed data is cleared.
 * 
 * @tparam T The type of the matrix elements.
 * @tparam storage The storage order of the matrix (row-wise or column-wise).
 */
template <class T, StorageOrder storage>
void SparseMatrix<T,storage>::uncompress() {
    if (m_compressed) {

        std::size_t start= m_inner[0], end;
        
        for(std::size_t i=1; i< m_inner.size() ; ++i){  //m_inner            

            //there are elements on i_th row/column
            if(m_inner[i]!=start){
                start= m_inner[i-1];
                end=m_inner[i];

                //loop over the specified range to understand the elements' positions
                for(std::size_t j=start; j<end; ++j){ //m_outer, m_values
                    std::array<std::size_t,2> f;
                    if constexpr(IsRowWise<storage>::value)
                        f={i-1, m_outer[j]};
                    else 
                        f={m_outer[j],i-1};
                    m_data_uncompressed.insert(std::make_pair(f,m_values[j]));
                }
            }
                    
        }

        //mark the matrix as uncompressed
        m_compressed = false;

        //clear the compressed data
        m_inner.clear();
        m_outer.clear();
        m_values.clear();
    }
};

/**
 * @brief Resizes the sparse matrix.
 * 
 * This function resizes the sparse matrix to the specified dimensions.
 * If the matrix is compressed, it first uncompresses the matrix, removes any elements that are out of range, and then compresses the matrix again.
 * 
 * @tparam T The type of the matrix elements.
 * @tparam storage The storage order of the matrix (row-wise or column-wise).
 * @param r_dir The new number of rows.
 * @param c_dir The new number of columns.
 */
template<class T, StorageOrder storage>
void SparseMatrix<T,storage>::resize(std::size_t r_dir, std::size_t c_dir){

    uncompress();

    //only if SparseMatrix shrinks
    if(r_dir<m_rows || c_dir<m_cols){
        auto it=m_data_uncompressed.begin();
        while(it!=m_data_uncompressed.end()){       
            if(it->first[0]>=r_dir)
               it=m_data_uncompressed.erase(it);    
            else if(it->first[1]>=c_dir)
               it=m_data_uncompressed.erase(it);
            else ++it;
        }
    }

    m_rows=r_dir;
    m_cols=c_dir;

};

/**
 * @brief Accesses the element at the specified position in the sparse matrix.
 * 
 * This function returns the value of the element at the specified position in the sparse matrix.
 * If the matrix is compressed, it uses the CSR or CSC format to access the element efficiently.
 * If the matrix is uncompressed, it uses the uncompressed data structure to access the element.
 * 
 * @tparam T The type of the matrix elements.
 * @tparam storage The storage order of the matrix (row-wise or column-wise).
 * @param r The row index of the element.
 * @param c The column index of the element.
 * @return The value of the element at the specified position.
 */
template <class T, StorageOrder storage>
T SparseMatrix<T,storage>::operator()(std::size_t r, std::size_t c) const{

    if (r<m_rows && c<m_cols){
    if (!m_compressed){
           std::array<std::size_t,2> key={r,c};
           auto it= m_data_uncompressed.find(key);
           if(it!= m_data_uncompressed.end())
              return it->second;
           else
              return T();
        }
    else {
        
        std::size_t index_for_inner, index_for_outer;
        if constexpr (IsRowWise<storage>::value) {  // CSR
            index_for_inner=r;
            index_for_outer=c;
        } 
        else { // CSC 
            index_for_inner=c;
            index_for_outer=r;  
        }

        //determine the start and end indices
        std::size_t start = m_inner[index_for_inner];
        std::size_t end = m_inner[index_for_inner+1];
        
        //search for the column/row index_for_outer in the specified range
        for (std::size_t i = start; i < end; ++i)
            if (m_outer[i] == index_for_outer) 
                return m_values[i];
        
        return T();  //default value of T
    }}  
    std::cerr << "Indexes are out of range\n";
    return T();
};

/**
 * @brief Accesses the element at the specified position in the sparse matrix.
 * 
 * This function returns a reference to the element at the specified position in the sparse matrix.
 * If the matrix is compressed, it uses the CSR or CSC format to access the element efficiently.
 * If the matrix is uncompressed, it uses the uncompressed data structure to access the element.
 * If the element does not exist, it inserts a new element at the specified position and returns a reference to it.
 * 
 * @tparam T The type of the matrix elements.
 * @tparam storage The storage order of the matrix (row-wise or column-wise).
 * @param r The row index of the element.
 * @param c The column index of the element.
 * @return A reference to the element at the specified position.
 */
template <class T, StorageOrder storage>
T & SparseMatrix<T,storage>::operator()(std::size_t r, std::size_t c){

    if (r<m_rows && c<m_cols){
    if (!m_compressed){
           std::array<std::size_t,2> key={r,c};
           return m_data_uncompressed[key] ;
        }
    else {
        
        std::size_t index_for_inner, index_for_outer;
        if constexpr (IsRowWise<storage>::value) { //CSR
            index_for_inner=r;
            index_for_outer=c;
        } 
        else { //CSC 
            index_for_inner=c;
            index_for_outer=r;  
        }
        //determine the start and end indices
        std::size_t start = m_inner[index_for_inner];
        std::size_t end = m_inner[index_for_inner+1];       
        
        //search for the column/row index_for_outer in the specified range
        for (std::size_t i = start; i < end; ++i) 
            if (m_outer[i] == index_for_outer) 
                return m_values[i];

        //if element is not present yet 
        return insertElementCompressed(r,c);  
    }}
    std::cerr<<"Indexes are out of range";
    static T default_val;
    return default_val;
    
};

/**
 * @brief Inserts a new element at the specified position in the compressed sparse matrix.
 * 
 * This function inserts a new element at the specified position in the compressed sparse matrix.
 * It finds the insertion point in the m_outer array for the specified row/column and adjusts the m_inner array accordingly.
 * It returns a reference to the newly inserted value.
 * 
 * @tparam T The type of the matrix elements.
 * @tparam storage The storage order of the matrix (row-wise or column-wise).
 * @param r The row index of the element.
 * @param c The column index of the element.
 * @return A reference to the newly inserted value.
 */
template <class T, StorageOrder storage>
T & SparseMatrix<T, storage>::insertElementCompressed(std::size_t r, std::size_t c) {

    std::size_t index_for_inner, index_for_outer;
    if constexpr (IsRowWise<storage>::value) { // CSR 
        index_for_inner=r;
        index_for_outer=c;
    } 
    else { // CSC 
        index_for_inner=c;
        index_for_outer=r;  
    }

    //find the insertion point in the m_outer array for the specified row/column
    std::size_t insertPos; 
    insertPos = std::upper_bound(m_outer.begin() + m_inner[index_for_inner], m_outer.begin() + m_inner[index_for_inner + 1], index_for_outer) - m_outer.begin();

    //adjust m_inner after the insertion
    for (std::size_t i = index_for_inner + 1; i < m_inner.size(); ++i) 
        ++m_inner[i]; 

    //insert the new index
    m_values.insert(m_values.begin() + insertPos, T{});
    m_outer.insert(m_outer.begin() + insertPos, index_for_outer);

    //return a reference to the newly inserted value
    return m_values[insertPos];
};

/**
 * @brief Performs matrix-vector multiplication.
 * 
 * This function performs matrix-vector multiplication between the sparse matrix and the vector.
 * It returns the resulting vector.
 * 
 * @tparam U The type of the matrix and vector elements.
 * @tparam s The storage order of the matrix (row-wise or column-wise).
 * @param m The sparse matrix.
 * @param v The vector.
 * @return The resulting vector of the matrix-vector multiplication.
 */
template<class U, StorageOrder s>
std::vector<U> operator*(const SparseMatrix<U,s> &m, const std::vector<U> &v){  
    
    if(m.m_cols!=v.size()  && !(m.m_cols==1 && m.m_rows==v.size())){
        std::cerr << "Dimensions are incompatible\n";
        return std::vector<U>();
    }
    else{
        std::vector<U> res(m.m_rows);
        
        bool one_column= (m.m_cols==1);
        //the vector is actually a scalar if matrix has one column
        if(one_column) 
           res.resize(1);

        if(m.is_compressed()){
          if(!one_column){
            if (IsRowWise<s>::value){ //CSR 
                for(std::size_t i = 0; i < m.m_rows; ++i){
                    U sum = U();
                    std::size_t start = m.m_inner[i];
                    std::size_t end = m.m_inner[i+1];
                    for(std::size_t j = start; j < end; ++j){
                        U val= m.m_values[j];
                        std::size_t v_idx= m.m_outer[j];
                        sum += val * v[v_idx];
                    }
                    res[i] = sum;
                }
            }

            else{
                for(std::size_t i=0; i< m.m_cols ; ++i){
                    for(std::size_t j=m.m_inner[i]; j<m.m_inner[i+1]; ++j)
                        res[m.m_outer[j]]+= m.m_values[j]*v[i];
                }
            }
          }
          else { //"vector"-vector
              if(IsRowWise<s>::value){
                for(std::size_t i=0; i< m.m_rows ; ++i){
                    std::size_t start = m.m_inner[i];
                    std::size_t end= m.m_inner[i+1];
                    if(start!=end){
                        U val= m.m_values[start];
                        res[0] += val*v[i];
                    }
                }
              }
              else {
                    for(std::size_t j=m.m_inner[0]; j<m.m_inner[1]; ++j)
                        res[0]+= m.m_values[j]*v[m.m_outer[j]];              
              }
            }
        }
        else {  
            if(!one_column) //matrix-vector
              //loop over non-zero elements of the map
              for(const auto &[key,value]: m.m_data_uncompressed)
                res[key[0]]+= value*v[key[1]];
            else //"vector"-vector
              //loop over non-zero elements of the map
              for(const auto &[key,value]: m.m_data_uncompressed)
                res[0]+= value*v[key[0]];
        }

        return res;
    }
} 



/**
 * \brief Overload of the operator<< for the SparseMatrix class
 * 
 * This function overloads the operator<< for the SparseMatrix class.
 * 
 * \tparam U Type of the stored element
 * \tparam s Storage order of the SparseMatrix
 * \param str The output stream
 * \param m The SparseMatrix object
 * \return Reference to the output stream
 * 
 */
template<class U, StorageOrder s>
std::ostream & operator<<(std::ostream &str, const SparseMatrix<U,s> & m){

    if(!m.m_compressed){
        std::cout<< "Map: " <<std::endl;
        for(auto it=m.m_data_uncompressed.begin(); it!= m.m_data_uncompressed.end(); ++it)
            str << "(" << it->first[0] << "," << it->first[1] << "): " << it->second <<"\n";
    }

    else{
        std::cout<< "\nm_inner: " <<std::endl;
        for(auto it=m.m_inner.begin(); it!= m.m_inner.end(); ++it)
            str << *it << " ";
        std::cout<< "\nm_outer: " <<std::endl;
        for(auto it=m.m_outer.begin(); it!= m.m_outer.end(); ++it)
            str << *it << " ";
        std::cout << "\nm_values: " << std::endl;
        for(auto it=m.m_values.begin(); it!= m.m_values.end(); ++it)
            str << *it << " ";
    }
    std::cout << "\n------------------------------\n";

    return str;
};



};


#endif /*SPARSEMATRIXIMPL_HPP*/