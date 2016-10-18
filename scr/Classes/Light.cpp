#include "Light.hpp"

Light::Light() {
};

Light::Light(const Light& orig){
}

Light::~Light(){
}

vector<Lights> Light::currentLights;
vector<GLenum> Light::lightsId;

void Light::addLight(Lights light){
  currentLights.push_back(light);
}

bool Light::deleteLight(string name){
  refreshLightsId();
  for(int pos=0; pos<currentLights.size(); pos++){
    if(currentLights[pos].name==name){
      glDisable(lightsId[pos]);
      currentLights.erase(currentLights.begin()+pos);
      return true;
    }
  }
  return false;
}

void Light::enableLight(string name){
  refreshLightsId();
  for(int pos=0; pos<currentLights.size(); pos++){
    if(currentLights[pos].name==name){
      glShadeModel(GL_SMOOTH);
    glMaterialfv(GL_FRONT,GL_SPECULAR, currentLights[pos].materialEspecular);
    glMateriali(GL_FRONT,GL_SHININESS,currentLights[pos].materialEspec);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, currentLights[pos].ambient);
    glLightfv(lightsId[pos], GL_AMBIENT, currentLights[pos].ambient);
    glLightfv(lightsId[pos], GL_DIFFUSE, currentLights[pos].difuse );
    glLightfv(lightsId[pos], GL_SPECULAR, currentLights[pos].especular );
    glLightfv(lightsId[pos], GL_POSITION, currentLights[pos].position );
    nTColor tmp=GL::getColor();
    GL::setColor(currentLights[pos].color);
    glEnable(GL_COLOR_MATERIAL);
    GL::setColor(tmp);
    glEnable(GL_LIGHTING);
    glEnable(lightsId[pos]);
    glEnable(GL_DEPTH_TEST);
    }
  }
}

void Light::refreshLightsId(){
  lightsId.clear();
  if(GL_MAX_LIGHTS>1)
    lightsId.push_back(GL_LIGHT0);
  else
    return;
  if(GL_MAX_LIGHTS>2)
    lightsId.push_back(GL_LIGHT1);
  else
    return;
  if(GL_MAX_LIGHTS>3)
    lightsId.push_back(GL_LIGHT2);
  else
    return;
  if(GL_MAX_LIGHTS>4)
    lightsId.push_back(GL_LIGHT3);
  else
    return;
  if(GL_MAX_LIGHTS>5)
    lightsId.push_back(GL_LIGHT4);
  else
    return;
  if(GL_MAX_LIGHTS>6)
    lightsId.push_back(GL_LIGHT5);
  else
    return;
  if(GL_MAX_LIGHTS>7)
    lightsId.push_back(GL_LIGHT6);
  else
    return;
  if(GL_MAX_LIGHTS>8)
    lightsId.push_back(GL_LIGHT7);
}

void Light::enableAllLights(){
  refreshLightsId();
  for(int pos=0; pos<currentLights.size(); pos++){  
    glShadeModel(GL_SMOOTH);
    // Define a reflet�ncia do material
    glMaterialfv(GL_FRONT,GL_SPECULAR, currentLights[pos].materialEspecular);
    // Define a concentra��o do brilho
    glMateriali(GL_FRONT,GL_SHININESS,currentLights[pos].materialEspec);
    // Ativa o uso da luz ambiente
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, currentLights[pos].ambient);
    // Define os par�metros da luz de n�mero 0
    glLightfv(lightsId[pos], GL_AMBIENT, currentLights[pos].ambient);
    glLightfv(lightsId[pos], GL_DIFFUSE, currentLights[pos].difuse );
    glLightfv(lightsId[pos], GL_SPECULAR, currentLights[pos].especular );
    glLightfv(lightsId[pos], GL_POSITION, currentLights[pos].position );
    // Habilita a defini��o da cor do material a partir da cor corrente
    nTColor tmp=GL::getColor();
    GL::setColor(currentLights[pos].color);
    glEnable(GL_COLOR_MATERIAL);
    GL::setColor(tmp);
    //Habilita o uso de ilumina��o
    glEnable(GL_LIGHTING);
    // Habilita a luz de n�mero 0
    glEnable(lightsId[pos]);
    // Habilita o depth-buffering
    //glEnable(GL_DEPTH_TEST);
  }
}

void Light::disableAllLights(){
    for(int pos=0; pos<currentLights.size(); pos++){
    glDisable(GL_LIGHTING);
    glDisable(lightsId[pos]);
    //glDisable(GL_DEPTH_TEST);
  }
}

void Light::setAmbientLight(){
    string i;
    Lights l;
    GLfloat a[4], b[4], c[4], d[4], e[4];
    GLint f;
    f=60;
    a[0]=a[1]=a[2]=a[3]=0.5;
    b[0]=b[1]=b[2]=b[3]=0.5;
    c[0]=c[1]=c[2]=c[3]=0.5;
    d[0]=d[1]=d[2]=d[3]=0.5;
    e[0]=e[1]=e[2]=e[3]=0.5;
    char h;
    h=GL_SMOOTH;
    nTColor g;
    g.setColor(0.0, 0.0, 0.0, 1);
    i="Ambiente";

    l.setLight(a,b,c,d,e,f,h,g,i);
    Light::addLight(l);
    Light::enableLight(i);
}