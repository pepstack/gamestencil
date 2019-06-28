/**
* SIPtr.h
*    Smart Interface Pointer
*
* Use: SIPtr<IX> spIX;
*     Do not use with IUniversal; SIPtr<IUniversal>
*       will not compile.  Instead, use IUniversalPtr. 
*
* Refer:
*   <<Inside COM>>
*
* Init Created: 2016-06-10
* Last Updated: 2016-06-10
*/
#ifndef SIPTR_H
#define SIPTR_H

#include "Universal.h"

#include <assert.h>

typedef struct {
    void *pv;
} void_type;


template <class T> class SIPtr
{
public:

    // Constructors
    SIPtr() {
        m_pI = 0;
    }


    SIPtr(T* lp) {
        m_pI = lp;
        if ( m_pI ) {
            m_pI->retain();
        }
    }


    SIPtr(IUniversal* pI) {
        m_pI = 0;
        if ( pI ) {
            pI->query(T::IID, (void **) & m_pI);
        }
    }


    // Destructor
    ~SIPtr() {
        release();
    }


    // Reset
    void release() {
        if ( m_pI ) {
            T* pOld = m_pI;
            m_pI = 0;
            pOld->release();
        }
    }


    // Attach to an existing interface (does not retain)
    void attach(T * pI) {
        if (m_pI != pI) {
            IUniversal* pOld = m_pI;

            m_pI = pI;

            if (pOld) {
                // Release the old interface
                pOld->release();
            }
        }
    }


    // Detach the interface (does not release)
    T* detach() {
        T* pOld = m_pI;
        m_pI = 0;
        return pOld;
    }

    T* get() {
        return m_pI;
    }

    // Conversion
    operator T*() {
        return m_pI;
    }


    // Pointer operations
    T& operator*() {
        assert(m_pI);
        return * m_pI;
    }


    T** operator&() {
        assert(!m_pI);
        return &m_pI;
    }


    T* operator->() {
        assert(m_pI);
        return m_pI;
    }


    // Assignment from the same interface
    T* operator=(T* pI) {
        if (m_pI != pI) {
            // Save current value
            IUniversal* pOld = (IUniversal *) m_pI;

            // Assign new value
            m_pI = pI;

            if (m_pI) {
                m_pI->retain();
            }

            if (pOld) {
                // Release the old interface
                pOld->release();
            }
        }
        return m_pI;
    }


    // Assignment from another interface
    T* operator=(IUniversal* pI) {
        // Save current value
        IUniversal* pOld = m_pI;
        m_pI = 0;

        // Query for correct interface
        if ( pI ) {
            lresult_t hr = pI->query(T::iid_interface, (void**) & m_pI);
            assert(hr == lres_success && m_pI);
        }

        if ( pOld ) {
            // Release old pointer
            pOld->release();
        } 
        return m_pI ; 
    } 

    // bool functions 
    bool operator!() {
        return m_pI ? false : true;
    }


    // Requires a compiler that supports BOOL
    operator bool() const {
        return m_pI ? true : false;
    }


    // Interface ID
    iid_t iid() {
        return T::IID;
    } 


private: 
    // Pointer variable 
    T* m_pI;
};


/**
* IUniversalPtr is a smart interface for IUniversal
*/
class IUniversalPtr
{
public:
    // Constructors
    IUniversalPtr() {
        m_pI = 0;
    }


    IUniversalPtr(IUniversal* lp) {
        m_pI = lp;
        if ( m_pI ) {
            m_pI->retain();
        }
    }


    // Destructor
    ~IUniversalPtr() {
        release();
    }


    // Reset
    void release() {
        if (m_pI) {
            IUniversal* pOld = m_pI;
            m_pI = 0;
            pOld->release();
        }
    }


    // Conversion
    operator IUniversal*() {
        return (IUniversal*) m_pI;
    }


    // Pointer operations
    IUniversal& operator*() {
        assert(m_pI);
        return *m_pI;
    }


    IUniversal** operator&() {
        assert(!m_pI);
        return &m_pI;
    }


    IUniversal* operator->() {
        assert(m_pI);
        return m_pI;
    }


    // Assignment
    IUniversal* operator=(IUniversal* pI) {
        if (m_pI != pI) {
            // Save current value
            IUniversal* pOld = m_pI;

            // Assign new value
            m_pI = pI;

            if ( m_pI ) {
                m_pI->retain();
            }

            if ( pOld ) {
                // Release the old interface
                pOld->release();
            }
        }
        return m_pI;
    }


    // Boolean functions 
    bool operator!() {
        return m_pI ? false : true;
    }


    operator bool() const {
        return m_pI ? true : false;
    }


private:
    // Pointer variable
    IUniversal* m_pI;
}; 

#endif /* SIPTR_H */
