/**
* IEntityMgmt.h
*
* Author: master@pepstack.com
*
* Refer:
*   http://www.richardlord.net/blog/what-is-an-entity-framework
*   http://blog.csdn.net/i_dovelemon/article/details/30250049
*   http://blog.csdn.net/zhao_92221/article/details/46629553
*   http://blog.csdn.net/ubuntu64fan/article/details/8839778
*
* Init Created: 2016-06-15
* Last Updated: 2016-06-15
*/
#ifndef IENTITY_MGMT_H
#define IENTITY_MGMT_H


#include "IEntity.h"


namespace ecs {

interface IEntityMgmt : IUniversal
{
    static const iid_t IID = ((iid_t) 0x00E002);

    virtual lresult_t createEntity(const char * entityName, IEntity **ppvOut) = 0;
    virtual lresult_t removeEntity(IEntity *entity) = 0;
    virtual void removeAllEntities(void) = 0;
    virtual lresult_t firstEntity(IEntity ** ppvFirst) = 0;
    virtual lresult_t lastEntity(IEntity ** ppvLast) = 0;
};

}; /* namespace ecs */

#endif /* IENTITY_MGMT_H */
