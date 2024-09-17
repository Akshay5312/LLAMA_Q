#pragma once
#include "node.h"
#include "port.h"

namespace LLAMA
{
    namespace hgraph
    {

        /**
         * @brief
         *
         * @param toAdd
         * @return int
         */
        int node::addPort(port *toAdd)
        {
            if (toAdd == nullptr)
            {
                return -1;
            }
            if(!toAdd->nodeIsSet()){
                if (!toAdd->setNode(this))
                {
                    return -1;
                }
            }

            return _nodeports.append(toAdd);
        }

        Lists::AList<arc> node::getConnected()
        {
            Lists::AList<arc> conn;
            for (int i = 0; i < _nodeports.getSize(); i++)
            {
                // the port in this node to analyze. If a port does not exist, don't add it.
                port *analPort = _nodeports.getValue(i);
                if (analPort == nullptr){ continue; }

                // the connected arc to append
                arc *toApp = analPort->getArc();
                if (toApp == nullptr){ continue;}

                conn.append(toApp);
            }

            return conn;
        }

        void node::addSource(port* src){
            _sources.append(src);
                  
        }


    }
}
