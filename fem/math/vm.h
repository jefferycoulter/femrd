#ifndef MV_INCL
#define MV_INCL

#include <cstdlib> // definition of size_t
#include "logger/logger.h"

#include <initializer_list>

/**
 * @brief enumeration for determining matrix storage type. not implemented
 */
typedef enum class StorageType : short
{
    Dense, Sparse
} StorageType;

/**
 * @brief template class defining a dense matrix object and related functions. matrix elements 
 * are stored in row major order. vectors are a implemented as a special case of the matrix class
 * with C = 1.
 * @tparam T data type
 * @tparam R number of rows
 * @tparam C number of columns
 */
template<typename T, size_t R, size_t C>
class Matrix
{
    public: // constructors

        /**
         * @brief construct an empty matrix object with memory allocated for R*C elements
         */
        Matrix();

        /**
         * @brief construct a matrix object using an initializer list
         * @param l initializer list containing the matrix elements
         */
        Matrix(std::initializer_list<T> l);

        /**
         * @brief construct a matrix object using a c-style array
         * @param l array containing the matrix elements
         */
        Matrix(const T *m);

        /**
         * @brief construct a matrix object with all elements set to the constant value a
         * @param a element value
         */
        Matrix(const T &a);

        Matrix(const Matrix<T, R, C>&) = default;
        Matrix<T, R, C>(Matrix<T, R, C>&&) = default;
        Matrix<T, R, C>& operator=(Matrix<T, R, C>&&) = default;

    public: // computational methods
        /**
         * @brief compute the dot matrix-matrix or matrix-vector product
         * @tparam r number of rows for rhs
         * @tparam c number of columns for rhs
         * @param rhs matrix/vector to multiply with
         * @return Matrix<T, R, c> 
         */
        template<size_t r, size_t c>
        Matrix<T, R, c> Dot(const Matrix<T, r, c> &rhs);

        /**
         * @brief compute the outer product
         * @tparam r number of rows for rhs
         * @tparam c number of columns for rhs
         * @param rhs matrix/vector to multiply with
         * @return Matrix<T, R, c> 
         */
        template<size_t r, size_t c>
        Matrix<T, R*r, C*c> Outer(const Matrix<T, r, c> &rhs);

        /**
         * @brief compute the matrix transpose
         * @return Matrix<T, R, C> 
         */
        Matrix<T, C, R> Transpose();

        /**
         * @brief compute the matrix inverse if possible
         * @return Matrix<T, R, C> 
         */
        Matrix<T, R, C> Inverse();

        /**
         * @brief compute the determinant if possible
         * @return Matrix<T, R, C> 
         */
        T Determinant();

        /**
         * @brief compute the trace
         * @return T 
         */
        T Trace();

    public: // operator overloads

        /**
         * @brief overload to allow for indexing elements using A(i,j) notation. allows for
         * both read and write access
         * @param i row index
         * @param j column index
         * @return T& 
         */
        T& operator()(size_t i, size_t j);

        /**
         * @brief overload to allow for indexing elements using A(i,j) notation. only allows for
         * both read access
         * @param i row index
         * @param j column index
         * @return T& 
         */
        const T& operator()(size_t i, size_t j) const;

        /**
         * @brief set *this matrix to be equal to another matrix (copy operator)
         * @param m matrix to copy
         * @return Matrix<T, R, C>& 
         */
        Matrix<T, R, C>& operator=(const Matrix<T, R, C> &m);

        /**
         * @brief check for equality between *this and another matrix
         * @param m matrix to test equality with
         * @return true 
         * @return false 
         */
        bool operator==(const Matrix<T, R, C> &m);

        // scalar-matrix operations

        /**
         * @brief add a scalar to each element of *this matrix and return a new matrix, leaving
         * *this unaltered
         * @param s scalar value
         * @return Matrix<T, R, C> 
         */
        Matrix<T, R, C> operator+(const T &s);

        /**
         * @brief add a scalar to each element of *this matrix
         * @param s scalar value
         */
        void operator+=(const T &s);

        /**
         * @brief subtract a scalar from each element of *this matrix and return a new matrix, leaving
         * *this unaltered
         * @param s scalar value
         * @return Matrix<T, R, C> 
         */
        Matrix<T, R, C> operator-(const T &s);

        /**
         * @brief subtract a scalar from each element of *this matrix
         * @param s scalar value
         */
        void operator-=(const T &s);

        /**
         * @brief multiply each element of *this matrix by a scalar and return a new matrix, leaving
         * *this unaltered
         * @param s scalar value
         * @return Matrix<T, R, C> 
         */
        Matrix<T, R, C> operator*(const T &s);

        /**
         * @brief multiply each element of *this matrix by a scalar value
         * @param s scalar value
         */
        void operator*=(const T &s);

        /**
         * @brief divide each element of *this matrix by a scalar and return a new matrix, leaving
         * *this unaltered
         * @param s scalar value
         * @return Matrix<T, R, C> 
         */
        Matrix<T, R, C> operator/(const T &s);

        /**
         * @brief divide each element of *this matrix by a scalar value
         * @param s scalar value
         */
        void operator/=(const T &s);

        // matrix-matrix operations

        /**
         * @brief component-wise addition with *this, returning a new matrix
         * @param M 
         * @return Matrix<T, R, C> 
         */
        Matrix<T, R, C> operator+(const Matrix<T, R, C> &M);

        /**
         * @brief component-wise addition with *this
         * @param M 
         */
        void operator+=(const Matrix<T, R, C> &M);

        /**
         * @brief component-wise subtraction with *this, returning a new matrix
         * @param M 
         * @return Matrix<T, R, C> 
         */
        Matrix<T, R, C> operator-(const Matrix<T, R, C> &M);

        /**
         * @brief component-wise subtraction with *this
         * @param M 
         */
        void operator-=(const Matrix<T, R, C> &M);

        /**
         * @brief component-wise multiplication with *this, returning a new matrix
         * @param M 
         * @return Matrix<T, R, C> 
         */
        Matrix<T, R, C> operator*(const Matrix<T, R, C> &M);

        /**
         * @brief component-wise multiplication with *this
         * @param M 
         */
        void operator*=(const Matrix<T, R, C> &M);

        /**
         * @brief component-wise division with *this, returning a new matrix
         * @param M 
         * @return Matrix<T, R, C> 
         */
        Matrix<T, R, C> operator/(const Matrix<T, R, C> &M);

        /**
         * @brief component-wise division with *this
         * @param M 
         */
        void operator/=(const Matrix<T, R, C> &M);

    public: // other methods
        /**
         * @brief returns an n by n identity Matrixrix
         * @param n dimension of Matrixrix
         * @return Matrix<T, R, C> 
         */
        static Matrix<T, R, C> Eye();

        /**
         * @brief returns a square n by n Matrixrix of zeros
         * @param n dimensions of Matrixrix
         * @return Matrix<T, R, C> 
         */
        static Matrix<T, R, C> Zeros();

        /**
         * @brief returns a square n by n Matrixrix of ones
         * @param n 
         * @return Matrix<T, R, C> 
         */
        static Matrix<T, R, C> Ones();

        /** @brief print matrix elements. only used for testing */
        void print();

    private: // methods
        /**
         * @brief subroutine for computing matrix transpose
         * @link https://stackoverflow.com/questions/5200338/a-cache-efficient-matrix-transpose-program
         * @param rb row beginning index
         * @param re row end index
         * @param cb column beginning index
         * @param ce column end index
         * @param m matrix to transpose
         */
        void transpose(int rb, int re, int cb, int ce, Matrix<T, C, R> m);

        /** @brief ensures that there is at least one row and one column */
        void DimCheck();
        
        /**
         * @brief reallocate memory capacity if storage runs out
         * @param new_cap 
         */
        void Realloc(size_t new_cap);

    private: // variables
        size_t rows;    /** @brief number of rows */
        size_t cols;    /** @brief number of cols */
        size_t size;    /** @brief number of elements */
        size_t cap;     /** @brief memory allocated to matrix */
        T *elems;       /** @brief matrix elements */
};

template<typename T, size_t R>
using Vector = Matrix<T, R, 1>;

// implementation of matrix class
#include "vm_impl.h"

#endif // MV_INCL