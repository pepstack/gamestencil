/**
* EntityMgmt.h
*
* Refer:
*   http://www.richardlord.net/blog/what-is-an-entity-framework
*   http://blog.csdn.net/i_dovelemon/article/details/30250049
*   http://blog.csdn.net/zhao_92221/article/details/46629553
*   http://blog.csdn.net/ubuntu64fan/article/details/8839778
*
* Init Created: 2016-06-15
* Last Updated: 2016-06-16
*/
#ifndef ENTITY_MGMT_H
#define ENTITY_MGMT_H

#include "interface/IEntityMgmt.h"

#include "entity/EntityImpl.h"

#include "../../common/hashmap.h"
#include "../../common/dhlist.h"

#include <memory>
#include <vector>
using namespace std;

#include "../../thirdparty/mxml-2.9/mxml.h"
#define ecsEntityListNodeName  "entity-bundle"
#define ecsEntityNodeName  "entity"


namespace ecs {


class EntityMgmt :
    public UniversalImpl,
    public IEntityMgmt

{
    friend class GameStencil;
    friend class EntityImpl;

private:

    // Creation
    CREATE_INSTANCE_FUNCTION(EntityMgmt)

    static const int HASHSIZE = 0x0FFF;

    int gethash(IEntity * pIEntity) {
        return get_int_hash((void*) pIEntity, EntityMgmt::HASHSIZE);
    }


    // Constructor 
    EntityMgmt() {
        printf("EntityMgmt\n");
    }


    // Destructor
    virtual ~EntityMgmt() {
        finalRelease();
        printf("~EntityMgmt\n");
    }


    virtual lresult_t init(unsigned threadctx) {
        thread_ctx = threadctx;

        INIT_LIST_HEAD(&list1);
        for (int hash = 0; hash <= HASHSIZE; hash++) {
            INIT_HLIST_HEAD(&hlist[hash]);
        }

        return lres_success;
    }


    virtual void finalRelease() {
        ref_count = 1;

        // release all IEntity * in list
        removeAllEntities();
    }

    static int hmap_free_entity(IEntity * entity, mxml_node_t * entity_list);

    virtual lresult_t updateStencilXML(mxml_node_t *parent);


    // IUniversal
    //
    virtual lresult_t query(iid_t iid, void **ppv) {
        if (iid == IUniversal::IID || iid == IEntityMgmt::IID) {
            *ppv = static_cast<IEntityMgmt *> (this);
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

#ifdef _MSC_VER
    #pragma warning(push)
    #pragma warning(disable : 4996)
#endif
    void insertEntity(const char *name, IEntity* pIEntity) {
        EntityImpl * entity = (EntityImpl *) pIEntity;

        if (name) {
            if (strstr(name, ENTITY_NAME_PREFIX)) {
                strncpy(entity->entityName, name, ENTITY_NAME_MAXLEN);
            } else {
                strncpy(entity->entityName + ENTITY_NAME_PREFIX_LEN, name,
                    ENTITY_NAME_MAXLEN - ENTITY_NAME_PREFIX_LEN);
            }
            entity->entityName[ENTITY_NAME_MAXLEN] = 0;
        }

        list_add_tail(&entity->i_list, &list1);
        hlist_add_head(&entity->i_hash, &hlist[gethash(pIEntity)]);
    }
#ifdef _MSC_VER
    #pragma warning(pop)
#endif

    // IEntityMgmt
    //
    virtual lresult_t createEntity(const char * entityName, IEntity **ppvOut) {
        IEntity * pIEntity = 0;
        lresult_t hr = EntityImpl::createInstance(thread_ctx, EntityImpl::IID, (void**) &pIEntity);
        if (hr != lres_success) {
            return hr;
        }

        hr = pIEntity->query(IEntity::IID, (void**)ppvOut);
        if (hr != lres_success) {
            pIEntity->release();
            return hr;
        }
        insertEntity(entityName, pIEntity);
        return lres_success;
    }


    virtual lresult_t removeEntity(IEntity *entity) {
        if (entity) {
            IEntity * pIEntityDel = 0;
            if (lres_success == entity->query(EntityImpl::IID, (void**)&pIEntityDel)) {
                EntityImpl * delEntity = (EntityImpl *) pIEntityDel;

                hlist_del(&delEntity->i_hash);
                list_del(&delEntity->i_list);
                pIEntityDel->release();

                pIEntityDel->release();
                return lres_success;
            }
            else {
                return lres_e_nointerface;
            }
        }
        else {
            return lres_e_pointer;
        }
    }


    virtual void removeAllEntities(void) {
        struct list_head *list, *node;

        list_for_each_safe(list, node, &list1) {
            EntityImpl * delEntity = list_entry(list, class EntityImpl, i_list);

            hlist_del(&delEntity->i_hash);
            list_del(list);
            ((IEntity *)delEntity)->release();
        }
    }


    virtual lresult_t firstEntity(IEntity** ppvFirst) {
        struct list_head *list;
        list_for_each(list, &list1) {
            EntityImpl * pEntity = list_entry(list, class EntityImpl, i_list);
            return pEntity->query(IEntity::IID, (void**)ppvFirst);
        }
        *ppvFirst = 0;
        return lres_e_notfound;
    }


    virtual lresult_t lastEntity(IEntity ** ppvLast) {
        struct list_head *list;
        list_for_each_prev(list, &list1) {
            EntityImpl * pEntity = list_entry(list, class EntityImpl, i_list);
            return pEntity->query(IEntity::IID, (void**)ppvLast);
        }
        *ppvLast = 0;
        return lres_e_notfound;
    }


protected:

    struct list_head list1;
    struct hlist_head hlist[HASHSIZE + 1];
};

}; /* namespace ecs */

#endif /* ENTITY_MGMT_H */
