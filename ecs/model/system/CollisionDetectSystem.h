/**
* CollisionDetectSystem.h
*
* Init Created: 2016-06-14
* Last Updated: 2016-06-14
*/
#ifndef COLLISION_DETECT_SYSTEM_H
#define COLLISION_DETECT_SYSTEM_H

#include "AbstractSystemImpl.h"


namespace ecs {

class CollisionDetectSystem :
    public AbstractSystemImpl
{
    friend class SystemMgmt;

public:
    static const iid_t IID = ((iid_t) 0x00D700);
    static const int PRIORITY = (700);

private:
    // Creation
	CREATE_INSTANCE_FUNCTION(CollisionDetectSystem)

    CollisionDetectSystem() {
        printf("CollisionDetectSystem\n");
    }


    virtual ~CollisionDetectSystem() {
        finalRelease();

        printf("~CollisionDetectSystem\n");
    }


    // IUniversal
    //
    virtual lresult_t query(iid_t iid, void **ppv) {
        if (iid == IUniversal::IID || iid == ISystem::IID || iid == CollisionDetectSystem::IID) {
            *ppv = static_cast<ISystem *> (this);
        }
        else {
            *ppv = 0;
            return lres_e_nointerface;
        }
        reinterpret_cast<IUniversal*>(*ppv)->retain();
        return lres_success;
    }

    virtual unsigned long retain(void) {
        return UniversalImpl::retain();
    }

    virtual unsigned long release(void) {
        return UniversalImpl::release();
    }


    // ISystem
    //
    virtual const char * getSystemName() const {
        return "CollisionDetectSystem";
    }

    virtual iid_t getSystemIID() const {
        return CollisionDetectSystem::IID;
    }


    virtual bool start() {
        return true;
    }


    virtual void end() {

    }


    virtual void update(float dt) {
        printf("CollisionDetectSystem::priority(%d)\n", getPriority());
    }
};

}; /* namespace ecs */

#endif /* COLLISION_DETECT_SYSTEM_H */