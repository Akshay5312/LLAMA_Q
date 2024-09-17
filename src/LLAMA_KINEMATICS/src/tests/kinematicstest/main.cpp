#include "../LinkFK.h"

#include <iostream>
#include <fstream>

using namespace std;

using namespace LLAMA;

void setupLinkage(LinkFK<3> &hArm){
    hArm.addLinks(4);

    hArm.setActuationLaw(hArm.addJoint(0,1, GEOM::I), 0, new Actuation::RotaryActuationLaw());
    hArm.setActuationLaw(hArm.addJoint(1,2, GEOM::dZ(5) * GEOM::rX(3.14159/2)), 1, new Actuation::RotaryActuationLaw());
    hArm.setActuationLaw(hArm.addJoint(2,3, GEOM::dX(5)), 2, new Actuation::RotaryActuationLaw());

    hArm.setControlKey(hArm.addKey(3, GEOM::dX(5)));

    hArm.getSolver()->setWeights(BLA::Matrix<6>(0,0,0,1,1,1));

    hArm.setHead(0);
    hArm.propogate();
}


class traj : BLA::MatrixFunctor<6,1>{
    float speed = 5;
    float offset = 2* 3.14159;
    public:
    traj():BLA::MatrixFunctor<6,1>(){}
    BLA::Matrix<6> operator()(BLA::Matrix<1> t){
        t = (t + offset) * speed;
        return 
            {0,0,1, 
                7 * cos(t(0)/10) + cos(t(0))*cos(t(0)/10), 
                7 * sin(t(0)/10)+ cos(t(0))*sin(t(0)/10),
                sin(10*t(0)/10) + 5};
    } 
};

int main(){
    LinkFK<3> hArm;
    setupLinkage(hArm);

    traj t_t;
    BLA::Matrix<3> q = {2.2,1,1};
    BLA::Matrix<6,1> t = hArm.FK(q);
    BLA::Matrix<3> p = {t(3),t(4),t(5)};

    float time = 0;
    BLA::Matrix<6> p_d = t_t({time});

    
    std::ofstream myfile;
    myfile.open("log.csv");

    while(time < 14){

        p_d = t_t({time});
        // BLA::Matrix<3> err = p_d - p;

        q = hArm.IK(p_d, q, 15);
        t = hArm.FK(q);
        p = {t(3),t(4),t(5)};

        myfile<< p(0)<< ", "<<p(1)<<", "<<p(2) <<", "<< q(0) <<", " << q(1) <<", " <<q(2) <<", "<< time<<'\n';

        time += 0.1;
    }
    myfile.close();




    return true;
}