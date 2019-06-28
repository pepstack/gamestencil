/**
* GameStencil.h
*   The GameStencil class is the central point for creating
*     and managing your game state.
*
* Refer:
*   http://www.richardlord.net/blog/what-is-an-entity-framework
*   http://blog.csdn.net/i_dovelemon/article/details/30250049
*   http://blog.csdn.net/zhao_92221/article/details/46629553
*   http://blog.csdn.net/ubuntu64fan/article/details/8839778
*
* Init Created: 2016-06-12
* Last Updated: 2016-06-14
*/
#ifndef GAME_STENCIL_H
#define GAME_STENCIL_H

#include "interface/IGameStencil.h"

#include "SystemMgmt.h"
#include "EntityMgmt.h"


#include <memory>
using namespace std;

#define ecsGameStencilNodeName  "game-stencil"


namespace ecs {


class GameStencil :
    public IGameStencil,
    public UniversalImpl   
{
public:
    // Creation
    //
    static lresult_t getInstance(unsigned threadctx, iid_t objectIID, void **ppvOut) {
        static auto_ptr<GameStencil> singleton(new GameStencil());
        static lresult_t initresult = singleton->init(threadctx);

        if (initresult != lres_success) {
            *ppvOut = 0;
            return initresult;
        }

        if (threadctx != singleton->thread_ctx) {
            // thread_ctx cannot be changed after init
            return lres_e_argument;
        }

        if (objectIID == IEntityMgmt::IID) {
            return singleton->m_spEntityMgmt->query(IEntityMgmt::IID, (void**)ppvOut);
        }
        else if (objectIID == ISystemMgmt::IID) {
            return singleton->m_spSystemMgmt->query(ISystemMgmt::IID, (void**)ppvOut);
        }
        else if (objectIID == IGameStencil::IID) {
            return singleton->query(IGameStencil::IID, (void**)ppvOut);
        } else {
            *ppvOut = 0;
            return lres_e_notimpl;
        }
    }


    // Destructor
    virtual ~GameStencil() {
        finalRelease();

        printf("~GameStencil\n");
    }


private:

    // Constructor
    GameStencil() : updating(false) {
        printf("GameStencil\n");
    }


	// Initialization
    //
	virtual lresult_t init(unsigned threadctx) {
        lresult_t hr;
        thread_ctx = threadctx;

        hr = SystemMgmt::createInstance(threadctx, ISystemMgmt::IID, (void**)&m_spSystemMgmt);
        if (hr != lres_success) {
            return lres_e_outmemory;
        }

        hr = EntityMgmt::createInstance(threadctx, IEntityMgmt::IID, (void**)&m_spEntityMgmt);
        if (hr != lres_success) {
            return lres_e_outmemory;
        }

        return lres_success;
    }


    virtual void finalRelease() {
        ref_count = 1;
    }


    // IUniversal
    //
    virtual lresult_t query(iid_t iid, void **ppv) {
        if (iid == IUniversal::IID || iid == IGameStencil::IID) {
            *ppv = static_cast<IGameStencil *> (this);
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


    // IGameStencil
    //
    virtual lresult_t getSystemMgmt(ISystemMgmt** ppvOut) {
        return m_spSystemMgmt->query(ISystemMgmt::IID, (void**) ppvOut);
    }

    virtual lresult_t getEntityMgmt(IEntityMgmt** ppvOut) {
        return m_spEntityMgmt->query(IEntityMgmt::IID, (void**)ppvOut);
    }


    virtual lresult_t saveStencilXML(const char * stencilXmlFile);


    // Update the engine.
    // This causes the engine update loop to run, calling update on all the
    //  systems in the engine.
    virtual void update(float dt) {
        updating = true;

        m_spSystemMgmt->update(dt);        

        updating = false;

        // Call: updateCompleteEvent
    }


private:
    bool updating;

    SIPtr<ISystemMgmt> m_spSystemMgmt;
    SIPtr<IEntityMgmt> m_spEntityMgmt;
};

}; /* namespace ecs */

#endif /* GAME_STENCIL_H */