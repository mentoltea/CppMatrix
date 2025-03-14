#ifndef MATRIX_H
#define MATRIX_H

#include <fstream>

template <typename T>
class Matrix {
public:
    unsigned int a, b; // dimensions of matrix
    
    // @param a Number of rows
    // @param b Number of columns
    // @param initvalue Value to initialize matrix with
    Matrix(unsigned int a, unsigned int b, T initvalue=0);
    Matrix(std::ifstream& fd);
    
    Matrix(const Matrix& other);
    Matrix(Matrix&& other);
    ~Matrix();
    
    Matrix& operator=(const Matrix& other);
    Matrix& operator=(Matrix&& other);
    
    T* operator[](unsigned int y);
    
    
    Matrix operator*(const Matrix& other);
    Matrix operator+(const Matrix& other);
    Matrix operator-(const Matrix& other);
    
    Matrix& operator*=(const Matrix& other);
    Matrix& operator+=(const Matrix& other);
    Matrix& operator-=(const Matrix& other);
    
    Matrix operator^(unsigned int n);
    
    bool operator==(const Matrix& other);

    T det();
    
    Matrix inverse();

    Matrix trans();

    unsigned int rank();

    T **get_data();

    static Matrix Idenity(unsigned int n);

    void save(std::ofstream& fd);

private:
    T **data = nullptr;
};


#endif // MATRIX_H 


#ifdef MATRIX_IMPL


#endif // MATRIX_IMPL