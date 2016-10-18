#ifndef LOADFILES_H
#define LOADFILES_H
#include "Globals.h"
#include "../Classes/Util.hpp"
#include "../Classes/AL.hpp"
#include "../Classes/GL.hpp"

typedef struct settings{
  bool sound=true,music=true;
  bool fog,light,shader,particles;
}SETTINGS;

bool loadTextures();
bool loadStrings();
bool loadSounds();
bool loadSettings();
bool saveSettings();
bool loadObj();
bool loadMap(string path,nTPoint *sizeAndChannel, unsigned char *map);
#endif	// LOADFILES_H
