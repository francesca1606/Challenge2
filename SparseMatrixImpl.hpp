#include "SparseMatrix.hpp"
#include <iostream>


namespace algebra{

template <class T, StorageOrder storage>
void Matrix<T, storage>::compress(){
    if (!m_compressed) {
        // Sort the uncompressed data by row (for CSR) or column (for CSC)
        bool key_index;

        //std::vector<std::pair<std::array<std::size_t, 2>, T>> sortedData(m_data_uncompressed.begin(), m_data_uncompressed.end());
        if constexpr (IsRowWise<storage>::value){ 
            //std::sort(sortedData.begin(), sortedData.end(), [](const auto& a, const auto& b) {return a.first[0] < b.first[0] || (a.first[0] == b.first[0] && a.first[1] < b.first[1]);});
            m_inner.resize(m_rows +1);
            key_index=0;

        }
        else{ 
            m_inner.resize(m_cols +1);
            key_index=1;
        }
        // Initialize the compressed data
        m_inner[0]=0; // Start of the first row/column
        m_outer.resize(m_data_uncompressed.size());
        m_values.resize(m_data_uncompressed.size());

        int which=0;
        int idx_outer=0;
        int nnz=0;
   
        for (const auto& [key, value] : m_data_uncompressed) {    

            if(key[key_index]!=which){
                for(int j=which +2; j<= key[key_index]; ++j)     
                    m_inner[j]=m_inner[which+1];
                which=key[key_index];
            }
            
            nnz++;
            m_inner[which+1]+=nnz;

            m_outer[idx_outer]= key[!key_index];

            m_values[idx_outer]= value;

            ++idx_outer;
        }

        //finish m_inner
        for(int i=which+2; i<m_inner.size(); ++i)
            m_inner[i]=nnz;

        // Mark the matrix as compressed
        m_compressed = true;

        // Clear the uncompressed data
        m_data_uncompressed.clear();
    }  
    else 
        std::cout << "Matrix is already compressed" << std::endl;
};


template <class T, StorageOrder storage>
void Matrix<T,storage>::uncompress() {
    if (m_compressed) {
                                                         //PROVA A USARE ::ranges?
        int count_inner =0;
        int i_value=0;
        
        for(std::size_t i=1; i< m_inner.size(); ++i){  //m_inner            

            if(m_inner[i]!= i_value){

                count_inner = m_inner[i] - i_value;

                for(std::size_t j=i_value; j < i_value + count_inner; ++j){    //m_outer, m_values
                    std::array<std::size_t,2> f;
                    if constexpr(IsRowWise<storage>::value)
                        f={i-1, m_outer[j]};
                    else 
                        f={m_outer[j],i-1};
                    m_data_uncompressed.insert(std::make_pair(f,m_values[j]));
                }

                i_value= m_inner[i];
            }
                    
        }

        // Mark the matrix as uncompressed
        m_compressed = false;

        // Clear the compressed data
        m_inner.clear();
        m_outer.clear();
        m_values.clear();
    }
};


template<class T, StorageOrder storage>
void Matrix<T,storage>::resize(std::size_t r_dir, std::size_t c_dir){

    uncompress();

    {
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

};

template <class T, StorageOrder storage>
const T & Matrix<T,storage>::operator()(std::size_t r, std::size_t c) const{

    if(r<m_rows && c<m_cols)
    if (!m_compressed){
           std::array<std::size_t,2> key={r,c};
           return m_data_uncompressed[key] ;
        }
    else {
        
        std::size_t index_for_inner, index_for_outer;
        if constexpr (IsRowWise<storage>::value) {
            index_for_inner=r;
            index_for_outer=c;
        } 
        else { // CSC format
            index_for_inner=c;
            index_for_outer=r;  
        }

        // Determine the start and end indices
        std::size_t start = m_inner[index_for_inner];
        std::size_t end = m_inner[index_for_inner];

        // Search for the column c in the specified range
        for (std::size_t i = start; i < end; ++i) 
            if (m_outer[i] == index_for_outer) 
                return m_values[i];

        throw std::out_of_range("Indexes are out of range");
    }  
    throw std::out_of_range("Indexes are out of range");
};


template <class T, StorageOrder storage>
T & Matrix<T,storage>::operator()(std::size_t r, std::size_t c){

    if(r<m_rows && c<m_cols)
    if (!m_compressed){
           std::array<std::size_t,2> key={r,c};
           return m_data_uncompressed[key] ;
        }
    else {
        
        std::size_t index_for_inner, index_for_outer;
        if constexpr (IsRowWise<storage>::value) {
            index_for_inner=r;
            index_for_outer=c;
        } 
        else { // CSC format
            index_for_inner=c;
            index_for_outer=r;  
        }

        // Determine the start and end indices
        std::size_t start = m_inner[index_for_inner];
        std::size_t end = m_inner[index_for_inner];

        // Search for the column c in the specified range
        for (std::size_t i = start; i < end; ++i) 
            if (m_outer[i] == index_for_outer) 
                return m_values[i];

        return insertElementCompressed(r,c);
    }
    throw std::out_of_range("Indexes are out of range");
};


template <class T, StorageOrder storage>
T & Matrix<T, storage>::insertElementCompressed(std::size_t r, std::size_t c) {

    std::size_t index_for_inner, index_for_outer;
    if constexpr (IsRowWise<storage>::value) {
        index_for_inner=r;
        index_for_outer=c;
    } 
    else { // CSC format
        index_for_inner=c;
        index_for_outer=r;  
    }

    // Find the insertion point in the m_outer array for the specified row/column
    std::size_t insertPos; 
    insertPos = std::upper_bound(m_outer.begin() + m_inner[index_for_inner], m_outer.begin() + m_inner[index_for_inner + 1], index_for_outer) - m_outer.begin();

    // Adjust the inner pointers for the rows/columns after the insertion
    for (std::size_t i = index_for_inner + 1; i < m_inner.size(); ++i) {
        ++m_inner[i]; 
    }

    // Insert the new index
    m_values.insert(m_values.begin() + insertPos, T{});
    m_outer.insert(m_outer.begin() + insertPos, index_for_outer);

    // Return a reference to the newly inserted value
    return m_values[insertPos];
};



template <class T, StorageOrder storage>
void Matrix<T, storage>::print() const{
    std::cout<< "Map: " <<std::endl;
    bool inner_idx;
    if constexpr(IsRowWise<storage>::value)
       inner_idx=0;
    else 
       inner_idx=1;
    for(auto it=m_data_uncompressed.begin(); it!= m_data_uncompressed.end(); ++it){
        std::cout << "(" << it->first[0] << "," << it->first[1] << "): " << it->second << std::endl;
    }
    std::cout<< "m_inner: " <<std::endl;
    for(auto it=m_inner.begin(); it!= m_inner.end(); ++it){
        std::cout << *(it) << " ";
    }
    std::cout<< "\nm_outer: " <<std::endl;
    for(auto it=m_outer.begin(); it!= m_outer.end(); ++it){
        std::cout << *it << " ";
    }
    std::cout << "\nm_values: " << std::endl;
    for(auto it=m_values.begin(); it!= m_values.end(); ++it){
        std::cout << *it << " ";
    }
    std::cout << "\n";
};



};
