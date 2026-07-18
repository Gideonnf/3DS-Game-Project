#ifndef RENDERER_H
#define RENDERER_H

#include "../Component.h"
#include <3ds.h>
#include <citro3d.h>

struct Renderer : public Component<Renderer>
{
    // NOTE: Maybe make a model struct to reuse
    // Pointer to vertex array
    // NOTE: Must use linearAlloc() cause 3ds
    void* vboData;
    size_t vertexCount; // num of vertices
    size_t vertexStride; // size of a single vertex struct
    GPU_Primitive_t primitiveType; // type of primitive to draw (i.e triangles, etc)

    C3D_AttrInfo attrInfo; // attribute info for the vertex data
    C3D_Tex* texture; // pointer to the texture to use for rendering
    C3D_Mtx matMatrix; // pCan contain color components
    // 0: Ambient, 1: Diffuse, 2: Specular, 3: Emission

    struct TevConfig {
        u8 texEnvIndex;
        GPU_COMBINEFUNC colorFunc;
        GPU_COMBINEFUNC alphaFunc;
    } tev;
};

#endif