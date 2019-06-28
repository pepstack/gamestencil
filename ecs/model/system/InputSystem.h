/**
* InputSystem.h
*
* Init Created: 2016-06-14
* Last Updated: 2016-06-14
*/
#ifndef INPUT_SYSTEM_H
#define INPUT_SYSTEM_H

#include "AbstractSystemImpl.h"


namespace ecs {

class InputSystem :
    public AbstractSystemImpl
{
    friend class SystemMgmt;

public:
    static const iid_t IID = ((iid_t) 0x00D500);
    static const int PRIORITY = (500);

private:
    // Creation
	CREATE_INSTANCE_FUNCTION(InputSystem)


    InputSystem() {
        printf("InputSystem\n");
    }


    virtual ~InputSystem() {
        finalRelease();

        printf("~InputSystem\n");
    }

    // IUniversal
    //
    virtual lresult_t query(iid_t iid, void **ppv) {
        if (iid == IUniversal::IID || iid == ISystem::IID || iid == InputSystem::IID) {
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
        return "InputSystem";
    }

    virtual iid_t getSystemIID() const {
        return InputSystem::IID;
    }


    virtual bool start() {
        return true;
    }


    virtual void end() {

    }


    virtual void update(float dt) {
        printf("InputSystem::priority(%d)\n", getPriority());
    }
};

}; /* namespace ecs */

#endif /* INPUT_SYSTEM_H */