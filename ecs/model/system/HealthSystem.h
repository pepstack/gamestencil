/**
* HealthSystem.h
*
* Init Created: 2016-06-14
* Last Updated: 2016-06-14
*/
#ifndef HEALTH_SYSTEM_H
#define HEALTH_SYSTEM_H

#include "AbstractSystemImpl.h"


namespace ecs {

class HealthSystem :
    public AbstractSystemImpl
{
    friend class SystemMgmt;

public:
    static const iid_t IID = ((iid_t) 0x00D300);
    static const int PRIORITY = (300);

private:
    // Creation
	CREATE_INSTANCE_FUNCTION(HealthSystem)


    HealthSystem() {
        printf("HealthSystem\n");
    }


    virtual ~HealthSystem() {
        finalRelease();

        printf("~HealthSystem\n");
    }

    // IUniversal
    //
    virtual lresult_t query(iid_t iid, void **ppv) {
        if (iid == IUniversal::IID || iid == ISystem::IID || iid == HealthSystem::IID) {
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
        return "HealthSystem";
    }

    virtual iid_t getSystemIID() const {
        return HealthSystem::IID;
    }


    virtual bool start() {
        return true;
    }


    virtual void end() {

    }


    virtual void update(float dt) {
        printf("HealthSystem::priority(%d)\n", getPriority());
    }
};

}; /* namespace ecs */

#endif /* HEALTH_SYSTEM_H */