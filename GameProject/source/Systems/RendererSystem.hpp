#ifndef RENDERER_SYSTEM_H
#define RENDERER_SYSTEM_H

#include "../ECS/BaseSystem.h"
#include <3ds.h>
#include <citro3d.h>

class RendererSystem : public BaseSystem
{
    private:
        s8 uLoc_projection;
        s8 uLoc_modelView;
        s8 uLoc_material;
    public:
        void Init(shaderProgram_s* shdrPrgm);
        void Draw(const C3D_Mtx& projection, const C3D_Mtx& view);
};

#endif