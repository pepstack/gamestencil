/**
* ComponentInterfaceMap.h
*
* Init Created: 2016-06-17
* Last Updated: 2016-06-19
*/
#ifndef COMPONENT_INTERFACE_MAP_H
#define COMPONENT_INTERFACE_MAP_H

#define RBTREE_SUPPORTS_MULTI_OBJECTS (1)
#include "../../../common/red_black_tree.h"

#include "../interface/IComponent.h"

// Add supporting components below:
//
#include "AnimateComponent.h"
#include "CollisionComponent.h"
#include "EntityTypeComponent.h"
#include "HealthComponent.h"
#include "PositionComponent.h"
#include "RenderComponent.h"
#include "VelocityComponent.h"

/**
* Component Interface Map
*    map-key    =>     map-set
*-------------------------------------------
* IHealth::IID      [ HealthComponent, ... ]
* IMoveable::IID    [ PositionComponent, VelocityComponent, ... ]
* IRenderable::IID  [ RenderComponent, ... ]
*-------------------------------------------
*/

namespace ecs {

#define NODE_OBJECTS_BLOCK 2
#define CALC_CAPACITY(capacity)   (((capacity + NODE_OBJECTS_BLOCK - 1)/NODE_OBJECTS_BLOCK) * NODE_OBJECTS_BLOCK)


struct InterfaceMapNode
{
    iid_t interfaceIID;

    union {
        struct {
            unsigned short numCapacity;
            unsigned short numComponents;
        };
        unsigned int pairNumbers;
    };
};


#define InterfaceMapNodeCheckValid(nodeObject) \
    if (! nodeObject) exit(lres_e_outmemory)


#define InterfaceMapNodeGetComponentOffset(nodeObject, index, ppComponentAddress) \
    ppComponentAddress = ((IComponent**) (((unsigned char *)(nodeObject) + sizeof(InterfaceMapNode)) + sizeof(IComponent*) * (index)))


static void InterfaceMapNodeInit(InterfaceMapNode *nodeObject, iid_t interfaceIID) {
    nodeObject->interfaceIID = interfaceIID;
    nodeObject->pairNumbers = 0;
}


static InterfaceMapNode * InterfaceMapNodeAllocObject(iid_t interfaceIID, IComponent* pComponent) {
    int capacity = CALC_CAPACITY(1);

    InterfaceMapNode * nodeObject = (InterfaceMapNode*) malloc(sizeof(InterfaceMapNode) + sizeof(IComponent*)*capacity);
    InterfaceMapNodeCheckValid(nodeObject);

    nodeObject->interfaceIID = interfaceIID;
    nodeObject->numCapacity = capacity;
    nodeObject->numComponents = 0;

    IComponent** ppComponent;
    InterfaceMapNodeGetComponentOffset(nodeObject, nodeObject->numComponents++, ppComponent);
    *ppComponent = pComponent;

    return nodeObject;
}


static void InterfaceMapNodeFreeObject(InterfaceMapNode * nodeObject) {
    free(nodeObject);
}


static InterfaceMapNode * InterfaceMapNodeAddObject(InterfaceMapNode * nodeObject, IComponent* pComponent) {
    if (nodeObject->numComponents == nodeObject->numCapacity) {
        nodeObject->numCapacity = CALC_CAPACITY(nodeObject->numCapacity + 1);
        nodeObject = (InterfaceMapNode*) realloc(nodeObject, sizeof(InterfaceMapNode) + sizeof(IComponent*)*nodeObject->numCapacity);
        InterfaceMapNodeCheckValid(nodeObject);
    }

    IComponent** ppComponent;
    InterfaceMapNodeGetComponentOffset(nodeObject, nodeObject->numComponents++, ppComponent);
    *ppComponent = pComponent;

    return nodeObject;
}


static InterfaceMapNode * InterfaceMapNodeRemoveObject(InterfaceMapNode * nodeObject, IComponent* pComponent) {
    if (! nodeObject) {
        return 0;
    }

    int i = 0;
    int j = nodeObject->numComponents - 1;

    IComponent** ppCurrComponent;
    IComponent** ppLastComponent;

    while (i < nodeObject->numComponents) {
        InterfaceMapNodeGetComponentOffset(nodeObject, i, ppCurrComponent);

        if (*ppCurrComponent == pComponent) {
            // remove it from array
            if (i != j) {
                // set i with j
                InterfaceMapNodeGetComponentOffset(nodeObject, j, ppLastComponent);

                *ppCurrComponent = *ppLastComponent;

                j = --nodeObject->numComponents - 1;
            } else {
                --nodeObject->numComponents;
                break;
            }
        } else {
            ++i;
        }
    }

    if (nodeObject->numComponents == 0) {
        free(nodeObject);
        return 0;
    }

    // resize buffer if needed
    if (nodeObject->numCapacity - nodeObject->numComponents > NODE_OBJECTS_BLOCK) {
        nodeObject->numCapacity -= NODE_OBJECTS_BLOCK;
        nodeObject = (InterfaceMapNode*) realloc(nodeObject, sizeof(InterfaceMapNode) + sizeof(IComponent*)*nodeObject->numCapacity);
        InterfaceMapNodeCheckValid(nodeObject);
    }

    return nodeObject;
}


static int InterfaceMapNodeCompareObject(InterfaceMapNode * nodeObject, IComponent* pIComponent) {
    IComponent** ppComponent;
    for (int i = 0; i < nodeObject->numComponents; ++i) {
        InterfaceMapNodeGetComponentOffset(nodeObject, i, ppComponent);
        
        if (*ppComponent == pIComponent) {
            return 0;
        }
    }
    return 1;
}


class ComponentInterfaceMap
{
public:
    ComponentInterfaceMap() {
        rbtree_init(&tree, (fn_comp_func*) nodeObjectCompare);
    }


    ~ComponentInterfaceMap() {
        removeAll();
    }


    void removeAll() {
        rbtree_traverse(&tree, (fn_oper_func*) nodeObjectTraverse, 0);
        rbtree_clean(&tree);
    }


    void remove(IComponent* delCmpnt) {
        const iid_t* piid = &AbstractComponentImpl::intefaceIIDs[0];

        while (*piid) {
            void * pif;

            if (lres_success == delCmpnt->queryNotAddRef(*piid, &pif)) {
                InterfaceMapNode tmpObject;
                InterfaceMapNodeInit(&tmpObject, *piid);

                red_black_node_t * node = rbtree_find(&tree, (void*) &tmpObject);
                if (node) {
                    node->object = (void*) InterfaceMapNodeRemoveObject((InterfaceMapNode *) node->object, delCmpnt);
                    if (! node->object) {
                        rbtree_remove_at(&tree, node);
                    }
                }
            }

            piid++;
        }
    }


    void insert(IComponent* newCmpnt) {
        const iid_t* piid = & AbstractComponentImpl::intefaceIIDs[0];

        while (*piid) {
            void * pif;

            if (lres_success == newCmpnt->queryNotAddRef(*piid, &pif)) {
                InterfaceMapNode tmpObject;
                InterfaceMapNodeInit(&tmpObject, *piid);

                red_black_node_t * node = rbtree_find(&tree, (void*) &tmpObject);
                if (node) {
                    node->object = (void*) InterfaceMapNodeAddObject((InterfaceMapNode *) node->object, newCmpnt);
                } else {
                    rbtree_insert(&tree, (void*) InterfaceMapNodeAllocObject(*piid, newCmpnt));
                }
            }

            piid++;
        }
    }


    int find(iid_t intefaceIID, IComponent*** outComponents) {
        InterfaceMapNode tmpObject;
        InterfaceMapNodeInit(&tmpObject, intefaceIID);

        red_black_node_t * node = rbtree_find(&tree, (void*) &tmpObject);

        if (node && node->object) {
            *outComponents = (IComponent**) ((unsigned char *)(node->object) + sizeof(InterfaceMapNode));
            return ((InterfaceMapNode *)node->object)->numComponents;
        }

        return 0;
    }


private:
    // typedef void (fn_oper_func)(void *object, void *param);
    static void nodeObjectTraverse(InterfaceMapNode *nodeObject, void * flag) {
        if (!flag) {
            InterfaceMapNodeFreeObject(nodeObject);
        }
    }

    // Compare function for map nodes
    static int nodeObjectCompare(InterfaceMapNode *newObject, InterfaceMapNode *nodeObject) {
        return (newObject->interfaceIID < nodeObject->interfaceIID)? -1 :
            ((newObject->interfaceIID > nodeObject->interfaceIID)? 1 : 0);
    }

    // map tree
    red_black_tree_t  tree;
};

};

#endif /* COMPONENT_INTERFACE_MAP_H */