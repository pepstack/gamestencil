/**
* ISystemMgmt.h
*
* Author: master@pepstack.com
*
* Refer:
*   http://www.richardlord.net/blog/what-is-an-entity-framework
*   http://blog.csdn.net/i_dovelemon/article/details/30250049
*   http://blog.csdn.net/zhao_92221/article/details/46629553
*   http://blog.csdn.net/ubuntu64fan/article/details/8839778
*
* Init Created: 2016-06-10
* Last Updated: 2016-06-15
*/
#ifndef ISYSTEM_MGMT_H
#define ISYSTEM_MGMT_H


#include "ISystem.h"


namespace ecs {

interface ISystemMgmt : IUniversal
{
    static const iid_t IID = ((iid_t) 0x00D002);

    virtual lresult_t createSystem(iid_t systemIID, int priority, ISystem **ppvOut) = 0;
    virtual lresult_t removeSystem(iid_t systemIID) = 0;
    virtual void removeAllSystems(void) = 0;
    virtual lresult_t firstSystem(ISystem ** ppvFirst) = 0;
    virtual lresult_t lastSystem(ISystem ** ppvLast) = 0;

    virtual void update(float dt) = 0;
    virtual void pause() = 0;
    virtual void resume() = 0;
};

}; /* namespace ecs */

#endif /* ISYSTEM_MGMT_H */
