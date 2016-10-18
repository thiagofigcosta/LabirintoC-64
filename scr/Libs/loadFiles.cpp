#include "loadFiles.h"

bool loadTextures(){
    bool out = true;
    GL::loadTexture("paraboloide hiperbolico<3","tex/paraboloideHiperbolico.png");//n precisa de if
    
    GL::loadTexture("mapa","tex/bgnd/map.png");
    
    GL::loadTexture("LIDs","tex/logo/lid-silver.png");
    GL::loadTexture("LIDg","tex/logo/lid-gold.png");
    
    GL::loadTexture("SkyBoxB","tex/skybox/back.png");
    GL::loadTexture("SkyBoxD","tex/skybox/down.png");
    GL::loadTexture("SkyBoxF","tex/skybox/front.png");
    GL::loadTexture("SkyBoxL","tex/skybox/left.png");
    GL::loadTexture("SkyBoxR","tex/skybox/right.png");
    GL::loadTexture("SkyBoxU","tex/skybox/up.png");
    
    GL::loadTexture("jogar","tex/btns/Jogar.png");
    GL::loadTexture("normal","tex/btns/normal.png");
    GL::loadTexture("opcoes","tex/btns/opcoes.png");
    GL::loadTexture("procedural","tex/btns/procedural.png");
    GL::loadTexture("sair","tex/btns/sair.png");
    GL::loadTexture("voltar","tex/btns/Voltar.png");
    
    
    return out;
}
bool loadStrings(){
    bool out=true;
    //TODO:carregar strings de idiomas. retorna falso se nao carregou algum arquivo
    return out;
}

bool loadSounds(){
    bool out=true;
    if(!al->loadSound("audio/sound/click.wav","mouse",1,false)) out=false;
    if(!al->loadSound("audio/sound/walk.wav","walk",1,false)) out=false;
    if(!al->loadSound("audio/sound/run.wav","run",1,false)) out=false;
    
    if(!al->loadSound("audio/music/music0.wav","music0",1,true)) out=false;
    if(!al->loadSound("audio/music/music1.wav","music1",1,true)) out=false;
    if(!al->loadSound("audio/music/music2.wav","music2",1,true)) out=false;
    if(!al->loadSound("audio/music/music3.wav","music3",1,true)) out=false;
    if(!al->loadSound("audio/music/musicMenu.wav","menu",1,true)) out=false;
    return out;
}
bool loadSettings(){
    FILE *fp=fopen("savedGame.bin","rb");
  if(fp==NULL){
      cout<<"Arquivo savedGame.bin inexistente, criando novo arquivo"<<endl;
      saveSettings();
      return false;
  }
  SETTINGS tmp;
  fread(&tmp,sizeof(SETTINGS),1,fp);
  AL::setMusicState(tmp.music);
  AL::setSoundState(tmp.sound);
  fclose(fp);
  return true;
}
bool saveSettings(){
   FILE *fp=fopen("savedGame.bin","wb");
    if(fp==NULL){
      cout<<"Erro ao salvar arquivo savedGame.bin"<<endl;
      return false;
    }
    SETTINGS tmp;
    tmp.music=1;
    tmp.sound=1;
    fwrite(&tmp,sizeof(SETTINGS),1,fp);
    fclose(fp);
    return true;
}
