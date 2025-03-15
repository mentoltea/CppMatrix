#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <assert.h>

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
    
    Matrix submatrix(unsigned int yfrom, unsigned int ysize,
        unsigned int xfrom, unsigned int xsize) const;

    Matrix minor(unsigned int y, unsigned int x) const;

    T* operator[](unsigned int y);
    

    Matrix operator*(const Matrix& other) const;
    Matrix operator+(const Matrix& other) const;
    Matrix operator-(const Matrix& other) const;
    
    Matrix& operator+=(const Matrix& other);
    Matrix& operator-=(const Matrix& other);

    Matrix operator*(T k) const;
    Matrix& operator*=(T k);
    
    Matrix operator^(unsigned int n) const;
    
    bool operator==(const Matrix& other) const;
    bool operator!=(const Matrix& other) const;

    T det() const;
    
    Matrix trans() const;

    Matrix inverse() const;

    unsigned int rank() const;

    T **get_data();

    static Matrix Idenity(unsigned int n);

    static Matrix read(std::ifstream& fd) ;
    void save(std::ofstream& fd) const ;
    
    void print(int identation=3) const;


private:
    T **data = nullptr;
};

// template<typename T>
// Matrix<T> operator*(const Matrix<T>& A, const Matrix<T>& B);

// template<typename T>
// Matrix<T> Idenity(unsigned int n);

template<typename T>
Matrix<T> operator*(T k, const Matrix<T>& A);

#endif // MATRIX_H 


// #define MATRIX_IMPL
#ifdef MATRIX_IMPL

template<typename T>
Matrix<T>::Matrix(unsigned int a, unsigned int b, T initvalue) {
    this->a = a;
    this->b = b;
    this->data = new T*[a];
    for (int y=0; y<a; y++) {
        data[y] = new T[b];
        for (int x=0; x<b; x++) {
            data[y][x] = initvalue;
        }
    }
}

template<typename T>
Matrix<T>::Matrix(const Matrix& other) {
    this->a = other.a;
    this->b = other.b;
    this->data = new T*[a];

    for (int y=0; y<a; y++) {
        data[y] = new T[b];
        memcpy(this->data[y], other.data[y], b*sizeof(T));
    }
}

template<typename T>
Matrix<T>::Matrix(Matrix&& other) {
    this->a = other.a;
    this->b = other.b;
    this->data = other.data;

    other.a = 0;
    other.b = 0;
    other.data = nullptr;
}

template<typename T>
Matrix<T>::~Matrix() {
    for (int y=0; y<a; y++) {
        delete[] this->data[y];
    }
    delete[] this->data;
}


template<typename T>
Matrix<T>& Matrix<T>::operator=(const Matrix& other) {
    if (this != &other) {
        if (this->data) {
            for (int y=0; y<a; y++) {
                delete[] this->data[y];
            }
            delete[] this->data;
        }

        this->a = other.a;
        this->b = other.b;
        this->data = new T*[a];

        for (int y=0; y<a; y++) {
            data[y] = new T[b];
            memcpy(this->data[y], other.data[y], b*sizeof(T));
        }
    }
    return *this;
}

template<typename T>
Matrix<T>& Matrix<T>::operator=(Matrix&& other) {
    if (this != &other) {
        if (this->data) {
            for (int y=0; y<a; y++) {
                delete[] this->data[y];
            }
            delete[] this->data;
        }
        
        this->a = other.a;
        this->b = other.b;
        this->data = other.data;
        
        other.a = 0;
        other.b = 0;
        other.data = nullptr;
    }
    return *this;
}

template<typename T>
Matrix<T> Matrix<T>::submatrix(unsigned int yfrom, unsigned int ysize,
    unsigned int xfrom, unsigned int xsize) const {
    Matrix result(ysize, xsize);
    for (int y=0; y<ysize; y++) {
        for (int x=0; x<xsize; x++) {
            result.data[y][x] = this->data[yfrom+y][xfrom+x];
        }
    }
    return result;
}

template<typename T>
Matrix<T> Matrix<T>::minor(unsigned int y0, unsigned int x0) const {
    // assert(this->a == this->b);

    Matrix result(a-1, b-1);
    
    int my = 0;
    for (int y=0; y<a; y++) {
        if (y == y0) continue;
        int mx = 0;
        for (int x=0; x<b; x++) {
            if (x == x0) continue;
            result.data[my][mx] = this->data[y][x];
            mx++;
        }
        my++;
    }
    
    return result;
}

template<typename T>
T* Matrix<T>::operator[](unsigned int y) {
    return this->data[y];
}

// template<typename T>
// Matrix<T> operator*(const Matrix<T>& A, const Matrix<T>& B) {
//     assert(A.b == B.a);
// 
//     Matrix<T> result(A.a, B.b);
//     for (int y=0; y<A.a; y++) {
//         for (int x=0; x<B.b; x++) {
//             T sum = 0;
//             for (int i = 0; i<A.b; i++) {
//                 sum += A.data[y][i] * B.data[i][x];
//             }
//             result.data[y][x] = sum;
//         }
//     }
// 
//     return result;
// }

template<typename T>
Matrix<T> Matrix<T>::operator*(const Matrix& other) const {
    assert(this->b == other.a);

    Matrix<T> result(this->a, other.b);
    for (int y=0; y<this->a; y++) {
        for (int x=0; x<other.b; x++) {
            T sum = 0;
            for (int i = 0; i<this->b; i++) {
                sum += this->data[y][i] * other.data[i][x];
            }
            result.data[y][x] = sum;
        }
    }

    return result;
}

template<typename T>
Matrix<T> Matrix<T>::operator+(const Matrix& other) const {
    assert(this->a == other.a);
    assert(this->b == other.b);

    Matrix<T> result(a, b);
    for (int y=0; y<a; y++) {
        for (int x=0; x<b; x++) {
            result.data[y][x] = this->data[y][x] + other.data[y][x];
        }
    }

    return result;
}

template<typename T>
Matrix<T> Matrix<T>::operator-(const Matrix& other) const {
    assert(this->a == other.a);
    assert(this->b == other.b);
    
    Matrix<T> result(a, b);
    for (int y=0; y<a; y++) {
        for (int x=0; x<b; x++) {
            result.data[y][x] = this->data[y][x] - other.data[y][x];
        }
    }
    
    return result;
}

template<typename T>
Matrix<T>& Matrix<T>::operator+=(const Matrix& other) {
    assert(this->a == other.a);
    assert(this->b == other.b);

    for (int y=0; y<a; y++) {
        for (int x=0; x<b; x++) {
            this->data[y][x] = this->data[y][x] + other.data[y][x];
        }
    }

    return *this;
}

template<typename T>
Matrix<T>& Matrix<T>::operator-=(const Matrix& other) {
    assert(this->a == other.a);
    assert(this->b == other.b);

    for (int y=0; y<a; y++) {
        for (int x=0; x<b; x++) {
            this->data[y][x] = this->data[y][x] - other.data[y][x];
        }
    }

    return *this;
}

template<typename T>
Matrix<T> Matrix<T>::operator*(T k) const {
    Matrix<T> result(a, b);
    for (int y=0; y<a; y++) {
        for (int x=0; x<b; x++) {
            result.data[y][x] = this->data[y][x] * k;
        }
    }

    return result;
}

template<typename T>
Matrix<T>& Matrix<T>::operator*=(T k) {
    for (int y=0; y<a; y++) {
        for (int x=0; x<b; x++) {
            this->data[y][x] = this->data[y][x] * k;
        }
    }
    
    return *this;
}


template<typename T>
Matrix<T> Matrix<T>::operator^(unsigned int n) const {
    assert(this->a == this->b);
    Matrix result = Matrix::Idenity(this->a);
    for (int i=0; i<n; i++) {
        result = result * (*this);
    }
    return result;
}

template<typename T>
bool Matrix<T>::operator==(const Matrix& other) const {
    if (this->a != other.a || this->b != other.b) return false;
    for (int y=0; y<a; y++) {
        for(int x=0; x<b; x++) {
            if (this->data[y][x] != other.data[y][x]) return false;
        }
    }
    return true;
}

template<typename T>
bool Matrix<T>::operator!=(const Matrix& other) const {
    return !(*this == other);
}



template<typename T>
T Matrix<T>::det() const {
    assert(this->a == this->b);
    if (this->a == 1) {
        return data[0][0];
    }
    if (this->a == 2) {
        return data[0][0]*data[1][1] - data[1][0]*data[0][1];
    }
    
    T sum = 0;
    for (int x=0; x<b; x++) {
        T now = data[0][x] * minor(0, x).det();
        if (x%2 == 0) sum += now;
        else sum -= now;
    }
    return sum;
}


template<typename T>
Matrix<T> Matrix<T>::trans() const {
    Matrix result(b, a);

    for (int y=0; y<a; y++) {
        for (int x=0; x<b; x++) {
            result.data[x][y] = this->data[y][x];
        }
    }

    return result;
}


template<typename T>
Matrix<T> Matrix<T>::inverse() const {
    assert(this->a == this->b);

    T D = det();
    assert(D != 0);

    Matrix result(a, b);
    for (int y=0; y<a; y++) {
        for (int x=0; x<b; x++) {
            result.data[x][y] = minor(y, x).det() / D;
            if ((x+y)%2==1) result.data[x][y] = -result.data[x][y];
        }
    }
    return result;
}

template<typename T>
unsigned int Matrix<T>::rank() const {
    if ((*this) == Matrix<T>(a, b, 0)) return 0;
    
    unsigned int R = a;
    if (b < a) R = b;

    if (R==1) return 1;

    unsigned int size = std::min(this->a, this->b);
    bool stopflag = false;
    while (!stopflag) {
        for (int i=0; i < this->a - size + 1; i++) {
            for (int j=0; j < this->b - size + 1; j++) {
                Matrix sub = this->submatrix(i, size, j, size);
                T D = sub.det();
                if (D != 0) {
                    stopflag = true;
                    break;
                }
            }
            if (stopflag) break;
        }
        if (!stopflag) {size--;}
    }

    return size;
}

template<typename T>
T **Matrix<T>::get_data() {
    return this->data;
}


template<typename T>
Matrix<T> Matrix<T>::Idenity(unsigned int n) {
    Matrix<T> result(n, n);
    for (int i=0; i<n; i++) {
        result.data[i][i] = 1;
    }
    return result;
}

template<typename T>
void Matrix<T>::print(int identation) const {
    for (int y=0; y<a; y++) {
        std::cout << "|";
        for (int x=0; x<b; x++) {
            std::cout << std::setw(identation) << data[y][x] << std::setw(identation);
        }
        std::cout << "|" << std::endl;
    }
}


template<typename T>
Matrix<T> operator*(T k, const Matrix<T>& A) {
    return A*k;
}

#endif 