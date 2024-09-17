#pragma once
#include "arc.h"
#include "port.h"

namespace LLAMA{
    namespace hgraph{

        /**
         * @brief Construct a new arc between 2 ports. These ports _must_ be unpopulated.
         * 
         * @param p0 default source port
         * @param p1 default target port
         */
        arc::arc(port* p0, port* p1){
            if(p0 == nullptr){}else{p0->setArc(this);}
            if(p1 == nullptr){}else{p1->setArc(this);}

            _port0 = p0;
            _port1 = p1;
        }
        
        /**
         * @brief Get a pointer to the Ports conected by this arc
         *
         * @param whichPort which port to return (pointer to node 0 or node 1?)
         * @return port*
         */
        port* arc::getPort(bool whichPort)
        {
            if (whichPort){
                return _port1;
            }
            else{
                return _port0;
            }
        }


        node* arc::getNode(bool whichNode){
            if(whichNode){
                if(_port1 == nullptr){
                    return nullptr;
                }else{
                    return _port1->getNode();
                }
            }else{
                if(_port0 == nullptr){
                    return nullptr;
                }else{
                    return _port0->getNode();
                }
            }
        }


        /**
         * @brief Get the other port connected through this arc
         * 
         * @param thisPort the port of interest, the one which you want to get the "pair" of
         * @return port* the port connected to ||thisPort|| through the arc
         */
        port* arc::getOther(port* thisPort){
            if(_port0 == thisPort){
                return _port1;
            }
            if(_port1 == thisPort){
                return _port0;
            }

            return nullptr;
        }

        /**
         * @brief get the other node connected through this arc
         * 
         * @param thisNode the node of interest, the one which you want to get the "pair" of
         * @return node* the node connected to ||thisNode|| through the arc
         */
        node* arc::getOther(node* thisNode){
            if(_port0 == nullptr){return nullptr;}
            if(_port1 == nullptr){return nullptr;}

            node* node0 = _port0->getNode(); 
            if(node0 == thisNode){
                return _port1->getNode();
            }
            
            node* node1 = _port1->getNode(); 
            if(node1 == thisNode){
                return node0;
            }

            return nullptr;
        }

        /**
         * @brief get the source port of a directed arc
         * 
         * @return port* 
         */
        port* arc::getSource(){
            return getPort(_source);
        }

        /**
         * @brief get the target port of the directed arc
         * 
         * @return port* 
         */
        port* arc::getTarget(){
            return getPort(!_source);
        }

        void arc::setSource(){
            port* setInP = getTarget();
            if(setInP == nullptr){return;}

            node* setInN = setInP->getNode();
            if(setInN == nullptr){return;}

            setInN->addSource(setInP);
        }

    }
}