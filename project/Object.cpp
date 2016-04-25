//
//  Generic Object class
//
#include "Object.h"

//
//  Constructor
//
Object::Object(SciShieldOpengl *context)
{
    transform = Transform();
    glContext = context;
    material = NULL;
    hitbox = NULL;
    parent = NULL;
}

void Object::SetMaterial(Material *mat)
{
    material = mat;
}

void Object::SetHitbox(Hitbox *hb)
{
    hitbox = hb;
    hitbox->SetParent(&transform);
}

void Object::display(int stage)
{
    if (material == NULL)
    {
        return;
    }

    if (stage == material->visibleStage)
    {
        QMatrix4x4 modelview = glContext->view * transform.GetModelMatrix();
        QMatrix3x3 norm =  modelview.normalMatrix();

        material->PreRender(modelview, norm);
        Render();
        material->PostRender();
    }
}

void Object::Render()
{
    //   Attribute 0: vertex coordinate (vec3) at offset 0
    vertexBuffer.bind();
    material->shader.enableAttributeArray(0);
    material->shader.setAttributeBuffer(0,GL_FLOAT,0,3,sizeof(float)*8);

    material->shader.enableAttributeArray(1);
    material->shader.setAttributeBuffer(1,GL_FLOAT,3*sizeof(float),3,sizeof(float)*8);

    material->shader.enableAttributeArray(2);
    material->shader.setAttributeBuffer(2,GL_FLOAT,6*sizeof(float),2,sizeof(float)*8);

    // Draw the cube
    glDrawArrays(GL_TRIANGLES,0,vertexCount);

    //  Disable vertex arrays
    material->shader.disableAttributeArray(0);
    material->shader.disableAttributeArray(1);
    material->shader.disableAttributeArray(2);

    vertexBuffer.release();
}

void Object::Hit(QVector3D point)
{
    if (material != NULL)
    {
        material->Hit(point);
    }
}

void Object::Update()
{
    if (material != NULL)
    {
        material->Update();
    }
}

float Object::IsHit(QVector3D origin, QVector3D direction)
{
    if (hitbox != NULL)
    {
        return hitbox->IsHit(origin, direction);
    }
    else return -1;
}
