#ifndef NTPARTICLE_HPP
#define NTPARTICLE_HPP
#include "Util.hpp"
#include "GL.hpp"

class nTParticle {
public:
    nTParticle(bool enable,float fade,nTColor color,nTPoint pos,float size,nTPoint vel,nTPoint gravity,GLuint tex);
    nTParticle(const nTParticle& orig);
    virtual ~nTParticle();
    static void init(int type);
    static void draw(bool syncWithPlayer);
    static nTColor genColor(int type);
    static nTPoint genPos(int type);
    static nTPoint genVel(int type);
    static float genFade(int type);
    static vector<nTParticle*> particles;
    static bool useParticles;
private:
    bool enable;										
    float fade;					
    nTColor color;
    nTPoint pos;
    nTPoint vel;
    nTPoint gravity;
    GLuint tex;
    float size;
    static int maxParticles;

};

#endif /* NTPARTICLE_HPP */

