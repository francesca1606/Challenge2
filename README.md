# Sparse matrices' class and product with a vector

##  README for the 2nd Challenge of the PACS course

This program allows the user to work with sparse matrices stored in a compressed or uncompressed fashion, with row-wise or column-wise ordering.
<br/>
A compressed sparse matrix consists of 3 vectors:
- m_inner: if the storage ordering is row-wise, the vector stores the increase of non-zero elements from one row to the next; if it's column-wise, it stores the increase of non-zero elements from one column to the next;
- m_outer: if the storage ordering is row-wise, the vector stores the column index of the non-zero elements, otheriwse it stores the row index of the non-zero elements; 
- m_values: stores the values of the non-zero elements, following row-wise or column-wise ordering.
An uncompressed sparse matrix consists of an ordered map:
- key = array[row,column] of the non-zero element
- value = value of the non-zero element


The file is divided into :
- main.cpp 
- include
- Insp_131.mtx
- Makefile
- README
- LICENSE
- .gitignore  

<br/><br/>
The include folder contains:
- SparseMatrix.hpp, where inside the namespace algebra the SparseMatrix class is declared, along with the enumerator StorageOrder
- SparseMatrixImpl.hpp, which contains the definitions of SparseMatrix' methods and of the stream operator and the matrix-vector product (class' friends)
- readMatrixMarket.hpp, which contains the definition of the friend method for reading the matrix from Insp_131.mtx (MatrixMarket format)

<br/><br/><br/>
The main.cpp file includes chrono.hpp, that is needed to time the execution of the matrix-product vector.
To make the program include it, in the CPPFLAGS  of the Makefile I employed the environmental variable PACS_ROOT, which stands for the directory to pacs-examples/Examples
<br/>
To run the code, simply run make in the terminal
If you want to remove the object file and the executable, run make clean

          make; ./main; make clean

