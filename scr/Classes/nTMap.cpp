#include "nTMap.hpp"

Map::Map() {
};

Map::Map(const Map& orig) {
}

Map::~Map() {
}

vector<vector <int> > Map::current;
bool Map::fillWalls=true;
bool Map::useWallTexture=false;
nTPoint Map::size;
nTPoint Map::scale;

bool Map::loadmap(string path){
    unsigned char *map = NULL;
    int x,y,z;
    map = SOIL_load_image(Util::newPath((char*)path.c_str()),&x,&y,&z,SOIL_LOAD_RGB);
    if(map==NULL) return false;
    current.clear();
    size.setPoint(x,y,z);
    vector <int> tmp;
    for(int i=0,t=0;i<size.y;i++){
        for(int j=0;j<size.x;j++){
            tmp.push_back(analyseColor(map[t],map[t+1],map[t+2]));
            t+=3;
        }
        current.push_back(tmp);
        tmp.clear();
    }
    return true;
}

int Map::analyseColor(int R,int G,int B){
    if(R==0&&G==0&&B==0)
        return 1;//wall
    
    return 0;//air
}

void Map::draw(){
    float offset=0.5;
    for (int i=0;i<Map::current.size();i++)
        for (int j=0;j<Map::current[i].size();j++){
            if (Map::current[i][j]==1){
                glPushMatrix();               
                GL::setColor(GL::getColorByName("violet"));  
                glScalef (Map::scale.x,Map::scale.y,Map::scale.x); 
                glTranslatef((i+offset)*Map::scale.x,0,(j+offset)*Map::scale.x);      
                if(Map::fillWalls){
                    if(Map::useWallTexture){
                        GL::bindTexture("paraboloide hiperbolico<3");
                        GL::drawCube(Map::scale.x,GL_QUADS);
                        GL::unbindTexture();
                    }else
                        glutSolidCube(Map::scale.x);
                }else
                    glutWireCube(Map::scale.x);
                
                glPopMatrix();
            }
        } 
}