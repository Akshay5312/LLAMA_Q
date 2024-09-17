#pragma once
#include "link.h"
#include "portKey.h"
#include "joint.h"


namespace LLAMA{
    namespace LINKAGE{

        class Linkage : public hgraph::hypergraph{
            private:

                //the frames of interest in each link -- the key frames
                Lists::AList<portKey> _keys;
                //all actuation laws
                Lists::AList<Actuation::ActuationLaw<float>> _allActuationLaws;

            public:
                Linkage(){}

                int addLink(Link* toAdd){return addNode(toAdd);}
                int addLink();
                void addLinks(int);

                int addJoint(int, int, GEOM::SpatialT,  GEOM::SpatialT);

                bool setActuationLaw(int, Actuation::ActuationLaw<float>*);

                int addKey(int linkIndex, GEOM::SpatialT inLink);

                // void setActuators(BLA::Matrix<actuationDOF> q, bool useHard = true);

                portKey* getKey(int);
                Joint* getJoint(int);
                Link* getLink(int);

                Actuation::ActuationLaw<float>* getActuationLaw(int i){return _allActuationLaws.getValue(i);}
        };

        void Linkage::addLinks(int numberOfLinks){
            for(int i = 0; i< numberOfLinks; i++){
                addLink();
            }
        }

        int Linkage::addLink(){
            return addLink(new Link());
        }



        int Linkage::addJoint(int actuatorLink, int actuatedLink, GEOM::SpatialT inActuator, GEOM::SpatialT inActuated = GEOM::I){
            //create ports, create arc between them
            Link* actuator = getLink(actuatorLink);
            Link* actuated = getLink(actuatedLink);

            if(actuator == nullptr){return -1;}
            if(actuated == nullptr){return -1;}

            portKey* p0 = new portKey(actuator, inActuator);
            portKey* p1 = new portKey(actuated, inActuated);

            Joint* addedJ = new Joint(p0, p1); 

            return addArc((hgraph::arc *)addedJ);
        }

        int Linkage::addKey(int linkIndex, GEOM::SpatialT inLink){
            Link* kLink = getLink(linkIndex);
            if(kLink == nullptr){
                return -1;
            }

            portKey* kFrame = new portKey(kLink,inLink);
            return _keys.append(kFrame);
        }
        
        portKey* Linkage::getKey(int i){
            return _keys.getValue(i);
        }

        Joint* Linkage::getJoint(int i){
            return ((Joint*)_arcs.getValue(i));
        }

        Link* Linkage::getLink(int i){
            return ((Link*) _nodes.getValue(i));
        }

        
        bool Linkage::setActuationLaw(int i, Actuation::ActuationLaw<float>* law){
            Joint* J = getJoint(i);
            if(J == nullptr){return false;}
            return J->setActuationLaw(law);
        }
        
    }
}