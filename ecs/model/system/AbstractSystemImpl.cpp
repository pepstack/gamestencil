/**
* AbstractSystemImpl.cpp
*   The AbstractSystemImpl class is the abstract implementation of IComponent.
*
* Init Created: 2016-06-16
* Last Updated: 2016-06-16
*/
#include "AbstractSystemImpl.h"
#include "../SystemMgmt.h"

using namespace ecs;

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4996)
#endif


lresult_t AbstractSystemImpl::prevSystem(interface ISystemMgmt* mgmt, ISystem **ppv) {
    if (i_list.prev != &((SystemMgmt*)mgmt)->list1) {
        ISystem * p = (ISystem*)list_entry(i_list.prev, class AbstractSystemImpl, i_list);
        return p->query(ISystem::IID, (void**)ppv);
    }
    else {
        *ppv = 0;
        return lres_e_pointer;
    }
}


lresult_t AbstractSystemImpl::nextSystem(interface ISystemMgmt* mgmt, ISystem **ppv) {
    if (i_list.next != &((SystemMgmt*)mgmt)->list1) {
        ISystem * p = (ISystem*)list_entry(i_list.next, class AbstractSystemImpl, i_list);
        return p->query(ISystem::IID, (void**)ppv);
    }
    else {
        *ppv = 0;
        return lres_e_notfound;
    }
}


lresult_t AbstractSystemImpl::updateStencilXML(mxml_node_t *parent) {
    mxml_node_t *system;

    char buffer[256];

    system = mxmlNewElement(parent, ecsSystemNodeName);

    sprintf(buffer, "%d", getPriority());

    mxmlElementSetAttr(system, "priority", buffer);
    mxmlElementSetAttr(system, "prototype", getSystemName());


    return 0;
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif