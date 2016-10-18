#include "Scenes.hpp"
#include "AL.hpp"
#include "nTMap.hpp"
#include "../Libs/loadFiles.h"
#include "nTCamera.hpp"
#include "Light.hpp"
#include "Shader.hpp"
#include "Player.hpp"
Scenes::Scenes() {
}

Scenes::Scenes(const Scenes& orig) {
}

Scenes::~Scenes() {
}

int Scenes::current=-565;
const int Scenes::mapEdit=98746;
const int Scenes::menu=-565;
const int Scenes::game=6516;
const int Scenes::credits=12;
const int Scenes::splash=300;
const int Scenes::options=91;
const int Scenes::preGame=-144;
const int Scenes::preCampaign=-9;
const int Scenes::preFreeMode=-551;
const int Scenes::posGame=78;
const int Scenes::posGameEnd=69;
const int Scenes::posYouWin=7454;
bool Scenes::menuCalled=false;
bool Scenes::gameCalled=false;
bool Scenes::mapEditCalled=false;
bool Scenes::splashCalled=false;
bool Scenes::optionsCalled=false;
bool Scenes::creditsCalled=false;
bool Scenes::preGameCalled=false;
bool Scenes::preCampaignCalled=false;
bool Scenes::preFreeModeCalled=false;
bool Scenes::freeGameMode=false;
bool Scenes::skipScene=false;
bool Scenes::posGameCalled=false;
bool Scenes::posGameEndCalled=false;
bool Scenes::posYouWinCalled=false;

bool Scenes::freeCam=false;
Camera Scenes::camera;

vector<int> Scenes::getUnityVector(int value){
    vector<int> out;
    out.push_back(value);
    return out;
}

void Scenes::putCameraOnOrigin(){
    gluLookAt(camera.x.movedCam*-1,camera.y.movedCam*-1,0,camera.x.movedCam*-1,camera.y.movedCam*-1,-1,0,1,0);
    camera.x.movedCam=0;
    camera.y.movedCam=0;
    gluLookAt(0,0,0,0,0,-1,0,1,0);
}

void Scenes::lookAt(nTPoint pos){
    pos.x-=GL::defaultSize.x/2+camera.x.movedCam;
    pos.y-=GL::defaultSize.y/2+camera.y.movedCam;
    gluLookAt(pos.x,pos.y,0,pos.x,pos.y,-1,0,1,0);
    camera.x.movedCam+=pos.x;
    camera.y.movedCam+=pos.y;
    gluLookAt(0,0,0,0,0,-1,0,1,0);
}

bool Scenes::isInTheScreen(nTRectangle collision){
    bool out=false;
    nTPoint offSet;
    offSet.setPoint(GL::defaultSize.x*0.2,GL::defaultSize.y*0.3,0);
    offSet.setPoint(0,0,0);
    if(camera.x.movedCam-offSet.x<=collision.p0.x&&collision.p0.x<GL::defaultSize.x+camera.x.movedCam+offSet.x)
        if(camera.y.movedCam-offSet.y<=collision.p0.y&&collision.p0.y<GL::defaultSize.y+camera.y.movedCam+offSet.y)
            out=true;
    if(camera.x.movedCam-offSet.x<=collision.p1.x&&collision.p1.x<GL::defaultSize.x+camera.x.movedCam+offSet.x)
        if(camera.y.movedCam-offSet.y<=collision.p0.y&&collision.p0.y<GL::defaultSize.y+camera.y.movedCam+offSet.y)
            out=true;
    if(camera.x.movedCam-offSet.x<=collision.p1.x&&collision.p1.x<GL::defaultSize.x+camera.x.movedCam+offSet.x)
        if(camera.y.movedCam-offSet.y<=collision.p1.y&&collision.p1.y<GL::defaultSize.y+camera.y.movedCam+offSet.y)
            out=true;
    if(camera.x.movedCam-offSet.x<=collision.p0.x&&collision.p0.x<GL::defaultSize.x+camera.x.movedCam+offSet.x)
        if(camera.y.movedCam-offSet.y<=collision.p1.y&&collision.p1.y<GL::defaultSize.y+camera.y.movedCam+offSet.y)
            out=true;
    return out;
}

void Scenes::setAllCalledFalseExcept(vector<int> except){
    gameCalled=false;
    mapEditCalled=false;
    menuCalled=false;
    splashCalled=false;
    optionsCalled=false;
    creditsCalled=false;
    preGameCalled=false;
    preCampaignCalled=false;
    preFreeModeCalled=false;
    posGameCalled=false;
    posGameEndCalled=false;
    posYouWinCalled=false;
    Light::disableAllLights();

    GL::framesInGame=0;
    putCameraOnOrigin();
    for(int i=0;i<except.size();i++)
        switch(except[i]){
            case game: gameCalled=true; break;
            case Scenes::mapEdit: mapEditCalled=true; break;
            case menu: menuCalled=true; break;
            case splash:  splashCalled=true;break;
            case options: optionsCalled=true;break;
            case credits: creditsCalled=true;break;
            case preGame: preGameCalled=true;break;
            case preCampaign: preCampaignCalled=true;break;
            case preFreeMode: preFreeModeCalled=true;break;
            case posGame: posGameCalled=true;break;
            case posGameEnd: posGameEndCalled=true;break;
            case posYouWin: posYouWinCalled=true;break;
        }
}
nTMesh jugg;
nTShape obj;
void Scenes::drawGame(){
    if(!gameCalled){
        GL::set3D(GL::currentSize.x,GL::currentSize.y);
        setAllCalledFalseExcept(getUnityVector(game));
        GL::isPaused=false;
        vector<int> tmp;
        al->stopAllSoundsExcept(tmp);
        nTMap::scale.setPoint(3,5,0);
        if(nTMap::isProcedural)
            nTMap::genMap();
        else{
            char bffs[10];
            snprintf(bffs,10, "%d", nTMap::currentMap);
            string idxMa(bffs);
            nTMap::loadmap("maps/mapa"+idxMa+".png");
        }
        GL::cam.rotation.y=M_PI/2;
        GL::cam.direction.x=sin(GL::cam.rotation.y);
        GL::cam.direction.z=-1*cos(GL::cam.rotation.y);
        Light::setAmbientLight();
        Shader::compileShader("shaders/shadowVrtx.vs","shaders/shadowFrag.vs",true,true,true);     
        glutSetCursor(GLUT_CURSOR_NONE);
        jugg=GL::loadObjCplx("obj/juggernault.obj");//("obj/juggernault.obj"); ("obj/cube.obj");
        obj=GL::loadObj("obj/cube.obj");
        nTMap::draw();
        Player::spawn(nTMap::begin);
        char bff[10];
        snprintf(bff,10, "%d", rand()%4);
        string idM(bff);
        al->playSoundByName("music"+idM);
   }
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(GL::cam.pos.x, GL::cam.pos.y, GL::cam.pos.z, GL::cam.pos.x+GL::cam.direction.x, GL::cam.pos.y+GL::cam.direction.y, GL::cam.pos.z+GL::cam.direction.z, 0, 1, 0);
    //if(fmod(GL::framesInGame,GL::getFPS())==0)cout<<GL::cam;
    glUseProgram(Shader::shaderID);
        //DRAW  shader stuff
    if(!Shader::isOn)glUseProgram(0);

    GL::drawObj(jugg,GL::getColorByName("red"));
    GL::drawShapeObj(obj,GL::getColorByName("red"));
    
    nTMap::draw();
    Player::draw();
    GL::drawSkyBox("SkyBox",Util::nTPointSet(GL::cam.pos.x-GL::defaultSize.z/2,GL::cam.pos.y-GL::defaultSize.z/2,GL::cam.pos.z-GL::defaultSize.z/2),GL::defaultSize.z);
    if(GL::isPaused){
        GL::drawPause();
    }else{
        GL::drawHUD();
    }
}

void Scenes::drawMapEdit(){
    if(!mapEditCalled){
        setAllCalledFalseExcept(getUnityVector(Scenes::mapEdit));
        vector<int> tmp;
        tmp.push_back(AL::getSoundByName("menu"));
        al->stopAllSoundsExcept(tmp);
    }
}

void Scenes::drawMenu(){
    if(!menuCalled){
        GL::set2D(GL::currentSize.x,GL::currentSize.y);
        setAllCalledFalseExcept(getUnityVector(menu));
        vector<int> tmp;
        //tmp.push_back(AL::getSoundByName("menuSong"));
        al->stopAllSoundsExcept(tmp);
        al->playSoundByName("menu");
    }
    GL::drawTexture(Util::nTRectangleSet(0,GL::defaultSize.y,GL::defaultSize.x,0,-0.9,-0.9),Util::nTColorSet(1.0,1.0,1.0,1),GL::getTextureByName("mapa"),1);
    GL::buttonBehave(Util::nTRectangleSet(GL::defaultSize.x/2-GL::defaultSize.x/3,200,GL::defaultSize.x/2+GL::defaultSize.x/3,10,0.4,0.4),Util::nTColorSet(0.4,0.4,0.4,1),GL::getTextureByName("LIDs"),NULL,NULL,NULL,NULL);
    if(GL::buttonBehave(Util::nTRectangleSet(GL::defaultSize.x/2-GL::defaultSize.x/6,300,GL::defaultSize.x/2+GL::defaultSize.x/6,200,0.4,0.4),Util::nTColorSet(0.4,0.4,0.4,1),GL::getTextureByName("jogar"),NULL,NULL,NULL,NULL))
      Scenes::current=preCampaign;
    if(GL::buttonBehave(Util::nTRectangleSet(GL::defaultSize.x/2-GL::defaultSize.x/6,300+110,GL::defaultSize.x/2+GL::defaultSize.x/6,200+110,0.4,0.4),Util::nTColorSet(0.4,0.4,0.4,1),GL::getTextureByName("opcoes"),NULL,NULL,NULL,NULL))
      Scenes::current=options;
    if(GL::buttonBehave(Util::nTRectangleSet(GL::defaultSize.x/2-GL::defaultSize.x/6,300+220,GL::defaultSize.x/2+GL::defaultSize.x/6,200+220,0.4,0.4),Util::nTColorSet(0.4,0.4,0.4,1),GL::getTextureByName("sair"),NULL,NULL,NULL,NULL))
      exit(0);

}

void Scenes::drawOptions(){
  if(!optionsCalled){
      GL::set2D(GL::currentSize.x,GL::currentSize.y);
      setAllCalledFalseExcept(getUnityVector(options));
      vector<int> tmp;
      al->stopAllSoundsExcept(tmp);
  }
  GL::drawTexture(Util::nTRectangleSet(0,GL::defaultSize.y,GL::defaultSize.x,0,-0.9,-0.9),Util::nTColorSet(1.0,1.0,1.0,1),GL::getTextureByName("mapa"),1);
  if(GL::buttonBehave(Util::nTRectangleSet(GL::defaultSize.x/2-GL::defaultSize.x/6,200,GL::defaultSize.x/2-GL::defaultSize.x/8,100,0.4,0.4),Util::nTColorSet(0.4,0.4,0.4,1),GL::getTextureByName("paraboloide hiperbolico<3"),NULL,NULL,NULL,NULL))
    {}
  if(GL::buttonBehave(Util::nTRectangleSet(GL::defaultSize.x/2+GL::defaultSize.x/6 -(GL::defaultSize.x/6 -GL::defaultSize.x/8),200,GL::defaultSize.x/2+GL::defaultSize.x/6 ,100,0.4,0.4),Util::nTColorSet(0.4,0.4,0.4,1),GL::getTextureByName("paraboloide hiperbolico<3"),NULL,NULL,NULL,NULL))
    {}
  if(GL::buttonBehave(Util::nTRectangleSet(GL::defaultSize.x/2-GL::defaultSize.x/3+(GL::defaultSize.x/8-GL::defaultSize.x/6),200,GL::defaultSize.x/2-GL::defaultSize.x/3,100,0.4,0.4),Util::nTColorSet(0.4,0.4,0.4,1),GL::getTextureByName("paraboloide hiperbolico<3"),NULL,NULL,NULL,NULL))
    {}
  if(GL::buttonBehave(Util::nTRectangleSet(GL::defaultSize.x/2+GL::defaultSize.x/6 -(GL::defaultSize.x/6 -GL::defaultSize.x/8)+6*(GL::defaultSize.x/8-GL::defaultSize.x/6),200,GL::defaultSize.x/2+GL::defaultSize.x/6+6*(GL::defaultSize.x/8-GL::defaultSize.x/6) ,100,0.4,0.4),Util::nTColorSet(0.4,0.4,0.4,1),GL::getTextureByName("paraboloide hiperbolico<3"),NULL,NULL,NULL,NULL))
    {}
  if(GL::buttonBehave(Util::nTRectangleSet(GL::defaultSize.x/2+GL::defaultSize.x/6,300+220,GL::defaultSize.x/2+2*GL::defaultSize.x/6,200+220,0.4,0.4),Util::nTColorSet(0.4,0.4,0.4,1),GL::getTextureByName("paraboloide hiperbolico<3"),NULL,NULL,NULL,NULL))
    Scenes::current=menu;
}

void Scenes::drawCredits(){
  if(!creditsCalled){
      setAllCalledFalseExcept(getUnityVector(credits));
      vector<int> tmp;
      al->stopAllSoundsExcept(tmp);
  }

}

void Scenes::drawSplash(){
    if(!splashCalled){
        setAllCalledFalseExcept(getUnityVector(splash));
        vector<int> tmp;
        al->stopAllSoundsExcept(tmp);
    }

}

void Scenes::drawPreCampaign(){
  if(!preCampaignCalled){
      GL::set2D(GL::currentSize.x,GL::currentSize.y);
      setAllCalledFalseExcept(getUnityVector(preCampaign));
      freeGameMode=false;
      vector<int> tmp;
        al->stopAllSoundsExcept(tmp);
  }
  GL::drawTexture(Util::nTRectangleSet(0,GL::defaultSize.y,GL::defaultSize.x,0,-0.9,-0.9),Util::nTColorSet(1.0,1.0,1.0,1),GL::getTextureByName("mapa"),1);
  if(GL::buttonBehave(Util::nTRectangleSet(GL::defaultSize.x/2-GL::defaultSize.x/3,200,GL::defaultSize.x/2-GL::defaultSize.x/6,100,0.4,0.4),Util::nTColorSet(0.4,0.4,0.4,1),GL::getTextureByName("normal"),NULL,NULL,NULL,NULL)){
    Scenes::current=game;
    nTMap::isProcedural=false;
  }
  if(GL::buttonBehave(Util::nTRectangleSet(GL::defaultSize.x/2+GL::defaultSize.x/6,200,GL::defaultSize.x/2+GL::defaultSize.x/3,100,0.4,0.4),Util::nTColorSet(0.4,0.4,0.4,1),GL::getTextureByName("procedural"),NULL,NULL,NULL,NULL)){
      Scenes::current=game;
        nTMap::isProcedural=true;
  }
  if(GL::buttonBehave(Util::nTRectangleSet(GL::defaultSize.x/2+GL::defaultSize.x/6,300+220,GL::defaultSize.x/2+2*GL::defaultSize.x/6,200+220,0.4,0.4),Util::nTColorSet(0.4,0.4,0.4,1),GL::getTextureByName("voltar"),NULL,NULL,NULL,NULL))
    Scenes::current=menu;

}


void Scenes::drawPreFreeMode(){
  if(!preFreeModeCalled){
      setAllCalledFalseExcept(getUnityVector(preFreeMode));
      freeGameMode=true;
      vector<int> tmp;
      tmp.push_back(AL::getSoundByName("menu"));
      al->stopAllSoundsExcept(tmp);
  }
}

void Scenes::drawPreGame(){
  if(!preGameCalled){
      setAllCalledFalseExcept(getUnityVector(preGame));
      vector<int> tmp;
      tmp.push_back(AL::getSoundByName("menu"));
      al->stopAllSoundsExcept(tmp);
  }
}

void Scenes::drawPosGame(){
  if(!posGameCalled){
      setAllCalledFalseExcept(getUnityVector(posGame));
      vector<int> tmp;
      al->stopAllSoundsExcept(tmp);
  }
}

void Scenes::drawEndGame(){
  if(!posGameEndCalled){
      setAllCalledFalseExcept(getUnityVector(posGameEnd));
      vector<int> tmp;
      al->stopAllSoundsExcept(tmp);
  }
}

void Scenes::drawYouWin(){
  if(!posYouWinCalled){
      setAllCalledFalseExcept(getUnityVector(posYouWin));
      vector<int> tmp;
      al->stopAllSoundsExcept(tmp);
  }
}
