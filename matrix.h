#ifndef MATRIX_H
#define MATRIX_H

class Matrix {
public:
    unsigned int a, b; // dimensions of matrix
    double **data;

    Matrix(unsigned int a, unsigned int b, double initvalue=0);
    ~Matrix();

    
};

#endif // MATRIX_H 