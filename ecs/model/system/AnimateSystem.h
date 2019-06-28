/**
* AnimateSystem.h
*
* Init Created: 2016-06-14
* Last Updated: 2016-06-14
*/
#ifndef ANIMATE_SYSTEM_H
#define ANIMATE_SYSTEM_H

#include "AbstractSystemImpl.h"


namespace ecs {

class AnimateSystem :
    public AbstractSystemImpl
{
    friend class SystemMgmt;

public:
    static const iid_t IID = ((iid_t) 0x00D600);
    static const int PRIORITY = (600);

private:
    // Creation
	CREATE_INSTANCE_FUNCTION(AnimateSystem)


    AnimateSystem() {
        printf("AnimateSystem\n");
    }


    virtual ~AnimateSystem() {
        finalRelease();

        printf("~AnimateSystem\n");
    }


    // IUniversal
    //
    virtual lresult_t query(iid_t iid, void **ppv) {
        if (iid == IUniversal::IID || iid == ISystem::IID || iid == AnimateSystem::IID) {
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
        return "AnimateSystem";
    }

    virtual iid_t getSystemIID() const {
        return AnimateSystem::IID;
    }


    virtual bool start() {
        return true;
    }


    virtual void end() {

    }


    virtual void update(float dt) {
        printf("AnimateSystem::priority(%d)\n", getPriority());
    }

};

}; /* namespace ecs */

#endif /* ANIMATE_SYSTEM_H */