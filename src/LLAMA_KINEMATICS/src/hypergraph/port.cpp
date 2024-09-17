#pragma once
#include "port.h"
#include "node.h"
#include "arc.h"

namespace LLAMA{
    namespace hgraph{
        /**
         * @brief Construct a new port with a parent node
         * 
         * @param nodeIn the parent node
         */
        port::port(node* nodeIn){
            _nodeIn = nodeIn;
            if(_nodeIn == nullptr){
                return;
            }
            nodeIn->addPort(this);
        }

        /**
         * @brief add a node to the port _ONLY IF_ there is no node already connected
         * 
         * @param toAdd the node to set as parent
         * @return true 
         * @return false 
         */
        bool port::setNode(node* toSet){
            if(toSet == nullptr){
                return false;
            }
            if(_nodeIn == nullptr){
                return false;
            }

            _nodeIn = toSet;
            _nodeIn->addPort(this);

            return true;
        }

        /**
         * @brief Set the Arc.
         * 
         * @param toSet 
         * @return true 
         * @return false 
         */
        bool port::setArc(arc* toSet){
            if(toSet == nullptr){
                return false;
            }
            _route = toSet;
            return true;
        }
        
        node* port::getOtherNode(){
            if(_route == nullptr){
                return nullptr;
            }
            node* otherNode = _route->getOther(_nodeIn);

            return otherNode;
        }

        port* port::getOtherPort(){
            if(_route == nullptr){
                return nullptr;
            }
            port* otherPort = _route->getOther(this);

            return otherPort;
        }
    }
}