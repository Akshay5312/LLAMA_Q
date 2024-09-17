#pragma once
#include "definitions.h"

namespace LLAMA{
    namespace hgraph{
        class hypernode{
            private:
                //head hypernode
                hypernode* _head = nullptr;

                hypernode* _toplevel = nullptr;

                //all base nodes added
                Lists::AList<hypernode> _basenodes;
                //all consolidated nodes under this 
                Lists::AList<hypernode> _consolidatedNodes;
                //containers this is in in ascending order -- ending with this
                Lists::AList<hypernode> _containerNodes;

                //all outer arcs
                Lists::AList<arc> _outerArcs;
                //parent arcs
                Lists::AList<arc> _parentArcs;
                
                
                //consolidate subnodes into a consolidated node. It will be a new node under "this". 
                hypernode* consolidate(Lists::AList<hypernode>);
                
            public:
                hypernode(){
                    _containerNodes.append(this);
                    _toplevel = this;
                }

                hypernode(Lists::AList<hypernode> subnodes, Lists::AList<hypernode> containerNodes, Lists::AList<arc> inarcs){
                    // 
                }
                
                //add a base node
                int addBaseNode();

                //add a new subnode
                int addNode(hypernode*);

                //set head
                void setHead(int);

                //set head by pointer
                void setHead(hypernode*);


                void computeParents();

                hypernode* getParent();

                //add an arc between base nodes
                void addArc(int, int);

                //add an arc between this node and another node
                void addOuterArc(arc*);

                //is the "tocheck" node in this hypernode
                bool contains(hypernode* toCheck);

                //set arc directions with a head seed
                void propogate(hypernode*);
                void propogate(){propogate(_head);}

                //get the outward facing arcs with 
                Lists::AList<arc> getChildArcs();

                //get the containernode this is under at the level under "getUnder" 
                hypernode* getContainerUnder(hypernode*);

                hypernode* getContainerAbove();

                Lists::AList<hypernode> getToGround(int);

                void addTopLevel(hypernode* topNode){_toplevel = topNode;}
        };
    }
}