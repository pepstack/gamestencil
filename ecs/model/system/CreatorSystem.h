/**
* CreatorSystem.h
*
* Init Created: 2016-06-14
* Last Updated: 2016-06-14
*/
#ifndef CREATOR_SYSTEM_H
#define CREATOR_SYSTEM_H

#include "AbstractSystemImpl.h"


namespace ecs {

class CreatorSystem :
    public AbstractSystemImpl
{
    friend class SystemMgmt;

public:
    static const iid_t IID = ((iid_t) 0x00D400);
    static const int PRIORITY = (400);

private:
    // Creation
	CREATE_INSTANCE_FUNCTION(CreatorSystem)


    CreatorSystem() {
        printf("CreatorSystem\n");
    }


    virtual ~CreatorSystem() {
        finalRelease();

        printf("~CreatorSystem\n");
    }

    // IUniversal
    //
    virtual lresult_t query(iid_t iid, void **ppv) {
        if (iid == IUniversal::IID || iid == ISystem::IID || iid == CreatorSystem::IID) {
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
        return "CreatorSystem";
    }

    virtual iid_t getSystemIID() const {
        return CreatorSystem::IID;
    }


    virtual bool start() {
        return true;
    }


    virtual void end() {

    }


    virtual void update(float dt) {
        printf("CreatorSystem::priority(%d)\n", getPriority());
    }
};

}; /* namespace ecs */

#endif /* CREATOR_SYSTEM_H */