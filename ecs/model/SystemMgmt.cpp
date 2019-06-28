/**
* SystemMgmt.cpp
*
* Init Created: 2016-06-20
* Last Updated: 2016-06-20
*/
#include "SystemMgmt.h"

using namespace ecs;


lresult_t SystemMgmt::updateStencilXML(mxml_node_t *parent)
{
    mxml_node_t *system_list;

    system_list = mxmlNewElement(parent, ecsSystemListNodeName);

    do {
        ISystem * pISystem;
        if (lres_success == firstSystem(&pISystem)) {
            ((AbstractSystemImpl *)pISystem)->updateStencilXML(system_list);

            ISystem * pISystemNext;
            while (lres_success == pISystem->nextSystem(this, &pISystemNext)) {

                ((AbstractSystemImpl *)pISystemNext)->updateStencilXML(system_list);

                pISystem->release();
                pISystem = pISystemNext;
            }

            pISystem->release();
        }
    } while (0);

    return lres_success;
}
