#pragma once
#include "linkage/linkage.h"
#include "SpaceTransforms/Q2CTransformation.h"

namespace LLAMA{
    class controlMapping{
        public:
        int joint_index = 0;
        int q_index = 0;
        Actuation::ActuationLaw<float>* _law = nullptr;

        controlMapping(int jointIndex, int qIndex, Actuation::ActuationLaw<float>* law) {
            joint_index = jointIndex;
            q_index = qIndex;
            _law = law;
        }
    };

    template<int inDOF>
    class LinkFK : public SpaceTransformations::QtoCTransformation<inDOF, 6>, public LINKAGE::Linkage{
            Lists::AList<controlMapping> controlMappings;

            LINKAGE::portKey* _ctrKey = nullptr;
        public:
            LinkFK():SpaceTransformations::QtoCTransformation<inDOF, 6>(), LINKAGE::Linkage(){   
            }

            ~LinkFK(){}
            
            /**
             * @brief Set the controlled Actuation Laws 
             * 
             * @param i {the actuation joint index, the index off the control value}
             * @param law 
             * @return true 
             * @return false 
             */
            bool setActuationLaw(int joint_index, int q_index, Actuation::ActuationLaw<float>* law){
                bool validAppend = true;
                if((joint_index >= inDOF) || (joint_index < 0)){
                    //this is a problem, ignore the control mapping and set it as a fixed
                    validAppend = false;
                }else{
                    controlMapping* appVal = new controlMapping(joint_index, q_index, law);
                    controlMappings.append(appVal);
                }
                return (LINKAGE::Linkage::setActuationLaw(joint_index, law) && validAppend);
            }

            BLA::Matrix<6> _y(BLA::Matrix<inDOF> q){

                setActuators(q);
                if(_ctrKey == nullptr){return GEOM::I;}

                return _ctrKey->getInGround();
            }

            void setActuators(BLA::Matrix<inDOF> q){
                for(int i = 0; i < controlMappings.getSize(); i++){
                    controlMapping* map = controlMappings.getValue(i);
                    int q_i = map->q_index;
                    Actuation::ActuationLaw<float>* law = map->_law; 

                    law->setVal(q(q_i));
                }
            }
            
            bool setControlKey(int keyIndex){
                LINKAGE::portKey* ctrk = getKey(keyIndex);

                if(ctrk == nullptr){return false;}

                _ctrKey = ctrk;
                return true;
            }
    };
}