#include "GL.hpp"
#include "../Libs/GL.Callbacks.h"
#include "nTCamera.hpp"
#include "Map.hpp"

GL::GL(string name,float FPS,GLbitfield displayMode,nTPoint defaultsize,bool blend,vector<GLenum>&enables,int argc, char** argv) {
    this->FPS=FPS;
    srand(time(NULL));
    glutInit(&argc, argv);
    glutInitDisplayMode(displayMode);
    defaultSize.setPoint(defaultsize.x,defaultsize.y,defaultsize.z);
    glutInitWindowSize(defaultsize.x, defaultsize.y);
    currentSize=defaultsize;
    glutInitWindowPosition(0, 0);
    glutCreateWindow(name.c_str());
    glewInit();
    for(GLenum en:enables)
        glEnable(en);
    if(blend)
        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glutSpecialUpFunc(specialKeyboardUp);
    glutKeyboardUpFunc(keyboardUp);
    glutSpecialFunc(specialKeyboard);
    glutDisplayFunc(drawScene);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, update, -1);
    glutPassiveMotionFunc(mousePassiveMotion);
    glutMotionFunc(mouseActiveMotion);
    glutMouseFunc(mousePress);
    glClearColor(clearcolor.R, clearcolor.G, clearcolor.B, clearcolor.A);
    nTCamera camera;
    cam=camera;
    Map::scale.setPoint(1,3,1);
};

GL::GL(const GL& orig) {
}

GL::~GL() {
}

float GL::FPS;
nTColor GL::currentColor=Util::nTColorSet(1,1,1,1);
void* GL::currentFont=GLUT_BITMAP_TIMES_ROMAN_24;
vector<GLuint> GL::textures;
vector<string> GL::textureNames;
nTPoint GL::defaultSize;
nTPoint GL::currentSize;
bool GL::isPaused=false;
bool GL::leftMouseClicked=false;
bool GL::leftMouseReleased=false;
bool GL::rightMouseClicked=false;
bool GL::rightMouseReleased=false;
nTColor GL::clearcolor;
unsigned long int GL::framesInGame=0;
nTPoint GL::mousePos;
nTPoint GL::rawMousePos;
GLuint GL::GLlist=glGenLists(1);
nTCamera GL::cam;
nTColor GL::bindColor;


void GL::setFPS(float FPS){
    GL::FPS=FPS;
}
float GL::getFPS(){
    return FPS;
}
float GL::getMs(){
    return 1000/FPS;
}
void GL::start(){
    glutMainLoop();
}
GLuint GL::loadTexture(string name,char* path){
    GLuint temp=SOIL_load_OGL_texture(Util::newPath(path),SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);
    if(temp==0){
        if(Util::DEBUG) cout<<"falha ao carregar textura "<<Util::newPath(path)<<endl;
    }else{
        int i=0;
        for(string n:textureNames)
            if(n==name){
                textures[i]=temp;
                return temp;
            }else
                i++;
        textures.push_back(temp);
        textureNames.push_back(name);
    }
    return temp;
}
vector<GLuint> GL::loadTextures(string name,int nOfTex,char* path){
    vector<GLuint> tex;
    GLuint tmp;
    for(int i=0;i<nOfTex;i++){
        tmp=SOIL_load_OGL_texture(Util::newPath(Util::getDinamicPath(path,i,".png")),SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);
        if(tmp){
            tex.push_back(NULL);
            tex[i]=tmp;
            textures.push_back(tex[i]);
            textureNames.push_back(Util::getDinamicPath((char*)name.c_str(),i,""));
        }else{if(Util::DEBUG)cout<<"erro em:"<<path<<endl;}
    }
    return tex;
}
void GL::setColor(nTColor color){
    GL::currentColor=color;
    glColor4f(color.R,color.G,color.B,color.A);
}
nTColor GL::getColor(){
    return GL::currentColor;
}
void GL::drawRectangle(nTRectangle pos,nTColor color){
    nTColor tmp=getColor();
    setColor(color);
    glBegin(GL_POLYGON);
        glVertex3f(pos.p0.x, pos.p0.y, pos.p0.z);
        glVertex3f(pos.p1.x, pos.p0.y, pos.p0.z);
        glVertex3f(pos.p1.x, pos.p1.y, pos.p0.z);
        glVertex3f(pos.p0.x, pos.p1.y, pos.p0.z);
    glEnd();
    setColor(tmp);
}
void GL::drawTexture(nTRectangle pos,nTColor color,GLuint tex,int Orientation){
    nTColor tmp=getColor();
    setColor(color);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex);
    glBegin(GL_QUADS);
    if(Orientation<=0){
      glTexCoord2f(1, 0); glVertex3f(pos.p0.x, pos.p0.y, pos.p0.z);
      glTexCoord2f(0, 0); glVertex3f(pos.p1.x, pos.p0.y, pos.p0.z);
      glTexCoord2f(0, 1); glVertex3f(pos.p1.x, pos.p1.y, pos.p0.z);
      glTexCoord2f(1, 1); glVertex3f(pos.p0.x, pos.p1.y, pos.p0.z);
    }else if(Orientation==1){
      glTexCoord2f(0, 0); glVertex3f(pos.p0.x, pos.p0.y, pos.p0.z);
      glTexCoord2f(1, 0); glVertex3f(pos.p1.x, pos.p0.y, pos.p0.z);
      glTexCoord2f(1, 1); glVertex3f(pos.p1.x, pos.p1.y, pos.p0.z);
      glTexCoord2f(0, 1); glVertex3f(pos.p0.x, pos.p1.y, pos.p0.z);
    }else if(Orientation==2){
      glTexCoord2f(0, 0); glVertex3f(pos.p0.x, pos.p0.y, pos.p0.z);
      glTexCoord2f(0, 1); glVertex3f(pos.p1.x, pos.p0.y, pos.p0.z);
      glTexCoord2f(1, 1); glVertex3f(pos.p1.x, pos.p1.y, pos.p0.z);
      glTexCoord2f(1, 0); glVertex3f(pos.p0.x, pos.p1.y, pos.p0.z);
    }else if(Orientation==3){
      glTexCoord2f(1, 0); glVertex3f(pos.p0.x, pos.p0.y, pos.p0.z);
      glTexCoord2f(1, 1); glVertex3f(pos.p1.x, pos.p0.y, pos.p0.z);
      glTexCoord2f(0, 1); glVertex3f(pos.p1.x, pos.p1.y, pos.p0.z);
      glTexCoord2f(0, 0); glVertex3f(pos.p0.x, pos.p1.y, pos.p0.z);
    }
    glEnd();
    glDisable(GL_TEXTURE_2D);
    setColor(tmp);
}
nTColor GL::getColorByName(string name){
  float R=1,G=1,B=1;
  if(name=="black"){
      R=0;G=0;B=0;
  }else if(name=="red"){
      R=1;G=0;B=0;
  }else if(name=="green"){
      R=0;G=1;B=0;
  }else if(name=="blue"){
      R=0;G=0;B=1;
  }else if(name=="yellow"){
      R=1;G=1;B=0;
  }else if(name=="magenta"){
      R=1;G=0;B=1;
  }else if(name=="cyan"){
      R=0;G=1;B=1;
  }else if(name=="grey"){
      R=0.6;G=0.6;B=0.6;
  }else if(name=="violet"){   //mish 153,112,205 ou 99,70,CD
      R=0.6;G=0.4392156;B=0.80392156;
  }else if(name=="white"){
      R=1;G=1;B=1;
  }else if(name=="mouseSelected"){
      R=0.3;G=0.3;B=0.3;
  }
  return Util::nTColorSet(R,G,B,1);
}
void GL::setFont(void* font){
    currentFont=font;
}
void GL::setFontByIndex(int idx){
    switch(idx){
        default:
            setFont(GLUT_BITMAP_TIMES_ROMAN_24);
        break;
        case 1:
            setFont(GLUT_BITMAP_8_BY_13);
        break;
        case 2:
            setFont(GLUT_BITMAP_9_BY_15);
        break;
        case 3:
            setFont(GLUT_BITMAP_TIMES_ROMAN_10);
        break;
        case 4:
            setFont(GLUT_BITMAP_HELVETICA_10);
        break;
        case 5:
            setFont(GLUT_BITMAP_HELVETICA_12);
        break;
        case 6:
            setFont(GLUT_BITMAP_HELVETICA_18);
        break;
    }
}
void GL::drawText(nTPoint point,char* text,nTColor color){
    nTColor tmp2=getColor();
    setColor(color);
    string tmp(text);
    glRasterPos3f(point.x,point.y,point.z);
    for (int i = 0;i<tmp.size();i++) {
       glutBitmapCharacter(currentFont, text[i]);
    }
    setColor(tmp2);
}
void GL::drawPolygon(nTPoint point,float radius,int edges){
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(point.x,point.y,point.z);
    for(int i=0;i<=edges;i++){
        glVertex3f(cos(2*M_PI*i/edges)*radius+point.x,sin(2*M_PI*i/edges)*radius+point.y,point.z);
    }
    glEnd();
}
GLuint GL::getTextureByName(string name){
    int i=0;
    for(string n:textureNames)
        if(n==name){
            return textures[i];
        }else
            i++;
    return 0;
}

void GL::bindTexture(string name){
    bindColor=getColor();
    setColor(GL::getColorByName("white"));
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, getTextureByName(name));
}

void GL::unbindTexture(){
    glDisable(GL_TEXTURE_2D);
    setColor(bindColor);
}

vector<GLuint> GL::getTexturesByName(string name,int nOfTex){
    vector<GLuint> out;
    GLuint tmp;
    for(int i=0;i<nOfTex;i++){
        tmp=getTextureByName(Util::getDinamicPath((char*)name.c_str(),i,""));
        if(tmp)
            out.push_back(tmp);
    }
    return out;
}
bool GL::buttonBehave(nTRectangle collision,nTColor pressedColor,GLuint tex,void(*clickFunction)(int,int),void(*releaseFunction)(int,int),void(*RclickFunction)(int,int),void(*RreleaseFunction)(int,int)){
    if(GL::mousePos.x>=collision.p0.x&&GL::mousePos.x<=collision.p1.x&&((GL::mousePos.y>=collision.p0.y&&GL::mousePos.y<=collision.p1.y)||(GL::mousePos.y>=collision.p1.y&&GL::mousePos.y<=collision.p0.y))){
        if(tex)
            GL::drawTexture(collision,pressedColor,tex,1);
        else
            GL::drawRectangle(collision,pressedColor);
        if(GL::leftMouseReleased||GL::rightMouseReleased)
            al->playSoundByName("mouse");
        if(GL::leftMouseClicked){
            if(*clickFunction!=NULL){
                clickFunction((int)GL::mousePos.x,(int)GL::mousePos.y);
            }
            return true;
        }else if(GL::leftMouseReleased){
            if(*releaseFunction!=NULL){
                 releaseFunction((int)GL::mousePos.x,(int)GL::mousePos.y);
            }
            GL::leftMouseReleased=0;
            return true;
        }
        if(GL::rightMouseClicked){
            if(*RclickFunction!=NULL){
                RclickFunction((int)GL::mousePos.x,(int)GL::mousePos.y);
            }
            return false;
        }else if(GL::rightMouseReleased){
            if(*RreleaseFunction!=NULL){
                 RreleaseFunction((int)GL::mousePos.x,(int)GL::mousePos.y);
            }
            GL::rightMouseReleased=0;
            return false;
        }
    }else{
        if(tex)
            GL::drawTexture(collision,GL::getColorByName("white"),tex,1);
    }
    return false;
}
ostream& operator<<(ostream &strm, const GL &gl) {
    if(Util::DEBUG)
        return strm <<"GL:["<<"FPS("<<gl.FPS<<"),"<<"CurrentColor("<<"R:"<<gl.currentColor.R<<" G:"<<gl.currentColor.G<<" B:"<<gl.currentColor.B<<" A:"<<gl.currentColor.A<<"),"<<
                "Loaded Texutres("<<gl.textures.size()<<"),"<<"ScreenSize("<<"x:"<<gl.currentSize.x<<" y:"<<gl.currentSize.y<<"),"<<"Is paused("<<gl.isPaused<<"),"<<"]\n";
    return strm;
}

void modifySound(int a,int b){
  AL::setSoundState(!AL::getSoundState());
  saveSettings();
}

void modifyMusic(int a,int b){
  AL::setMusicState(!AL::getMusicState());
  saveSettings();
}

vector<vector<float> > GL::getRotateMatrix(float angle){
    float rad=Util::angleToRad(angle);
    vector<vector<float> >out;
    out.resize(4,vector<float>(4,0));
    out[0][0]=cos(rad);
    out[0][1]=-sin(rad);
    out[1][1]=cos(rad);
    out[1][0]=sin(rad);
    out[2][2]=1;
    out[3][3]=1;
    return out;
}

vector<vector<float> > GL::getTranslateMatrix(nTPoint point){
    vector<vector<float> >out;
    out.resize(4,vector<float>(4,0));
    out[0][0]=1;
    out[1][1]=1;
    out[2][2]=1;
    out[3][3]=1;

    out[0][3]=point.x;
    out[1][3]=point.y;
    out[2][3]=point.z;
    return out;
}

nTPoint GL::rotatePoint(nTPoint point,nTPoint center, float angle){
    vector<vector<float> >Mpoint;
    Mpoint.resize(4,vector<float>(1,0));
    Mpoint[0][0]=point.x;
    Mpoint[1][0]=point.y;
    Mpoint[2][0]=point.z;
    Mpoint[3][0]=1;

    center.setPoint(-center.x,-center.y,-center.z);
    Mpoint=Util::multiplyMatrix(GL::getTranslateMatrix(center),Mpoint);
    Mpoint=Util::multiplyMatrix(GL::getRotateMatrix(angle),Mpoint);
    center.setPoint(-center.x,-center.y,-center.z);
    Mpoint=Util::multiplyMatrix(GL::getTranslateMatrix(center),Mpoint);
    point.setPoint(Mpoint[0][0],Mpoint[1][0],Mpoint[2][0]);

    return point;
}

nTPoint GL::getModelViewPoint(nTPoint point){
    GLfloat matrixf[16];
    vector<vector<float> >Mpoint;
    vector<vector<float> >out;

    Mpoint.resize(4,vector<float>(1,0));
    out.resize(4,vector<float>(4,0));

    glGetFloatv(GL_MODELVIEW_MATRIX, matrixf);

    Mpoint[0][0]=point.x;
    Mpoint[1][0]=point.y;
    Mpoint[2][0]=point.z;
    Mpoint[3][0]=1;

    out[0][0]=(float)matrixf[0];
    out[0][0]=(float)matrixf[1];
    out[0][0]=(float)matrixf[2];
    out[0][0]=(float)matrixf[3];
    out[0][0]=(float)matrixf[4];
    out[0][0]=(float)matrixf[5];
    out[0][0]=(float)matrixf[6];
    out[0][0]=(float)matrixf[7];
    out[0][0]=(float)matrixf[8];
    out[0][0]=(float)matrixf[9];
    out[0][0]=(float)matrixf[10];
    out[0][0]=(float)matrixf[11];
    out[0][0]=(float)matrixf[12];
    out[0][0]=(float)matrixf[13];
    out[0][0]=(float)matrixf[14];
    out[0][0]=(float)matrixf[15];
    Mpoint=Util::multiplyMatrix(out,Mpoint);

    point.setPoint(Mpoint[0][0],Mpoint[1][0],Mpoint[2][0]);
    return point;
}

float GL::getGameMs(){
    return GL::framesInGame*1000/GL::FPS;
}

void GL::drawCube(GLfloat size, GLenum type){
  static GLfloat n[6][3] =
  {
    {-1.0, 0.0, 0.0},
    {0.0, 1.0, 0.0},
    {1.0, 0.0, 0.0},
    {0.0, -1.0, 0.0},
    {0.0, 0.0, 1.0},
    {0.0, 0.0, -1.0}
  };
  static GLint faces[6][4] =
  {
    {0, 1, 2, 3},
    {3, 2, 6, 7},
    {7, 6, 5, 4},
    {4, 5, 1, 0},
    {5, 6, 2, 1},
    {7, 4, 0, 3}
  };
  GLfloat v[8][3];
  GLint i;
  v[0][0] = v[1][0] = v[2][0] = v[3][0] = -size / 2;
  v[4][0] = v[5][0] = v[6][0] = v[7][0] = size / 2;
  v[0][1] = v[1][1] = v[4][1] = v[5][1] = -size / 2;
  v[2][1] = v[3][1] = v[6][1] = v[7][1] = size / 2;
  v[0][2] = v[3][2] = v[4][2] = v[7][2] = -size / 2;
  v[1][2] = v[2][2] = v[5][2] = v[6][2] = size / 2;
  for (i = 5; i >= 0; i--) {
    glBegin(type);
     glNormal3fv(&n[i][0]);
     if(i==4)glTexCoord2f(0, 0);else if(i==0)glTexCoord2f(1, 0);else if(i==2)glTexCoord2f(0, 1);else glTexCoord2f(1, 1);glVertex3fv(&v[faces[i][0]][0]);
     if(i==4)glTexCoord2f(0, 1);else if(i==0)glTexCoord2f(0, 0);else if(i==2)glTexCoord2f(1, 1);else glTexCoord2f(1, 0);glVertex3fv(&v[faces[i][1]][0]);
     if(i==4)glTexCoord2f(1, 1);else if(i==0)glTexCoord2f(0, 1);else if(i==2)glTexCoord2f(1, 0);else glTexCoord2f(0, 0);glVertex3fv(&v[faces[i][2]][0]);
     if(i==4)glTexCoord2f(1, 0);else if(i==0)glTexCoord2f(1, 1);else if(i==2)glTexCoord2f(0, 0);else glTexCoord2f(0, 1);glVertex3fv(&v[faces[i][3]][0]);
    glEnd();
  }
}

void GL::drawSkyBox(string name,nTPoint center, float renderDist){
    
    nTColor tmp=getColor();
    setColor(GL::getColorByName("white"));
    glEnable(GL_TEXTURE_2D);    
    glBindTexture(GL_TEXTURE_2D, GL::getTextureByName(name+"B"));
    glBegin(GL_QUADS);      
        glTexCoord2f(1, 0); glVertex3f(center.x+renderDist,center.y,center.z);
        glTexCoord2f(1, 1); glVertex3f(center.x+renderDist,center.y+renderDist,center.z); 
        glTexCoord2f(0, 1); glVertex3f(center.x,center.y + renderDist,center.z);
        glTexCoord2f(0, 0); glVertex3f(center.x,center.y,center.z);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, GL::getTextureByName(name+"F"));
    glBegin(GL_QUADS);  
        glTexCoord2f(1, 0); glVertex3f(center.x,center.y,center.z+renderDist);
        glTexCoord2f(1, 1); glVertex3f(center.x,center.y+renderDist,center.z+renderDist);
        glTexCoord2f(0, 1); glVertex3f(center.x+renderDist,center.y+renderDist,center.z+renderDist); 
        glTexCoord2f(0, 0); glVertex3f(center.x+renderDist,center.y,center.z +renderDist);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, GL::getTextureByName(name+"D"));
    glBegin(GL_QUADS);      

        glTexCoord2f(1, 0); glVertex3f(center.x,center.y,center.z);
        glTexCoord2f(1, 1); glVertex3f(center.x,center.y,center.z+renderDist);
        glTexCoord2f(0, 1); glVertex3f(center.x+renderDist,center.y,center.z+renderDist); 
        glTexCoord2f(0, 0); glVertex3f(center.x+renderDist,center.y,center.z);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, GL::getTextureByName(name+"U"));
    glBegin(GL_QUADS);      
        glTexCoord2f(0, 0); glVertex3f(center.x+renderDist,center.y+renderDist,center.z);
        glTexCoord2f(1, 0); glVertex3f(center.x+renderDist,center.y+renderDist,center.z+renderDist); 
        glTexCoord2f(1, 1); glVertex3f(center.x,center.y + renderDist,center.z+renderDist);
        glTexCoord2f(0, 1); glVertex3f(center.x,center.y+renderDist,center.z);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, GL::getTextureByName(name+"L"));
    glBegin(GL_QUADS);      
        glTexCoord2f(1, 1); glVertex3f(center.x,center.y+renderDist,center.z); 
        glTexCoord2f(0, 1); glVertex3f(center.x,center.y+renderDist,center.z+renderDist); 
        glTexCoord2f(0, 0); glVertex3f(center.x,center.y,center.z+renderDist);
        glTexCoord2f(1, 0); glVertex3f(center.x,center.y,center.z);     

    glEnd();
    glBindTexture(GL_TEXTURE_2D, GL::getTextureByName(name+"R"));
    glBegin(GL_QUADS);  
        glTexCoord2f(0, 0); glVertex3f(center.x+renderDist,center.y,center.z);
        glTexCoord2f(1, 0); glVertex3f(center.x+renderDist,center.y,center.z+renderDist);
        glTexCoord2f(1, 1); glVertex3f(center.x+renderDist,center.y+renderDist,center.z+renderDist); 
        glTexCoord2f(0, 1); glVertex3f(center.x+renderDist,center.y+renderDist,center.z);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    setColor(tmp);
}


void GL::drawHUD(){
 
}

void GL::drawPause(){
    glClear(GL_COLOR_BUFFER_BIT);
    glNewList(GLlist,GL_COMPILE_AND_EXECUTE);
  
    glEndList();
}

