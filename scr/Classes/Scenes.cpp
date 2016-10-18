#include "Scenes.hpp"
#include "AL.hpp"
#include "nTMap.hpp"
#include "../Libs/loadFiles.h"
#include "nTCamera.hpp"
#include "Light.hpp"
#include "Shader.hpp"
#include "Player.hpp"
#include "nTParticle.hpp"
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
bool Scenes::cheatMode=false;
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
    Light::disableFog();
    glutSetCursor(GLUT_CURSOR_INHERIT);
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
void Scenes::drawGame(){
    if(!gameCalled){
        GL::set3D(GL::currentSize.x,GL::currentSize.y);
        setAllCalledFalseExcept(getUnityVector(game));
        GL::isPaused=false;
        vector<int> tmp;
        al->stopAllSoundsExcept(tmp);
        nTMap::scale.setPoint(3,7,0);//350
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
        if(Shader::useShader)Shader::compileShader("shaders/finalVrtx.vs","shaders/textureFrag.vs",true,true,true);     
        char bff[10];
        snprintf(bff,10, "%d", rand()%4);
        string idM(bff);
        al->playSoundByName("music"+idM);
        if(nTParticle::useParticles)nTParticle::init(0);
        if(Light::useFog)Light::setFog();
        if(Light::useLight){Light::setAmbientLight();Light::setPontualLight();}      
   }
    if(GL::isPaused){
        GL::drawPause();
    }else{
        GL::drawHUD();
        GL::cam.behave();
        nTParticle::draw(0);
        if(Shader::isOn)glUseProgram(Shader::shaderID);
        GL::drawSkyBox("SkyBox",Util::nTPointSet(GL::cam.pos.x-GL::defaultSize.z/2,GL::cam.pos.y-GL::defaultSize.z/2,GL::cam.pos.z-GL::defaultSize.z/2),GL::defaultSize.z);
        glUseProgram(0);
        nTMap::draw();
        Player::draw();
        Player::checkGG();
        
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

void scenescbOpt(int x,int y){
    Scenes::current=Scenes::options;
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
    GL::buttonBehave(Util::nTRectangleSet(GL::defaultSize.x/2-GL::defaultSize.x/6,300+110,GL::defaultSize.x/2+GL::defaultSize.x/6,200+110,0.4,0.4),Util::nTColorSet(0.4,0.4,0.4,1),GL::getTextureByName("opcoes"),NULL,*scenescbOpt,NULL,NULL);
    if(GL::buttonBehave(Util::nTRectangleSet(GL::defaultSize.x/2-GL::defaultSize.x/6,300+220,GL::defaultSize.x/2+GL::defaultSize.x/6,200+220,0.4,0.4),Util::nTColorSet(0.4,0.4,0.4,1),GL::getTextureByName("sair"),NULL,NULL,NULL,NULL))
      exit(0);
     if(AL::getSoundState()){
        GL::buttonBehave(Util::nTRectangleSet(700+Scenes::camera.x.movedCam,50+Scenes::camera.y.movedCam,740+Scenes::camera.x.movedCam,10+Scenes::camera.y.movedCam,1,1),Util::nTColorSet(0,0,0,1),GL::getTextureByName("soundOn"),NULL,*modifySound,NULL,NULL);
      }else{
        GL::buttonBehave(Util::nTRectangleSet(700+Scenes::camera.x.movedCam,50+Scenes::camera.y.movedCam,740+Scenes::camera.x.movedCam,10+Scenes::camera.y.movedCam,1,1),Util::nTColorSet(0,0,0,1),GL::getTextureByName("soundOff"),NULL,*modifySound,NULL,NULL);
      }
      if(AL::getMusicState()){
        GL::buttonBehave(Util::nTRectangleSet(640+Scenes::camera.x.movedCam,50+Scenes::camera.y.movedCam,680+Scenes::camera.x.movedCam,10+Scenes::camera.y.movedCam,1,1),Util::nTColorSet(0,0,0,1),GL::getTextureByName("musicOn"),NULL,*modifyMusic,NULL,NULL);
      }else{
        GL::buttonBehave(Util::nTRectangleSet(640+Scenes::camera.x.movedCam,50+Scenes::camera.y.movedCam,680+Scenes::camera.x.movedCam,10+Scenes::camera.y.movedCam,1,1),Util::nTColorSet(0,0,0,1),GL::getTextureByName("musicOff"),NULL,*modifyMusic,NULL,NULL);
      }

}

void scenescbFog(int x,int y){
    Light::useFog=!Light::useFog;
    saveSettings();
}
void scenescbLight(int x,int y){
    Light::useLight=!Light::useLight;
    saveSettings();
}
void scenescbShader(int x,int y){
    Shader::useShader=!Shader::useShader;
    saveSettings();
}
void scenescbPart(int x,int y){
    nTParticle::useParticles=!nTParticle::useParticles;
    saveSettings();
}
void scenescbFs(int x,int y){
    GL::setFullScreen(!GL::getFullScreen());
    saveSettings();
}

void Scenes::drawOptions(){
  if(!optionsCalled){
      GL::set2D(GL::currentSize.x,GL::currentSize.y);
      setAllCalledFalseExcept(getUnityVector(options));
      vector<int> tmp;
      al->stopAllSoundsExcept(tmp);
      al->playSoundByName("menu");
  }
  GL::setFontByIndex(2);
  GL::drawText(Util::nTPointSet(20,20,1),"Se seu PC não suportar o shader, o jogo fechara automaticamente",GL::getColorByName("blue"));
  GL::drawTexture(Util::nTRectangleSet(0,GL::defaultSize.y,GL::defaultSize.x,0,-0.9,-0.9),Util::nTColorSet(1.0,1.0,1.0,1),GL::getTextureByName("mapa"),1);
  if(AL::getSoundState()){
        GL::buttonBehave(Util::nTRectangleSet(700+Scenes::camera.x.movedCam,50+Scenes::camera.y.movedCam,740+Scenes::camera.x.movedCam,10+Scenes::camera.y.movedCam,1,1),Util::nTColorSet(0,0,0,1),GL::getTextureByName("soundOn"),NULL,*modifySound,NULL,NULL);
      }else{
        GL::buttonBehave(Util::nTRectangleSet(700+Scenes::camera.x.movedCam,50+Scenes::camera.y.movedCam,740+Scenes::camera.x.movedCam,10+Scenes::camera.y.movedCam,1,1),Util::nTColorSet(0,0,0,1),GL::getTextureByName("soundOff"),NULL,*modifySound,NULL,NULL);
      }
      if(AL::getMusicState()){
        GL::buttonBehave(Util::nTRectangleSet(640+Scenes::camera.x.movedCam,50+Scenes::camera.y.movedCam,680+Scenes::camera.x.movedCam,10+Scenes::camera.y.movedCam,1,1),Util::nTColorSet(0,0,0,1),GL::getTextureByName("musicOn"),NULL,*modifyMusic,NULL,NULL);
      }else{
        GL::buttonBehave(Util::nTRectangleSet(640+Scenes::camera.x.movedCam,50+Scenes::camera.y.movedCam,680+Scenes::camera.x.movedCam,10+Scenes::camera.y.movedCam,1,1),Util::nTColorSet(0,0,0,1),GL::getTextureByName("musicOff"),NULL,*modifyMusic,NULL,NULL);
      }

    GL::buttonBehave(Util::nTRectangleSet(100,200,350,100,0.4,0.4),Util::nTColorSet(0.4,0.4,0.4,1),GL::getTextureByName("fog"),NULL,*scenescbFog,NULL,NULL);
    GL::buttonBehave(Util::nTRectangleSet(400,200,750,100,0.4,0.4),Util::nTColorSet(0.4,0.4,0.4,1),GL::getTextureByName("light"),NULL,*scenescbLight,NULL,NULL);
    GL::buttonBehave(Util::nTRectangleSet(100,400,350,300,0.4,0.4),Util::nTColorSet(0.4,0.4,0.4,1),GL::getTextureByName("shader"),NULL,*scenescbShader,NULL,NULL);
    GL::buttonBehave(Util::nTRectangleSet(400,400,750,300,0.4,0.4),Util::nTColorSet(0.4,0.4,0.4,1),GL::getTextureByName("particles"),NULL,*scenescbPart,NULL,NULL);
    GL::buttonBehave(Util::nTRectangleSet(100,590,350,490,0.4,0.4),Util::nTColorSet(0.4,0.4,0.4,1),GL::getTextureByName("fs"),NULL,*scenescbFs,NULL,NULL);
  if(!Light::useFog)GL::drawTexture(Util::nTRectangleSet(100,200,350,100,0.5,0.5),GL::getColorByName("white"),GL::getTextureByName("X"),1);
  if(!Light::useLight)GL::drawTexture(Util::nTRectangleSet(400,100,750,200,0.5,0.5),GL::getColorByName("white"),GL::getTextureByName("X"),1);
  if(!Shader::useShader)GL::drawTexture(Util::nTRectangleSet(100,300,350,400,0.5,0.5),GL::getColorByName("white"),GL::getTextureByName("X"),1);
  if(!nTParticle::useParticles)GL::drawTexture(Util::nTRectangleSet(400,300,750,400,0.5,0.5),GL::getColorByName("white"),GL::getTextureByName("X"),1);
  if(!GL::getFullScreen())GL::drawTexture(Util::nTRectangleSet(100,590,350,490,0.5,0.5),GL::getColorByName("white"),GL::getTextureByName("X"),1);
  if(GL::buttonBehave(Util::nTRectangleSet(GL::defaultSize.x/2+GL::defaultSize.x/6,300+220,GL::defaultSize.x/2+2*GL::defaultSize.x/6,200+220,0.4,0.4),Util::nTColorSet(0.4,0.4,0.4,1),GL::getTextureByName("voltar"),NULL,NULL,NULL,NULL))
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
        al->playSoundByName("menu");
  }
  GL::drawTexture(Util::nTRectangleSet(0,GL::defaultSize.y,GL::defaultSize.x,0,-0.9,-0.9),Util::nTColorSet(1.0,1.0,1.0,1),GL::getTextureByName("mapa"),1);
  if(GL::buttonBehave(Util::nTRectangleSet(GL::defaultSize.x/2-GL::defaultSize.x/3,200,GL::defaultSize.x/2-GL::defaultSize.x/6,100,0.4,0.4),Util::nTColorSet(0.4,0.4,0.4,1),GL::getTextureByName("normal"),NULL,NULL,NULL,NULL)){
    Scenes::current=game;
    nTMap::isProcedural=false;
  }
  if(GL::buttonBehave(Util::nTRectangleSet(GL::defaultSize.x/2+GL::defaultSize.x/10,200,GL::defaultSize.x/2+GL::defaultSize.x/3,100,0.4,0.4),Util::nTColorSet(0.4,0.4,0.4,1),GL::getTextureByName("procedural"),NULL,NULL,NULL,NULL)){
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
  GL::setFontByIndex(0);
  GL::drawText(Util::nTPointSet(20,300,0.9),"Parabens vc é deslabirintizador :D",GL::getColorByName("white"));
   if(GL::buttonBehave(Util::nTRectangleSet(GL::defaultSize.x/2+GL::defaultSize.x/6,300+220,GL::defaultSize.x/2+2*GL::defaultSize.x/6,200+220,0.4,0.4),Util::nTColorSet(0.4,0.4,0.4,1),GL::getTextureByName("voltar"),NULL,NULL,NULL,NULL))
    Scenes::current=menu;
}

void Scenes::drawYouWin(){
  if(!posYouWinCalled){
      setAllCalledFalseExcept(getUnityVector(posYouWin));
      vector<int> tmp;
      al->stopAllSoundsExcept(tmp);
  }
}
