#include "nTCamera.hpp"
#include "GL.hpp"
#include "Player.hpp"

nTCamera::nTCamera() {
    pos.setPoint(0,0,0);
    rotation.setPoint(0,0,0);
    direction.setPoint(0,0,0);
};

nTCamera::nTCamera(const nTCamera& orig) {
}

nTCamera::~nTCamera() {
}

void nTCamera::moveYaxis(int dir){
    pos.y+=nTCamera::steepVar*dir;
}
void nTCamera::rotateZaxis(int dir){
    rotation.z+=nTCamera::angleVar*dir;
    direction.y=tan(rotation.z);   
    Player::syncWithCamera();
}
void nTCamera::rotateYaxis(int dir){
    rotation.y+=nTCamera::angleVar*dir;
    if(Player::firstPerson){
        direction.x=sin(rotation.y);
        direction.z=-1*cos(rotation.y);
    }else{
        direction.x=-sin(rotation.y);
        direction.z=1*cos(rotation.y);
    }
    Player::syncWithCamera();
}

void nTCamera::behave(){
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(pos.x, pos.y, pos.z, pos.x+direction.x, pos.y+direction.y, pos.z+direction.z, 0, 1, 0);
    //if(fmod(GL::framesInGame,GL::getFPS())==0)cout<<GL::cam;
}

ostream& operator<<(ostream &strm, const nTCamera &cam) {
    if(Util::DEBUG)
        return strm <<"Cam:["<<"Pos("<<"X:"<<cam.pos.x<<" Y:"<<cam.pos.y<<" Z:"<<cam.pos.z<<"),"<<"Rotation("<<"X:"<<cam.rotation.x<<" Y:"<<cam.rotation.y<<" Z:"<<cam.rotation.z<<"),"
                <<"Direction("<<"X:"<<cam.direction.x<<" Y:"<<cam.direction.y<<" Z:"<<cam.direction.z<<"),"<<"]\n";
    return strm;
}

float nTCamera::angleVar=0.01;
float nTCamera::steepVar=0.5;
float nTCamera::mouseX=800;
float nTCamera::mouseY=600;
bool nTCamera::putMouseOnMiddle=true;