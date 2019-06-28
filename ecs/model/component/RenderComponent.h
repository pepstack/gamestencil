/**
* RenderComponent.h
*
* Init Created: 2016-05-30
* Last Updated: 2016-06-15
*/

#ifndef RENDER_COMPONENT_H
#define RENDER_COMPONENT_H

#include "AbstractComponentImpl.h"

namespace ecs {


class RenderComponent :
    public AbstractComponentImpl,
    public IRenderable
{
    friend class EntityImpl;
    friend class AbstractComponentImpl;

public:
    static const iid_t IID = ((iid_t) 0x01C006);

private:
    // Creation
	CREATE_INSTANCE_FUNCTION(RenderComponent)


    RenderComponent() {
        printf("RenderComponent\n");
    }


    virtual ~RenderComponent() {
        finalRelease();

        printf("~RenderComponent\n");
    }


    // IUniversal
    //
    virtual lresult_t query(iid_t iid, void **ppv) {
        if (iid == IUniversal::IID || iid == IComponent::IID || iid == RenderComponent::IID) {
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
        if (iid == IRenderable::IID) {
            *ppvNotAddRef = static_cast<IRenderable*> (this);
            return lres_success;
        }
        else {
            return AbstractComponentImpl::queryNotAddRef(iid, ppvNotAddRef);
        }
    }


    virtual const char * getComponentName() const {
        return "RenderComponent";
    }


    virtual iid_t getComponentIID() const {
        return RenderComponent::IID;
    }


    virtual void update(float dt) {

    }

    // IRenderable
    //
    void render(float dt) {
        // TODO:
    }
};

}; /* namespace ecs */

#endif /* RENDER_COMPONENT_H */