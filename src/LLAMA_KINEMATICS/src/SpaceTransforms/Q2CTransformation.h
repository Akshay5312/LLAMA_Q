
#include "SpaceTransform.h"

namespace LLAMA{
    namespace SpaceTransformations{
    /**
     * @brief transformation from joint space to task space
     * 
     * @tparam inDOF 
     * @tparam outDOF 
     * @tparam inType must cast to BLA::Matrix<inDOF>
     * @tparam outType must cast to BLA::Matrix<outDOF>
     */
    template<int inDOF, int outDOF, class inType = BLA::Matrix<inDOF>, class outType = BLA::Matrix<outDOF>>
    class QtoCTransformation : public SpaceTransformation<inDOF, outDOF, inType, outType>{
        private:
        protected:
            virtual outType _y(inType q) = 0;
        public:

            QtoCTransformation() : SpaceTransformation<inDOF, outDOF, inType, outType>(){}

            virtual outType FK(inType q){return this->FT(q);}

            virtual BLA::Matrix<outDOF, inDOF> Jacobian(inType q){
                return BLA::Jacobian<outDOF,inDOF>((*(BLA::MatrixFunctor<outDOF, inDOF>*)(this)), q);
            }
            virtual inType IK(outType q){
                return this->IT(q);
            }

            BLA::Matrix<3> IK(BLA::Matrix<outDOF> c, BLA::Matrix<inDOF> bestGuess, int timeComplex = 10000){
                return this->IT(c, bestGuess, timeComplex);
            }     

    };
    }
}