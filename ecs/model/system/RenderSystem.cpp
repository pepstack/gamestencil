/**
* RenderSystem.cpp
*
* Init Created: 2016-06-16
* Last Updated: 2016-06-16
*/
#include "RenderSystem.h"

#include "../GameStencil.h"

using namespace ecs;


void RenderSystem::updateEntity(float dt, IEntity * entity)
{
    printf("----%s\n", entity->getEntityName(0));

    // Only query supported interfaces:
    //
    int num;
    IComponent ** components;
    
    // IRenderable
    num = entity->getComponentInterfaceMap(IRenderable::IID, &components);
    while(num-- > 0) {
        IRenderable* pIFace;
        if (lres_success == components[num]->queryNotAddRef(IRenderable::IID, (void**) &pIFace)) {
            pIFace->render(dt);
        }
    }

    // IMoveable
    num = entity->getComponentInterfaceMap(IMoveable::IID, &components);
    while(num-- > 0) {
        IMoveable* pIFace;
        if (lres_success == components[num]->queryNotAddRef(IMoveable::IID, (void**) &pIFace)) {
            pIFace->setPosition(0, 1);
        }
    }

    // IHealth
    num = entity->getComponentInterfaceMap(IHealth::IID, &components);
    while(num-- > 0) {
        IHealth* pIFace;
        if (lres_success == components[num]->queryNotAddRef(IHealth::IID, (void**) &pIFace)) {
            pIFace->setHealth(100);
        }
    }
}


void RenderSystem::update(float dt)
{
    printf("RenderSystem::priority(%d)\n", getPriority());

    IEntityMgmt * pIEntityMgmt;

    if (lres_success == GameStencil::getInstance(thread_ctx, IEntityMgmt::IID, (void**)&pIEntityMgmt)) {
        IEntity* pIEntity;

        if (lres_success == pIEntityMgmt->firstEntity(&pIEntity)) {
            // call updateEntity
            updateEntity(dt, pIEntity);

            IEntity* pIEntityNext;
            while (lres_success == pIEntity->nextEntity(pIEntityMgmt, &pIEntityNext)) {
                // call updateEntity
                updateEntity(dt, pIEntityNext);

                pIEntity->release();
                pIEntity = pIEntityNext;
            }
            
            pIEntity->release();
        }
        
        pIEntityMgmt->release();
    }
}
