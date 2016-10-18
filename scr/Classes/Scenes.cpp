#include "Scenes.hpp"
#include "AL.hpp"
#include "Map.hpp"
#include "../Libs/loadFiles.h"
#include "nTCamera.hpp"
#include "Light.hpp"
#include "Shader.hpp"
Scenes::Scenes() {
}

Scenes::Scenes(const Scenes& orig) {
}

Scenes::~Scenes() {
}

int Scenes::current=6516;
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

void Scenes::drawGame(){
    if(!gameCalled){
        setAllCalledFalseExcept(getUnityVector(game));
        GL::isPaused=false;
        vector<int> tmp;
        al->stopAllSoundsExcept(tmp);
        Map::loadmap("mapa.png");
        GL::cam.rotation.y=M_PI/2;
        GL::cam.direction.x=sin(GL::cam.rotation.y);
        GL::cam.direction.z=-1*cos(GL::cam.rotation.y);
        Light::setAmbientLight();
        Shader::compileShader("shaders/vertex2.vs","shaders/fragment2.vs",true,true,false);
   }
    //glUseProgram(Shader::shaderID);
    //cout<<GL::cam;
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(GL::cam.pos.x, GL::cam.pos.y, GL::cam.pos.z, GL::cam.pos.x+GL::cam.direction.x, GL::cam.pos.y+GL::cam.direction.y, GL::cam.pos.z+GL::cam.direction.z, 0, 1, 0);
    Map::scale.setPoint(3,5,0);
    
    //chao
    GL::drawRectangle(Util::nTRectangleSet(-1,-1,Map::current.size()*Map::scale.x,-1,-1,Map::current[0].size()*Map::scale.x),GL::getColorByName("green"));
    
    for (int i=0;i<Map::current.size();i++)
        for (int j=0;j<Map::current[i].size();j++){
            if (Map::current[i][j]==1){
                glPushMatrix();               
                GL::setColor(GL::getColorByName("violet"));  
                glScalef (Map::scale.x,Map::scale.y,Map::scale.x); 
                glTranslatef(i*Map::scale.x,0,j*Map::scale.x);      
                if(Map::fillWalls){
                    if(Map::useWallTexture){
                        GL::bindTexture("paraboloide hiperbolico<3");
                        GL::drawCube(Map::scale.x,GL_QUADS);
                        GL::unbindTexture();
                    }else
                        glutSolidCube(Map::scale.x);
                }else
                    glutWireCube(Map::scale.x);
                
                glPopMatrix();
            }
        }   
    nTPoint SB;
    SB.setPoint(GL::cam.pos.x-GL::defaultSize.z/2,GL::cam.pos.y-GL::defaultSize.z/2,GL::cam.pos.z-GL::defaultSize.z/2);
    GL::drawSkyBox("SkyBox",SB,GL::defaultSize.z);
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
        tmp.push_back(AL::getSoundByName("menuSong"));
        al->stopAllSoundsExcept(tmp);
    }    
}

void Scenes::drawMenu(){
    if(!menuCalled){
        setAllCalledFalseExcept(getUnityVector(menu));
        vector<int> tmp;
        tmp.push_back(AL::getSoundByName("menuSong"));
        al->stopAllSoundsExcept(tmp);
    }
}

void Scenes::drawOptions(){

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
      setAllCalledFalseExcept(getUnityVector(preCampaign));
      freeGameMode=false;
      vector<int> tmp;
        al->stopAllSoundsExcept(tmp);
  }

}
void Scenes::drawPreFreeMode(){
  if(!preFreeModeCalled){
      setAllCalledFalseExcept(getUnityVector(preFreeMode));
      freeGameMode=true;
      vector<int> tmp;
      tmp.push_back(AL::getSoundByName("menuSong"));
      al->stopAllSoundsExcept(tmp);
  }
}

void Scenes::drawPreGame(){
  if(!preGameCalled){
      setAllCalledFalseExcept(getUnityVector(preGame));
      vector<int> tmp;
      tmp.push_back(AL::getSoundByName("menuSong"));
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