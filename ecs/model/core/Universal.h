/**
* Universal.h
* 
* Refer:
*   <<Inside COM>>
*
* Init Created: 2016-06-10
* Last Updated: 2016-06-16
*/
#ifndef UNIVERSAL_H
#define UNIVERSAL_H

#include "Platform.h"


// IUniversal is a variant from IUnknown
//
interface IUniversal
{
    static const iid_t IID = ((iid_t) (0));

    virtual lresult_t query(iid_t iid, void **ppvOut) = 0;
    virtual unsigned long retain(void) = 0;
    virtual unsigned long release(void) = 0;
};

#define iid_IUniversal  (IUniversal::IID)


class UniversalImpl
{
protected:
    refcount_t  ref_count;
    unsigned  thread_ctx;

public:
    UniversalImpl() :
        ref_count(1),
        thread_ctx(thread_ctx_multiple) {
    }

    virtual ~UniversalImpl() {
    }


    void init(unsigned threadctx) {
        thread_ctx = threadctx;
    }


    // Notification to derived classes that we are releasing 
    void finalRelease() {
        // Increment reference count for final release
        ref_count = 1;
    }


    unsigned getThreadCtx() const {
        return thread_ctx;
    }


    // IUniversal
    //
    virtual lresult_t query(iid_t iid, void **ppv) = 0;


    virtual unsigned long retain(void) {
        return __interlock_inc(thread_ctx, ref_count);
    }


    virtual unsigned long release(void) {
        if (__interlock_dec(thread_ctx, ref_count) == 0) {
            delete this;
            return 0;
        }
        return ref_count;
    }
};


#define CREATE_INSTANCE_FUNCTION(className) \
    static lresult_t createInstance( \
            unsigned threadctx,\
            iid_t iid,\
            void **ppv) {\
        className * p = new className();\
        if ( ! p) {\
            return lres_e_outmemory;\
        }\
        lresult_t hr = p->init(threadctx);\
        if (hr != lres_success) {\
            p->release();\
            return hr;\
        }\
        hr = p->query(iid, ppv);\
        p->release();\
        return hr;\
    }


#endif /* UNIVERSAL_H */