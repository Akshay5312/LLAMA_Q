#pragma once

#include "Quaternion.h"

namespace LLAMA{
namespace GEOM{
    class SpatialT{
        private:
            QuaternionO rot = {1,0,0,0};
            spatialP pos = {0,0,0};

        public:
            SpatialT(QuaternionO setRot, spatialP setPos){
                rot = setRot;
                pos = setPos;
            }

            SpatialT(){}

            ~SpatialT(){}

            SpatialT(const SpatialT& Ts){
                rot = Ts.rot;
                pos = Ts.pos;
            }
            
            SpatialT(const BLA::Matrix<7,1>& Ts){
                rot = {Ts(0),Ts(1),Ts(2),Ts(3)};
                pos = {Ts(4),Ts(5),Ts(6)};
            }

            SpatialT(const BLA::Matrix<6,1>& Ts){
                BLA::Matrix<3> rot_vec = {Ts(0),Ts(1),Ts(2)};

                float angle = sqrt((rot_vec(0)*rot_vec(0)) + (rot_vec(1)*rot_vec(1)) + (rot_vec(2)*rot_vec(2)));
                BLA::Matrix<3> axis = rot_vec/angle;

                rot = QuaternionO(angle, axis);
                pos = {Ts(3),Ts(4),Ts(5)};
            }

            SpatialT& operator =(const SpatialT& Ts){
                rot = Ts.rot;
                pos = Ts.pos;
                return (*this);
            }

            /**
             * @brief cast to a 7 element vector. {quaternion; position} = {a,i,j,k, x,y,z}
             * 
             * @return BLA::Matrix<7,1> 
             */
            operator BLA::Matrix<7,1>()  {return {rot('w'), rot('i'), rot('j'), rot('k'), pos(0), pos(1), pos(2)};}
            /**
             * @brief cast to 6 element vector. {AxisAngle; position}
             * 
             * @return BLA::Matrix<6,1> 
             */
            operator BLA::Matrix<6,1>()  {
                BLA::Matrix<3,1> aar = getAxisAngleRotation();
                return {aar(0), aar(1), aar(2), pos(0), pos(1), pos(2)};
            }

            bool operator == (const SpatialT& st){
                spatialP disp = pos - st.pos;
                float diffMag = sqrt(pow(disp(0), 2) + pow(disp(1), 2) + pow(disp(2), 2));
                bool retVal = (rot == st.rot);
                retVal &= (diffMag < 0.005);
                return (retVal); 
            }

            SpatialT operator * (const SpatialT& Ts){
                SpatialT retVal;
                retVal.pos = (rot*Ts.pos) + pos;
                retVal.rot = rot*Ts.rot;

                return retVal;
            }


            SpatialT getInverse(){
                SpatialT st;
                st.rot = rot.getInverse();
                st.pos = rot.getInverse().transform(pos) * -1;

                return st;
            }

            spatialP operator * (const spatialP& V3){
                spatialP retVal = rot*V3 + pos;
                return retVal;
            }

            

            BLA::Matrix<4,4> getMatrix(){
                spatialP xT = rot.transform(i);
                spatialP yT = rot.transform(j);
                spatialP zT = rot.transform(k);

                
                BLA::Matrix<4,4> retVal = {1,0,0,0,  0,1,0,0, 0,0,1,0, 0,0,0,1};
                for(int j = 0; j < 3; j++){
                    retVal(j, 0) = xT(j);
                    retVal(j, 1) = yT(j);
                    retVal(j, 2) = zT(j);
                    retVal(j, 3) = pos(j);
                }

                return retVal;
            }


            spatialP getPos(){return pos;}

            QuaternionO getQuaternionOrientation(){return rot;}

            BLA::Matrix<3> getAxisAngleRotation(){return rot.getAxisAngle();}
    };

    SpatialT I = SpatialT();

    /**
     * @brief rotX, rotation transformation about x
     * 
     * @param rad angle of rotation
     * @return spatialT 
     */
    SpatialT rX(float rad){
        return SpatialT(QuaternionO(rad, i), {0,0,0});
    }

    /**
     * @brief roty, rotation transformation about y
     * 
     * @param rad angle of rotation
     * @return spatialT 
     */
    SpatialT rY(float rad){
        return SpatialT(QuaternionO(rad, j), {0,0,0});
    }

    /**
     * @brief rotZ, rotation transformation about z
     * 
     * @param rad 
     * @return spatialT 
     */
    SpatialT rZ(float rad){
        return SpatialT(QuaternionO(rad, k), {0,0,0});
    }


    /**
     * @brief disp x
     * 
     * @param d the displacement about the x axis
     * @return SpatialT 
     */
    SpatialT dX(float d){
        return SpatialT(QuaternionO(), i*d);
    }
    
    /**
     * @brief disp y
     * 
     * @param d the displacement about the y axis
     * @return SpatialT 
     */
    SpatialT dY(float d){
        return SpatialT(QuaternionO(), j*d);
    }

    /**
     * @brief disp z
     * 
     * @param d the displacement about the z axis
     * @return SpatialT 
     */
    SpatialT dZ(float d){
        return SpatialT(QuaternionO(), k*d);
    }


    /**
     * @brief axis angle transformation
     * 
     * @param angle angle of rotation
     * @param axis axis about which rotation takes place 
     * @return SpatialT 
     */
    SpatialT axisAngle(float angle, spatialP axis = k){
        return SpatialT(   
            QuaternionO(angle, axis), 
            {0,0,0}
        );
    }

    /**
     * @brief axis disp transformation
     * 
     * @param disp displacement in set units (m)
     * @param axis axis of rotation
     * @return SpatialT 
     */
    SpatialT axisDisp(float disp, spatialP axis){
        return SpatialT(QuaternionO(), axis * disp);
    }
}
}