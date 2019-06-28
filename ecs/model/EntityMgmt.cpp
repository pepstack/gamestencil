/**
* EntityMgmt.cpp
*
* Init Created: 2016-06-20
* Last Updated: 2016-06-20
*/
#include "EntityMgmt.h"

using namespace ecs;


int EntityMgmt::hmap_free_entity(IEntity * entity, mxml_node_t * entity_list) {
    ((EntityImpl*)entity)->updateStencilXML(entity_list);
    entity->release();
    return 0;
}


lresult_t EntityMgmt::updateStencilXML(mxml_node_t *parent)
{
    mxml_node_t *entity_list;

    entity_list = mxmlNewElement(parent, ecsEntityListNodeName);

    hmap_t map = hashmap_create();

    IEntity* pIEntity;
    if (lres_success == firstEntity(&pIEntity)) {
        if (HMAP_S_OK == hashmap_put(map, (char*)pIEntity->getEntityName(0), pIEntity)) {
            pIEntity->retain();
        }

        IEntity* pIEntityNext;
        while (lres_success == pIEntity->nextEntity(this, &pIEntityNext)) {
            if (HMAP_S_OK == hashmap_put(map, (char*)pIEntityNext->getEntityName(0), pIEntityNext)) {
                pIEntityNext->retain();
            }

            pIEntity->release();
            pIEntity = pIEntityNext;
        }

        pIEntity->release();
    }

    hashmap_destroy(map, (hmap_callback_func) hmap_free_entity, (void*)entity_list);

    return lres_success;
}
