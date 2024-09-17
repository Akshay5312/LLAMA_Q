#pragma once

#include "../hypergraph/hypergraph.h"

#include "portKey.h"

namespace LLAMA{
    namespace LINKAGE{
        class Link : public hgraph::node{
            private:
                //the frames of interest in each link -- the key frames
                Lists::AList<portKey> _keys;
            public:
                Link():hgraph::node(){}

                // GEOM::SpatialT getInSource();

                int addKey(GEOM::SpatialT inLink);

                portKey* getKey(int);

                GEOM::SpatialT getInGround();
        };

        
        int Link::addKey(GEOM::SpatialT inLink){
            portKey* kFrame = new portKey(this,inLink);
            return _keys.append(kFrame);
        }
        
        portKey* Link::getKey(int i){
            return _keys.getValue(i);
        }

        GEOM::SpatialT Link::getInGround(){
            //find parent arc
            //get this in parent

            portKey* parent = (portKey*)_sources.getValue(0);
            if(parent == nullptr){
                return GEOM::I;
            }

            Link* otherLink = (Link*) parent->getOtherNode();
            GEOM::SpatialT otherLinkInGnd = GEOM::I;
            if(otherLink == nullptr){
                return otherLinkInGnd;
            }

            otherLinkInGnd = otherLink->getInGround();

            return otherLinkInGnd*parent->getTargetInSource();
        }

    }
}