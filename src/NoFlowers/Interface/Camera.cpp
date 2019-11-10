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

Matrix4f Camera::projectionMatrix() const
{
    return _projection;
}

Matrix4f Camera::viewMatrix() const
{
    Matrix4f mat;

    mat = Matrix4f::identity();
    rotate(mat, Vector3f(1, 0, 0), 90.f);
    rotate(mat, Vector3f(0, 1, 0), _hRotation);
    translate(mat, -_position);

    return mat;
}

void Camera::setPerspective(float fov, float aspect, float znear, float zfar)
{
    _projection = perspectiveProjection(fov, aspect, znear, zfar);
}

void Camera::move(Vector3f delta)
{
    _position += delta;
}
