#include "../linkage.h"

#include <iostream>


using namespace LLAMA;

void setupLinkage(LINKAGE::Linkage &hArm){

    hArm.addLinks(4);

    hArm.addJoint(0,1, GEOM::I);
    hArm.addJoint(1,2, GEOM::dZ(5) * GEOM::rX(3.14159/2));
    hArm.addJoint(2,3, GEOM::dX(5));

    hArm.addKey(3, GEOM::dX(5));

    hArm.setHead(0);

    hArm.propogate();
}


int main(){
    LINKAGE::Linkage hArm;
    setupLinkage(hArm);
    BLA::Matrix<6,1> t = hArm.getKey(0)->getInGround();
    bool success = ((GEOM::SpatialT)t == (GEOM::dZ(5) * GEOM::rX(3.14159/2) * GEOM::dX(5) * GEOM::dX(5))); 
    //std::cout<<success;
    return success;
}