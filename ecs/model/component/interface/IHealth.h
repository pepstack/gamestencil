/**
* IHealth.h
*
* Author: master@pepstack.com
*
* Init Created: 2016-06-15
* Last Updated: 2016-06-15
*/
#ifndef IHEALTH_H
#define IHEALTH_H

#include "../../core/SIPtr.h"

namespace ecs {

// NOT COM interface.
//  Using queryNotAddref for it!

interface IHealth
{
    static const iid_t IID = ((iid_t) 0x1F000002);

    virtual void setHealth(int v) = 0;

    virtual int getHealth(void) = 0;
};

}; /* namespace ecs */

#endif /* IMOVEABLE_H */
