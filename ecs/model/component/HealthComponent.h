/**
* HealthComponent.h
*
* Init Created: 2016-05-30
* Last Updated: 2016-06-15
*/

#ifndef HEALTH_COMPONENT_H
#define HEALTH_COMPONENT_H

#include "AbstractComponentImpl.h"

namespace ecs {


class HealthComponent :
    public AbstractComponentImpl,
    public IHealth
{
    friend class EntityImpl;
    friend class AbstractComponentImpl;

public:
    static const iid_t IID = ((iid_t) 0x01C004);

private:
    // Creation
	CREATE_INSTANCE_FUNCTION(HealthComponent)

    
    HealthComponent() : health(100) {
        printf("HealthComponent\n");
    }


    virtual ~HealthComponent() {
        finalRelease();

        printf("~HealthComponent\n");
    }


    // IUniversal
    //
    virtual lresult_t query(iid_t iid, void **ppv) {
        if (iid == IUniversal::IID || iid == IComponent::IID || iid == HealthComponent::IID) {
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
    virtual lresult_t queryNotAddRef(iid_t iid, void** ppvNotAddRef) {
        if (iid == IHealth::IID) {
            *ppvNotAddRef = static_cast<IHealth*> (this);
            return lres_success;
        }
        else {
            return AbstractComponentImpl::queryNotAddRef(iid, ppvNotAddRef);
        }
    }


    virtual const char * getComponentName() const {
        return "HealthComponent";
    }


    virtual iid_t getComponentIID() const {
        return HealthComponent::IID;
    }


    virtual void update(float dt) {

    }

    // IHealth
    //
    void setHealth(int v) {
        health = (v < 0? 0 : (v > 100? 100 : v));
    }


    int getHealth(void) {
        return health;
    }


private:

    int health;
};

}; /* namespace ecs */

#endif /* HEALTH_COMPONENT_H */