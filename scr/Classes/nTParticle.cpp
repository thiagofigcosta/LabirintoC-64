#include "nTParticle.hpp"
#include "Player.hpp"
#include "nTMap.hpp"

nTParticle::nTParticle(bool enable,float fade,nTColor color,nTPoint pos,float size,nTPoint vel,nTPoint gravity,GLuint tex){
    this->enable=enable;
    this->fade=fade;
    this->color=color;
    this->pos=pos;
    this->vel=vel;
    this->gravity=gravity;
    this->tex=tex;
    this->size=size;
};

nTParticle::nTParticle(const nTParticle& orig) {
}

nTParticle::~nTParticle() {
}
int nTParticle::maxParticles;
bool nTParticle::useParticles=true;
vector<nTParticle*> nTParticle::particles;

void nTParticle::init(int type){
    for(int i=0;i<particles.size();i++){
        delete particles[i];
    }
    particles.clear();
    switch(type){
        case 0:
            maxParticles=800;
            for(int i=0;i<maxParticles;i++){
                nTParticle *tmp=new nTParticle(true,genFade(type),genColor(type),genPos(type),0.1,genVel(type),Util::nTPointSet(0,-10,0),GL::getTextureByName("particle0"));
                particles.push_back(tmp);
            }       
        break;
    }
}

void nTParticle::draw(bool syncWithPlayer){
    if(!useParticles) return;
    bool eLight=glIsEnabled(GL_LIGHTING), eFog=glIsEnabled(GL_LIGHTING);
    glDisable(GL_FOG);
    glDisable(GL_LIGHTING);
    int fi=0,fj=0;
        for(int i=0;i<nTMap::current.size();i++)
            for(int j=0;j<nTMap::current[i].size();j++)
                if(nTMap::current[i][j]==3){
                    fi=i;
                    fj=j;
                    break;
                }
    float sizeOfEachBlock=nTMap::scale.x*nTMap::scale.x;
    glPushMatrix();
    glTranslatef((nTMap::scale.x+sizeOfEachBlock*(fi)),-0.5,(nTMap::scale.x+sizeOfEachBlock*(fj)));    
    glRotatef(Util::radToAngle(-GL::cam.rotation.y),0,1,0);
    if(syncWithPlayer)glTranslatef((Player::pos.x-3*GL::cam.direction.x), 0, (Player::pos.z-3*GL::cam.direction.z));     
    nTColor tmp=GL::getColor();
    for(int i=0;i<particles.size();i++){
        if(particles[i]->enable){
                float offset=particles[i]->size;
                float x=particles[i]->pos.x;					
                float y=particles[i]->pos.y;					
                float z=particles[i]->pos.z;
                GL::setColor(particles[i]->color);
                GL::bindTexture(GL::getNameByTexture(particles[i]->tex),0);
                glBegin(GL_TRIANGLE_STRIP);					
                    glTexCoord2d(1,1); glVertex3f(x+offset,y+offset,z); 
                    glTexCoord2d(0,1); glVertex3f(x-offset,y+offset,z);
                    glTexCoord2d(1,0); glVertex3f(x+offset,y-offset,z);
                    glTexCoord2d(0,0); glVertex3f(x-offset,y-offset,z);
                glEnd();										
                GL::unbindTexture();
                particles[i]->pos.x+=particles[i]->vel.x/GL::getFPS();
                particles[i]->pos.y+=particles[i]->vel.y/GL::getFPS();
                particles[i]->pos.x+=particles[i]->vel.z/GL::getFPS();
                particles[i]->vel.x+=particles[i]->gravity.x/GL::getFPS();
                particles[i]->vel.y+=particles[i]->gravity.y/GL::getFPS();
                particles[i]->vel.x+=particles[i]->gravity.z/GL::getFPS();
                particles[i]->color.A-=particles[i]->fade;		
                if (particles[i]->color.A<0){
                        particles[i]->color.A=1;					
                        particles[i]->fade=genFade(0);	
                        particles[i]->pos=genPos(0);					
                        particles[i]->vel=genVel(0);
                        particles[i]->color=genColor(0);
                }
        }
    }
    glPopMatrix();
    GL::setColor(tmp);
    if(eFog)glEnable(GL_FOG);
    if(eLight)glEnable(GL_LIGHTING);
}

nTColor nTParticle::genColor(int type){
    switch(type){
        case 0: return Util::nTColorSet(fmod(rand(),0.6)+0.4,fmod(rand(),0.6)+0.4,fmod(rand(),0.6)+0.4,1);
    } 
    return  Util::nTColorSet(1,1,1,1);
}
nTPoint nTParticle::genVel(int type){
    switch(type){
        case 0: return Util::nTPointSet(((rand()%50)-26.0f)*2,((rand()%50)-26.0f)*2,((rand()%50)-26.0f)*2);
    } 
    return  Util::nTPointSet(0,0,0);
}
nTPoint nTParticle::genPos(int type){
    return  Util::nTPointSet(0,0,0);
}

float nTParticle::genFade(int type){
    switch(type){
        case 0: return (rand()%100)/1000.0f+0.003f;
    } 
    return  .007;
}