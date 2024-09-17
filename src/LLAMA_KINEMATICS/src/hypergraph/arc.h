#pragma once
#include "definitions.h"

namespace LLAMA
{
    namespace hgraph
    {
        class arc
        {
        protected:
            port *_port0;
            port *_port1;

            bool _source = 0;

        public:
            arc();
            arc(port *, port *);
            
            port* getPort(bool);
            node* getNode(bool);


            port* getOther(port*);
            node* getOther(node*);

            port* getSource();
            port* getTarget();

            //flip the source
            void flip(){_source = !_source;}

            void setSource();
        };
    }
}