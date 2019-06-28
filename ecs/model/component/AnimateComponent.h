/**
 * AnimateComponent.h
 *
 * Init Created: 2016-05-30
 * Last Updated: 2016-06-15
 */

#ifndef ANIMATE_COMPONENT_H
#define ANIMATE_COMPONENT_H

#include "AbstractComponentImpl.h"

namespace ecs {


class AnimateComponent :
    public AbstractComponentImpl
{
    friend class EntityImpl;
    friend class AbstractComponentImpl;

public:
    static const iid_t IID = ((iid_t) 0x01C001);

private:
    // Creation
	CREATE_INSTANCE_FUNCTION(AnimateComponent)

private:

    AnimateComponent() {
        printf("AnimateComponent\n");
    }


    virtual ~AnimateComponent() {
        finalRelease();

        printf("~AnimateComponent\n");
    }


    // IUniversal
    //
    virtual lresult_t query(iid_t iid, void **ppv) {
        if (iid == IUniversal::IID || iid == IComponent::IID || iid == AnimateComponent::IID) {
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
        return "AnimateComponent";
    }


    virtual iid_t getComponentIID() const {
        return AnimateComponent::IID;
    }


    virtual void update(float dt) {

    }

    virtual lresult_t queryNotAddRef(iid_t iid, void** ppvNotAddRef) {
        return AbstractComponentImpl::queryNotAddRef(iid, ppvNotAddRef);
    }
};

}; /* namespace ecs */

#endif /* ANIMATE_COMPONENT_H */