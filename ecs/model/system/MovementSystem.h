/**
* MovementSystem.h
*
* Init Created: 2016-06-14
* Last Updated: 2016-06-14
*/
#ifndef MOVEMENT_SYSTEM_H
#define MOVEMENT_SYSTEM_H

#include "AbstractSystemImpl.h"


namespace ecs {

class MovementSystem :
    public AbstractSystemImpl
{
    friend class SystemMgmt;

public:
    static const iid_t IID = ((iid_t) 0x00D200);
    static const int PRIORITY = (200);

private:
    // Creation
	CREATE_INSTANCE_FUNCTION(MovementSystem)


    MovementSystem() {
        printf("MovementSystem\n");
    }


    virtual ~MovementSystem() {
        finalRelease();

        printf("~MovementSystem\n");
    }

    // IUniversal
    //
    virtual lresult_t query(iid_t iid, void **ppv) {
        if (iid == IUniversal::IID || iid == ISystem::IID || iid == MovementSystem::IID) {
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
        return "MovementSystem";
    }

    virtual iid_t getSystemIID() const {
        return MovementSystem::IID;
    }


    virtual bool start() {
        return true;
    }


    virtual void end() {

    }


    virtual void update(float dt) {
        printf("MovementSystem::priority(%d)\n", getPriority());
    }
};

}; /* namespace ecs */

#endif /* MOVEMENT_SYSTEM_H */