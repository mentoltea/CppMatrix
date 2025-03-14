#ifndef MATRIX_H
#define MATRIX_H

class Matrix {
public:
    unsigned int a, b; // dimensions of matrix
    double **data;

    // @param a Number of rows
    // @param b Number of columns
    // @param initvalue Value to initialize matrix with
    Matrix(unsigned int a, unsigned int b, double initvalue=0);
    ~Matrix();

    
};

#endif // MATRIX_H 