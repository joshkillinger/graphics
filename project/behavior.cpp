#include "behavior.h"

Behavior::Behavior()
{
    parent = NULL;
}

void Behavior::SetParent(Object *parent)
{
    this->parent = parent;
}
