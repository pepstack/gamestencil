/**
* IRenderable.h
*
* Author: master@pepstack.com
*
* Init Created: 2016-06-15
* Last Updated: 2016-06-15
*/
#ifndef IRENDERABLE_H
#define IRENDERABLE_H

#include "../../core/SIPtr.h"

namespace ecs {

// NOT COM interface.
//  Using queryNotAddref for it!

interface IRenderable
{
    static const iid_t IID = ((iid_t) 0x1F000003);

    virtual void render(float dt) = 0;
};

}; /* namespace ecs */

#endif /* IRENDERABLE_H */
