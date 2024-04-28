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
To run the code, you need to make some changes to the Makefile:
- in CPPFLAGS, substitute -I../pacs-examples/Examples/include with the paths to chrono.hpp 

          CPPFLAGS ?= -O3 -Wall -I. -Wno-conversion-null -Wno-deprecated-declarations -Ipath/to/chrono.hpp

After this, simply run make in the terminal
If you want to remove the object file and the executable, run make clean

          make; ./main; make clean

