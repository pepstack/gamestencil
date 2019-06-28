///////////////////////////////////////////////////////////////////////
// main.cpp
// 2016-06-16
///////////////////////////////////////////////////////////////////////
#ifdef WIN32
    // Refer:
    //   http ://www.cnblogs.com/skynet/archive/2011/02/20/1959162.html
    #define _CRTDBG_MAP_ALLOC
    #include <stdlib.h>
    #include <crtdbg.h>
#else
    #include <stdlib.h>
#endif

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "model/GameStencil.h"

using namespace ecs;

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4996)
#endif


void testSystemMgmt()
{
    printf("======== testSystemMgmt Start ========\n");

    lresult_t hr;

    // ������Ϸģ��
    SIPtr<IGameStencil> spGame;
    hr = GameStencil::getInstance(thread_ctx_single, spGame.iid(), (void**) &spGame);
    assert(hr == lres_success);

    // ȡ����Ϸϵͳ������
    SIPtr<ISystemMgmt> spSystemMgmt;
    spGame->getSystemMgmt(&spSystemMgmt);

    // ����һ������ϵͳ: InputSystem
    SIPtr<ISystem> spInputSystem;
    hr = spSystemMgmt->createSystem(InputSystem::IID, InputSystem::PRIORITY, &spInputSystem);
    assert(hr == lres_success);

    // ������һ������ϵͳ: InputSystem2
    SIPtr<ISystem> spInputSystem2;
    hr = spSystemMgmt->createSystem(InputSystem::IID, InputSystem::PRIORITY, &spInputSystem2);
    
    // ϵͳ������: һ��ϵͳ(IID)ֻ����һ��ʵ��
    assert(spInputSystem.get() == spInputSystem2.get());

    // ϵͳ������: ����ϵͳ����IIDɾ��ϵͳ
    hr = spSystemMgmt->removeSystem(spInputSystem->getSystemIID());
    assert(hr == lres_success);

    // ϵͳ������: ɾ��ȫ��ϵͳ
    spSystemMgmt->removeAllSystems();

    // ɾ��һ�������ڵ�ϵͳ
    hr = spSystemMgmt->removeSystem(spInputSystem->getSystemIID());
    assert(hr == lres_e_notfound);

    spGame->saveStencilXML("c:/temp/game-stencil-01.xml");

    printf("======== testSystemMgmt End ========\n");
}



void testSystemMgmt2()
{
    printf("======== testSystemMgmt2 Start ========\n");

    lresult_t hr;

    // ������Ϸģ��(����)
    SIPtr<IGameStencil> spGame;
    hr = GameStencil::getInstance(thread_ctx_single, spGame.iid(), (void**) &spGame);
    assert(hr == lres_success);

    // ȡ����Ϸϵͳ������(����)
    SIPtr<ISystemMgmt> spSystemMgmt;
    spGame->getSystemMgmt(&spSystemMgmt);

    // ����ϵͳ: InputSystem
    SIPtr<ISystem> spInputSystem;
    hr = spSystemMgmt->createSystem(InputSystem::IID, InputSystem::PRIORITY, &spInputSystem);
    assert(hr == lres_success);

    // ����ϵͳ: AnimateSystem
    SIPtr<ISystem> spAnimateSystem;
    hr = spSystemMgmt->createSystem(AnimateSystem::IID, AnimateSystem::PRIORITY, &spAnimateSystem);
    assert(hr == lres_success);

    // ����ϵͳ: RenderSystem
    SIPtr<ISystem> spRenderSystem;
    hr = spSystemMgmt->createSystem(RenderSystem::IID, RenderSystem::PRIORITY, &spRenderSystem);
    assert(hr == lres_success);

    // ����ϵͳ: CollisionDetectSystem
    SIPtr<ISystem> spCollisionDetectSystem;
    hr = spSystemMgmt->createSystem(CollisionDetectSystem::IID, CollisionDetectSystem::PRIORITY, &spCollisionDetectSystem);
    assert(hr == lres_success);

    // ����ϵͳ: HealthSystem
    SIPtr<ISystem> spHealthSystem;
    hr = spSystemMgmt->createSystem(HealthSystem::IID, HealthSystem::PRIORITY, &spHealthSystem);
    assert(hr == lres_success);

    // ����ϵͳ: MovementSystem
    SIPtr<ISystem> spMovementSystem;
    hr = spSystemMgmt->createSystem(MovementSystem::IID, MovementSystem::PRIORITY, &spMovementSystem);
    assert(hr == lres_success);

    // ����ϵͳ: BoundaryCheckSystem
    SIPtr<ISystem> spBoundaryCheckSystem;
    hr = spSystemMgmt->createSystem(BoundaryCheckSystem::IID, BoundaryCheckSystem::PRIORITY, &spBoundaryCheckSystem);
    assert(hr == lres_success);

    // ����ϵͳ: CreatorSystem
    SIPtr<ISystem> spCreatorSystem;
    hr = spSystemMgmt->createSystem(CreatorSystem::IID, CreatorSystem::PRIORITY, &spCreatorSystem);
    assert(hr == lres_success);

    spGame->update(0.1f);

    do {
        // �������ȫ��ϵͳ: 0,1,2,3,...
        printf("traverse all systems:\n");
        ISystem * pISystem;
        if (lres_success == spSystemMgmt->firstSystem(&pISystem)) {
            printf("----> use system(%d): %s\n", pISystem->getPriority(), pISystem->getSystemName());

            ISystem * pISystemNext;
            while (lres_success == pISystem->nextSystem(spSystemMgmt, &pISystemNext)) {
                printf("----> use system(%d): %s\n", pISystemNext->getPriority(), pISystemNext->getSystemName());

                pISystem->release();
                pISystem = pISystemNext;
            }

            pISystem->release();
        }
    } while (0);


    do {
        // �������ȫ��ϵͳ: n, n-1, ..., 3, 2, 1
        printf("reverse traverse all systems:\n");
        ISystem * pISystem;
        if (lres_success == spSystemMgmt->lastSystem(&pISystem)) {
            printf("----> use system(%d): %s\n", pISystem->getPriority(), pISystem->getSystemName());

            ISystem * pISystemPrev;
            while (lres_success == pISystem->prevSystem(spSystemMgmt, &pISystemPrev)) {
                printf("----> use system(%d): %s\n", pISystemPrev->getPriority(), pISystemPrev->getSystemName());

                pISystem->release();
                pISystem = pISystemPrev;
            }

            pISystem->release();
        }
    } while (0);

    spGame->saveStencilXML("c:/temp/game-stencil-02.xml");

    printf("======== testSystemMgmt2 End ========\n");
}


void testEntityMgmt()
{
    printf("======== testEntityMgmt Start ========\n");

    lresult_t hr;
    int ENTITY_NUM = 10;

    // ������Ϸģ��(����)
    SIPtr<IGameStencil> spGame;
    hr = GameStencil::getInstance(thread_ctx_single, spGame.iid(), (void**) &spGame);
    assert(hr == lres_success);

    SIPtr<IGameStencil> spGame2;
    hr = GameStencil::getInstance(thread_ctx_single, spGame2.iid(), (void**) &spGame2);
    assert(hr == lres_success);
    
    // ��֤��Ϸģ�����Ϊ����
    assert(spGame.get() == spGame2.get());

    // ȡ����Ϸʵ�������(����)
    SIPtr<IEntityMgmt> spEntityMgmt;
    spGame->getEntityMgmt(&spEntityMgmt);

    // ���Ӵ���ʵ��
    char buf[20];
    for (int i = 0; i < ENTITY_NUM; ++i) {
        // (IEntity *) much fast than SIPtr<IEntity>
        IEntity * pIEntity;

        sprintf(buf, "%d", i+1);
        hr = spEntityMgmt->createEntity(buf, &pIEntity);
        assert(hr == lres_success);

        // must release after used
        pIEntity->release();
    }

    spGame->update(0.1f);

    spGame->saveStencilXML("c:/temp/game-stencil-03.xml");

    printf("======== testEntityMgmt End ========\n");
}


void testComponents()
{
    lresult_t hr;

    // ������Ϸģ��(����)
    SIPtr<IGameStencil> spGame;
    hr = GameStencil::getInstance(thread_ctx_single, spGame.iid(), (void**) &spGame);
    assert(hr == lres_success);

    // ȡ����Ϸʵ�������
    SIPtr<IEntityMgmt> spEntityMgmt;
    spGame->getEntityMgmt(&spEntityMgmt);

    // ���һ��ʵ��
    SIPtr<IEntity> spRedEntity;
    spEntityMgmt->createEntity("RedSprite", & spRedEntity);

    // ����ʵ������
    printf("* entity created: %s\n", spRedEntity->getEntityName(0));

    // Ϊʵ��������
    do {
        // (IComponent *) much fast than SIPtr<IComponent>
        SIPtr<IComponent> spComponent;
        hr = spRedEntity->createComponent(EntityTypeComponent::IID, &spComponent);
        assert(hr == lres_success);
        printf("    added component(iid=%d): %s\n", spComponent->getComponentIID(), spComponent->getComponentName());

        // ������: PositionComponent
        spComponent.release();
        hr = spRedEntity->createComponent(PositionComponent::IID, &spComponent);
        assert(hr == lres_success);

        // ɾ�����
        spRedEntity->removeComponent(PositionComponent::IID);

        // �ٴ�������
        spComponent.release();
        hr = spRedEntity->createComponent(PositionComponent::IID, &spComponent);
        assert(hr == lres_success);

        IMoveable * pMoveable;
        hr = spComponent->queryNotAddRef(IMoveable::IID, (void**)&pMoveable);
        if (hr == lres_success) {
            pMoveable->setPosition(100, 200);
        }

        // ������
        spComponent.release();
        hr = spRedEntity->createComponent(VelocityComponent::IID, &spComponent);
        assert(hr == lres_success);

        // ������: HealthComponent
        spComponent.release();
        hr = spRedEntity->createComponent(HealthComponent::IID, &spComponent);
        assert(hr == lres_success);

        IHealth * pHealth;
        hr = spComponent->queryNotAddRef(IHealth::IID, (void**)&pHealth);
        if (hr == lres_success) {
            pHealth->setHealth(50);
        }
    } while (0);

    spGame->update(0.1f);

    spGame->saveStencilXML("c:/temp/game-stencil-04.xml");
}


///////////////////////////////////////////////////////////////////////
// ���� GameStencil ��Ϸģ��ϵͳ
//
// interfaceIID=>IComponent
//
int main()
{
#ifdef _CRTDBG_MAP_ALLOC
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    printf("******** main.cpp start\n");

    do {
        testSystemMgmt();

        testSystemMgmt2();

        testEntityMgmt();

        testComponents();
    } while (0);

    printf("******** main.cpp exit.\n");

    system("PAUSE");

    return 0;
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif