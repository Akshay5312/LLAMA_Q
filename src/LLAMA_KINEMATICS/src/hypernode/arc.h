#pragma once
#include "../../Ldeps/Ldeps.h"
#include "definitions.h"

namespace LLAMA
{
    namespace hgraph
    {
        class arc
        {
        protected:
            hypernode *_node0;
            hypernode *_node1;

            bool _source = 0;
            bool _isSet = false;
            
            Lists::AList<hypernode> _containers;

        public:
            arc();
            arc(hypernode *, hypernode *);
            
            hypernode* getNode(bool);

            //gets the other connected hypernode _at the same level__ as the passed hypernode 
            hypernode* getOther(hypernode*);

            //get the other hypernode at a specified level. 
            hypernode* getOther(hypernode*, hypernode*);
            
            hypernode* getSource();
            hypernode* getTarget();

            //flip the source
            void flip(){_source = !_source;}

            void set(){_isSet = true;}

            bool isSet(){return _isSet;}

            // void setSource(bool);

            void setSource(hypernode*);

            void addContainer(hypernode*);

            //checks if a node is in the target path or source path
            bool isInTarget(hypernode* toCheck);

        };
    }
}