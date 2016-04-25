#include "transform.h"

Transform::Transform()
{
    position = QVector3D(0,0,0);
    rotation = QQuaternion();
    scale = QVector3D(1,1,1);
    parent = NULL;
}

QVector3D Transform::GetPosition()
{
    return position;
}

QVector3D Transform::GetWorldPosition()
{
    if (parent != NULL)
    {
        return parent->GetModelMatrix() * position;
    }
    else
    {
        return position;
    }
}

void Transform::SetPosition(QVector3D pos)
{
    position = pos;
}

QQuaternion Transform::GetRotation()
{
    return rotation;
}

void Transform::SetRotation(QQuaternion rot)
{
    rotation = rot;
}

QVector3D Transform::GetScale()
{
    return scale;
}

void Transform::SetScale(QVector3D sc)
{
    scale = sc;
}

void Transform::Translate(QVector3D offset)
{
    position += offset;
}

void Transform::Rotate(float angle, QVector3D axis)
{
    rotation *= QQuaternion::fromAxisAndAngle(axis, angle);
}

QMatrix4x4 Transform::GetModelMatrix()
{
    QMatrix4x4 modelMatrix;
    modelMatrix.translate(position);
    modelMatrix.rotate(rotation);
    modelMatrix.scale(scale);

    if (parent != NULL)
    {
        return parent->GetModelMatrix() * modelMatrix;
    }
    else
    {
        return modelMatrix;
    }
}

void Transform::SetParent(Transform *p)
{
        parent = p;
}
