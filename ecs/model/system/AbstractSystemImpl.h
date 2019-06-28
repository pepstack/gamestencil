/**
* AbstractSystemImpl.h
*   The AbstractSystemImpl class is the abstract implementation of ISystem.
*
* Refer:
*   http://www.richardlord.net/blog/what-is-an-entity-framework
*   http://blog.csdn.net/i_dovelemon/article/details/30250049
*   http://blog.csdn.net/zhao_92221/article/details/46629553
*   http://blog.csdn.net/ubuntu64fan/article/details/8839778
*
* Init Created: 2016-06-14
* Last Updated: 2016-06-14
*/
#ifndef ABSTRACT_SYSTEM_IMPL_H
#define ABSTRACT_SYSTEM_IMPL_H

#include "../interface/ISystem.h"

#include "../component/interface/IHealth.h"
#include "../component/interface/IMoveable.h"
#include "../component/interface/IRenderable.h"

#include "../../../common/dhlist.h"

#include "../../../thirdparty/mxml-2.9/mxml.h"
#define ecsSystemNodeName  "system"


namespace ecs {

class AbstractSystemImpl :
    public ISystem,
    public UniversalImpl   
{
    friend class SystemMgmt;

protected:

    // Constructor
    AbstractSystemImpl() : priority(0) {
        printf("AbstractSystemImpl\n");
    }


    // Destructor
    virtual ~AbstractSystemImpl() {
        finalRelease();

        printf("~AbstractSystemImpl\n");
    }


	// Initialization
    //
    virtual lresult_t init(unsigned threadctx) {
        thread_ctx = threadctx;
        return lres_success;
    }


    virtual void finalRelease() {
        ref_count = 1;
    }


    virtual lresult_t updateStencilXML(mxml_node_t *parent);


    // IUniversal
    //
    virtual lresult_t query(iid_t iid, void **ppv) = 0;


    // ISystem
    //
    virtual int getPriority() const {
        return priority;
    }

    virtual lresult_t prevSystem(interface ISystemMgmt *mgmt, ISystem **ppv);
    virtual lresult_t nextSystem(interface ISystemMgmt *mgmt, ISystem **ppv);

    virtual const char* getSystemName() const = 0;
    virtual iid_t getSystemIID() const = 0;

    virtual bool start() = 0;
    virtual void end() = 0;
    virtual void update(float dt) = 0;


protected:

    int priority;

    struct list_head  i_list;
    struct hlist_node i_hash;
};

}; /* namespace ecs */

#endif /* ABSTRACT_SYSTEM_IMPL_H */