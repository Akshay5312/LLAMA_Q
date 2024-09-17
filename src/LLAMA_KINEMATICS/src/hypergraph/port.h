#pragma once
#include "definitions.h"

namespace LLAMA{
    namespace hgraph{
        class port{
        protected:
            // the node this port is in
            node* _nodeIn = nullptr;
            
            // the connected arc
            arc* _route = nullptr;

        public:
            port() {}
            port(node*);

            bool setNode(node*);
            bool setArc(arc* toSet);

            node* getNode() { return _nodeIn; }
            arc* getArc() { return _route; }

            node* getOtherNode();
            port* getOtherPort();

            bool nodeIsSet(){return !(_nodeIn == nullptr);}
        };
    }
}
        