#pragma once 
#include "../../Ldeps/Ldeps.h"
#pragma once 
namespace LLAMA{

    /**
     * @brief a function denoting a space transformation
     * 
     * @tparam inDOF 
     * @tparam outDOF 
     * @tparam inType 
     * @tparam outType 
     */
    template<int inDOF, int outDOF, class inType = BLA::Matrix<inDOF>, class outType = BLA::Matrix<outDOF>>
    class SpaceTransformation : public BLA::MatrixFunctor<outDOF, inDOF>{
        private:
            OPT::Solver<outDOF, inDOF, SpaceTransformation>* _inverter;// = OPT::Solver<outDOF, inDOF>(this)
            
        public:
        
            BLA::Matrix<outDOF> operator()(BLA::Matrix<inDOF> x){
                return _y(x);
            }
        
            SpaceTransformation() : BLA::MatrixFunctor<outDOF, inDOF>(){
                _inverter = new OPT::Solver<outDOF, inDOF, SpaceTransformation>(this);
            }

            /**
             * @brief the value of the transformation
             * 
             * @param x 
             * @return outType 
             */
            virtual outType _y(inType x) = 0;

            /**
             * @brief A forward transformation
             * 
             * @param x 
             * @return outType 
             */
            virtual outType FT(inType x){return _y(x);}

            /**
             * @brief The jacobian
             * 
             * @param x the point to take the jacobian at
             * @return BLA::Matrix<outDOF, inDOF> 
             */
            virtual BLA::Matrix<outDOF, inDOF> Jacobian(inType x){
                return BLA::Jacobian<outDOF,inDOF>((*(BLA::MatrixFunctor<outDOF, inDOF>*)(this)), x);
            }
            //to implement

            /**
             * @brief the inverse of the transformation (computed using an optimization based solver)
             * 
             * @param y 
             * @return inType 
             */
            virtual inType IT(outType y){
                _inverter->setTarget(y);
                _inverter->gd()->setInit(inType());
                return _inverter->gd()->solve();
            }

            /**
             * @brief the inverse of the transformation
             * 
             * @param y the point at
             * @param best_guess the best guess of the solution 
             * @param maxSteps the maximum amount of steps -- lowers time and accuracy
             * @return inType 
             */
            virtual inType IT(outType y, inType best_guess, int maxSteps){
                _inverter->setTarget(y);
                _inverter->gd()->setInit(best_guess);
                _inverter->gd()->setStepSize(0.2);
                

                return _inverter->gd()->stepAmount(maxSteps);
            }

            

            OPT::Solver<outDOF, inDOF, SpaceTransformation>* getSolver(){return _inverter;}

    };
}