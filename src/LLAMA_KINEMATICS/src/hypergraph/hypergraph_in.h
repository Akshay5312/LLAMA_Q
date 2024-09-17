#pragma once

#include "../../Ldeps/Ldeps.h"
#include "node.h"
#include "port.h"
#include "arc.h"

namespace LLAMA{
    namespace hgraph{
        class hypergraph : public node{
            protected:
                node* _head = nullptr;

                Lists::AList<node> _nodes;
                Lists::AList<arc> _arcs;
                Lists::AList<port> _exports;

            public:
            
                hypergraph() : node(){}
                
                int addNode(node*);

                node* getNode(int index){return _nodes.getValue(index);}

                int addArc(int node0Index, int node1Index);

                int addArc(node* node0, node* node1);

                int addArc(port* port0, port* port1);

                int addArc(arc* a){return _arcs.append(a);}

                bool setHead(int headIndex);

                bool propogate();
        };
    }
}

