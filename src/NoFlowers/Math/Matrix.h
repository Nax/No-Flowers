#ifndef MATH_MATRIX_H
#define MATH_MATRIX_H 1

#include <NoFlowers/Math/Vector.h>

namespace internal
{
    template <typename T, size_t N>
    class MatrixHelper
    {
    public:
        MatrixHelper(T* data)
        : _helper(data)
        {

        }

        T& operator[](size_t i)
        {
            return _helper[i];
        }

        const T& operator[](size_t i) const
        {
            return _helper[i];
        }

    private:
        T* _helper;
    };

}
/**
 * @endcond
 */

 /**
  * @brief A class representing a square matrix
  * @tparam N The matrix dimension
  * @tparam T The matrix type
  */
template <typename T, size_t N>
class Matrix
{
public:
    /**
     * Create a null matrix
     */
    Matrix()
    : _data()
    {

    }

    /**
     * Multiply a matrix
     * @param rhs The other matrix
     * @return Self
     */
    Matrix<T, N>&
    operator*=(const Matrix<T, N>& rhs)
    {
        *this = *this * rhs;
        return *this;
    }

    /**
     * Get a proxy line
     * @param i The line index
     * @return A matrix line
     */
    internal::MatrixHelper<T, N>
    operator[](size_t i)
    {
        return internal::MatrixHelper<T, N>(_data + i * N);
    }

    /**
     * Get a proxy line
     * @param i The line index
     * @return A matrix line
     */
    const internal::MatrixHelper<const T, N>
    operator[](size_t i) const
    {
        return internal::MatrixHelper<const T, N>(_data + i * N);
    }

    /**
     * Get a raw buffer from the matrix
     * @return The raw buffer
     */
    T*
    data()
    {
        return _data;
    }

    /**
     * Get a raw buffer from the matrix
     * @return The raw buffer
     */
    const T*
    data() const
    {
        return _data;
    }

    /**
     * Get the identity matrix
     * @return The identity matrix
     */
    static Matrix<T, N>
    identity()
    {
        Matrix<T, N> m;

        for (size_t i = 0; i < N; ++i)
            m[i][i] = 1;
        return m;
    }

private:
    T _data[N * N];
};

/**
 * Multiply two matrices
 * @param lhs The left matrix
 * @param rhs The right matrix
 * @return The product of the matrices
 */
template <typename T, size_t N>
Matrix<T, N>
operator*(const Matrix<T, N>& lhs, const Matrix<T, N>& rhs)
{
    Matrix<T, N> m;

    for (size_t j = 0; j < N; ++j)
    {
        for (size_t i = 0; i < N; ++i)
        {
            for (size_t k = 0; k < N; ++k)
                m[i][j] += lhs[i][k] * rhs[k][j];
        }
    }
    return m;
}

/**
 * Multiply a matrix and a scalar
 * @param lhs The matrix
 * @param rhs The scalar
 * @return The product
 */
template <typename T, size_t N>
Matrix<T, N>
operator*(const Matrix<T, N>& lhs, T rhs)
{
    Matrix<T, N> m;

    for (size_t j = 0; j < N; ++j)
    {
        for (size_t i = 0; i < N; ++i)
            m[i][j] = lhs[i][j] * rhs;
    }
    return m;
}

/**
 * Multiply a matrix and a scalar
 * @param lhs The scalar
 * @param rhs The matrix
 * @return The product
 */
template <typename T, size_t N>
Matrix<T, N>
operator*(T lhs, const Matrix<T, N>& rhs)
{
    Matrix<T, N> m;

    for (size_t j = 0; j < N; ++j)
    {
        for (size_t i = 0; i < N; ++i)
            m[i][j] = lhs * rhs[i][j];
    }
    return m;
}

template <typename T, size_t N>
Vector<T, N>
operator*(const Matrix<T, N>& mat, const Vector<T, N>& vect)
{
    Vector<T, N> result;
    for (size_t j = 0; j < N; ++j)
    {
        for (size_t i = 0; i < N; ++i)
            result[j] += mat[i][j] * vect[i];
    }
    return result;
}

using Matrix2f = Matrix<float, 2>;
using Matrix3f = Matrix<float, 3>;
using Matrix4f = Matrix<float, 4>;

#endif
