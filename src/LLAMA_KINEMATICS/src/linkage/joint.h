#pragma once

#include "../hypergraph/hypergraph.h"

#include "portKey.h"
#include "../Actuation/ActuationLaw.h"

namespace LLAMA{
    namespace LINKAGE{
        class Joint : public hgraph::arc{
            private:
            //actuationLaw 
            Actuation::ActuationLaw<float>* _transformationActuation = nullptr;

            public:
            Joint() : hgraph::arc(){}
            Joint(portKey* k0, portKey* k1):hgraph::arc(k0, k1){}

            /**
             * @brief Get the Target port in the source port
             * 
             * @param usehard is the transformation going to be accessed more than once, 
             * do you want to be able to store the transformation of the joint as a state of the joint?
             * 
             * @return GEOM::SpatialT 
             */
            GEOM::SpatialT getTargetInSource(bool usehard = true){
                //source is node 0, actuator
                if(!_source){
                    return getActuatedInActuator(usehard);
                }else{
                    return getActuatorInActuated(usehard);
                }
            }

            /**
             * @brief Get the source port in the target port
             * 
             * @param usehard is the transformation going to be accessed more than once, 
             * do you want to be able to store the transformation of the joint as a state of the joint?
             * 
             * @return GEOM::SpatialT 
             */
            GEOM::SpatialT getSourceInTarget(bool usehard = true){
                //source is node 1, actuated 
                if(_source){
                    return getActuatedInActuator(usehard);
                }else{
                    return getActuatorInActuated(usehard);
                }
            }

            
            /**
             * @brief Get the actuator port in the actuated port
             * 
             * @param usehard is the transformation going to be accessed more than once, 
             * do you want to be able to store the transformation of the joint as a state of the joint?
             * 
             * @return GEOM::SpatialT 
             */
            GEOM::SpatialT getActuatorInActuated(bool usehard = true){
                GEOM::SpatialT JinJ = GEOM::I;
                GEOM::SpatialT K1 = GEOM::I;
                GEOM::SpatialT K0 = GEOM::I;
                
                if(_transformationActuation == nullptr){JinJ = GEOM::I;}else{
                    JinJ = _transformationActuation->inv_transform_val(usehard);
                }

                if(_port0 == nullptr){K0 = GEOM::I;}else{
                    K0 = ((portKey*)_port0)->getTransformation();
                }
                if(_port1 == nullptr){K1 = GEOM::I;}else{
                    K1 = ((portKey*)_port1)->getTransformation();
                }
                
                return K1 * JinJ * K0.getInverse();
            }
      
            /**
             * @brief Get the actuator port in the actuated port
             * 
             * @param usehard is the transformation going to be accessed more than once, 
             * do you want to be able to store the transformation of the joint as a state of the joint?
             * 
             * @return GEOM::SpatialT 
             */
            GEOM::SpatialT getActuatedInActuator(bool usehard = true){
                GEOM::SpatialT JinJ = GEOM::I;
                GEOM::SpatialT K1 = GEOM::I;
                GEOM::SpatialT K0 = GEOM::I;
                
                if(_transformationActuation == nullptr){JinJ = GEOM::I;}else{
                    JinJ = _transformationActuation->transform_val(usehard);
                    JinJ.getAxisAngleRotation();
                    //
                }

                if(_port0 == nullptr){K0 = GEOM::I;}else{
                    K0 = ((portKey*)_port0)->getTransformation();
                }
                if(_port1 == nullptr){K1 = GEOM::I;}else{
                    K1 = ((portKey*)_port1)->getTransformation();
                }
                
                GEOM::SpatialT retVal = K0 * JinJ * K1.getInverse();
                return retVal;
            }

            //set actuation law
            bool setActuationLaw(Actuation::ActuationLaw<float>* transformationActuation){
                if(transformationActuation == nullptr){
                    return false;
                }
                _transformationActuation = transformationActuation;
                return true;
            }
        };
    }
}