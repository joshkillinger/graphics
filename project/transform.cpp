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

QQuaternion Transform::GetWorldRotation()
{
    if (parent != NULL)
    {
        return parent->GetWorldRotation() * rotation;
    }
    else
    {
        return rotation;
    }
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

QQuaternion rotationTo(const QVector3D &from, const QVector3D &to)
{
    // Based on Stan Melax's article in Game Programming Gems

    const QVector3D v0(from.normalized());
    const QVector3D v1(to.normalized());

    float d = QVector3D::dotProduct(v0, v1) + 1.0f;

    // if dest vector is close to the inverse of source vector, ANY axis of rotation is valid
    if (qFuzzyIsNull(d)) {
        QVector3D axis = QVector3D::crossProduct(QVector3D(1.0f, 0.0f, 0.0f), v0);
        if (qFuzzyIsNull(axis.lengthSquared()))
            axis = QVector3D::crossProduct(QVector3D(0.0f, 1.0f, 0.0f), v0);
        axis.normalize();

        // same as QQuaternion::fromAxisAndAngle(axis, 180.0f)
        return QQuaternion(0.0f, axis.x(), axis.y(), axis.z());
    }

    d = std::sqrt(2.0f * d);
    const QVector3D axis(QVector3D::crossProduct(v0, v1) / d);

    return QQuaternion(d * 0.5f, axis).normalized();
}

QQuaternion fromRotationMatrix(const QMatrix3x3 &rot3x3)
{
    // Algorithm from:
    // http://www.j3d.org/matrix_faq/matrfaq_latest.html#Q55

    float scalar;
    float axis[3];

    const float trace = rot3x3(0, 0) + rot3x3(1, 1) + rot3x3(2, 2);
    if (trace > 0.00000001f) {
        const float s = 2.0f * std::sqrt(trace + 1.0f);
        scalar = 0.25f * s;
        axis[0] = (rot3x3(2, 1) - rot3x3(1, 2)) / s;
        axis[1] = (rot3x3(0, 2) - rot3x3(2, 0)) / s;
        axis[2] = (rot3x3(1, 0) - rot3x3(0, 1)) / s;
    } else {
        static int s_next[3] = { 1, 2, 0 };
        int i = 0;
        if (rot3x3(1, 1) > rot3x3(0, 0))
            i = 1;
        if (rot3x3(2, 2) > rot3x3(i, i))
            i = 2;
        int j = s_next[i];
        int k = s_next[j];

        const float s = 2.0f * std::sqrt(rot3x3(i, i) - rot3x3(j, j) - rot3x3(k, k) + 1.0f);
        axis[i] = 0.25f * s;
        scalar = (rot3x3(k, j) - rot3x3(j, k)) / s;
        axis[j] = (rot3x3(j, i) + rot3x3(i, j)) / s;
        axis[k] = (rot3x3(k, i) + rot3x3(i, k)) / s;
    }

    return QQuaternion(scalar, axis[0], axis[1], axis[2]);
}

QQuaternion fromAxes(const QVector3D &xAxis, const QVector3D &yAxis, const QVector3D &zAxis)
{
    QMatrix3x3 rot3x3(Qt::Uninitialized);
    rot3x3(0, 0) = xAxis.x();
    rot3x3(1, 0) = xAxis.y();
    rot3x3(2, 0) = xAxis.z();
    rot3x3(0, 1) = yAxis.x();
    rot3x3(1, 1) = yAxis.y();
    rot3x3(2, 1) = yAxis.z();
    rot3x3(0, 2) = zAxis.x();
    rot3x3(1, 2) = zAxis.y();
    rot3x3(2, 2) = zAxis.z();

    return fromRotationMatrix(rot3x3);
}

void Transform::SetRotation(QVector3D direction)
{
    QVector3D up(0,1,0);

    if (qFuzzyIsNull(direction.x()) && qFuzzyIsNull(direction.y()) && qFuzzyIsNull(direction.z()))
    {
        rotation = QQuaternion();
        return;
    }

    const QVector3D zAxis(direction.normalized());
    QVector3D xAxis(QVector3D::crossProduct(up, zAxis));
    if (qFuzzyIsNull(xAxis.lengthSquared()))
    {
        // collinear or invalid up vector; derive shortest arc to new direction
        rotation = rotationTo(QVector3D(0.0f, 0.0f, 1.0f), zAxis);
        return;
    }

    xAxis.normalize();
    const QVector3D yAxis(QVector3D::crossProduct(zAxis, xAxis));

    rotation = fromAxes(xAxis, yAxis, zAxis);
}
