#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <QtGui>
class Transform
{
private:
    QVector3D position;    //  Location
    QQuaternion rotation; //  Rotation (angle and axis)
    QVector3D scale;
    Transform *parent;

public:
    Transform();
    QVector3D GetPosition();
    QVector3D GetWorldPosition();
    void SetPosition(QVector3D pos);
    QQuaternion GetRotation();
    QQuaternion GetWorldRotation();
    void SetRotation(QQuaternion rot);
    void SetRotation(QVector3D direction);
    QVector3D GetScale();
    void SetScale(QVector3D sc);
    void Translate(QVector3D offset);
    void Rotate(float angle, QVector3D axis);
    QMatrix4x4 GetModelMatrix();
    QMatrix3x3 GetNormalMatrix();
    void SetParent(Transform *p);
};

#endif // TRANSFORM_H
