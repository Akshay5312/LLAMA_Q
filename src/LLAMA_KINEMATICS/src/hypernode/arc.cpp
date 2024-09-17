#pragma once
#include "arc.h"
#include "hypernode_.h"

namespace LLAMA
{
    namespace hgraph
    {
            arc::arc(){}
            arc::arc(hypernode * source , hypernode * target){
                if((source == nullptr) || (target == nullptr)){return;}

                _node0 = source;
                _node1 = target;
                source->addOuterArc(this);
                target->addOuterArc(this);   
            }
            
            hypernode* arc::getNode(bool toGet){
                if(toGet){return _node1;}else{return _node0;}
            }

            //gets the other connected hypernode _at the same level__ as the passed hypernode 
            hypernode* arc::getOther(hypernode* toGetOtherOf){
                //go up a level on each till you find the one to get other of

                if(toGetOtherOf == _node0){return _node1;}
                if(toGetOtherOf == _node1){return _node0;}
                hypernode* containerLevel = toGetOtherOf->getContainerAbove();
                return getOther(toGetOtherOf, containerLevel);
                
            }

            //get the other hypernode at a specified level. 
            hypernode* arc::getOther(hypernode* toGetOtherOf, hypernode* containerLevel){
                hypernode* nodeToLookFor = _node0;
                // if(containerLevel == nullptr){}
                if(!toGetOtherOf->contains(_node0)){
                    if(!toGetOtherOf->contains(_node1)){
                        //the supplied node is not connected to this arc
                        return nullptr;
                    }
                }else{
                    nodeToLookFor = _node1;
                }

                if(containerLevel == nullptr){
                    return nodeToLookFor;
                }
                
                hypernode* retval = nodeToLookFor->getContainerUnder(containerLevel);
                // //get the nodes directly under the container
                // Lists::AList<hypernode> subnodes = containerLevel->getSubNodes();

                // for(int i = 0; i < subnodes.getSize(); i++){
                //     hypernode* currNode = subnodes.getValue(i);
                //     if(currNode == nullptr){continue;}
                //     if(currNode->contains(nodeToLookFor)){
                //         return currNode;
                //     }
                // }

                return retval;
            }
            
            hypernode* arc::getSource(){
                if(_source){return _node1;}else{return _node0;}
            }
            hypernode* arc::getTarget(){
                if(!_source){return _node1;}else{return _node0;}
            }


            // void arc::setSource(bool targetSource){
            //     _source = targetSource;
            //     set();
            // }

            void arc::setSource(hypernode* targetSource){
                hypernode* target = getTarget();
                if(targetSource->contains(target)){
                    flip();
                }
                set();
            }

            void arc::addContainer(hypernode* toAdd){
                _containers.append(toAdd);
            }

            //checks if a node is in the target path or source path
            bool arc::isInTarget(hypernode* toCheck){
                return getTarget()->contains(toCheck);
            }
    }
}