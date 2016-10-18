#include "loadFiles.h"

bool loadTextures(){
    bool out = true;
    GL::loadTexture("paraboloide hiperbolico<3","tex/paraboloideHiperbolico.png");//n precisa de if
    
    GL::loadTexture("SkyBoxB","tex/skybox/back.png");
    GL::loadTexture("SkyBoxD","tex/skybox/down.png");
    GL::loadTexture("SkyBoxF","tex/skybox/front.png");
    GL::loadTexture("SkyBoxL","tex/skybox/left.png");
    GL::loadTexture("SkyBoxR","tex/skybox/right.png");
    GL::loadTexture("SkyBoxU","tex/skybox/up.png");
    
    return out;
}
bool loadStrings(){
    bool out=true;
    //TODO:carregar strings de idiomas. retorna falso se nao carregou algum arquivo
    return out;
}

bool loadSounds(){
    bool out=true;
    if(!al->loadSound("Sounds/hey.wav","heyListen",2,false)) out=false;
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
       
    fwrite(&tmp,sizeof(SETTINGS),1,fp);
    fclose(fp);
    return true;
}
