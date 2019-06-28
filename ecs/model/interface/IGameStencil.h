/**
* IGameStencil.h
*
* Author: master@pepstack.com
*
* Refer:
*   http://www.richardlord.net/blog/what-is-an-entity-framework
*   http://blog.csdn.net/i_dovelemon/article/details/30250049
*   http://blog.csdn.net/zhao_92221/article/details/46629553
*   http://blog.csdn.net/ubuntu64fan/article/details/8839778
*
* Init Created: 2016-06-13
* Last Updated: 2016-06-15
*/
#ifndef IGAME_STENCIL_H
#define IGAME_STENCIL_H


#include "../core/SIPtr.h"

#include "ISystemMgmt.h"
#include "IEntityMgmt.h"

namespace ecs {

interface IGameStencil : IUniversal {
    static const iid_t IID = ((iid_t) 0x00F001);

    virtual lresult_t getSystemMgmt(ISystemMgmt** ppvOut) = 0;
    virtual lresult_t getEntityMgmt(IEntityMgmt** ppvOut) = 0;

    virtual lresult_t saveStencilXML(const char * stencilXmlFile) = 0;

    virtual void update(float dt) = 0;
};

}; /* namespace ecs */

#endif /* IGAME_STENCIL_H */
