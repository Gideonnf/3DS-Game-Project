#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H

#include "../Component.h"
#include <citro3d.h>

struct Transform : public Component<Transform>
{
    C3D_FVec position; // 3D position of the entity
    C3D_FVec rotation; // 3D rotation of the entity (in degrees
    C3D_FVec scale;    // 3D scale of the entity
};

#endif