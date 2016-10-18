#include "importLibs.h"

int main(int argc, char** argv) {
    vector<GLenum> enables;
    enables.push_back(GL_BLEND);
    enables.push_back(GL_ALPHA_TEST);
    nTPoint size;
    size.setPoint(800,600,1000);
    GL *gl=new GL("LID 64-Lost In Ditadura",30,GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH|GLUT_ALPHA|GLUT_STENCIL,
         size,true,enables,argc,argv);
    
    if(!loadSettings()) if(Util::DEBUG) cout<<"erro ao carregar configs"<<endl;
    
    al=new AL();
    
    if(!loadSounds()) if(Util::DEBUG) cout<<"erro ao carregar sons"<<endl;
    if(!loadTextures()) if(Util::DEBUG) cout<<"erro ao carregar texturas"<<endl;
        
    gl->start();
    return 0;
}