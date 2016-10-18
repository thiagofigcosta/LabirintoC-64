#include "nTCamera.hpp"

nTCamera::nTCamera() {
    pos.setPoint(0,0,0);
    rotation.setPoint(0,0,0);
    direction.setPoint(0,0,0);
};

nTCamera::nTCamera(const nTCamera& orig) {
}

nTCamera::~nTCamera() {
}

ostream& operator<<(ostream &strm, const nTCamera &cam) {
    if(Util::DEBUG)
        return strm <<"Cam:["<<"Pos("<<"X:"<<cam.pos.x<<" Y:"<<cam.pos.y<<" Z:"<<cam.pos.z<<"),"<<"Rotation("<<"X:"<<cam.rotation.x<<" Y:"<<cam.rotation.y<<" Z:"<<cam.rotation.z<<"),"
                <<"Direction("<<"X:"<<cam.direction.x<<" Y:"<<cam.direction.y<<" Z:"<<cam.direction.z<<"),"<<"]\n";
    return strm;
}

float nTCamera::angleVar=0.1;
float nTCamera::steepVar=0.5;