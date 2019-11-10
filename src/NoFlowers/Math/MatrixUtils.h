#ifndef MATH_MATRIX_UTILS_H
#define MATH_MATRIX_UTILS_H 1

#include <cmath>
#include <NoFlowers/Math/Matrix.h>
#include <NoFlowers/Math/Constants.h>

inline Matrix4f frustumProjection(float left, float right, float bottom, float top, float znear, float zfar)
{
    Matrix4f m;
    m[0][0] = 2.f * znear / (right - left);
    m[1][1] = 2.f * znear / (top - bottom);
    m[0][2] = (right + left) / (right - left);
    m[1][2] = (top + bottom) / (top - bottom);
    m[2][2] = -(zfar + znear) / (zfar - znear);
    m[3][2] = -1.f;
    m[2][3] = -(2.f * zfar * znear) / (zfar - znear);
    return m;
}

inline Matrix4f perspectiveProjection(float fov, float aspect, float znear, float zfar)
{
    const float ymax = znear * tanf(fov * Math::pi / 360.f);
    const float xmax = ymax * aspect;
    return frustumProjection(-xmax, xmax, -ymax, ymax, znear, zfar);
}

inline Matrix4f orthogonalProjection(float left, float right, float bottom, float top, float znear, float zfar)
{
    Matrix4f m;

    m[0][0] = 2.f / (right - left);
    m[1][1] = 2.f / (top - bottom);
    m[2][2] = -2.f / (zfar - znear);
    m[0][3] = -(right + left) / (right - left);
    m[1][3] = -(top + bottom) / (top - bottom);
    m[2][3] = -(zfar + znear) / (zfar - znear);
    m[3][3] = 1.f;
    return m;
}

inline void translate(Matrix4f& matrix, float x, float y, float z)
{
    Matrix4f m = Matrix4f::identity();

    m[0][3] = x;
    m[1][3] = y;
    m[2][3] = z;

    matrix *= m;
}

inline void translate(Matrix4f& matrix, Vector3f v)
{
    translate(matrix, v.x, v.y, v.z);
}

inline void translate(Matrix3f& matrix, float x, float y)
{
    Matrix3f m = Matrix3f::identity();

    m[2][0] = x;
    m[2][1] = y;

    matrix *= m;
}

inline void translate(Matrix3f& matrix, Vector2f v)
{
    translate(matrix, v.x, v.y);
}


inline void rotate(Matrix4f& matrix, Vector3f origin, float angle)
{
    angle = angle * (Math::pi / 180.f);
    const float c = cosf(angle);
    const float s = sinf(angle);

    Vector3f axis(origin);
    Vector3f temp(axis * (1.f - c));

    Matrix4f rot;

    rot[0][0] = c + temp[0] * axis[0];
    rot[0][1] = 0 + temp[0] * axis[1] + s * axis[2];
    rot[0][2] = 0 + temp[0] * axis[2] - s * axis[1];

    rot[1][0] = 0 + temp[1] * axis[0] - s * axis[2];
    rot[1][1] = c + temp[1] * axis[1];
    rot[1][2] = 0 + temp[1] * axis[2] + s * axis[0];

    rot[2][0] = 0 + temp[2] * axis[0] + s * axis[1];
    rot[2][1] = 0 + temp[2] * axis[1] - s * axis[0];
    rot[2][2] = c + temp[2] * axis[2];

    rot[3][3] = 1.f;

    matrix *= rot;
}

inline void rotate(Matrix4f& matrix, float x, float y, float z, float angle)
{
    rotate(matrix, Vector3f(x, y, z), angle);
}

inline void rotate(Matrix3f& matrix, float angle)
{
    const float rad = angle * (Math::pi / 180.f);
    const float s = sinf(rad);
    const float c = cosf(rad);

    Matrix3f m = Matrix3f::identity();

    m[0][0] = c;
    m[1][0] = -s;
    m[0][1] = s;
    m[1][1] = c;
    matrix *= m;
}

inline void scale(Matrix4f& matrix, Vector3f scale)
{
    Matrix4f m;
    m[0][0] = scale.x;
    m[1][1] = scale.y;
    m[2][2] = scale.z;
    m[3][0] = 1.f;
    matrix *= m;
}

#endif
