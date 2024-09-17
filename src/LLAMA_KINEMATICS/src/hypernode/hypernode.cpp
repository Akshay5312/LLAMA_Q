#pragma once

#include "hypernode_.h"
#include "arc.h"

namespace LLAMA{
    namespace hgraph{
        //add a base node
        int hypernode::addBaseNode(){
            hypernode* newNode = new hypernode();
            newNode->addTopLevel(_toplevel);
            return _basenodes.append(newNode);
        }

        void hypernode::setHead(int index){_head = _basenodes.getValue(index);}
        
        void hypernode::setHead(hypernode* head){_head = head;}
        
        //add an arc between base nodes
        void hypernode::addArc(int sourceIndex, int targetIndex){
            hypernode* srcNode = _basenodes.getValue(sourceIndex);
            hypernode* trgNode = _basenodes.getValue(targetIndex);

            if((srcNode == nullptr) || (trgNode == nullptr)){return;}

            arc* appArc = new arc(srcNode, trgNode);
        }

        //add an arc between this node and another node
        void hypernode::addOuterArc(arc* appArc){
            _outerArcs.append(appArc);
        }
        
        bool hypernode::contains(hypernode* toCheck){
            //check each of the consolidated nodes

            if(toCheck == this){return true;}
            for(int i = 0; i < _consolidatedNodes.getSize(); i++){
                hypernode* analNode = _consolidatedNodes.getValue(i);
                if(analNode == nullptr){continue;}

                if(analNode->contains(toCheck)){return true;}
            }

            //check each of the base nodes
            for(int i = 0; i < _basenodes.getSize(); i++){
                hypernode* analNode = _basenodes.getValue(i);
                if(analNode == nullptr){continue;}

                if(analNode->contains(toCheck)){return true;}
            }

            return false;
        }

        //set arc directions
        void hypernode::propogate(hypernode* headSeed){
            _head = headSeed;
            if(headSeed == nullptr){return;}
            if(headSeed == this){
                computeParents();
                return;
            }
            // if(_head == this){computeParents(); return;}
            Lists::PQ<hypernode> pq; 
            // Lists::AList<hypernode> visited; 

            //head at this level
            // hypernode* headUnder = headSeed->getContainerUnder(this);

            pq.add(headSeed, 0);
            //add it to the pq
            while(!pq.isEmpty()){
                int popcost = pq.getPopCost();
                hypernode* poppedNode = pq.pop();
                if(poppedNode == nullptr){continue;}
                
                hypernode* poppedNodeAtLevel = poppedNode->getContainerUnder(this);
                poppedNodeAtLevel->propogate(poppedNode);

                Lists::AList<arc> children = poppedNodeAtLevel->getChildArcs();
                for(int i = 0; i< children.getSize(); i++){
                    //get the children at this level (under parent)
                    arc* analArc = children.getValue(i);
                    if(analArc == nullptr){continue;}
                    // if(analArc->isSet)

                    hypernode* othernode = analArc->getOther(poppedNodeAtLevel);
                    // hypernode* targetNode = analArc->getTarget();

                    if(othernode == nullptr){
                        //if no container under parent, add to outer arcs
                        _outerArcs.append(analArc);
                    }else{
                        pq.add(analArc->getOther(poppedNode), popcost + 1);
                    }

                    //set arc 
                    if(!analArc->isSet())
                        analArc->setSource(poppedNode);


                }
            }

            computeParents();
        }

        //compute and store the parent arcs
        void hypernode::computeParents(){
            for(int i = 0; i < _outerArcs.getSize(); i++){
                arc* analArc = _outerArcs.getValue(i);
                if(analArc == nullptr){continue;}

                if( contains(analArc->getTarget()) && analArc->isSet()){
                    _parentArcs.append(analArc);
                }
            }
        }

        //get the parent
        hypernode* hypernode::getParent(){
            arc* parentArc = nullptr;
            for(int i = 0 ; i < _parentArcs.getSize(); i++){
                parentArc = _parentArcs.getValue(i);
                if(parentArc == nullptr){
                    continue;
                }else{
                    return parentArc->getSource();
                }
            }
            return nullptr;
        }

        //get the outward facing arcs with 
        Lists::AList<arc> hypernode::getChildArcs(){
            Lists::AList<arc> childArcs;
            for(int i = 0; i < _outerArcs.getSize(); i++){
                arc* analArc = _outerArcs.getValue(i);
                if(analArc == nullptr){continue;}

                hypernode* targetNode = analArc->getTarget();
                if(!contains(targetNode)){
                    childArcs.append(analArc);
                }
            }
            return childArcs;
        }

        hypernode* hypernode::getContainerUnder(hypernode* getUnder){
            if(getUnder == _toplevel){ return _containerNodes.getValue(0);}
            if(getUnder == this){ return this;}
            for(int i = 0; i < _containerNodes.getSize()-1; i++){
                hypernode* analNode = _containerNodes.getValue(i);
                if(analNode == nullptr){continue;}

                if(getUnder == analNode){
                    return _containerNodes.getValue(i+1);
                }
            }
            return nullptr;
        }
    
        Lists::AList<hypernode> hypernode::getToGround(int index){
            Lists::AList<hypernode> path;
            hypernode* searchSource = _basenodes.getValue(index);
            hypernode* lastPopped = searchSource;
            path.append(searchSource);

            if(searchSource == nullptr){return path;}

            while(!(_head == lastPopped)){
                //get parent node at base
                lastPopped = lastPopped->getParent();
                
                if(lastPopped == nullptr){return path;}

                if(path.contains(lastPopped)){
                    //issue
                    return path;
                }
                
                path.append(lastPopped);
            }
            return path;
        }

        hypernode* hypernode::consolidate(Lists::AList<hypernode> toConsolidate){
            //add a new node with new nodes under it
            hypernode* consolidatedNode = new hypernode(); 
            for(int i = 0; i < toConsolidate.getSize(); i++){
                hypernode* analNode = toConsolidate.getValue(i);
            }
        }

        hypernode* hypernode::getContainerAbove(){
            hypernode* retval = _containerNodes.getValue(_containerNodes.getSize() - 2);
            if(retval == nullptr){return _toplevel;}
            return retval;
        }

    }
}