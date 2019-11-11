#include <cmath>
#include <NoFlowers/Math/MatrixUtils.h>
#include <NoFlowers/Interface/Camera.h>

Camera::Camera()
: _projection(Matrix4f::identity())
, _position(Vector3f(0.f, 0.f, 0.f))
, _hRotation(0.f)
, _vRotation(0.f)
{
    _position = { 0, -30, 0 };
}

Vector3f Camera::forward() const
{
    float alpha;
    float beta;
    Vector3f v;

    alpha = (_hRotation) * (Math::pi / 180.f);
    beta = (-_vRotation) * (Math::pi / 180.f);

    v.x = sinf(alpha) * cosf(beta);
    v.y = cosf(alpha) * cosf(beta);
    v.z = sinf(beta);

    return v;
}

Vector3f Camera::side() const
{
    Vector3f up;
    Vector3f fwd;

    fwd = forward();
    up = Vector3f(0.f, 0.f, 1.f);
    return normal(cross(fwd, up));
}

Matrix4f Camera::projectionMatrix() const
{
    return _projection;
}

Matrix4f Camera::viewMatrix() const
{
    Matrix4f mat;

    mat = Matrix4f::identity();
    ::rotate(mat, Vector3f(1, 0, 0), 90.f - _vRotation);
    ::rotate(mat, Vector3f(0, 0, 1), -_hRotation);
    translate(mat, -_position);

    return mat;
}

void Camera::setPerspective(float fov, float aspect, float znear, float zfar)
{
    _projection = perspectiveProjection(fov, aspect, znear, zfar);
}

void Camera::setOrtho(float left, float right, float bottom, float top, float znear, float zfar)
{
    _projection = orthogonalProjection(left, right, bottom, top, znear, zfar);
}

void Camera::move(Vector3f delta)
{
    _position += delta;
}

void Camera::rotate(Vector2f delta)
{
    _hRotation += delta.x;
    _vRotation += delta.y;

    if (_vRotation > 89.9f)
        _vRotation = 89.9f;
    if (_vRotation < -89.9f)
        _vRotation = -89.9f;
}
