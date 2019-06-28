/**
* ISystem.h
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

#ifndef ISYSTEM_H
#define ISYSTEM_H

#include "../core/SIPtr.h"

namespace ecs {

// define max count of supporting interfaces by components
#define MAX_SUPPORTING_INTERFACES  510


interface ISystem : IUniversal {
    static const iid_t IID = ((iid_t) 0x00D001);

    virtual const char* getSystemName() const = 0;
    virtual iid_t getSystemIID() const = 0;
    virtual int getPriority() const = 0;

    virtual lresult_t prevSystem(interface ISystemMgmt *mgmt, ISystem **ppv) = 0;
    virtual lresult_t nextSystem(interface ISystemMgmt *mgmt, ISystem **ppv) = 0;

    virtual bool start() = 0;
    virtual void end() = 0;
    virtual void update(float dt) = 0;
};

}; /* namespace ecs */

#endif /* ISYSTEM_H */