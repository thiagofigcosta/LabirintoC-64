#ifndef LIGHT_H
#define LIGHT_H
#include "Util.hpp"
#include "GL.hpp"


typedef struct lights{
  GLfloat *ambient, *difuse, *especular, *position, *materialEspecular;
  GLint materialEspec;
  nTColor color;
  char shader;
  string name;
  void setLight(GLfloat *ambient_,GLfloat *difuse_,GLfloat *especular_,GLfloat *position_,
    GLfloat *materialEspecular_, GLint materialEspec_, char shader_, nTColor color_, string name_){
      ambient=ambient_; difuse=difuse_; especular=especular_; position=position_; materialEspecular=materialEspecular_;
      materialEspec=materialEspec_; shader=shader_; color=color_; name=name_;
  };
}Lights;

class Light {
public:
    Light();
    Light(const Light& orig);
    virtual ~Light();
    static void refreshLightsId();
    static void addLight(Lights light);
    static bool deleteLight(string name);
    static void enableAllLights();
    static void disableAllLights();
    static void setAmbientLight();
    static void enableLight(string name);
    static vector<Lights> currentLights;
    static vector<GLenum> lightsId;
};
#endif /* LIGHT_H */
