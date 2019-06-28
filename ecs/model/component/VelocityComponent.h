/**
* VelocityComponent.h
*
* Init Created: 2016-05-30
* Last Updated: 2016-06-15
*/

#ifndef VELOCITY_COMPONENT_H
#define VELOCITY_COMPONENT_H

#include "AbstractComponentImpl.h"

namespace ecs {


class VelocityComponent :
    public AbstractComponentImpl,
    public IMoveable
{
    friend class EntityImpl;
    friend class AbstractComponentImpl;

public:
    static const iid_t IID = ((iid_t) 0x01C007);

private:
    // Creation
	CREATE_INSTANCE_FUNCTION(VelocityComponent)


    VelocityComponent() {
        printf("VelocityComponent\n");
    }


    virtual ~VelocityComponent() {
        finalRelease();

        printf("~VelocityComponent\n");
    }


    // IUniversal
    //
    virtual lresult_t query(iid_t iid, void **ppv) {
        if (iid == IUniversal::IID || iid == IComponent::IID || iid == VelocityComponent::IID) {
            *ppv = static_cast<IComponent*> (this);
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


    // IComponent
    //
    virtual const char * getComponentName() const {
        return "VelocityComponent";
    }


    virtual iid_t getComponentIID() const {
        return VelocityComponent::IID;
    }


    virtual void update(float dt) {

    }


    virtual lresult_t queryNotAddRef(iid_t iid, void** ppvNotAddRef) {
        if (iid == IMoveable::IID) {
            *ppvNotAddRef = static_cast<IMoveable*> (this);
            return lres_success;
        }
        else {
            return AbstractComponentImpl::queryNotAddRef(iid, ppvNotAddRef);
        }
    }

    // IMoveable
    //
    void setPosition(float x, float y) {

    }
};

}; /* namespace ecs */

#endif /* VELOCITY_COMPONENT_H */