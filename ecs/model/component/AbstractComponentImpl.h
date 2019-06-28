/**
* AbstractComponentImpl.h
*   The AbstractComponentImpl class is the abstract implementation of IComponent.
*
* Refer:
*   http://www.richardlord.net/blog/what-is-an-entity-framework
*   http://blog.csdn.net/i_dovelemon/article/details/30250049
*   http://blog.csdn.net/zhao_92221/article/details/46629553
*   http://blog.csdn.net/ubuntu64fan/article/details/8839778
*
* Init Created: 2016-06-14
* Last Updated: 2016-06-15
*/
#ifndef ABSTRACT_COMPONENT_IMPL_H
#define ABSTRACT_COMPONENT_IMPL_H

#include "../interface/IComponent.h"


// NotAddRef Interfaces
//
#include "interface/IHealth.h"
#include "interface/IMoveable.h"
#include "interface/IRenderable.h"


#include "../../../common/dhlist.h"

#include "../../../thirdparty/mxml-2.9/mxml.h"
#define ecsComponentListNodeName  "component-bundle"
#define ecsComponentNodeName  "component"


namespace ecs {

class AbstractComponentImpl :
    public IComponent,
    public UniversalImpl
{
    friend class EntityImpl;

public:
    static iid_t const intefaceIIDs[];

protected:

    // Constructor
    AbstractComponentImpl() {
        printf("AbstractComponentImpl\n");
    }


    // Destructor
    virtual ~AbstractComponentImpl() {
        finalRelease();

        printf("~AbstractComponentImpl\n");
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

    lresult_t updateStencilXML(mxml_node_t *parent);

    // IUniversal
    //
    virtual lresult_t query(iid_t iid, void **ppv) = 0;


    // IComponent
    //
    virtual lresult_t queryNotAddRef(iid_t iid, void** ppvNotAddRef) {
        *ppvNotAddRef = 0;
        return lres_e_notimpl;
    }

    virtual lresult_t prevComponent(interface IEntity *entity, IComponent **ppv);
    virtual lresult_t nextComponent(interface IEntity *entity, IComponent **ppv);

    virtual const char* getComponentName() const = 0;
    virtual iid_t getComponentIID() const = 0;
    virtual void update(float dt) = 0;

protected:

    struct list_head  i_list;
    struct hlist_node i_hash;
};

}; /* namespace ecs */

#endif /* ABSTRACT_COMPONENT_IMPL_H */
