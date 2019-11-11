#ifndef INTERFACE_CAMERA_H
#define INTERFACE_CAMERA_H

#include <NoFlowers/Math/Matrix.h>

class Camera
{
public:
    Camera();

    Vector3f forward() const;
    Vector3f side() const;

    Matrix4f    projectionMatrix() const;
    Matrix4f    viewMatrix() const;

    void setPerspective(float fov, float aspect, float znear, float zfar);
    void setOrtho(float left, float right, float bottom, float top, float znear, float zfar);
    void move(Vector3f delta);
    void rotate(Vector2f delta);

private:
    Matrix4f    _projection;
    Vector3f    _position;
    float       _hRotation;
    float       _vRotation;
};

#endif
