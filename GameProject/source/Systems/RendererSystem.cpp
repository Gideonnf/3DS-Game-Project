#include "RendererSystem.hpp"

#include "../ECS/SystemManager.h"
#include "../ECS/ComponentManager.h"
#include "../ECS/Components/Renderer.hpp"
#include "../ECS/Components/Transform.hpp"

void RendererSystem::Init(shaderProgram_s* shdrPrgm)
{
    // set up signature for system for ECS
    mSignature.set(ComponentManager::GetInstance()->GetComponentID<Transform>());
    mSignature.set(ComponentManager::GetInstance()->GetComponentID<Renderer>());
    SystemManager::GetInstance()->SetSignature<RendererSystem>(mSignature);

    uLoc_projection = shaderInstanceGetUniformLocation(shdrPrgm->vertexShader, "projection");
    uLoc_modelView = shaderInstanceGetUniformLocation(shdrPrgm->vertexShader, "modelView");
    uLoc_material = shaderInstanceGetUniformLocation(shdrPrgm->vertexShader, "material");
}

void RendererSystem::Draw(const C3D_Mtx& projection, const C3D_Mtx& view)
{
    for (auto const& entity : mEntitiesSet)
    {
        auto& transform = ComponentManager::GetInstance()->GetComponent<Transform>(entity);
        auto& renderer = ComponentManager::GetInstance()->GetComponent<Renderer>(entity);

        // Set the shader uniforms
        C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, uLoc_projection, &projection);

        for (const Entity& entity : mEntitiesSet)
        {
            auto& transform = ComponentManager::GetInstance()->GetComponent<Transform>(entity);
            auto& renderer = ComponentManager::GetInstance()->GetComponent<Renderer>(entity);

            C3D_Mtx model;
            Mtx_Identity(&model);
            Mtx_Translate(&model, transform.position.x, transform.position.y, transform.position.z, true);
            Mtx_RotateX(&model, transform.rotation.x, true);
            Mtx_RotateY(&model, transform.rotation.y, true);
            Mtx_RotateZ(&model, transform.rotation.z, true);
            Mtx_Scale(&model, transform.scale.x, transform.scale.y, transform.scale.z);    
            
            C3D_AttrInfo* attrInfo = C3D_GetAttrInfo();
            *attrInfo = renderer.attrInfo;

            C3D_BufInfo* bufInfo = C3D_GetBufInfo();
            BufInfo_Init(bufInfo);
            BufInfo_Add(bufInfo, renderer.vboData, renderer.vertexStride, 3, 0x210);
            if (renderer.texture)
            {
                C3D_TexBind(0, renderer.texture);
            }  

            C3D_DrawArrays(renderer.primitiveType, 0, renderer.vertexCount);
        }

    }
}
