/**
* EntityImpl.h
*   The EntityImpl class is for Entity.
*
* Refer:
*   http://www.richardlord.net/blog/what-is-an-entity-framework
*   http://blog.csdn.net/i_dovelemon/article/details/30250049
*   http://blog.csdn.net/zhao_92221/article/details/46629553
*   http://blog.csdn.net/ubuntu64fan/article/details/8839778
*
* Init Created: 2016-06-15
* Last Updated: 2016-06-17
*/
#ifndef ENTITY_IMPL_H
#define ENTITY_IMPL_H

#include "../interface/IEntity.h"

#include "../component/ComponentInterfaceMap.h"

#include "../../../common/hashmap.h"
#include "../../../common/dhlist.h"

#include "../../../thirdparty/mxml-2.9/mxml.h"
#define ecsEntityNodeName  "entity"


namespace ecs {

class EntityImpl :
    public IEntity,
    public UniversalImpl   
{
    friend class EntityMgmt;
    friend class AbstractComponentImpl;

private:
    // Creation
    CREATE_INSTANCE_FUNCTION(EntityImpl)


private:
    friend class EntityMgmt;

    static const int HASHSIZE = 0x1F;

    int gethash(iid_t componentIID) {
        return get_int_hash((void*) componentIID, EntityImpl::HASHSIZE);
    }

protected:

#ifdef _MSC_VER
    #pragma warning(push)
    #pragma warning(disable : 4996)
#endif
    // Constructor
    EntityImpl() {
        strncpy(entityName, ENTITY_NAME_PREFIX, ENTITY_NAME_PREFIX_LEN);
        entityName[ENTITY_NAME_PREFIX_LEN] = 0;
    }
#ifdef _MSC_VER
    #pragma warning(pop)
#endif

    // Destructor
    virtual ~EntityImpl() {
        finalRelease();
    }


	// Initialization
    //
    virtual lresult_t init(unsigned threadctx) {
        thread_ctx = threadctx;

        INIT_LIST_HEAD(&component_list1);
        for (int hash = 0; hash <= HASHSIZE; hash++) {
            INIT_HLIST_HEAD(&component_hlist[hash]);
        }

        return lres_success;
    }


    virtual void finalRelease() {
        ref_count = 1;
        
        // release all IComponent * in list
        removeAllComponents();
    }


    virtual lresult_t updateStencilXML(mxml_node_t *parent);


    lresult_t findComponent(iid_t componentIID, int hash, IComponent **ppvOut) {
        *ppvOut = 0;
        do {
            // TODO: using lock
            struct hlist_node *hp;

            hlist_for_each(hp, &component_hlist[hash]) {
                IComponent * pIComponent = (IComponent *)hlist_entry(hp, class AbstractComponentImpl, i_hash);

                if (pIComponent->getComponentIID() == componentIID) {
                    return pIComponent->query(componentIID, (void**)ppvOut);
                }
            }
        } while (0);

        return lres_e_notfound;
    }


    void insertComponent(IComponent* pIComponent, int hash) {
        AbstractComponentImpl * component = (AbstractComponentImpl *) pIComponent;

        list_add_tail(&component->i_list, &component_list1);
        hlist_add_head(&component->i_hash, &component_hlist[hash]);

        interfaceMap.insert(component);
    }


    // IUniversal
    //
    virtual lresult_t query(iid_t iid, void **ppv) {
        if (iid == IUniversal::IID || iid == IEntity::IID) {
            *ppv = static_cast<IEntity *> (this);
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

    
    // IEntity
    //
    #define CREATE_COMPONENT_INSTANCE(ComponentClassName)  do {\
        hr = ComponentClassName::createInstance(thread_ctx, componentIID, (void**)&pIComponent);\
        if (hr == lres_success) {\
            hr = pIComponent->query(IComponent::IID, (void**)ppvOut);\
            if (hr != lres_success) {\
                pIComponent->release();\
                return hr;\
            }\
            insertComponent(pIComponent, hash);\
        } else {\
            pIComponent->release();\
            pIComponent = 0;\
        }\
    } while (0)


#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4996)
#endif
    virtual const char * getEntityName(char * optOutName) {
        if (optOutName) {
            strncpy(optOutName, entityName, ENTITY_NAME_MAXLEN);
            optOutName[ENTITY_NAME_MAXLEN] = 0;
            return optOutName;
        }
        else {
            return entityName;
        }
    }
#ifdef _MSC_VER
#pragma warning(pop)
#endif


    virtual lresult_t createComponent(iid_t componentIID, IComponent **ppvOut) {
        if (ppvOut) {
            *ppvOut = 0;
        }

        IComponent * pIComponent = 0;
        int hash = gethash(componentIID);
        lresult_t hr = findComponent(componentIID, hash, &pIComponent);

        if (hr == lres_success) {
            if (ppvOut) {
                *ppvOut = pIComponent;
                return lres_success;
            } else {
                pIComponent->release();
                return lres_existed;
            }
        } else if (! ppvOut) {
            // lres_e_notfound
            return hr;
        }

        switch (componentIID) {
        case EntityTypeComponent::IID:
            CREATE_COMPONENT_INSTANCE(EntityTypeComponent);
            break;

        case AnimateComponent::IID:
            CREATE_COMPONENT_INSTANCE(AnimateComponent);
            break;

        case CollisionComponent::IID:
            CREATE_COMPONENT_INSTANCE(CollisionComponent);
            break;

        case HealthComponent::IID:
            CREATE_COMPONENT_INSTANCE(HealthComponent);
            break;

        case PositionComponent::IID:
            CREATE_COMPONENT_INSTANCE(PositionComponent);
            break;

        case RenderComponent::IID:
            CREATE_COMPONENT_INSTANCE(RenderComponent);
            break;

        case VelocityComponent::IID:
            CREATE_COMPONENT_INSTANCE(VelocityComponent);
            break;

        default:
            hr = lres_e_notimpl;
        }

        return hr;
    }


    virtual lresult_t queryComponent(iid_t componentIID, IComponent **ppComponent) {
        return findComponent(componentIID, gethash(componentIID), ppComponent);
    }


    virtual lresult_t removeComponent(iid_t componentIID) {
        int hash = gethash(componentIID);

        struct hlist_node *hp;
        struct hlist_node *hn;

        hlist_for_each_safe(hp, hn, &component_hlist[hash]) {
            AbstractComponentImpl * delComponent = hlist_entry(hp, class AbstractComponentImpl, i_hash);

            if (delComponent->getComponentIID() == componentIID) {
                hlist_del(hp);
                list_del( &delComponent->i_list );

                interfaceMap.remove(delComponent);

                ((IComponent*)delComponent)->release();
                return lres_success;
            }
        }

        return lres_e_notfound;
    }


    // remove all components
    virtual void removeAllComponents() {
        interfaceMap.removeAll();

        struct list_head *list, *node;

        list_for_each_safe(list, node, &component_list1) {
            AbstractComponentImpl * delComponent = list_entry(list, class AbstractComponentImpl, i_list);

            hlist_del( &delComponent->i_hash );
            list_del( list );
            ((IComponent *) delComponent)->release();
        }
    }


    virtual lresult_t firstComponent(IComponent** ppvFirst) {
        struct list_head *list;

        list_for_each(list, &component_list1) {
            AbstractComponentImpl * pComponent = list_entry(list, class AbstractComponentImpl, i_list);
            return pComponent->query(IComponent::IID, (void**)ppvFirst);
        }
        *ppvFirst = 0;
        return lres_e_notfound;
    }


    virtual lresult_t lastComponent(IComponent ** ppvLast) {
        struct list_head *list;

        list_for_each_prev(list, &component_list1) {
            AbstractComponentImpl * pComponent = list_entry(list, class AbstractComponentImpl, i_list);
            return pComponent->query(IComponent::IID, (void**)ppvLast);
        }
        *ppvLast = 0;
        return lres_e_notfound;
    }

    virtual int getComponentInterfaceMap(iid_t interfaceIID, IComponent*** outComponents) {
        return interfaceMap.find(interfaceIID, outComponents);
    }

    virtual lresult_t prevEntity(interface IEntityMgmt* mgmt, IEntity **ppv);
    virtual lresult_t nextEntity(interface IEntityMgmt* mgmt, IEntity **ppv);

protected:
    char entityName[ENTITY_NAME_MAXLEN + 1];

    ComponentInterfaceMap interfaceMap;

    // components
    struct list_head component_list1;
    struct hlist_head component_hlist[HASHSIZE + 1];

    // entity node
    struct list_head  i_list;
    struct hlist_node i_hash;
};

}; /* namespace ecs */

#endif /* ENTITY_IMPL_H */