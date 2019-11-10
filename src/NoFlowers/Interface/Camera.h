#ifndef INTERFACE_CAMERA_H
#define INTERFACE_CAMERA_H

#include <NoFlowers/Math/Matrix.h>

class Camera
{
public:
    Camera();

    Matrix4f    projectionMatrix() const;
    Matrix4f    viewMatrix() const;

    void setPerspective(float fov, float aspect, float znear, float zfar);
    void move(Vector3f delta);

private:
    Matrix4f    _projection;
    Vector3f    _position;
    float       _hRotation;
    float       _vRotation;
};

#endif
