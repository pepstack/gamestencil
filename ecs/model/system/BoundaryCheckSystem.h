/**
* BoundaryCheckSystem.h
*
* Init Created: 2016-06-14
* Last Updated: 2016-06-14
*/
#ifndef BOUNDARY_CHECK_SYSTEM_H
#define BOUNDARY_CHECK_SYSTEM_H

#include "AbstractSystemImpl.h"


namespace ecs {

class BoundaryCheckSystem :
    public AbstractSystemImpl
{
    friend class SystemMgmt;

public:
    static const iid_t IID = ((iid_t) 0x00D800);
    static const int PRIORITY = (800);

private:
    // Creation
	CREATE_INSTANCE_FUNCTION(BoundaryCheckSystem)


    BoundaryCheckSystem() {
        printf("BoundaryCheckSystem\n");
    }


    virtual ~BoundaryCheckSystem() {
        finalRelease();

        printf("~BoundaryCheckSystem\n");
    }

    // IUniversal
    //
    virtual lresult_t query(iid_t iid, void **ppv) {
        if (iid == IUniversal::IID || iid == ISystem::IID || iid == BoundaryCheckSystem::IID) {
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
        return "BoundaryCheckSystem";
    }

    virtual iid_t getSystemIID() const {
        return BoundaryCheckSystem::IID;
    }


    virtual bool start() {
        return true;
    }


    virtual void end() {

    }


    virtual void update(float dt) {
        printf("BoundaryCheckSystem::priority(%d)\n", getPriority());
    }
};

}; /* namespace ecs */

#endif /* BOUNDARY_CHECK_SYSTEM_H */