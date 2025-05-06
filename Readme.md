# Matrix
# 1. Description:  
Matrix is a header-only library for simple matrix operations in C++.
Since it is a HOL, to include implementation you need to first define `MATRIX_IMPL` before including library.

# 2. Dependencies:  
The library is fully based on c++ standart library and libc. No additional dependencies included.

# 3. Usage:  
The library is used in raw form (.hpp) because it uses templates.  
You can precompile it with desired types and assemble a static or dynamic library yourself, 
since it requires only C++ compiler.
1. You include the header
2. You find a place you want the implementation to be, `#define MATRIX_IMPL` here and then include the header
3. If you are using multiple source files and you use different types in different files, 
you need to explicitely point the compiler to seek implementation elsewhere by writing `extern`
in all files excluding one that includes implementation

## Matrix file format
Matrices are stored in .txt file as plane text.
The first line of the file stores 2 values separated with space: sizes of matrix.
After that the content of matrix is stored row by row, separating columns with spaces.
An example of valid matrix file:
```
3 3
1 2 3
4 5 6
7 8 9
```

# 4. Authors:
**Agoev T.** - *developer* - [gitlab](https://vgit.mirea.ru/s7k) [github](https://github.com/mentoltea)
