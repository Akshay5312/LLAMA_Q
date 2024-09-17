#pragma once

#include "../../Ldeps/Ldeps.h"
namespace LLAMA{
    namespace Actuation{
        template<class T = float>
        class ActuationLaw{
            private:
                //A "hard value". when the same value needs to be used multiple times
                T _hardVal = 0;
                //A "soft value". when values need to be changed easily
                T _softVal = 0;

                //A transformation of the hard value. reduces computation time for when transformations need to be determined multiple times rapidly 
                GEOM::SpatialT _hardRetVal = calcTransform(_hardVal);
                GEOM::SpatialT _invhardRetVal = inv_calcTransform(_hardVal);
            public:
                ActuationLaw(){}

                ~ActuationLaw(){}

                ActuationLaw(const ActuationLaw<T> &A){
                    _hardVal = A._hardVal;
                    _softVal = A._softVal;
                    _hardRetVal = A._hardRetVal;
                    _invhardRetVal = A._invhardRetVal;
                }

                ActuationLaw& operator = (const ActuationLaw<T> &A){
                    _hardVal = A._hardVal;
                    _softVal = A._softVal;
                    _hardRetVal = A._hardRetVal;
                    _invhardRetVal = A._invhardRetVal;
                    return (*this);
                }

                /**
                 * @brief Get the Transformation for a set actuation value. default is set to rot_z
                 * 
                 * @return GEOM::SpatialT 
                 */
                virtual GEOM::SpatialT calcTransform(T input){
                    return GEOM::rZ(_softVal);
                }
                /**
                 * @brief Get the inverse transformation for a set actuation value. default inverts "calcTransform" 
                 * 
                 * @param input 
                 * @return GEOM::SpatialT 
                 */
                virtual GEOM::SpatialT inv_calcTransform(T input){
                    return calcTransform(input).getInverse();
                }

                /**
                 * @brief Set the value of the actuator. This could set the "hard value" or the "soft value"
                 * 
                 * @param input 
                 * @param usehard 
                 */
                void setVal(T input, bool usehard = true){
                    if(usehard){
                        _hardVal = input;
                        _hardRetVal = calcTransform(input);
                        _invhardRetVal = inv_calcTransform(input);
                    }else{
                        _softVal = input;
                    }
                }

                /**
                 * @brief 
                 * 
                 * @param useHard 
                 * @return GEOM::SpatialT 
                 */
                GEOM::SpatialT transform_val(bool useHard = true){
                    if(useHard){
                        return _hardRetVal;
                    }
                    else{
                        return calcTransform(_softVal);
                    }
                }

                /**
                 * @brief 
                 * 
                 * @param useHard 
                 * @return GEOM::SpatialT 
                 */
                GEOM::SpatialT inv_transform_val(bool useHard = true){
                    if(useHard){
                        return _invhardRetVal;
                    }
                    else{
                        return inv_calcTransform(_softVal);
                    }
                }
        };

        template<class T = float>
        class RotaryActuationLaw : public ActuationLaw<T>{
                BLA::Matrix<3,1> _axis = GEOM::k;
            public:
                RotaryActuationLaw(BLA::Matrix<3,1> axis = GEOM::k) : ActuationLaw<T>(){
                    _axis = axis;
                }

                ~RotaryActuationLaw(){}

                RotaryActuationLaw(const RotaryActuationLaw &A) : ActuationLaw<T>(A){
                    _axis = A._axis;
                }
                
                RotaryActuationLaw<T>& operator = (RotaryActuationLaw<T> &A){
                    ActuationLaw<T>::operator = (A);
                    _axis = A._axis;
                    return (*this);
                }

                /**
                 * @brief Get the Transformation for a set actuation value. default is set to rot_z
                 * 
                 * @return GEOM::SpatialT 
                 */
                GEOM::SpatialT calcTransform(T input){
                    return GEOM::axisAngle(input, _axis);
                }
                
                /**
                 * @brief get the inverse transformation
                 * 
                 * @param input 
                 * @return GEOM::SpatialT 
                 */
                GEOM::SpatialT inv_calcTransform(T input){
                    return GEOM::axisAngle(-input, _axis);
                }
        };

        
        template<class T = float>
        class LinearActuationLaw : public ActuationLaw<T>{
                BLA::Matrix<3,1> _axis = GEOM::k;
            public:
                LinearActuationLaw(BLA::Matrix<3,1> axis = GEOM::k) : ActuationLaw<T>(){
                    _axis = axis;
                }

                ~LinearActuationLaw(){}

                LinearActuationLaw(const LinearActuationLaw &A) : ActuationLaw<T>(A){
                    _axis = A._axis;
                }

                LinearActuationLaw<T>& operator = (LinearActuationLaw<T> &A){
                    ActuationLaw<T>::operator = (A);
                    _axis = A._axis;

                    return (*this);
                }

                /**
                 * @brief Get the Transformation for a set actuation value. default is set to rot_z
                 * 
                 * @return GEOM::SpatialT 
                 */
                GEOM::SpatialT calcTransform(T input){
                    return GEOM::axisDisp(input, _axis);
                }

                /**
                 * @brief get the inverse transformation
                 * 
                 * @param input 
                 * @return GEOM::SpatialT 
                 */
                GEOM::SpatialT inv_calcTransform(T input){
                    return GEOM::axisDisp(-input, _axis);
                }
        };
        
        template<class T = float>
        class CustomActuationLaw : public ActuationLaw<T>{
                //a pointer to a function returning a transformation
                GEOM::SpatialT (*_calcTransformPtr)(T);
            public:
                CustomActuationLaw(GEOM::SpatialT (*calcTransformPtr)(T)) : ActuationLaw<T>(){
                    _calcTransformPtr = calcTransformPtr;
                }

                CustomActuationLaw(const CustomActuationLaw &A) : ActuationLaw<T>(A){
                    _calcTransformPtr = A._calcTransformPtr;
                }

                ~CustomActuationLaw(){
                    //do not destroy the function pointer, it's a function
                }

                CustomActuationLaw<T>& operator = (CustomActuationLaw<T> &A){
                    ActuationLaw<T>::operator = (A);
                    _calcTransformPtr = A._calcTransformPtr;
                    return (*this);
                }

                /**
                 * @brief Get the Transformation for a set actuation value.
                 * 
                 * @return GEOM::SpatialT 
                 */
                virtual GEOM::SpatialT calcTransform(T input){
                    return _calcTransformPtr(input);
                }
        };
    }
}