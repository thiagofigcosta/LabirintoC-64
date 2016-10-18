#ifndef GL_H
#define GL_H
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <time.h>
#include "../Libs/soil/SOIL.h"
#include "Util.hpp"
#include "nTCamera.hpp"

class GL {
public:
    GL(string name,float FPS,GLbitfield displayMode,nTPoint defaultsize,bool blend,vector<GLenum>&enables,int argc, char** argv);
    GL(const GL& orig);
    virtual ~GL();
    static void setFPS(float FPS);
    static float getFPS();
    static float getMs();
    static void setColor(nTColor color);
    static nTColor getColor();
    static GLuint loadTexture(string name,char* path);
    static vector<GLuint> loadTextures(string name,int nOfTex,char* path);
    static void drawRectangle(nTRectangle pos,nTColor color);
    static void drawTexture(nTRectangle pos,nTColor color,GLuint tex,int Orientation);
    static nTColor getColorByName(string name);
    static void setFont(void* font);
    static void setFontByIndex(int idx);
    static void drawText(nTPoint point,char* text,nTColor color);
    static void drawPolygon(nTPoint point,float radius,int edges);
    static GLuint getTextureByName(string name);
    static vector<GLuint> getTexturesByName(string name,int nOfTex);
    static bool isPaused;
    static bool buttonBehave(nTRectangle collision,nTColor pressedColor,GLuint tex,void(*clickFunction)(int,int),void(*releaseFunction)(int,int),void(*RclickFunction)(int,int),void(*RreleaseFunction)(int,int));
    static void drawPause();
    static void drawHUD();
    static vector<vector<float> > getRotateMatrix(float angle);
    static vector<vector<float> > getTranslateMatrix(nTPoint point);
    static nTPoint getModelViewPoint(nTPoint point);
    static nTPoint rotatePoint(nTPoint point,nTPoint center,float angle);
    static void bindTexture(string name);
    static void drawCube(GLfloat size, GLenum type);
    static void drawSkyBox(string name,nTPoint center, float renderDist);
    static void unbindTexture();
    static nTPoint defaultSize;
    static nTPoint currentSize;
    static bool leftMouseClicked;
    static bool leftMouseReleased;
    static bool rightMouseClicked;
    static bool rightMouseReleased;
    static nTPoint mousePos;
    static nTPoint rawMousePos;
    static vector<GLuint> textures;
    void start();
    static unsigned long int framesInGame;
    static float getGameMs();
    static nTColor clearcolor;
    static nTCamera cam;
private:
    friend ostream& operator<<(ostream &strm, const GL &gl);
    static GLuint GLlist;
    static float FPS;
    static nTColor currentColor;
    static void* currentFont;
    static vector<string> textureNames;
    static nTColor bindColor;
};
void drawScene(void);
void reshape(int width, int height);
void keyboard(unsigned char key, int x, int y);
void update(int n);
void mousePress(int button,int state,int x,int y);
void mousePassiveMotion(int x,int y);
void mouseActiveMotion(int x,int y);
void modifySound(int a,int b);
void modifyMusic(int a,int b);
#endif /* GL_H */
