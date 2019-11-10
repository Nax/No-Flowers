#ifndef MATH_VECTOR_H
#define MATH_VECTOR_H 1

#include <cmath>

template <typename T, unsigned Dimension>
class Vector;

namespace priv
{
    /* Helper */
    template <int L, int R>
    struct VectorHelperMin
    {
        enum { value = (L > R) ? R : L };
    };

    /* Vector data */
    template <typename T, unsigned Dimension>
    struct VectorData
    {
        VectorData() {}
        T   _data[Dimension];
    };

    template <typename T>
    struct VectorData<T, 1>
    {
        VectorData() {}
        union
        {
            T   _data[1];
            T   x;
        };
    };

    template <typename T>
    struct VectorData<T, 2>
    {
        VectorData() {}
        union
        {
            T   _data[2];
            struct
            {
                T   x;
                T   y;
            };
        };
    };

    template <typename T>
    struct VectorData<T, 3>
    {
        VectorData() {}
        union
        {
            T               _data[3];
            struct
            {
                T           x;
                T           y;
                T           z;
            };
            Vector<T, 2>    xy;
        };
    };

    template <typename T>
    struct VectorData<T, 4>
    {
        VectorData() {}
        union
        {
            T               _data[4];
            struct
            {
                T           x;
                T           y;
                T           z;
                T           w;
            };
            Vector<T, 2>    xy;
            Vector<T, 3>    xyz;
        };
    };

    /* Vector builder */
    template <typename T, int Size>
    struct VectorBuilder
    {
        template <typename... Args>
        static void construct(T* data, T value, Args... args)
        {
            *data = value;
            VectorBuilder<T, Size - 1>::construct(data + 1, args...);
        }

        template <typename OtherT, unsigned OtherDimension, typename... Args>
        static void construct(T* data, const Vector<OtherT, OtherDimension>& other, Args... args)
        {
            for (unsigned i = 0; i < VectorHelperMin<OtherDimension, Size>::value; ++i)
            {
                data[i] = T(other[i]);
            }
            VectorBuilder<T, Size - VectorHelperMin<OtherDimension, Size>::value>::construct(data + OtherDimension, args...);
        }

        static void construct(T* data)
        {
            *data = T();
            VectorBuilder<T, Size - 1>::construct(data + 1, args...);
        }
    };

    template <typename T>
    struct VectorBuilder<T, 0>
    {
        static void construct(T* data)
        {

        }
    };
}

template <typename T, unsigned Dimension>
class Vector : public priv::VectorData<T, Dimension>
{
public:

    Vector()
    {
        for (unsigned i = 0; i < Dimension; ++i)
            _data[i] = T();
    }

    template <typename Head, typename... Args>
    Vector(Head head, Args... args)
    {
        priv::VectorBuilder<T, Dimension>::construct(_data, head, args...);
    }

    Vector<T, Dimension>& operator=(const Vector<T, Dimension>& rhs)
    {
        for (unsigned i = 0; i < Dimension; ++i)
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

    Vector<T, Dimension>& operator+=(T scalar)
    {
        for (unsigned i = 0; i < Dimension; ++i)
            _data[i] += scalar;
        return *this;
    }

    Vector<T, Dimension>& operator-=(T scalar)
    {
        for (unsigned i = 0; i < Dimension; ++i)
            _data[i] -= scalar;
        return *this;
    }

    Vector<T, Dimension>& operator*=(T scalar)
    {
        for (unsigned i = 0; i < Dimension; ++i)
            _data[i] *= scalar;
        return *this;
    }

    Vector<T, Dimension>& operator/=(T scalar)
    {
        for (unsigned i = 0; i < Dimension; ++i)
            _data[i] /= scalar;
        return *this;
    }

    Vector<T, Dimension>& operator+=(const Vector<T, Dimension>& vector)
    {
        for (unsigned i = 0; i < Dimension; ++i)
            _data[i] += vector._data[i];
        return *this;
    }

    Vector<T, Dimension>& operator-=(const Vector<T, Dimension>& vector)
    {
        for (unsigned i = 0; i < Dimension; ++i)
            _data[i] -= vector._data[i];
        return *this;
    }

    Vector<T, Dimension>& operator*=(const Vector<T, Dimension>& vector)
    {
        for (unsigned i = 0; i < Dimension; ++i)
            _data[i] *= vector._data[i];
        return *this;
    }

    Vector<T, Dimension>& operator/=(const Vector<T, Dimension>& vector)
    {
        for (unsigned i = 0; i < Dimension; ++i)
            _data[i] /= vector._data[i];
        return *this;
    }
};

template <typename T, unsigned Dimension>
bool operator==(const Vector<T, Dimension>& lhs, const Vector<T, Dimension>& rhs)
{
    for (unsigned i = 0; i < Dimension; ++i)
    {
        if (lhs[i] != rhs[i])
            return false;
    }
    return true;
}

template <typename T, unsigned Dimension>
bool operator!=(const Vector<T, Dimension>& lhs, const Vector<T, Dimension>& rhs)
{
    return !(lsh == rhs);
}

template <typename T, unsigned Dimension>
Vector<T, Dimension> operator+(const Vector<T, Dimension>& vector)
{
    return vector;
}

template <typename T, unsigned Dimension>
Vector<T, Dimension> operator-(const Vector<T, Dimension>& vector)
{
    Vector<T, Dimension> v;
    for (unsigned i = 0; i < Dimension; ++i)
        v[i] = -(vector[i]);
    return v;
}

template <typename T, unsigned Dimension>
Vector<T, Dimension> operator+(const Vector<T, Dimension>& vector, T scalar)
{
    Vector<T, Dimension> v(vector);
    v += scalar;
    return v;
}

template <typename T, unsigned Dimension>
Vector<T, Dimension> operator-(const Vector<T, Dimension>& vector, T scalar)
{
    Vector<T, Dimension> v(vector);
    v -= scalar;
    return v;
}

template <typename T, unsigned Dimension>
Vector<T, Dimension> operator*(const Vector<T, Dimension>& vector, T scalar)
{
    Vector<T, Dimension> v(vector);
    v *= scalar;
    return v;
}

template <typename T, unsigned Dimension>
Vector<T, Dimension> operator*(T scalar, const Vector<T, Dimension>& vector)
{
    Vector<T, Dimension> v;
    for (unsigned i = 0; i < Dimension; ++i)
        v[i] = scalar * vector[i];
    return v;
}

template <typename T, unsigned Dimension>
Vector<T, Dimension> operator/(const Vector<T, Dimension>& vector, T scalar)
{
    Vector<T, Dimension> v(vector);
    v /= scalar;
    return v;
}

template <typename T, unsigned Dimension>
Vector<T, Dimension> operator+(const Vector<T, Dimension>& lhs, const Vector<T, Dimension>& rhs)
{
    Vector<T, Dimension> v(lhs);
    v += rhs;
    return v;
}

template <typename T, unsigned Dimension>
Vector<T, Dimension> operator-(const Vector<T, Dimension>& lhs, const Vector<T, Dimension>& rhs)
{
    Vector<T, Dimension> v(lhs);
    v -= rhs;
    return v;
}

template <typename T, unsigned Dimension>
Vector<T, Dimension> operator*(const Vector<T, Dimension>& lhs, const Vector<T, Dimension>& rhs)
{
    Vector<T, Dimension> v(lhs);
    v *= rhs;
    return v;
}

template <typename T, unsigned Dimension>
Vector<T, Dimension> operator/(const Vector<T, Dimension>& lhs, const Vector<T, Dimension>& rhs)
{
    Vector<T, Dimension> v(lhs);
    v /= rhs;
    return v;
}

template <typename T, unsigned Dimension>
bool null(const Vector<T, Dimension>& vector)
{
    return (vector == Vector<T, Dimension>);
}

template <typename T, unsigned Dimension>
T length2(const Vector<T, Dimension>& vector)
{
    T acc = T();
    for (unsigned i = 0; i < Dimension; ++i)
        acc += vector[i] * vector[i];
    return acc;
}

template <typename T, unsigned Dimension>
T length(const Vector<T, Dimension>& vector)
{
    return sqrt(length2(vector));
}

template <typename T, unsigned Dimension>
T distance2(const Vector<T, Dimension>& from, const Vector<T, Dimension>& to)
{
    return length2(to - from);
}

template <typename T, unsigned Dimension>
T distance(const Vector<T, Dimension>& from, const Vector<T, Dimension>& to)
{
    return sqrt(distance2(from, to));
}

template <typename T, unsigned Dimension>
Vector<T, Dimension> normal(const Vector<T, Dimension>& vector)
{
    return vector / length(vector);
}

template <typename T, unsigned Dimension>
T dot(const Vector<T, Dimension>& lhs, const Vector<T, Dimension>& rhs)
{
    Vector<T, Dimension> v(lhs * rhs);
    T acc = T();
    for (unsigned i = 0; i < Dimension; ++i)
        acc += v[i];
    return acc;
}

template <typename T>
T cross(const Vector<T, 2> & lhs, const Vector<T, 2> & rhs)
{
    return lhs.x * rhs.y - lhs.y * rhs.x;
}

template <typename T>
Vector<T, 2> cross(const Vector<T, 2> & vector)
{
    return Vector<2, T>(vector.y, -vector.x);
}

template <typename T>
Vector<T, 3> cross(const Vector<T, 3> & lhs, const Vector<T, 3> & rhs)
{
    return Vector<3, T>(
        lhs.y * rhs.z - lhs.z * rhs.y,
        lhs.z * rhs.x - lhs.x * rhs.z,
        lhs.x * rhs.y - lhs.y * rhs.x
    );
}

using Vector1f = Vector<float, 1>;
using Vector2f = Vector<float, 2>;
using Vector3f = Vector<float, 3>;
using Vector4f = Vector<float, 4>;

#endif
