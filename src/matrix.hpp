#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <assert.h>
#include <stdint.h>
#include <exception>
#include <initializer_list>

// @param T
// It can be any type as long as it:
// 
// 1. Supports initialization with 0
// 
// 2. Direct copy with `memcpy`
// 
// 3. Initialization with `>>`
// 
// 4. Serialization with `<<`
// 
// 5. Basic arithmetic operations (+, -, *, /, +=, -=, *=, /=)
template <typename T>
class Matrix {
public:
    unsigned int a; // number of rows of matrix
    unsigned int b; // number of columns of matrix
    
    // @param a Number of rows
    // @param b Number of columns
    // @param initvalue Value to initialize matrix with
    // @throws `std::runtime_error` on error (if one of sizes is 0)
    Matrix(unsigned int a, unsigned int b, T initvalue=0);

    // @param fd Opened `IFSTREAM` file descriptor to matrix file
    // @note Is based on `Matrix::read(std::ifstream& fd)`
    // @throws `std::runtime_error` on error (look `Matrix::read(std::ifstream& fd)` exceptions)
    Matrix(std::ifstream& fd);

    // @param m Initializer list = `{ {...}, {...}, ...}`
    // @note If inner lists sizes are different, the minimal size is choosen
    // @throws `std::runtime_error` on error (if one of sizes is 0)
    Matrix(const std::initializer_list< std::initializer_list<T> > &m);
    
    // @note Uses `memcpy` to copy rows
    Matrix(const Matrix& other);
    Matrix(Matrix&& other);
    ~Matrix();

    // @note Uses `memcpy` to copy rows
    Matrix& operator=(const Matrix& other);
    Matrix& operator=(Matrix&& other);
    
    // @returns Submatrix with size (ysize, xsize) taken from (yfrom, xfrom)
    Matrix submatrix(unsigned int yfrom, unsigned int ysize, unsigned int xfrom, unsigned int xsize) const;

    // @returns A matrix missing specified row and column
    // @note If you are using it to calculate determinant or inverse matrix, better use `.det()` or `.inverse()`
    Matrix minor(unsigned int y, unsigned int x) const;

    // @returns Pointer to specified row
    // @note Changing data the pointer refers to WILL affect the matrix itself. 
    // @note DO NOT FREE THE POINTER !
    T* operator[](unsigned int y);
    
    // @throws `std::runtime_error` on mismatching sizes (this.b != other.a)
    Matrix operator*(const Matrix& other) const;
    // @throws `std::runtime_error` on mismatching sizes
    Matrix operator+(const Matrix& other) const;
    // @throws `std::runtime_error` on mismatching sizes
    Matrix operator-(const Matrix& other) const;
    Matrix operator-() const;
    
    // @throws `std::runtime_error` on mismatching sizes
    Matrix& operator+=(const Matrix& other);
    // @throws `std::runtime_error` on mismatching sizes
    Matrix& operator-=(const Matrix& other);

    Matrix operator*(T k) const;
    Matrix& operator*=(T k);
    
    // @note Raises matrix to non-negative power
    // @note Only for square matrices !
    // @throws `std::runtime_error` on non-square matrix
    Matrix operator^(unsigned int n) const;
    
    bool operator==(const Matrix& other) const;
    bool operator!=(const Matrix& other) const;

    // @returns Matrix' determinant
    // @note Only square matrices!
    // @throws `std::runtime_error` on non-square matrix
    T det() const;
    
    // @returns Transcripted matrix
    Matrix trans() const;

    // @return The inverse matrix
    // @note Only square matrices with det != 0 !
    // @throws `std::runtime_error` on non-square matrix or if determinant = 0
    Matrix inverse() const;

    // @returns The rank of matrix
    // @note Works with both square and non-square matrices
    unsigned int rank() const;

    // @returns Pointer [to pointers] to data
    // @note It is highly suggested NOT TO USE it directly
    T **get_data();

    // @param n Size
    // @returns Idenity square matrix of size (n, n)
    static Matrix Idenity(unsigned int n);

    // @returns Matrix readen from given file descriptor
    // @param fd Opened `IFSTREAM` file descriptor to matrix file
    // @throws `std::runtime_error` on error (0 sizes or too early EOF)
    static Matrix read(std::ifstream& fd) ;

    // @param fd Opened `OFSTREAM` file descriptor to output file 
    void save(std::ofstream& fd) const ;
    
    // @note Prints matrix to the `stdout`
    // @param identation The distance between columns
    void print(int identation=3) const;

private:
    T **data = nullptr;
};


template<typename T>
Matrix<T> operator*(T k, const Matrix<T>& A);

#endif // MATRIX_H 


// #define MATRIX_IMPL
#ifdef MATRIX_IMPL

template<typename T>
Matrix<T> Matrix<T>::read(std::ifstream& fd) {
    unsigned int height, width;
    fd >> height;
    fd >> width;

    // assert(height != 0);
    // assert(width != 0);
    if (height==0) {
        throw std::runtime_error("READING: Cannot create matrix with 0 rows");
    }
    if (width==0) {
        throw std::runtime_error("READING: Cannot create matrix with 0 columns");
    }
    
    Matrix<T> result(height, width);
    for (int y=0; y<height; y++) {
        for (int x=0; x<width; x++) {
            if (fd.eof()) {
                // assert(0 && "File ended");
                throw std::runtime_error("READING: File ended before matrix was filled");
            }
            fd >> result[y][x];
        }
        fd.ignore(INT64_MAX, '\n');
    }

    return result;
}

template<typename T>
void Matrix<T>::save(std::ofstream& fd) const  {
    fd << this->a << " " << this->b << std::endl;

    for (int y=0; y<a; y++) {
        for (int x=0; x<b; x++) {
            fd << this->data[y][x];
            if (x != b-1) fd << " ";
        }
        fd << std::endl;
    }
}

template<typename T>
Matrix<T>::Matrix(std::ifstream& fd) {
    (*this) = Matrix<T>::read(fd);
}

template<typename T>
Matrix<T>::Matrix(unsigned int a, unsigned int b, T initvalue) {
    if (a==0) {
        throw std::runtime_error("CREATING: Cannot create matrix with 0 rows");
    }
    this->a = a;
    if (b==0) {
        throw std::runtime_error("CREATING: Cannot create matrix with 0 columns");
    }
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
Matrix<T>::Matrix(const std::initializer_list< std::initializer_list<T> > &m) {
    this->a = m.size();
    if (a==0) {
        throw std::runtime_error("CREATING: Cannot create matrix with 0 rows");
    }
    unsigned int minb = m.begin()->size();
    for (auto it = m.begin(); it != m.end(); it++) {
        if (it->size() < minb) minb = it->size();
    }
    if (minb==0) {
        throw std::runtime_error("CREATING: Cannot create matrix with 0 columns");
    }
    b = minb;

    this->data = new T*[a];
    
    auto ity = m.begin();
    for (int y=0; y<a; y++) {
        data[y] = new T[b];
        auto itx = ity->begin();
        for (int x=0; x<b; x++) {
            data[y][x] = *itx;
            itx++;
        }
        ity++;
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
    if (data) {
        for (int y=0; y<a; y++) {
            if (this->data[y]) delete[] this->data[y];
        }
        delete[] this->data;
    }
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

//     return result;
// }

template<typename T>
Matrix<T> Matrix<T>::operator*(const Matrix& other) const {
    // assert(this->b == other.a);
    if (this->b != other.a) 
        throw std::runtime_error("MATRIX MULTIPLICATION: can multiply only matrices (M x N) * (N x K)");

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
    // assert(this->a == other.a);
    // assert(this->b == other.b);
    if (this->a != other.b || this->b != other.b) 
        throw std::runtime_error("ADDING: can add only matrices with the same size");

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
    // assert(this->a == other.a);
    // assert(this->b == other.b);
    if (this->a != other.b || this->b != other.b) 
        throw std::runtime_error("SUBTRACTION: can subtract only matrices with the same size");

    Matrix<T> result(a, b);
    for (int y=0; y<a; y++) {
        for (int x=0; x<b; x++) {
            result.data[y][x] = this->data[y][x] - other.data[y][x];
        }
    }
    
    return result;
}

template<typename T>
Matrix<T> Matrix<T>::operator-() const {
    Matrix<T> result(a, b);
    for (int y=0; y<a; y++) {
        for (int x=0; x<b; x++) {
            result.data[y][x] = -data[y][x];
        }
    }
    
    return result;
}

template<typename T>
Matrix<T>& Matrix<T>::operator+=(const Matrix& other) {
    // assert(this->a == other.a);
    // assert(this->b == other.b);
    if (this->a != other.b || this->b != other.b) 
        throw std::runtime_error("ADDING: can add only matrices with the same size");
        
    for (int y=0; y<a; y++) {
        for (int x=0; x<b; x++) {
            this->data[y][x] = this->data[y][x] + other.data[y][x];
        }
    }

    return *this;
}

template<typename T>
Matrix<T>& Matrix<T>::operator-=(const Matrix& other) {
    // assert(this->a == other.a);
    // assert(this->b == other.b);
    if (this->a != other.b || this->b != other.b) 
        throw std::runtime_error("SUBTRACTION: can subtract only matrices with the same size");

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
    // assert(this->a == this->b);
    if (this->a != this->b) 
        throw std::runtime_error("POWER: can power only square matrices");

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
    // assert(this->a == this->b);
    if (this->a != this->b) 
        throw std::runtime_error("DETERMINANT: can compute det only for square matrices");

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
    // assert(this->a == this->b);
    if (this->a != this->b) 
        throw std::runtime_error("INVERSE: can compute inverse only for square matrices");


    T D = det();
    // assert(D != 0);
    if (D == 0) 
        throw std::runtime_error("INVERSE: can compute inverse only for matrices with non-zero determinant");


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