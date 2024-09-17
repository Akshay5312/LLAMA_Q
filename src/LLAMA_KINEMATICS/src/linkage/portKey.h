#pragma once

#include "../hypergraph/hypergraph.h"

namespace LLAMA{
    namespace LINKAGE{
        class Link;

        class portKey : public hgraph::port{
            private:
                GEOM::SpatialT _inLink = GEOM::I;
            public:
                portKey():hgraph::port(){}
                portKey(Link* src, GEOM::SpatialT inLink = GEOM::I):hgraph::port((hgraph::node*)src){
                    _inLink = inLink;
                    }

                GEOM::SpatialT getTransformation(){return _inLink;}

                GEOM::SpatialT getInGround();
                
                GEOM::SpatialT getTargetInSource();

        };
    }
}