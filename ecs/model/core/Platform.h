/**
* Platform.h
*
*
* Init Created: 2016-06-10
* Last Updated: 2016-06-16
*/
#ifndef PLATFORM_H
#define PLATFORM_H

#if defined _MSC_VER || WIN32
    #ifndef OS_PLATFORM_WIN
        #define OS_PLATFORM_WIN
    #endif  
#endif

#ifdef OS_PLATFORM_WIN
    #include <windows.h>
    #include <process.h>
#else
    #include <pthread.h>
    #include <unistd.h>
#endif

#ifndef interface
#define interface struct
#endif


/**
* interface iid
*/
typedef unsigned int iid_t;


/**
* long result
*/
typedef long lresult_t;

#define lres_success             0
#define lres_existed            (1)
#define lres_error             (-1)
#define lres_e_pointer         (-2)
#define lres_e_argument        (-3)
#define lres_e_outmemory       (-4)
#define lres_e_notfound        (-5)
#define lres_e_notimpl         (-6)
#define lres_e_writefile       (-7)
#define lres_e_readfile        (-8)
#define lres_e_nointerface     (-11)
#define lres_e_noaggregation   (-12)


/**
* thread_ctx
*/
#define thread_ctx_single    0
#define thread_ctx_multiple  1


/**
* ref count type
*/
#ifdef OS_PLATFORM_WIN
    typedef volatile unsigned long refcount_t;
    #define __interlock_inc(thread_ctx, add) (!thread_ctx? ++add : InterlockedIncrement(&add))
    #define __interlock_dec(thread_ctx, sub) (!thread_ctx? --sub : InterlockedDecrement(&sub))
#else
    typedef volatile size_t refcount_t;
    #define __interlock_inc(thread_ctx, add) (!thread_ctx? ++add : __sync_add_and_fetch(&add, 1))
    #define __interlock_dec(thread_ctx, sub) (!thread_ctx? --sub : __sync_sub_and_fetch(&sub, 1))
#endif


#endif /* PLATFORM_H */