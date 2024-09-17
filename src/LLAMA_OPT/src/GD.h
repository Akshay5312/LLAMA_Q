#pragma once
#include "../Ldeps/Ldeps.h"

namespace LLAMA{
    namespace OPT{

        /**
         * @brief An inequality constraint
         * 
         */
        class Constraint{
            protected:
                BLA::MatrixFunctor<1,1>* _fn;
            public:
            
            /**
             * @brief Construct a new Constraint from a function. right now only R1 -> R1 functions can be used
             * 
             * @param fn 
             */
            Constraint(BLA::MatrixFunctor<1,1>* fn){_fn = fn;}
            
            Constraint(){}
            
            /**
             * @brief if f(x) <= 0, the constrant is satisfied
             * 
             * @param x 
             * @return true 
             * @return false 
             */
            virtual bool violates(float &x){
                if(_fn == nullptr){return false;}
                BLA::Matrix<1,1> fn_a = (*_fn)({x});
                if(fn_a(0) > 0){return true;}
                return false;
            };
        };

        /**
         * @brief signifies that there is no constraint, the current (not great) way of doing constraints is to add one per input variable, and that needs to be improved
         * 
         */
        class EmptyConstraint : public Constraint{
            private:
            public:
                EmptyConstraint():Constraint(){}

                bool violates(float &x){return false;}
        };

        /**
         * @brief a linear constraint (is (aX - b) <= 0)
         * 
         */
        class LinearConstraint : public BLA::MatrixFunctor<1,1>, public Constraint{
            float _offset = 0;
            float _scale = 1;

            public:
                LinearConstraint(float scale = 1, float offset = 0) : BLA::MatrixFunctor<1,1>(), Constraint(this){
                    _offset = offset;
                    _scale = scale;
                }

                LinearConstraint(const LinearConstraint& lc){
                    _fn = this;

                    _offset = lc._offset;
                    _scale = lc._scale;
                }

                ~LinearConstraint(){}


                BLA::Matrix<1,1> operator()(BLA::Matrix<1,1> x){
                    return {_scale * x(0) - _offset};
                }
        };

        /**
         * @brief a ranged constraint (is (min <= x <= max))
         * 
         */
        class RangedConstraint : public Constraint{
        protected:
            float _range = 0;
            float _min = 0;
            float _max = 0;
            LinearConstraint _lower;
            LinearConstraint _higher;
        public:
            RangedConstraint(float max, float min) : Constraint(){

                    if(max < min){
                        _max = min;
                        _min = max;
                    }else{
                        _max = max;
                        _min = min;
                    }

                    _range = _max - _min;
                    _lower = LinearConstraint(-1, _min);
                    _higher = LinearConstraint(1, _max);
                }

            RangedConstraint(const RangedConstraint & Rc){
                _range = Rc._range;
                _min = Rc._min;
                _max = Rc._max;
                
                LinearConstraint _lower = Rc._lower;
                LinearConstraint _higher = Rc._higher;    
            }

                
            virtual bool violates(float &x){
                if(!(_lower.violates(x)) || (_higher.violates(x))){
                    return false;
                }
                return true;
            };


        };
        
        /**
         * @brief a wrapped constraint. This will change x to move it in bounds if it violates the ranged constraint by a small enough amount. Used for rotations or harmonic variables. 
         * 
         */
        class WrappedConstraint : public RangedConstraint{

                LinearConstraint _twicelower;
                LinearConstraint _twicehigher;
            public:
                WrappedConstraint(float max, float min) : RangedConstraint(max, min){
                    _twicelower = LinearConstraint(-1, _min - _range);
                    _twicehigher = LinearConstraint(1, _max + _range);
                }

                
                virtual bool violates(float &x){
                    if(_twicehigher.violates(x) || _twicelower.violates(x)){
                        return true;
                    }

                    if(!(_lower.violates(x)) || (_higher.violates(x))){
                        return false;
                    }                    

                    if(_lower.violates(x)){
                        x = x + _range;
                    }
                    if(_higher.violates(x)){
                        x = x - _range;
                    }
                    return true;
                };
        };

        template<int inputs>
        class GradientDescender{
            private:    
                //objective function
                BLA::MatrixFunctor<1,inputs>* f0;

                //constraints (just 1 DOF for now, constraint applied independantly to each domain), constraint n is applied to input n
                Lists::AList<Constraint> fn;

                //init value            
                BLA::Matrix<inputs> _x_0;

                //curr value            
                BLA::Matrix<inputs> _x_t;

                float _phi_t = 3.402823466e+38F;

                float _stepSize = 1;
            public:
                GradientDescender(BLA::MatrixFunctor<1,inputs> *Objective, float init_stepSize = 1){
                    f0 = Objective; 
                    if(init_stepSize > 0){
                        _stepSize = init_stepSize;
                    }
                }

                void setConstraint_n(Constraint* f_n){
                    //no ability to add with indeces yet -- sorry
                    fn.append(f_n);
                }

                void setStepSize(float h){_stepSize = h;}

                //the gradient of the objective 
                BLA::Matrix<inputs> Gradient(BLA::Matrix<inputs> _x){
                    BLA::Matrix<1,inputs> J = BLA::Jacobian<1, inputs>(*f0, _x);
                    BLA::Matrix<inputs, 1> G;
                    for(int i = 0; i < inputs; i++){
                        G(i) = J(0,i);
                    }
                    return G;
                }

                bool valid(BLA::Matrix<inputs> &x){
                    bool violated = 0;
                    for(int i = 0; i < inputs; i++){
                        Constraint* f_i = fn.getValue(i);
                        if(f_i == nullptr){
                            continue;
                        }

                        violated |= f_i->violates(x(i));
                    }
                    return !violated;
                }

                bool step(){
                    BLA::Matrix<inputs> G = Gradient(_x_t);

                    BLA::Matrix<inputs> currStep = G / BLA::Norm<inputs,1>(G) * _stepSize * -1;

                    BLA::Matrix<inputs> x_tp1 = _x_t + currStep;

                    bool inBounds = valid(x_tp1);
                    
                    if(!inBounds){
                        updateStep(false);
                    }else{
                        float phi = cost(x_tp1);

                        if(phi < _phi_t){
                            //successful step
                            updateStep(true);

                            _phi_t = phi;
                            _x_t = x_tp1;
                        }else{
                            updateStep(false);
                        }
                    }

                    return isSolved();
                    
                }

                void updateStep(bool stepWorked){
                    //if successful, h = h * 1.1
                    //else h = h * 0.7 
                    _stepSize *= ((stepWorked*0.1) + (!stepWorked * -0.3) + 1); 
                }

                bool isSolved(){
                    return ((_stepSize < 0.000001) || (_phi_t < 0.0001));
                }

                float cost(BLA::Matrix<inputs> x){
                    if(f0 == nullptr){return 3.402823466e+38F;}
                    BLA::Matrix<1> phi = (*f0)(x);
                    return phi(0);
                }

                BLA::Matrix<inputs> solve(){
                    while(!step()){
                    }
                    return _x_t;
                }

                BLA::Matrix<inputs> stepAmount(int maxSteps){
                    int stepsHere = 0;
                    while(!step() && (stepsHere < maxSteps)){
                        stepsHere++;
                    }
                    return _x_t;
                }

                bool setInit(BLA::Matrix<inputs> x_0){
                    if(valid(x_0)){
                        _x_0 = x_0;
                        _x_t = x_0;
                        if(f0 == nullptr){}else{
                            BLA::Matrix<1,1> p = (*f0)(_x_t);
                            _phi_t = p(0);
                        }
                        return true;
                    }
                    return false;
                }
        };

        /**
         * @brief A solver that uses Gradient descent to minimize e(x)' * Q * e(x), where e is the target - f(x)
         * 
         * @tparam outDOF 
         * @tparam inDOF 
         * @tparam function_class -- this is a vector valued function f(x)
         */
        template<int outDOF, int inDOF, class function_class>
        class Solver : public BLA::MatrixFunctor<1, inDOF>{
                function_class* _fX;

                GradientDescender<inDOF>* gder;

                BLA::Matrix<outDOF> _target = BLA::Zeros<outDOF>();

                BLA::Matrix<outDOF, outDOF> _Q = BLA::Identity<outDOF, outDOF>();

            public:

                BLA::Matrix<1> operator()(BLA::Matrix<inDOF> x){
                    if(_fX == nullptr){return {0};}

                    BLA::Matrix<1> retVal;
                    BLA::Matrix<outDOF> d = 
                    (
                        (*_fX)(x) 
                        - _target
                    );
                    BLA::Matrix<1,outDOF> dT;
                    for(int i = 0; i < outDOF; i++){
                        dT(0,i) = d(i);
                    }
                    retVal = dT * (_Q * d);
                    return retVal;
                }


                Solver(function_class* fX) : BLA::MatrixFunctor<1,inDOF>(){
                    _fX = fX;

                    gder = new GradientDescender(this, 1);
                }

                void setTarget(BLA::Matrix<outDOF> target){_target = target;}

                void setWeights(BLA::Matrix<outDOF> weights){
                    BLA::Matrix<outDOF, outDOF> Q = BLA::Identity<outDOF, outDOF>();
                    for(int i = 0; i < outDOF; i++){
                        Q(i,i) = weights(i);
                    }
                    _Q = Q;
                }

                void setWeights(BLA::Matrix<outDOF,outDOF> weights){
                    _Q = weights;
                }

                GradientDescender<inDOF>* gd(){return gder;}
        };

    }
}