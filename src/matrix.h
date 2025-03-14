#ifndef MATRIX_H
#define MATRIX_H

#include <fstream>

class Matrix {
public:
    unsigned int a, b; // dimensions of matrix
    
    // @param a Number of rows
    // @param b Number of columns
    // @param initvalue Value to initialize matrix with
    Matrix(unsigned int a, unsigned int b, double initvalue=0);
    Matrix(std::ifstream fd);
    
    Matrix(const Matrix& other);
    Matrix(Matrix&& other);
    ~Matrix();
    
    Matrix& operator=(const Matrix& other);
    Matrix& operator=(Matrix&& other);
    
    
    Matrix operator*(const Matrix& other);
    Matrix operator+(const Matrix& other);
    Matrix operator-(const Matrix& other);
    
    Matrix& operator*=(const Matrix& other);
    Matrix& operator+=(const Matrix& other);
    Matrix& operator-=(const Matrix& other);
    
    Matrix operator^(unsigned int n);
    
    double det();
    
    Matrix inverse();

    Matrix trans();

    unsigned int rank();

    double **get_data();

private:
    double **data = nullptr;
};



#endif // MATRIX_H 