#ifndef GLCALLBACKS_H
#define GLCALLBACKS_H
#include "../Classes/GL.hpp"
#include "../Classes/Util.hpp"
#include "../Classes/Scenes.hpp"
#include "../Libs/Globals.h"
#include "loadFiles.h"

void drawScene(void);
void keyboard(unsigned char key, int x, int y);
void specialKeyboard(int key,int x,int y);
void keyboardUp(unsigned char key,int x,int y);
void specialKeyboardUp(int key,int x,int y);
void reshape(int width, int height);
void update(int n);
void mousePress(int button,int state,int x,int y);
void mousePassiveMotion(int x,int y);
void mouseActiveMotion(int x,int y);
#endif	// GLCALLBACKS_H
