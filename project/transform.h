#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <QMatrix3x3>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector3D>

class Transform
{
private:
    QVector3D position;    //  Location
    QQuaternion rotation; //  Rotation (angle and axis)
    QVector3D scale;

public:
    Transform();
    QVector3D GetPosition();
    void SetPosition(QVector3D pos);
    QQuaternion GetRotation();
    void SetRotation(QQuaternion rot);
    QVector3D GetScale();
    void SetScale(QVector3D sc);
    void Translate(QVector3D offset);
    void Rotate(float angle, QVector3D axis);
    QMatrix4x4 GetModelMatrix();
    QMatrix3x3 GetNormalMatrix();
};

#endif // TRANSFORM_H
