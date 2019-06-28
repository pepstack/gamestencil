/**
* IMoveable.h
*
* Author: master@pepstack.com
*
* Init Created: 2016-06-15
* Last Updated: 2016-06-15
*/
#ifndef IMOVEABLE_H
#define IMOVEABLE_H

#include "../../core/SIPtr.h"

namespace ecs {

// NOT COM interface.
//  Using queryNotAddref for it!

interface IMoveable
{
    static const iid_t IID = ((iid_t) 0x1F000001);

    virtual void setPosition(float x, float y) = 0;
};

}; /* namespace ecs */

#endif /* IMOVEABLE_H */
