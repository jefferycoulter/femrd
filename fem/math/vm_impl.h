#ifndef VM_IMPL_H
#define VM_IMPL_H

#include <iostream>

/////////////////////////////////////////////////////////////////////////////////////
// constructors
/////////////////////////////////////////////////////////////////////////////////////

template<typename T, size_t R, size_t C>
Matrix<T, R, C>::Matrix()
: size(R*C), elems(nullptr) 
{
    cap = size*sizeof(T);
    elems = (T*)calloc(size, sizeof(T));
}

template<typename T, size_t R, size_t C>
Matrix<T, R, C>::Matrix(std::initializer_list<T> l)
: size(R*C), cap(sizeof(R*C))
{
    Assert(size == l.size(), "invalid number of elements in initializer list")
    cap = size*sizeof(T);
    elems = (T*)calloc(size, sizeof(T));
    int idx = 0;
    for (auto it = l.begin(); it != l.end(); it++)
    {
        elems[idx] = *it;
        idx++;
    }
}

template<typename T, size_t R, size_t C>
Matrix<T, R, C>::Matrix(const T *m)
: size(R*C), cap(sizeof(R*C))
{
    cap = size*sizeof(T);
    elems = (T*)calloc(size, sizeof(T));
    for (size_t i = 0; i < R; i++)
    {
        for (size_t j = 0; j < C; j++)
        {
            elems[i*C + j] = m[i*C + j];
        }
    }
}

template<typename T, size_t R, size_t C>
Matrix<T, R, C>::Matrix(const T &a)
: size(R*C), cap(sizeof(R*C))
{
    cap = size*sizeof(T);
    elems = (T*)calloc(size, sizeof(T));
    for (size_t i = 0; i < R; i++)
    {
        for (size_t j = 0; j < C; j++)
        {
            elems[i*C + j] = a;
        }
    }
}

template<typename T, size_t R, size_t C>
Matrix<T, R, C> Matrix<T, R, C>::Eye()
{
    Assert(R == C, "matrix is not square")
    Matrix<T, R, C> m;
    for (size_t i = 0; i < R; i++)
    {
        m(i,i) = 1;
    }
    return m;
}

template<typename T, size_t R, size_t C>
Matrix<T, R, C> Matrix<T, R, C>::Zeros()
{
    Matrix<T, R, C> m;
    return m;
}

template<typename T, size_t R, size_t C>
Matrix<T, R, C> Matrix<T, R, C>::Ones()
{
    Matrix<T, R, C> m;
    T one = 1;
    for (size_t i = 0; i < R; i++)
    {
        for (size_t j = 0; j < C; j++)
        {
            m(i,j) = one;
        }
    }
    return m;
}

/////////////////////////////////////////////////////////////////////////////////////
// computational functions
/////////////////////////////////////////////////////////////////////////////////////

template<typename T, size_t R, size_t C>
template<size_t r, size_t c>
Matrix<T, R, c> Matrix<T, R, C>::Dot(const Matrix<T, r, c> &rhs)
{
    Assert(C == r, "dimension mismatch")
    Matrix<T, R, c> m;
    for (size_t i = 0; i < R; i++) // loop over lhs rows
    {
        for (size_t j = 0; j < c; j++) // loop over rhs cols
        {
            for (size_t k = 0; k < r; k++) // loop over sum of products
            {
                m(i,j) += elems[i*C + k]*rhs(k,j);
            }
        }
    }
    return m;
}

// TODO
template<typename T, size_t R, size_t C>
template<size_t r, size_t c>
Matrix<T, R*r, C*c> Matrix<T, R, C>::Outer(const Matrix<T, r, c> &rhs)
{
    Matrix<T, R, c> m;
    for (size_t i = 0; i < R*r; i++) // 
    {
        for (size_t j = 0; j < c*c; j++)
        {
            m(i,j) += nullptr;
        }
    }
    return m;
}

template<typename T, size_t R, size_t C>
Matrix<T, C, R> Matrix<T, R, C>::Transpose()
{
    Matrix<T, C, R> m;
    transpose(0, R, 0, C, m);
    return m;
}

template<typename T, size_t R, size_t C>
void Matrix<T, R, C>::transpose(int rb, int re, int cb, int ce, Matrix<T, C, R> M)
{
    int r = re - rb, c = ce - cb;
    if (r <= 16 && c <= 16) 
    {
        for (int i = rb; i < re; i++) 
        {
            for (int j = cb; j < ce; j++) 
            {
                M(j,i) = elems[i*C + j];
            }
        }
    } 
    else if (r >= c) 
    {
        transpose(rb, rb + (r / 2), cb, ce, M);
        transpose(rb + (r / 2), re, cb, ce, M);
    } 
    else 
    {
        transpose(rb, re, cb, cb + (c / 2), M);
        transpose(rb, re, cb + (c / 2), ce, M);
    }
}

template<typename T, size_t R, size_t C>
Matrix<T, R, C> Matrix<T, R, C>::Inverse()
{
    Assert(R > 1, "cannot invert a row vector")
    Assert(C > 1, "cannot invert a column vector")
    Assert(R == C, "cannot invert a non-square matrix")

    // compute determinant
    T det = Determinant();
    Assert(det != 0, "matrix is singular")
}

// TODO
template<typename T, size_t R, size_t C>
T Matrix<T, R, C>::Determinant()
{
    Assert(R > 1, "cannot compute the determinant for a row vector")
    Assert(C > 1, "cannot compute the determinant for a column vector")
    Assert(R == C, "cannot compute the determinant for a non-square matrix")
}

// TODO
template<typename T, size_t R, size_t C>
T Matrix<T, R, C>::Trace()
{
    Assert(R > 1, "cannot compute the trace a row vector")
    Assert(C > 1, "cannot compute the trace a column vector")
    Assert(R == C, "cannot compute the trace a non-square matrix")
}

/////////////////////////////////////////////////////////////////////////////////////
// operator overloads
/////////////////////////////////////////////////////////////////////////////////////

template<typename T, size_t R, size_t C>
T& Matrix<T, R, C>::operator()(size_t i, size_t j)
{
    return elems[i*C + j];
}

template<typename T, size_t R, size_t C>
const T& Matrix<T, R, C>::operator()(size_t i, size_t j) const
{
    return elems[i*C + j];
}

template<typename T, size_t R, size_t C>
Matrix<T, R, C>& Matrix<T, R, C>::operator=(const Matrix<T, R, C> &m)
{
    Matrix<T, R, C> mm;
    for (size_t i = 0; i < R; i ++)
    {
        for (size_t j = 0; j < R; j ++)
        {
            mm[i*C + j] = m(i,j);
        }
    } 

    return mm;
}

template<typename T, size_t R, size_t C>
bool Matrix<T, R, C>::operator==(const Matrix<T, R, C> &m)
{
    for (size_t i = 0; i < R; i ++)
    {
        for (size_t j = 0; j < R; j ++)
        {
            if (elems[i*C + j] != m(i,j)) { return false; };
        }
    } 
    return true;
}

// scalar-matrix operations

template<typename T, size_t R, size_t C>
Matrix<T, R, C> Matrix<T, R, C>::operator+(const T &s)
{
    Matrix<T, R, C> m;
    for (size_t i = 0; i < R; i++)
    {
        for (size_t j = 0; j < C; j++)
        {
            m(i,j) = elems[i*C + j] + s;
        }
    }
    return m;
}

template<typename T, size_t R, size_t C>
void Matrix<T, R, C>::operator+=(const T &s)
{
    for (size_t i = 0; i < R; i++)
    {
        for (size_t j = 0; j < C; j++)
        {
            elems[i*C + j] += s;
        }
    }
}

template<typename T, size_t R, size_t C>
Matrix<T, R, C> Matrix<T, R, C>::operator-(const T &s)
{
    Matrix<T, R, C> m;
    for (size_t i = 0; i < R; i++)
    {
        for (size_t j = 0; j < C; j++)
        {
            m(i,j) = elems[i*C + j] - s;
        }
    }
    return m;
}

template<typename T, size_t R, size_t C>
void Matrix<T, R, C>::operator-=(const T &s)
{
    for (size_t i = 0; i < R; i++)
    {
        for (size_t j = 0; j < C; j++)
        {
            elems[i*C + j] -= s;
        }
    }
}

template<typename T, size_t R, size_t C>
Matrix<T, R, C> Matrix<T, R, C>::operator*(const T &s)
{
    Matrix<T, R, C> m;
    for (size_t i = 0; i < R; i++)
    {
        for (size_t j = 0; j < C; j++)
        {
            m(i,j) = elems[i*C + j] * s;
        }
    }
    return m;
}

template<typename T, size_t R, size_t C>
void Matrix<T, R, C>::operator*=(const T &s)
{
    for (size_t i = 0; i < R; i++)
    {
        for (size_t j = 0; j < C; j++)
        {
            elems[i*C + j] *= s;
        }
    }
}

template<typename T, size_t R, size_t C>
Matrix<T, R, C> Matrix<T, R, C>::operator/(const T &s)
{
    Matrix<T, R, C> m;
    for (size_t i = 0; i < R; i++)
    {
        for (size_t j = 0; j < C; j++)
        {
            m(i,j) = elems[i*C + j] / s;
        }
    }
    return m;
}

template<typename T, size_t R, size_t C>
void Matrix<T, R, C>::operator/=(const T &s)
{
    for (size_t i = 0; i < R; i++)
    {
        for (size_t j = 0; j < C; j++)
        {
            elems[i*C + j] /= s;
        }
    }
}

// matrix-matrix operations

template<typename T, size_t R, size_t C>
Matrix<T, R, C> Matrix<T, R, C>::operator+(const Matrix<T, R, C> &M)
{
    Matrix<T, R, C> m;
    for (size_t i = 0; i < R; i++)
    {
        for (size_t j = 0; j < C; j++)
        {
            m(i,j) = elems[i*C + j] + M(i,j);
        }
    }
    return m;
}

template<typename T, size_t R, size_t C>
void Matrix<T, R, C>::operator+=(const Matrix<T, R, C> &M)
{
    for (size_t i = 0; i < R; i++)
    {
        for (size_t j = 0; j < C; j++)
        {
            elems[i*C + j] += M(i,j);
        }
    }
}

template<typename T, size_t R, size_t C>
Matrix<T, R, C> Matrix<T, R, C>::operator-(const Matrix<T, R, C> &M)
{
    Matrix<T, R, C> m;
    for (size_t i = 0; i < R; i++)
    {
        for (size_t j = 0; j < C; j++)
        {
            m(i,j) = elems[i*C + j] - M(i,j);
        }
    }
    return m;
}

template<typename T, size_t R, size_t C>
void Matrix<T, R, C>::operator-=(const Matrix<T, R, C> &M)
{
    for (size_t i = 0; i < R; i++)
    {
        for (size_t j = 0; j < C; j++)
        {
            elems[i*C + j] -= M(i,j);
        }
    }
}

template<typename T, size_t R, size_t C>
Matrix<T, R, C> Matrix<T, R, C>::operator*(const Matrix<T, R, C> &M)
{
    Matrix<T, R, C> m;
    for (size_t i = 0; i < R; i++)
    {
        for (size_t j = 0; j < C; j++)
        {
            m(i,j) = elems[i*C + j] * M(i,j);
        }
    }
    return m;
}

template<typename T, size_t R, size_t C>
void Matrix<T, R, C>::operator*=(const Matrix<T, R, C> &M)
{
    for (size_t i = 0; i < R; i++)
    {
        for (size_t j = 0; j < C; j++)
        {
            elems[i*C + j] *= M(i,j);
        }
    }
}

template<typename T, size_t R, size_t C>
Matrix<T, R, C> Matrix<T, R, C>::operator/(const Matrix<T, R, C> &M)
{
    Matrix<T, R, C> m;
    for (size_t i = 0; i < R; i++)
    {
        for (size_t j = 0; j < C; j++)
        {
            m(i,j) = elems[i*C + j] / M(i,j);
        }
    }
    return m;
}

template<typename T, size_t R, size_t C>
void Matrix<T, R, C>::operator/=(const Matrix<T, R, C> &M)
{
    for (size_t i = 0; i < R; i++)
    {
        for (size_t j = 0; j < C; j++)
        {
            elems[i*C + j] /= M(i,j);
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////
// private functions
/////////////////////////////////////////////////////////////////////////////////////

template<typename T, size_t R, size_t C>
void Matrix<T, R, C>::DimCheck()
{
    Assert(R > 0, "matrix class requires at least 1 row")
    Assert(C > 0, "matrix class requires at least 1 column")
}

template<typename T, size_t R, size_t C>
void Matrix<T, R, C>::Realloc(size_t new_cap)
{
    T *new_block = (T*)calloc(size, sizeof(T));
    for (size_t i = 0;  i < size; i++) { new_block[i] = elems[i]; }
    free(elems);
    elems = new_block;
    cap = new_cap;
}

template<typename T, size_t R, size_t C>
void Matrix<T, R, C>::print()
{
    for (size_t i = 0; i < R; i++)
    {
        for (size_t j = 0; j < C; j++)
        {   
            std::cout << elems[i*C + j] << " ";
        }
        std::cout << "\n";
    }
}

#endif // VM_IMPL_H