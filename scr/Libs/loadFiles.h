#ifndef LOADFILES_H
#define LOADFILES_H
#include "Globals.h"
#include "../Classes/Util.hpp"
#include "../Classes/AL.hpp"
#include "../Classes/GL.hpp"

typedef struct settings{
  bool sound=true,music=true;
  int lives=0,checkpoint=0,currentStage=0,sword=0;
  float CR=0;
}SETTINGS;

bool loadTextures();
bool loadStrings();
bool loadSounds();
bool loadSettings();
bool saveSettings();
bool loadMap(string path,nTPoint *sizeAndChannel, unsigned char *map);
#endif	// LOADFILES_H
