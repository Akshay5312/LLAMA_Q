#pragma once


#include "../Ldeps/Ldeps.h"


namespace LLAMA{
namespace GEOM{ 
    // typedef BLA::Matrix<3> spatialP;
    using spatialP = BLA::Matrix<3>;

    //unit vectors
    spatialP i = {1,0,0};
    spatialP j = {0,1,0};
    spatialP k = {0,0,1};

    //magnitude of a spatial vector
    float mag(spatialP Vec){return sqrt(pow(Vec(0),2) + pow(Vec(1),2) + pow(Vec(2),2));}

    //cross product of 2 vectora
    spatialP cross(spatialP V1, spatialP V2){
        spatialP retVal;
        retVal(0) = V1(1) * V2(2) - V1(2) * V2(1);
        retVal(1) = -V1(0) * V2(2) + V1(2) * V2(0);
        retVal(2) = V1(0) * V2(1) - V1(1) * V2(0);
        return retVal;
    }

    //dot product of 2 spatial vectors
    float dot(spatialP V1, spatialP V2){return ((V1(0)*V2(0)) + (V1(1)*V2(1)) + (V1(2)*V2(2)));}

    class QuaternionO{
        private:

            //qw
            float q0 = 1;
            //qx
            spatialP axisVect = {0,0,0};

            //Q = q0 + qx*i + qy*j + qz*k
        public:

            //by values
            //list initialization
            QuaternionO(float qw = 1, float qx = 0, float qy = 0, float qz = 0){
                axisVect = {qx,qy,qz};
                q0 = qw;
            }

            //axis angle
            QuaternionO(float rad, spatialP vec){
                axisVect = ((vec) * sin(rad/2));
                
                q0 = cos(rad/2);
            }

            // //axis angle
            // QuaternionO(spatialP vec){
            //     float rad = 
            // }

            ~QuaternionO(){}

            QuaternionO(const QuaternionO& QO){q0 = QO.q0; axisVect = QO.axisVect;}
            QuaternionO& operator =(const QuaternionO& QO){q0 = QO.q0; axisVect = QO.axisVect; return (*this);}

            /**
             * @brief is equivalent? transform two independant unit vectors and check displacement between the two
             * 
             * @param QO 
             * @return true 
             * @return false 
             */
            bool operator == (const QuaternionO& QO){
                QuaternionO copyQO(QO.q0, QO.axisVect(0), QO.axisVect(1), QO.axisVect(2));
                spatialP iInThis = transform(i);
                spatialP iInThat = copyQO * (i);

                spatialP kInThis = transform(k);
                spatialP kInThat = copyQO * (k);

                spatialP ierror = iInThis - iInThat;
                spatialP kerror = kInThis - kInThat;
                
                float magI = mag(ierror);
                float magK = mag(kerror);
                return 
                    (
                        (magI < 0.05)
                        &&
                        (magK < 0.05)
                    );
            }
            

            //i,j,k
            // QuaternionO(char unitDirection, float angle);
            /**
             * @brief multiplication operator, transforms a quaternion through this
             * 
             * @param QO 
             * @return QuaternionO 
             */
            QuaternionO operator * (const QuaternionO& QO){
                float retq0 = q0 * QO.q0 - dot(axisVect, QO.axisVect);

                spatialP retVec = cross(axisVect, QO.axisVect) + QO.axisVect * q0 + axisVect * QO.q0;
                // Serial.print("retVec = ");
                // retVec.print();
                return QuaternionO(retq0, retVec(0), retVec(1), retVec(2));

            }

            /**
             * @brief Get the Imaginary Part of the quaternion
             * 
             * @return spatialP 
             */
            spatialP getImaginaryPart(){return axisVect;}

            /**
             * @brief get the c'th value of the quaternion. indexed from 0
             * 
             * @param c 
             * @return float& 
             */
            float& operator()(const char& c){
                switch(c){
                    case 'w':
                        return q0;
                    break;
                    case 'i':
                        return axisVect(0);
                    break;
                    case 'j':
                        return axisVect(1);
                    break;
                    case 'k':
                        return axisVect(2);
                    break;
                    case 0:
                        return q0;
                    break;
                    case 1:
                        return axisVect(0);
                    break;
                    case 2:
                        return axisVect(1);
                    break;
                    case 3:
                        return axisVect(2);
                    break;
                    default:
                        //junk 
                        return q0;
                        //do nothing
                };
            }

            float magnitude(){return sqrt(pow(q0,2) + pow(mag(axisVect),2));}

            /**
             * @brief Get the Inverse
             * 
             * @return QuaternionO 
             */
            QuaternionO getInverse(){
                spatialP invVect = axisVect * -1;
                return QuaternionO(q0, invVect(0), invVect(1), invVect(2));
            }

            /**
             * @brief transform a spatial vector
             * 
             * @param V3 
             * @return spatialP 
             */
            spatialP operator * (const spatialP& V3){
                return transform(V3);
            }

            /**
             * @brief transform a spatial vector
             * 
             * @param VectorInI 
             * @return spatialP 
             */
            spatialP transform(spatialP VectorInI){
                QuaternionO midVal(0,VectorInI(0),VectorInI(1),VectorInI(2));

                // float midq0 = 0;
                // spatialP midVect = {0,0,0};
                
                // midq0 = -dot(axisVect, VectorInI);
                // midVect = cross(axisVect, VectorInI) + VectorInI * q0;

                // //[midq0 midVect] * [q0, -axisVect]

                // float retq0 = q0 * midq0 + dot(axisVect, midVect);
                // spatialP retVec = cross(midVect, -axisVect) + midVect * q0 - axisVect * midq0;

                return ((*this) * midVal * getInverse()).getImaginaryPart();
            }

            /**
             * @brief Get the rotation axis
             * 
             * @return spatialP 
             */
            spatialP getAxis(){
                if(mag(axisVect) == 0){return {0,0,0};}

                return axisVect * (1/mag(axisVect)); 
            }

            /**
             * @brief Get the angle of displacement
             * 
             * @return float 
             */
            float getAngle(){return 2*acos(q0);}

            /**
             * @brief Get the Axis Angle representation
             * 
             * @return BLA::Matrix<3> 
             */
            BLA::Matrix<3> getAxisAngle(){return getAxis() * getAngle();}
    };
}
}