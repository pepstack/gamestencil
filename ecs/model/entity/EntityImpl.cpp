/**
* EntityImpl.cpp
*   The EntityImpl class is the abstract implementation of IComponent.
*
* Init Created: 2016-06-16
* Last Updated: 2016-06-16
*/
#include "EntityImpl.h"
#include "../EntityMgmt.h"

using namespace ecs;


lresult_t EntityImpl::updateStencilXML(mxml_node_t *parent)
{
    mxml_node_t *entity_node;
    entity_node = mxmlNewElement(parent, ecsEntityNodeName);

    mxmlElementSetAttr(entity_node, "prototype", getEntityName(0));

    IComponent* pIComponent;
    if (lres_success == firstComponent(&pIComponent)) {
        mxml_node_t * component_list = mxmlNewElement(entity_node, ecsComponentListNodeName);

        ((AbstractComponentImpl*)pIComponent)->updateStencilXML(component_list);

        IComponent* pIComponentNext;
        while (lres_success == pIComponent->nextComponent(this, &pIComponentNext)) {
            ((AbstractComponentImpl*)pIComponentNext)->updateStencilXML(component_list);

            pIComponent->release();
            pIComponent = pIComponentNext;
        }

        pIComponent->release();
    }

    return lres_success;
}


lresult_t EntityImpl::prevEntity(interface IEntityMgmt* mgmt, IEntity **ppv) {
    if (i_list.prev != &((EntityMgmt*)mgmt)->list1) {
        IEntity * p = (IEntity*)list_entry(i_list.prev, class EntityImpl, i_list);
        return p->query(IEntity::IID, (void**)ppv);
    }
    else {
        *ppv = 0;
        return lres_e_pointer;
    }
}


lresult_t EntityImpl::nextEntity(interface IEntityMgmt* mgmt, IEntity **ppv) {
    if (i_list.next != &((EntityMgmt*)mgmt)->list1) {
        IEntity * p = (IEntity*)list_entry(i_list.next, class EntityImpl, i_list);
        return p->query(IEntity::IID, (void**)ppv);
    }
    else {
        *ppv = 0;
        return lres_e_notfound;
    }
}
