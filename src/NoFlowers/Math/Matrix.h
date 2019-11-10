#ifndef MATH_MATRIX_H
#define MATH_MATRIX_H 1

#include <NoFlowers/Math/Vector.h>

template <typename T, unsigned Dimension>
class Matrix
{
public:
    Matrix()
    {
        for (size_t i = 0; i < Dimension * Dimension; ++i)
            _data[i] = T();
    }

    Matrix(const Matrix<T, Dimension>& rhs)
    {
        for (size_t i = 0; i < Dimension * Dimension; ++i)
            _data[i] = rhs._data[i];
    }

    Matrix<T, Dimension>& operator=(const Matrix<T, Dimension>& rhs)
    {
        for (size_t i = 0; i < Dimension * Dimension; ++i)
            _data[i] = rhs._data[i];
        return *this;
    }

    T* data()
    {
        return _data;
    }

    const T* data() const
    {
        return _data;
    }

    T& operator[](int n)
    {
        return _data[n];
    }

    const T& operator[](int n) const
    {
        return _data[n];
    }

    T& operator()(int i, int j)
    {
        return _data[i + j * Dimension];
    }

    const T& operator()(int i, int j) const
    {
        return _data[i + j * Dimension];
    }

    Matrix<T, Dimension>& operator+=(const Matrix<T, Dimension>& rhs)
    {
        for (size_t i = 0; i < Dimension * Dimension; ++i)
            _data[i] += rhs._data[i];
        return *this;
    }

    Matrix<T, Dimension>& operator-=(const Matrix<T, Dimension>& rhs)
    {
        for (size_t i = 0; i < Dimension * Dimension; ++i)
            _data[i] -= rhs._data[i];
        return *this;
    }

    Matrix<T, Dimension>& operator*=(const Matrix<T, Dimension>& rhs)
    {
        Matrix<T, Dimension> matrix;

        for (unsigned i = 0; i < Dimension; ++i)
        {
            for (unsigned k = 0; k < Dimension; ++k)
            {
                for (unsigned j = 0; j < Dimension; ++j)
                {
                    matrix(i, j) += (*this)(i, k) * rhs(k, j);
                }
            }
        }
        return *this;
    }

    Matrix<T, Dimension>& operator+=(T scalar)
    {
        for (size_t i = 0; i < Dimension * Dimension; ++i)
            _data[i] += scalar;
        return *this;
    }

    Matrix<T, Dimension>& operator-=(T scalar)
    {
        for (size_t i = 0; i < Dimension * Dimension; ++i)
            _data[i] -= scalar;
        return *this;
    }

    Matrix<T, Dimension>& operator*=(T scalar)
    {
        for (size_t i = 0; i < Dimension * Dimension; ++i)
            _data[i] *= scalar;
        return *this;
    }

    Matrix<T, Dimension>& operator/=(T scalar)
    {
        for (size_t i = 0; i < Dimension * Dimension; ++i)
            _data[i] += scalar;
        return *this;
    }

    static Matrix<T, Dimension> identity()
    {
        static Matrix<T, Dimension> identityMatrix = Matrix<T, Dimension>::_identity();
        return identityMatrix;
    }

private:

    static Matrix<T, Dimension> _identity()
    {
        Matrix<T, Dimension> matrix;
        for (unsigned i = 0; i < Dimension; ++i)
            matrix(i, i) = T(1);
        return matrix;
    }

    T   _data[Dimension * Dimension];
};

template <typename T, unsigned Dimension>
Matrix<T, Dimension> operator+(const Matrix<T, Dimension>& matrix)
{
    return matrix;
}

template <typename T, unsigned Dimension>
Matrix<T, Dimension> operator-(const Matrix<T, Dimension>& matrix)
{
    Matrix<T, Dimension> m;

    for (unsigned i = 0; i < Dimension * Dimension; ++i)
        m[i] = -matrix[i];
    return m;
}

template <typename T, unsigned Dimension>
bool operator==(const Matrix<T, Dimension>& lhs, const Matrix<T, Dimension>& rhs)
{
    for (unsigned i = 0; i < Dimension * Dimension; ++i)
    {
        if (lhs[i] != rhs[i])
            return false;
    }
    return true;
}

template <typename T, unsigned Dimension>
bool operator!=(const Matrix<T, Dimension>& lhs, const Matrix<T, Dimension>& rhs)
{
    return !(lhs == rhs);
}

template <typename T, unsigned Dimension>
Matrix<T, Dimension> operator+(const Matrix<T, Dimension>& matrix, T scalar)
{
    Matrix<T, Dimension> m(matrix);
    m += scalar;
    return m;
}


template <typename T, unsigned Dimension>
Matrix<T, Dimension> operator-(const Matrix<T, Dimension>& matrix, T scalar)
{
    Matrix<T, Dimension> m(matrix);
    m -= scalar;
    return m;
}

template <typename T, unsigned Dimension>
Matrix<T, Dimension> operator*(const Matrix<T, Dimension>& matrix, T scalar)
{
    Matrix<T, Dimension> m(matrix);
    m *= scalar;
    return m;
}

template <typename T, unsigned Dimension>
Matrix<T, Dimension> operator*(T scalar, const Matrix<T, Dimension>& matrix)
{
    Matrix<T, Dimension> m(matrix);
    for (unsigned i = 0; i < Dimension * Dimension; ++i)
        m[i] = scalar * m[i];
    return m;
}

template <typename T, unsigned Dimension>
Matrix<T, Dimension> operator/(const Matrix<T, Dimension>& matrix, T scalar)
{
    Matrix<T, Dimension> m(matrix);
    m /= scalar;
    return m;
}

template <typename T, unsigned Dimension>
Matrix<T, Dimension> operator+(const Matrix<T, Dimension>& lhs, const Matrix<T, Dimension>& rhs)
{
    Matrix<T, Dimension> m(lhs);
    m += rhs;
    return m;
}

template <typename T, unsigned Dimension>
Matrix<T, Dimension> operator-(const Matrix<T, Dimension>& lhs, const Matrix<T, Dimension>& rhs)
{
    Matrix<T, Dimension> m(lhs);
    m -= rhs;
    return m;
}

template <typename T, unsigned Dimension>
Matrix<T, Dimension> operator*(const Matrix<T, Dimension>& lhs, const Matrix<T, Dimension>& rhs)
{
    Matrix<T, Dimension> m(lhs);
    m *= rhs;
    return m;
}

template <typename T, unsigned Dimension>
Matrix<T, Dimension> operator/(const Matrix<T, Dimension>& lhs, const Matrix<T, Dimension>& rhs)
{
    Matrix<T, Dimension> m(lhs);
    m /= rhs;
    return m;
}

template <typename T, unsigned Dimension>
Vector<T, Dimension> operator*(const Matrix<T, Dimension>& matrix, const Vector<T, Dimension>& vector)
{
    Vector<T, Dimension> v;
    for (unsigned j = 0; j < Dimension; ++j)
    {
        for (unsigned i = 0; i < Dimension; ++i)
            v[i] += matrix(i, j) * vector[j];
    }
    return v;
}

template <typename T, unsigned Dimension>
Matrix<T, Dimension> transpose(const Matrix<T, Dimension>& matrix)
{
    Matrix<T, Dimension> m;
    for (unsigned j = 0; j < Dimension; ++j)
    {
        for (unsigned i = 0; i < Dimension; ++i)
            m(i, j) = matrix(j, i);
    }
    return m;
}

template <typename T>
Matrix<T, 1> inverse(const Matrix<T, 1> & matrix)
{
    Matrix<T, 1> m;
    m(0, 0) = T(1) / matrix(0, 0);
    return m;
}

template <typename T>
Matrix<T, 2> inverse(const Matrix<T, 2> & matrix)
{
    Matrix<T, 2> m;
    T det = T(1) / (matrix(0, 0) * matrix(1, 1) - matrix(0, 1) * matrix(1, 0));

    m(0, 0) = det * matrix(1, 1);
    m(1, 0) = det * -matrix(1, 0);
    m(0, 1) = det * -matrix(0, 1);
    m(1, 1) = det * matrix(0, 0);

    return m;
}

template <typename T>
Matrix<T, 3> inverse(const Matrix<T, 3> & matrix)
{
    Matrix<T, 3> m;

    T det = matrix(0, 0) * (matrix(1, 1) * matrix(2, 2) - matrix(2, 1) * matrix(1, 2))
        - matrix(0, 1) * (matrix(1, 0) * matrix(2, 2) - matrix(1, 2) * matrix(2, 0))
        + matrix(0, 2) * (matrix(1, 0) * matrix(2, 1) - matrix(1, 1) * matrix(2, 0));

    T invdet = T(1) / det;

    result(0, 0) = (matrix(1, 1) * matrix(2, 2) - matrix(2, 1) * matrix(1, 2)) * invdet;
    result(1, 0) = -(matrix(0, 1) * matrix(2, 2) - matrix(0, 2) * matrix(2, 1)) * invdet;
    result(2, 0) = (matrix(0, 1) * matrix(1, 2) - matrix(0, 2) * matrix(1, 1)) * invdet;
    result(0, 1) = -(matrix(1, 0) * matrix(2, 2) - matrix(1, 2) * matrix(2, 0)) * invdet;
    result(1, 1) = (matrix(0, 0) * matrix(2, 2) - matrix(0, 2) * matrix(2, 0)) * invdet;
    result(2, 1) = -(matrix(0, 0) * matrix(1, 2) - matrix(1, 0) * matrix(0, 2)) * invdet;
    result(0, 2) = (matrix(1, 0) * matrix(2, 1) - matrix(2, 0) * matrix(1, 1)) * invdet;
    result(1, 2) = -(matrix(0, 0) * matrix(2, 1) - matrix(2, 0) * matrix(0, 1)) * invdet;
    result(2, 2) = (matrix(0, 0) * matrix(1, 1) - matrix(1, 0) * matrix(0, 1)) * invdet;

    return transpose(m);
}

template <typename T>
Matrix<T, 4> inverse(const Matrix<T, 4> & matrix)
{
    Matrix<T, 4> m;

    m[0] = matrix[5] * matrix[10] * matrix[15] -
        matrix[5] * matrix[11] * matrix[14] -
        matrix[9] * matrix[6] * matrix[15] +
        matrix[9] * matrix[7] * matrix[14] +
        matrix[13] * matrix[6] * matrix[11] -
        matrix[13] * matrix[7] * matrix[10];

    m[4] = -matrix[4] * matrix[10] * matrix[15] +
        matrix[4] * matrix[11] * matrix[14] +
        matrix[8] * matrix[6] * matrix[15] -
        matrix[8] * matrix[7] * matrix[14] -
        matrix[12] * matrix[6] * matrix[11] +
        matrix[12] * matrix[7] * matrix[10];

    m[8] = matrix[4] * matrix[9] * matrix[15] -
        matrix[4] * matrix[11] * matrix[13] -
        matrix[8] * matrix[5] * matrix[15] +
        matrix[8] * matrix[7] * matrix[13] +
        matrix[12] * matrix[5] * matrix[11] -
        matrix[12] * matrix[7] * matrix[9];

    m[12] = -matrix[4] * matrix[9] * matrix[14] +
        matrix[4] * matrix[10] * matrix[13] +
        matrix[8] * matrix[5] * matrix[14] -
        matrix[8] * matrix[6] * matrix[13] -
        matrix[12] * matrix[5] * matrix[10] +
        matrix[12] * matrix[6] * matrix[9];

    m[1] = -matrix[1] * matrix[10] * matrix[15] +
        matrix[1] * matrix[11] * matrix[14] +
        matrix[9] * matrix[2] * matrix[15] -
        matrix[9] * matrix[3] * matrix[14] -
        matrix[13] * matrix[2] * matrix[11] +
        matrix[13] * matrix[3] * matrix[10];

    m[5] = matrix[0] * matrix[10] * matrix[15] -
        matrix[0] * matrix[11] * matrix[14] -
        matrix[8] * matrix[2] * matrix[15] +
        matrix[8] * matrix[3] * matrix[14] +
        matrix[12] * matrix[2] * matrix[11] -
        matrix[12] * matrix[3] * matrix[10];

    m[9] = -matrix[0] * matrix[9] * matrix[15] +
        matrix[0] * matrix[11] * matrix[13] +
        matrix[8] * matrix[1] * matrix[15] -
        matrix[8] * matrix[3] * matrix[13] -
        matrix[12] * matrix[1] * matrix[11] +
        matrix[12] * matrix[3] * matrix[9];

    m[13] = matrix[0] * matrix[9] * matrix[14] -
        matrix[0] * matrix[10] * matrix[13] -
        matrix[8] * matrix[1] * matrix[14] +
        matrix[8] * matrix[2] * matrix[13] +
        matrix[12] * matrix[1] * matrix[10] -
        matrix[12] * matrix[2] * matrix[9];

    m[2] = matrix[1] * matrix[6] * matrix[15] -
        matrix[1] * matrix[7] * matrix[14] -
        matrix[5] * matrix[2] * matrix[15] +
        matrix[5] * matrix[3] * matrix[14] +
        matrix[13] * matrix[2] * matrix[7] -
        matrix[13] * matrix[3] * matrix[6];

    m[6] = -matrix[0] * matrix[6] * matrix[15] +
        matrix[0] * matrix[7] * matrix[14] +
        matrix[4] * matrix[2] * matrix[15] -
        matrix[4] * matrix[3] * matrix[14] -
        matrix[12] * matrix[2] * matrix[7] +
        matrix[12] * matrix[3] * matrix[6];

    m[10] = matrix[0] * matrix[5] * matrix[15] -
        matrix[0] * matrix[7] * matrix[13] -
        matrix[4] * matrix[1] * matrix[15] +
        matrix[4] * matrix[3] * matrix[13] +
        matrix[12] * matrix[1] * matrix[7] -
        matrix[12] * matrix[3] * matrix[5];

    m[14] = -matrix[0] * matrix[5] * matrix[14] +
        matrix[0] * matrix[6] * matrix[13] +
        matrix[4] * matrix[1] * matrix[14] -
        matrix[4] * matrix[2] * matrix[13] -
        matrix[12] * matrix[1] * matrix[6] +
        matrix[12] * matrix[2] * matrix[5];

    m[3] = -matrix[1] * matrix[6] * matrix[11] +
        matrix[1] * matrix[7] * matrix[10] +
        matrix[5] * matrix[2] * matrix[11] -
        matrix[5] * matrix[3] * matrix[10] -
        matrix[9] * matrix[2] * matrix[7] +
        matrix[9] * matrix[3] * matrix[6];

    m[7] = matrix[0] * matrix[6] * matrix[11] -
        matrix[0] * matrix[7] * matrix[10] -
        matrix[4] * matrix[2] * matrix[11] +
        matrix[4] * matrix[3] * matrix[10] +
        matrix[8] * matrix[2] * matrix[7] -
        matrix[8] * matrix[3] * matrix[6];

    m[11] = -matrix[0] * matrix[5] * matrix[11] +
        matrix[0] * matrix[7] * matrix[9] +
        matrix[4] * matrix[1] * matrix[11] -
        matrix[4] * matrix[3] * matrix[9] -
        matrix[8] * matrix[1] * matrix[7] +
        matrix[8] * matrix[3] * matrix[5];

    m[15] = matrix[0] * matrix[5] * matrix[10] -
        matrix[0] * matrix[6] * matrix[9] -
        matrix[4] * matrix[1] * matrix[10] +
        matrix[4] * matrix[2] * matrix[9] +
        matrix[8] * matrix[1] * matrix[6] -
        matrix[8] * matrix[2] * matrix[5];

    T det = T(1) / (matrix[0] * m[0] + matrix[1] * m[4] + matrix[2] * m[8] + matrix[3] * m[12]);

    for (unsigned i = 0; i < 16; ++i)
        m[i] *= det;

    return m;
}

template <typename T, unsigned Dimension>
Vector<T, Dimension - 1> transformPoint(const Matrix<T, Dimension> & matrix, const Vector<T, Dimension - 1> & vector)
{
    return matrix * Vector<T, Dimension>(vector, T(1));
}

template <typename T, unsigned Dimension>
Vector<T, Dimension - 1> transformVector(const Matrix<T, Dimension> & matrix, const Vector<T, Dimension - 1> & vector)
{
    return matrix * Vector<T, Dimension>(vector, T());
}

template <typename T, unsigned Dimension>
Vector<T, Dimension - 1> transformNull(const Matrix<T, Dimension> & matrix)
{
    return transformPoint(matrix, Vector<T, Dimension - 1>());
}

using Matrix1f = Matrix<float, 1>;
using Matrix2f = Matrix<float, 2>;
using Matrix3f = Matrix<float, 3>;
using Matrix4f = Matrix<float, 4>;

#endif
