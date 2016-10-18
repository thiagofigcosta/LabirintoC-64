#include "Player.hpp"
#include "GL.hpp"
#include "nTMap.hpp"

Player::Player() {
}

Player::Player(const Player& orig) {
}

Player::~Player() {
}
nTPoint Player::pos;
nTPoint Player::size=Util::nTPointSet(1,2,1);
bool Player::canJump=true;
float Player::vSpeed=0;
bool Player::firstPerson=true;
bool Player::dontGiveAShitForCollision=false;

void Player::jump(){
    if(canJump){
        canJump=false;
        vSpeed+=40;
    }
}
void Player::applyGravity(){
    if(dontGiveAShitForCollision) return;
    pos.y+=vSpeed/GL::getFPS();
    if(pos.y!=-nTMap::scale.y/2){
        vSpeed-=100/GL::getFPS();
        if(pos.y<=-nTMap::scale.y/2){
            pos.y=-nTMap::scale.y/2;
            vSpeed=0;
        }
    }else canJump=true;
}
void Player::draw(){
    applyGravity();
    syncWithCamera();
    glPushMatrix();
        glTranslatef(pos.x, pos.y, pos.z);
        //glRotatef(Util::radToAngle(GL::cam.rotation.x),1,0,0);
        glRotatef(Util::radToAngle(-GL::cam.rotation.y),0,1,0);
        //glRotatef(Util::radToAngle(GL::cam.rotation.z),0,0,1);
        glTranslatef(-pos.x, -pos.y, -pos.z);
        if(!firstPerson)GL::drawBox(Util::getCollisionRectangle(pos,size),GL::getColorByName("red"));
    glPopMatrix();
}
void Player::spawn(nTPoint pos){
    Player::pos=pos;
    syncWithCamera();
}

void Player::syncWithCamera(){
    if(firstPerson)
        GL::cam.pos=pos;
    else{
        float distance=-2;
        GL::cam.pos.y=pos.y+2;
        GL::cam.pos.x=pos.x+GL::cam.direction.x*distance;
        GL::cam.pos.z=pos.z+GL::cam.direction.z*distance;
    }
}

bool Player::noCollisionWithMap(nTPoint posC){
    bool noCollsion=true;
    nTRectangle tmpRec;
    tmpRec=Util::getCollisionRectangle(posC,size);
    float sizeOfEachBlock=nTMap::scale.x*nTMap::scale.x;
    float sizeCol=sizeOfEachBlock+nTMap::scale.x/2;

    int playerInMapX=floor(posC.x/sizeOfEachBlock);
    int playerInMapZ=floor(posC.z/sizeOfEachBlock);

    if(playerInMapX<0||playerInMapX>=nTMap::current.size()){
        cout<<"math error, player pos X\n";
        return false;
    }
    if(playerInMapZ<0||playerInMapZ>=nTMap::current[0].size()){
        cout<<"math error, player pos Z\n";
        return false;
    }
    //colisao central
    if(nTMap::current[playerInMapX][playerInMapZ]==1)
        if(Util::analyseCollision(tmpRec,Util::getCollisionRectangle(Util::nTPointSet(nTMap::scale.x+sizeOfEachBlock*(playerInMapX),0,nTMap::scale.x+sizeOfEachBlock*(playerInMapZ)),Util::nTPointSet(sizeCol,0,sizeCol))))
            noCollsion=false;

    //colisao periferica
    if(playerInMapX!=0){
        if(nTMap::current[playerInMapX-1][playerInMapZ]==1)
        if(Util::analyseCollision(tmpRec,Util::getCollisionRectangle(Util::nTPointSet(nTMap::scale.x+sizeOfEachBlock*(playerInMapX-1),0,nTMap::scale.x+sizeOfEachBlock*(playerInMapZ)),Util::nTPointSet(sizeCol,0,sizeCol))))
            noCollsion=false;

    }else if(Util::analyseCollision(tmpRec,Util::getCollisionRectangle(Util::nTPointSet(nTMap::scale.x+sizeOfEachBlock*(playerInMapX-1),0,nTMap::scale.x+sizeOfEachBlock*(playerInMapZ)),Util::nTPointSet(sizeCol,0,sizeCol))))
            noCollsion=false;

    if(playerInMapX<nTMap::current.size()-1){
        if(nTMap::current[playerInMapX+1][playerInMapZ]==1)
        if(Util::analyseCollision(tmpRec,Util::getCollisionRectangle(Util::nTPointSet(nTMap::scale.x+sizeOfEachBlock*(playerInMapX+1),0,nTMap::scale.x+sizeOfEachBlock*(playerInMapZ)),Util::nTPointSet(sizeCol,0,sizeCol))))
            noCollsion=false;
    }else if(Util::analyseCollision(tmpRec,Util::getCollisionRectangle(Util::nTPointSet(nTMap::scale.x+sizeOfEachBlock*(playerInMapX+1),0,nTMap::scale.x+sizeOfEachBlock*(playerInMapZ)),Util::nTPointSet(sizeCol,0,sizeCol))))
            noCollsion=false;
    if(playerInMapZ!=0){
        if(nTMap::current[playerInMapX][playerInMapZ-1]==1)
        if(Util::analyseCollision(tmpRec,Util::getCollisionRectangle(Util::nTPointSet(nTMap::scale.x+sizeOfEachBlock*(playerInMapX),0,nTMap::scale.x+sizeOfEachBlock*(playerInMapZ-1)),Util::nTPointSet(sizeCol,0,sizeCol))))
            noCollsion=false;
    }else if(Util::analyseCollision(tmpRec,Util::getCollisionRectangle(Util::nTPointSet(nTMap::scale.x+sizeOfEachBlock*(playerInMapX),0,nTMap::scale.x+sizeOfEachBlock*(playerInMapZ-1)),Util::nTPointSet(sizeCol,0,sizeCol))))
            noCollsion=false;

    if(playerInMapZ<nTMap::current[0].size()-1){
        if(nTMap::current[playerInMapX][playerInMapZ+1]==1)
        if(Util::analyseCollision(tmpRec,Util::getCollisionRectangle(Util::nTPointSet(nTMap::scale.x+sizeOfEachBlock*(playerInMapX),0,nTMap::scale.x+sizeOfEachBlock*(playerInMapZ+1)),Util::nTPointSet(sizeCol,0,sizeCol))))
            noCollsion=false;
    }else if(Util::analyseCollision(tmpRec,Util::getCollisionRectangle(Util::nTPointSet(nTMap::scale.x+sizeOfEachBlock*(playerInMapX),0,nTMap::scale.x+sizeOfEachBlock*(playerInMapZ+1)),Util::nTPointSet(sizeCol,0,sizeCol))))
            noCollsion=false;
    return noCollsion;
}
void Player::moveFoward(float dir){
    nTPoint tmp=pos;
    tmp.x+=GL::cam.direction.x*nTCamera::steepVar*dir;
    tmp.z+=GL::cam.direction.z*nTCamera::steepVar*dir;
    if(noCollisionWithMap(tmp)||dontGiveAShitForCollision){
        pos=tmp;
        syncWithCamera();
    }
}
void Player::moveSide(int dir){
    nTPoint tmp=pos;
    tmp.x+=sin(GL::cam.rotation.y-M_PI/2)*nTCamera::steepVar*dir;
    tmp.z+=-1*cos(GL::cam.rotation.y-M_PI/2)*nTCamera::steepVar*dir;
    if(noCollisionWithMap(tmp)||dontGiveAShitForCollision){
        pos=tmp;
        syncWithCamera();
    }
}
