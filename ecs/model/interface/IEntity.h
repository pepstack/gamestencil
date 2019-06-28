/**
* IEntity.h
*
* Author: master@pepstack.com
*
* Refer:
*   http://www.richardlord.net/blog/what-is-an-entity-framework
*   http://blog.csdn.net/i_dovelemon/article/details/30250049
*   http://blog.csdn.net/zhao_92221/article/details/46629553
*   http://blog.csdn.net/ubuntu64fan/article/details/8839778
*
* Init Created: 2016-06-14
* Last Updated: 2016-06-15
*/
#ifndef IENTITY_H
#define IENTITY_H


#include "../core/SIPtr.h"

#include "IComponent.h"

#ifndef ENTITY_NAME_PREFIX
#define ENTITY_NAME_PREFIX  "Entity#"
#define ENTITY_NAME_PREFIX_LEN  7
#endif

#ifndef ENTITY_NAME_MAXLEN
#define ENTITY_NAME_MAXLEN  (ENTITY_NAME_PREFIX_LEN + 30)
#endif


namespace ecs {


interface IEntity : IUniversal {
    static const iid_t IID = ((iid_t) 0x00E001);

    virtual const char * getEntityName(char * optOutName) = 0;

    virtual lresult_t createComponent(iid_t componentIID, IComponent **ppComponent) = 0;
    virtual lresult_t queryComponent(iid_t componentIID, IComponent **ppComponent) = 0;
    virtual lresult_t removeComponent(iid_t componentIID) = 0;
    virtual void removeAllComponents(void) = 0;
    virtual lresult_t firstComponent(IComponent ** ppvFirst) = 0;
    virtual lresult_t lastComponent(IComponent ** ppvLast) = 0;

    virtual int getComponentInterfaceMap(iid_t interfaceIID, IComponent*** outComponents) = 0;

    virtual lresult_t prevEntity(interface IEntityMgmt* mgmt, IEntity **ppv) = 0;
    virtual lresult_t nextEntity(interface IEntityMgmt* mgmt, IEntity **ppv) = 0;
};

}; /* namespace ecs */

#endif /* IENTITY_H */
