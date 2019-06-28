/**
* SystemMgmt.h
*
* Refer:
*   http://www.richardlord.net/blog/what-is-an-entity-framework
*   http://blog.csdn.net/i_dovelemon/article/details/30250049
*   http://blog.csdn.net/zhao_92221/article/details/46629553
*   http://blog.csdn.net/ubuntu64fan/article/details/8839778
*
* Init Created: 2016-06-13
* Last Updated: 2016-06-14
*/
#ifndef SYSTEM_MGMT_H
#define SYSTEM_MGMT_H

#include "interface/ISystemMgmt.h"

#include "system/AbstractSystemImpl.h"

#include "system/InputSystem.h"
#include "system/AnimateSystem.h"
#include "system/BoundaryCheckSystem.h"
#include "system/CollisionDetectSystem.h"
#include "system/HealthSystem.h"
#include "system/InputSystem.h"
#include "system/MovementSystem.h"
#include "system/RenderSystem.h"
#include "system/CreatorSystem.h"

#include "../../common/hashmap.h"
#include "../../common/dhlist.h"

#include <memory>
using namespace std;

#include "../../thirdparty/mxml-2.9/mxml.h"

#define ecsSystemListNodeName  "system-bundle"


namespace ecs {


class SystemMgmt :
    public UniversalImpl,
    public ISystemMgmt
    
{
    friend class GameStencil;
    friend class AbstractSystemImpl;

private:

    // Creation
	CREATE_INSTANCE_FUNCTION(SystemMgmt)

    static const int HASHSIZE = 0xff;

    int gethash(iid_t systemIID) {
        return get_int_hash((void*) systemIID, SystemMgmt::HASHSIZE);
    }


	// Constructor 
    SystemMgmt() : paused(true) {
        printf("SystemMgmt\n");
    }


    // Destructor
    virtual ~SystemMgmt() {
        finalRelease();

        printf("~SystemMgmt\n");
    }


    virtual lresult_t init(unsigned threadctx) {
        thread_ctx = threadctx;

        INIT_LIST_HEAD(&list1);
        for (int hash = 0; hash <= HASHSIZE; hash++) {
            INIT_HLIST_HEAD(&hlist[hash]);
        }

        paused = false;
        return lres_success;
    }


    virtual void finalRelease() {
        ref_count = 1;

        // release all ISystem * in list
        removeAllSystems();
    }


    virtual lresult_t updateStencilXML(mxml_node_t *parent);


    // IUniversal
    //
    virtual lresult_t query(iid_t iid, void **ppv) {
        if (iid == IUniversal::IID || iid == ISystemMgmt::IID) {
            *ppv = static_cast<ISystemMgmt *> (this);
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


    lresult_t findSystem(iid_t systemIID, int hash, ISystem **ppvOut) {
        *ppvOut = 0;
        do {
            // TODO: using lock

            struct hlist_node *hp;

            hlist_for_each(hp, &hlist[hash]) {
                ISystem * pISystem = (ISystem *)hlist_entry(hp, class AbstractSystemImpl, i_hash);

                if (pISystem->getSystemIID() == systemIID) {
                    return pISystem->query(systemIID, (void**)ppvOut);
                }
            }
        } while (0);

        return lres_e_notfound;
    }


    void insertSystem(ISystem* pISystem, int hash, int priority) {
        AbstractSystemImpl * system = (AbstractSystemImpl *) pISystem;
        system->priority = priority;

        int added = 0;
        do {
            struct list_head *list;

            list_for_each(list, &list1) {
                class AbstractSystemImpl * p = list_entry(list, class AbstractSystemImpl, i_list);

                if (system->getPriority() > p->getPriority()) {

                    list_add_tail(&system->i_list, list);
                    hlist_add_head(&system->i_hash, &hlist[hash]);

                    added = 1;
                    break;
                }
            }
        } while (0);

        if (added == 0) {
            list_add_tail(&system->i_list, &list1);
            hlist_add_head(&system->i_hash, &hlist[hash]);
        }
    }


    // ISystemMgmt
    //
    #define CREATE_SYSTEM_INSTANCE(SystemClassName)  do {\
        hr = SystemClassName::createInstance(thread_ctx, systemIID, (void**)&pISystem);\
        if (hr == lres_success) {\
            hr = pISystem->query(ISystem::IID, (void**)ppvOut);\
            if (hr != lres_success) {\
                pISystem->release();\
                return hr;\
            }\
            insertSystem(pISystem, hash, priority);\
        } else {\
            pISystem->release();\
            pISystem = 0;\
        }\
    } while (0)


    virtual lresult_t createSystem(iid_t systemIID, int priority, ISystem **ppvOut) {
        if (ppvOut) {
            *ppvOut = 0;
        }

        int hash = gethash(systemIID);
        
        ISystem * pISystem = 0;
        lresult_t hr = findSystem(systemIID, hash, &pISystem);

        if (hr == lres_success) {
            if (ppvOut) {
                *ppvOut = pISystem;
                return lres_success;
            } else {
                pISystem->release();
                return lres_existed;
            }
        } else if (! ppvOut) {
            // lres_e_notfound
            return hr;
        }

        switch (systemIID) {
        case AnimateSystem::IID:
            CREATE_SYSTEM_INSTANCE(AnimateSystem);
            break;

        case BoundaryCheckSystem::IID:
            CREATE_SYSTEM_INSTANCE(BoundaryCheckSystem);
            break;

        case CollisionDetectSystem::IID:
            CREATE_SYSTEM_INSTANCE(CollisionDetectSystem);
            break;

        case CreatorSystem::IID:
            CREATE_SYSTEM_INSTANCE(CreatorSystem);
            break;

        case HealthSystem::IID:
            CREATE_SYSTEM_INSTANCE(HealthSystem);
            break;

        case InputSystem::IID:
            CREATE_SYSTEM_INSTANCE(InputSystem);
            break;

        case MovementSystem::IID:
            CREATE_SYSTEM_INSTANCE(MovementSystem);
            break;

        case RenderSystem::IID:
            CREATE_SYSTEM_INSTANCE(RenderSystem);
            break;

        default:
            hr = lres_e_notimpl;
        }

        return hr;
    }


    virtual lresult_t removeSystem(iid_t systemIID) {
        int hash = gethash(systemIID);

        struct hlist_node *hp;
        struct hlist_node *hn;

        hlist_for_each_safe(hp, hn, &hlist[hash]) {
            AbstractSystemImpl * delSystem = hlist_entry(hp, class AbstractSystemImpl, i_hash);

            if (delSystem->getSystemIID() == systemIID) {
                hlist_del(hp);
                list_del( &delSystem->i_list );
                ((ISystem*) delSystem)->release();
                return lres_success;
            }
        }

        return lres_e_notfound;
    }


    // remove all systems
    virtual void removeAllSystems() {
        struct list_head *list, *node;

        list_for_each_safe(list, node, &list1) {
            AbstractSystemImpl * delSystem = list_entry(list, class AbstractSystemImpl, i_list);

            hlist_del( &delSystem->i_hash );
            list_del( list );
            ((ISystem *) delSystem)->release();
        }
    }

    virtual lresult_t firstSystem(ISystem** ppvFirst) {
        struct list_head *list;
        list_for_each(list, &list1) {
            AbstractSystemImpl * pSystem = list_entry(list, class AbstractSystemImpl, i_list);
            return pSystem->query(ISystem::IID, (void**)ppvFirst);
        }
        *ppvFirst = 0;
        return lres_e_notfound;
    }


    virtual lresult_t lastSystem(ISystem ** ppvLast) {
        struct list_head *list;
        list_for_each_prev(list, &list1) {
            AbstractSystemImpl * pSystem = list_entry(list, class AbstractSystemImpl, i_list);
            return pSystem->query(ISystem::IID, (void**)ppvLast);
        }
        *ppvLast = 0;
        return lres_e_notfound;
    }


    // Update all the system
    virtual void update(float dt) {
        if (! paused) {
            // update all the systems
            struct list_head *list;

            list_for_each(list, &list1) {
                ISystem * pISystem = (ISystem *)list_entry(list, class AbstractSystemImpl, i_list);

                pISystem->start();

                pISystem->update(dt);

                pISystem->end();
            }
        }
    }


    // Pause all the systems
    virtual void pause() {
        paused = true;
    }


    // Resume all the systems
    virtual void resume() {
        paused = false;
    }


protected:
    bool paused;

    struct list_head list1;
    struct hlist_head hlist[HASHSIZE + 1];
};

}; /* namespace ecs */

#endif /* SYSTEM_MGMT_H */