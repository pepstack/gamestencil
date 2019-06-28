/**
* AbstractComponentImpl.cpp
*   The AbstractComponentImpl class is the abstract implementation of IComponent.
*
* Init Created: 2016-06-16
* Last Updated: 2016-06-16
*/
#include "AbstractComponentImpl.h"
#include "../entity/EntityImpl.h"

using namespace ecs;


iid_t const AbstractComponentImpl::intefaceIIDs[] = {IHealth::IID, IMoveable::IID, IRenderable::IID, 0};


lresult_t AbstractComponentImpl::updateStencilXML(mxml_node_t *parent)
{
    mxml_node_t *component_node;

    do {
        component_node = mxmlNewElement(parent, ecsComponentNodeName);
        mxmlElementSetAttr(component_node, "prototype", getComponentName());
    } while (0);

    return lres_success;
}


lresult_t AbstractComponentImpl::prevComponent(interface IEntity* entity, IComponent **ppv) {
    if (i_list.prev != &((EntityImpl*)entity)->component_list1) {
        IComponent * p = (IComponent*)list_entry(i_list.prev, class AbstractComponentImpl, i_list);
        return p->query(IComponent::IID, (void**)ppv);
    }
    else {
        *ppv = 0;
        return lres_e_pointer;
    }
}


lresult_t AbstractComponentImpl::nextComponent(interface IEntity* entity, IComponent **ppv) {
    if (i_list.next != &((EntityImpl*)entity)->component_list1) {
        IComponent * p = (IComponent*)list_entry(i_list.next, class AbstractComponentImpl, i_list);
        return p->query(IComponent::IID, (void**)ppv);
    }
    else {
        *ppv = 0;
        return lres_e_notfound;
    }
}
