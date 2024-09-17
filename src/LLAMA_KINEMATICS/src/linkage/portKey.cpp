#pragma once

#include "portKey.h"
#include "joint.h"
#include "link.h"

namespace LLAMA
{
    namespace LINKAGE
    {
        GEOM::SpatialT portKey::getInGround()
        {
            Link *parent = (Link *)getNode();

            if (parent == nullptr)
            {
                return _inLink;
            }

            return parent->getInGround() * _inLink;
        }

        GEOM::SpatialT portKey::getTargetInSource()
        {
            if (_route == nullptr)
            {
                return GEOM::I;
            }

            return ((Joint *)_route)->getTargetInSource();
        }
    }
}
