/**
* IComponent.h
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
* Last Updated: 2016-06-14
*/
#ifndef ICOMPONENT_H
#define ICOMPONENT_H

#include "../core/SIPtr.h"


namespace ecs {

interface IComponent : IUniversal {
    static const iid_t IID = ((iid_t) 0x00C001);

    virtual iid_t getComponentIID(void) const = 0;
    virtual const char* getComponentName() const = 0;
    virtual lresult_t prevComponent(interface IEntity *entity, IComponent **ppv) = 0;
    virtual lresult_t nextComponent(interface IEntity *entity, IComponent **ppv) = 0;

    virtual lresult_t queryNotAddRef(iid_t iid, void** ppvNotAddRef) = 0;

    virtual void update(float dt) = 0;
};

}; /* namespace ecs */

#endif /* ICOMPONENT_H */
