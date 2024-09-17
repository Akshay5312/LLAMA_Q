#pragma once
#include "definitions.h"
namespace LLAMA{
    namespace hgraph{
        class node{
        protected:
            Lists::AList<port> _nodeports;
            Lists::AList<port> _sources;

        public:
            node(){}

            int addPort(port*);

            port* getPort(int index){return _nodeports.getValue(index);}

            Lists::AList<arc> getConnected();

            void addSource(port*);
        };
    }
}