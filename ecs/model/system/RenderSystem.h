/**
* RenderSystem.h
*
* Init Created: 2016-06-14
* Last Updated: 2016-06-14
*/
#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include "AbstractSystemImpl.h"

#include "../interface/IEntity.h"


namespace ecs {


class RenderSystem :
    public AbstractSystemImpl
{
    friend class SystemMgmt;
    
public:
    static const iid_t IID = ((iid_t)0x00D100);
    static const int PRIORITY = (100);

private:
    // Creation
    CREATE_INSTANCE_FUNCTION(RenderSystem)


    RenderSystem() {
        printf("RenderSystem\n");
    }


    virtual ~RenderSystem() {
        finalRelease();

        printf("~RenderSystem\n");
    }

    // IUniversal
    //
    virtual lresult_t query(iid_t iid, void **ppv) {
        if (iid == IUniversal::IID || iid == ISystem::IID || iid == RenderSystem::IID) {
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
        return "RenderSystem";
    }

    virtual iid_t getSystemIID() const {
        return RenderSystem::IID;
    }


    virtual bool start() {
        return true;
    }


    virtual void end() {

    }


    virtual void update(float dt);

private:

    void updateEntity(float dt, IEntity * entity);
};

}; /* namespace ecs */

#endif /* RENDER_SYSTEM_H */