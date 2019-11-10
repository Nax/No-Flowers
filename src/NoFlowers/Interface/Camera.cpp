#include <NoFlowers/Math/MatrixUtils.h>
#include <NoFlowers/Interface/Camera.h>

Camera::Camera()
: _projection(Matrix4f::identity())
, _position(Vector3f(0.f, 0.f, 0.f))
, _hRotation(0.f)
, _vRotation(0.f)
{

}

Matrix4f Camera::projectionMatrix() const
{
    return _projection;
}

Matrix4f Camera::viewMatrix() const
{
    return Matrix4f::identity();
}

void Camera::setPerspective(float fov, float aspect, float znear, float zfar)
{
    _projection - perspectiveProjection(fov, aspect, znear, zfar);
}
