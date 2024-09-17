#pragma once
#include "hypergraph.h"


#include "arc.cpp"
#include "node.cpp"
#include "port.cpp"
namespace LLAMA{
    namespace hgraph{
        
        /**
         * @brief add a node to the hypergraph. This is a free node until an arc is added to it.
         * 
         * @param toAdd the node to add into the hypergraph
         * @return int index of the node
         */
        int hypergraph::addNode(node* toAdd){
            return _nodes.append(toAdd);
        }

        /**
         * @brief add an arc to connect two nodes
         * 
         * @param node0 
         * @param node1 
         * @return int 
         */
        int hypergraph::addArc(node* node0, node* node1){
            if(node0 == nullptr){
                return -1;
            }
            if(node1 == nullptr){
                return -1;
            }

            //add the ports
            port* port0 = new port(node0);
            port* port1 = new port(node1);

            //add the arc
            return addArc(port0, port1);
        }

        /**
         * @brief add a new arc to connect two ports
         * 
         * @param port0 
         * @param port1 
         * @return int 
         */
        int hypergraph::addArc(port* port0, port* port1){
            if(port0 == nullptr){
                return -1;
            }
            if(port1 == nullptr){
                return -1;
            }

            arc* newArc = new arc(port0, port1);
            return _arcs.append(newArc);
        }

        /**
         * @brief add an arc between two nodes using indeces
         * 
         * @param node0Index 
         * @param node1Index 
         * @return int 
         */
        int hypergraph::addArc(int node0Index, int node1Index){
            return addArc(getNode(node0Index), getNode(node1Index));
        }

        /**
         * @brief set the head node
         * 
         * @param headIndex index of the head node
         * @return true 
         * @return false 
         */
        bool hypergraph::setHead(int headIndex){
            node* head = _nodes.getValue(headIndex);
            if(head == nullptr){
                return false;
            }else{
                _head = head;
            }
            return true;
        }

        /**
         * @brief propogate away from the head and set the arc directions to create greaphs that propogate from the head.
         * 
         * @return true if successful
         * @return false 
         */
        bool hypergraph::propogate(){
            //note: this is to occur once at initialization. 
            //I don't reaaaaaallly need to focus hard on reducing time complexity

            //if there is no head to the graph you can't propogate through it. 
            if(_head == nullptr){
                return false;
            }

            //the priority queue
            Lists::PQ<arc> pq;
            //a list of the visited nodes
            Lists::AList<node> visited;


            //The head node is the first node to visit. Add all of the children (arcs) to a priority queue
            //the arc directions need to be set as pointing _away_ from the head node. 
            //The nodes they point towards are then visited. Process is repeated.
            visited.append(_head);

            //this could be a list of nodes. that would reduce the amount of code, but only marginally.
            //this seems more intuitive
            Lists::AList<arc> conn_head = _head->getConnected();

            for(int i = 0; i < conn_head.getSize(); i++){
                pq.add(conn_head.getValue(i), 0);
            }

            while(!pq.isEmpty()){
                //pop the value

                //the cost of the popped value
                int popcost = pq.getPopCost();
                //the popped value
                arc* popped = pq.pop();


                //////set the direction of the arc
                node* node0 = popped->getNode(0);
                node* node1 = popped->getNode(1);

                port* sourcePort = popped->getSource();
                if(sourcePort == nullptr){continue;}
                
                node* sourceNode = sourcePort->getNode();
                if(sourceNode == nullptr){continue;}

                node* desiredSourceNode = nullptr;
                node* desiredTrgNode = nullptr;

                if(visited.contains(node0)){
                    //if both of the nodes have been visited, leave it alone 
                    if(visited.contains(node1)){
                        continue;
                    }else{
                        //node 0 is the source
                        desiredSourceNode = node0;
                        desiredTrgNode = node1;
                    }
                }else{
                    desiredSourceNode = node1;
                    desiredTrgNode = node0;
                }

                // desiredTrgNode->addSource(desiredSourceNode);


                if(!(desiredSourceNode == sourceNode)){
                    //if the arc origin is not what you want it to be, then flip the arc
                    popped->flip();
                }

                popped->setSource();

                //////add the non-visited nodes' arcs to the pq
                visited.append(desiredTrgNode);
                Lists::AList<arc> conn = desiredTrgNode->getConnected();

                for(int i = 0; i < conn.getSize(); i++){
                    pq.add(conn.getValue(i), popcost + 1);
                }
            }

            return true;
        }
    }
}
