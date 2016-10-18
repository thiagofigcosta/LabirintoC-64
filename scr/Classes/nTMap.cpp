#include "nTMap.hpp"

nTMap::nTMap() {
};

nTMap::nTMap(const nTMap& orig) {
}

nTMap::~nTMap() {
}

vector<vector <int> > nTMap::current;
vector<vector <int> > nTMap::current2;
bool nTMap::fillWalls=true;
bool nTMap::useWallTexture=false;
nTPoint nTMap::size;
nTPoint nTMap::scale;
nTPoint nTMap::begin;
float nTMap::offset=0.5;
int nTMap::currentMap=0;
int nTMap::maxMaps=10;
bool nTMap::isProcedural=false;
vector<nTCoord> nTMap::wayOut;

void nTMap::genMap(){
    int maxI=rand()%50+30;
    int maxJ=rand()%50+50;
    size.setPoint(maxJ,maxI,-1);
    current.clear();
    vector <int> tmp;
    for(int i=0;i<maxI;i++){
        for(int j=0;j<maxJ;j++){
            tmp.push_back(1);//i==0||j==0||i==maxI-1||j==maxJ-1
        }
        current.push_back(tmp);
        tmp.clear();
    }
    int ci=(int)rand()%maxI/8,cj=(int)rand()%maxJ/8;
    current[ci][cj]=2;
    nTPoint pos;
    pos.setPoint((ci+offset)*scale.x*scale.x,-nTMap::scale.y/2,(cj+offset)*(scale.x*scale.x)+scale.x/2);
    begin=pos;
    vector<nTGenCoord> map;
    bool everDad=true;
    int dir=-1;
    while(everDad){
        nTGenCoord tmp;
        tmp.paths=dir;
        tmp.coord.set(ci,cj);
        map.push_back(tmp);
        int numberToRand;
        if(dir<0)
            numberToRand=4;
        else 
            numberToRand=66;
        switch(rand()%numberToRand){
            case 0: if(dir!=4)dir=1;else if(cj<maxJ/2)dir=2;else dir=3; break;
            case 1: if(dir!=1)dir=4;else if(cj<maxJ/2)dir=2;else dir=3; break;
            case 2: if(dir!=2)dir=3;else if(ci<maxI/2)dir=1;else dir=4; break;
            case 3: if(dir!=3)dir=2;else if(ci<maxI/2)dir=1;else dir=4; break;
        }
        switch(dir){
            case 4: ci--;break;
            case 1: ci++;break;
            case 2: cj++;break;
            case 3: cj--;break;
        }
        if((ci<0)||(cj<0)||(ci>=maxI)||(cj>=maxJ)||rand()%23==0){
            if(ci<0) ci=0;
            if(cj<0) cj=0;
            if(ci>=maxI) ci=maxI-1;
            if(cj>=maxJ) cj=maxJ-1;
            nTGenCoord tmp2;
            tmp2.coord.set(ci,cj);
            if(current[ci][cj]!=2)current[ci][cj]=0;
            int randNumber=(map.size()-(rand()%map.size()/2));
            if(randNumber>=map.size()||randNumber<0)randNumber=map.size()-1;
            ci=map[randNumber].coord.i;
            cj=map[randNumber].coord.j;
            map.push_back(tmp2);
            //dir=-1;
        }
        if(current[ci][cj]!=2)current[ci][cj]=0;
        int nOfZeros=1;
        for(int i=0;i<maxI;i++)
        for(int j=0;j<maxJ;j++)
            if(current[i][j]==0) nOfZeros++;
        if(nOfZeros>=(maxI*maxJ*(rand()%236/1000+0.23)))
            everDad=false;
    }
    for(int i=0;i<maxI;i++)
        for(int j=1;j<maxJ;j++)
            if(current[i][maxJ-j]==0){ current[i][maxJ-j]=3; break; i=90000;j=90000;}
    Player::spawn(nTMap::begin);
    wayOut=findExitOfCurrent();
}

bool nTMap::loadmap(string path){
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
            if(tmp[tmp.size()-1]==2){
                nTPoint pos;
                pos.setPoint((i+offset)*scale.x*scale.x,-nTMap::scale.y/2,(j+offset)*(scale.x*scale.x));
                begin=pos;
            }
            t+=3;
        }
        current.push_back(tmp);
        tmp.clear();
    }
    wayOut=findExitOfCurrent();
    Player::spawn(nTMap::begin);
    return true;
}

int nTMap::analyseColor(int R,int G,int B){
    if(R==0&&G==0&&B==0)
        return 1;//wall
    if(R==255&&G==0&&B==0)
        return 2;//begin
    if(R==0&&G==162&&B==232)
        return 3;//the end
    return 0;//chao
}

void nTMap::draw(){
    for (int i=0;i<nTMap::current.size();i++)
        for (int j=0;j<nTMap::current[i].size();j++){
            if (nTMap::current[i][j]==1){
                glPushMatrix();
                GL::setColor(GL::getColorByName("violet"));
                glScalef (nTMap::scale.x,nTMap::scale.y,nTMap::scale.x);
                glTranslatef((i+offset)*nTMap::scale.x,0,(j+offset)*nTMap::scale.x);
                if(nTMap::fillWalls){
                    if(nTMap::useWallTexture){
                        GL::bindTexture("paraboloide hiperbolico<3",1);
                        GL::drawCube(nTMap::scale.x,GL_QUADS);
                        GL::unbindTexture();
                    }else
                        glutSolidCube(nTMap::scale.x);
                }else
                    glutWireCube(nTMap::scale.x);

                glPopMatrix();
            } else if(nTMap::current[i][j]==2 || nTMap::current[i][j]==0){
              if(nTMap::current[i][j]==2){
                nTPoint pos;
                pos.setPoint((i-offset)*scale.x*scale.x,-nTMap::scale.y/2,(j-offset)*(scale.x*scale.x)+scale.x/2);
              }
              glPushMatrix();
              GL::setColor(GL::getColorByName("black"));
              glScalef (nTMap::scale.x,0.5,nTMap::scale.x);
              glTranslatef((i+offset)*nTMap::scale.x,(-0.5/0.5*2-nTMap::scale.y)*2,(j+offset)*nTMap::scale.x);
                      GL::bindTexture("gnd",1);
                      GL::drawCube(nTMap::scale.x,GL_QUADS);
                      GL::unbindTexture();
              glPopMatrix();
            }
        }
}
nTByte nTMap::waysToMove(int i,int j,int cameFrom){
    nTByte out=0;
    if(i<0||j<0||i>=current2.size()||j>=current2[0].size()){
        cout<<"error size of map\n";
        return out;
    }
    if(i!=0){
        if(current2[i-1][j]!=1&&cameFrom!=1&&current2[i-1][j]!=9)
            out+=1;//up
    }
    if(i<current2.size()-1){
        if(current2[i+1][j]!=1&&cameFrom!=2&&current2[i+1][j]!=9)
            out+=2;//down
    }
    if(j!=0){
        if(current2[i][j-1]!=1&&cameFrom!=4&&current2[i][j-1]!=9)
            out+=4;//left
    }
    if(j<current2[0].size()-1){
        if(current2[i][j+1]!=1&&cameFrom!=8&&current2[i][j+1]!=9)
            out+=8;//right
    }
    return out;
}
vector<nTCoord> nTMap::findExitOfCurrent(){
    current2=current;
    vector<nTComplexCoord> wayOut;
    vector<nTCoord> out;
    nTComplexCoord tmp;
    tmp.coord.set(-1,-1);
    for(int i=0;i<current.size();i++){
        for(int j=0;j<current[i].size();j++){
            if(current[i][j]==2){
                tmp.coord.set(i,j);
                break;
            }
        }
        if(tmp.coord.i!=-1) break;
    }
    if(tmp.coord.i==-1||tmp.coord.j==-1){
        cout<<"labirinto trosleiro sem inicio\n";
        return out;
    }
    wayOut.push_back(tmp);
    nTByte dirs;
    int cameFrom=0;
    dirs=waysToMove(wayOut[wayOut.size()-1].coord.i,wayOut[wayOut.size()-1].coord.j,0);
    if(!dirs){nTCoord convert; convert.set(wayOut[wayOut.size()-1].coord.i,wayOut[wayOut.size()-1].coord.j); out.push_back(convert);  return out;}
    if(dirs&1)wayOut[wayOut.size()-1].canMove.Up=true;
    if(dirs&2)wayOut[wayOut.size()-1].canMove.Down=true;
    if(dirs&4)wayOut[wayOut.size()-1].canMove.Left=true;
    if(dirs&8)wayOut[wayOut.size()-1].canMove.Right=true;
    if(dirs!=1&&dirs!=1&&dirs!=2&&dirs!=4&&dirs!=8){
        wayOut[wayOut.size()-1].isCheckPoint=true;
    }
    if(wayOut[wayOut.size()-1].canMove.Up&&!wayOut[wayOut.size()-1].alreadyMoved.Up){
        wayOut[wayOut.size()-1].alreadyMoved.Up=true;
        wayOut[wayOut.size()-1].footPrint=true;
        cameFrom=2;
        nTComplexCoord move;
        move.coord=wayOut[wayOut.size()-1].coord;
        move.coord.i--;
        wayOut.push_back(move);
    }
    if(wayOut[wayOut.size()-1].canMove.Down&&!wayOut[wayOut.size()-1].alreadyMoved.Down){
        wayOut[wayOut.size()-1].alreadyMoved.Down=true;
        wayOut[wayOut.size()-1].footPrint=true;
        cameFrom=1;
        nTComplexCoord move;
        move.coord=wayOut[wayOut.size()-1].coord;
        move.coord.i++;
        wayOut.push_back(move);
    }
    if(wayOut[wayOut.size()-1].canMove.Left&&!wayOut[wayOut.size()-1].alreadyMoved.Left){
        wayOut[wayOut.size()-1].alreadyMoved.Left=true;
        wayOut[wayOut.size()-1].footPrint=true;
        cameFrom=8;
        nTComplexCoord move;
        move.coord=wayOut[wayOut.size()-1].coord;
        move.coord.j--;
        wayOut.push_back(move);
    }
    if(wayOut[wayOut.size()-1].canMove.Right&&!wayOut[wayOut.size()-1].alreadyMoved.Right){
        wayOut[wayOut.size()-1].alreadyMoved.Right=true;
        wayOut[wayOut.size()-1].footPrint=true;
        cameFrom=4;
        nTComplexCoord move;
        move.coord=wayOut[wayOut.size()-1].coord;
        move.coord.j++;
        wayOut.push_back(move);
    }
    bool err=false;
    while(current[wayOut[wayOut.size()-1].coord.i][wayOut[wayOut.size()-1].coord.j]!=3&&!err){    
        if(!wayOut[wayOut.size()-1].isCheckPoint)dirs=waysToMove(wayOut[wayOut.size()-1].coord.i,wayOut[wayOut.size()-1].coord.j,cameFrom);
        if(!dirs||cameFrom==0){
            cameFrom=-1;
            for(int d=wayOut.size()-1;d>=0;d--){
                if(!(wayOut[d].isCheckPoint)){
                    current2[wayOut[d].coord.i][wayOut[d].coord.j]=9;
                    wayOut.erase(wayOut.begin()+d);
                    if(wayOut.size()<=0){err=true;return out;}
                }else{
                    if(!((wayOut[d].canMove.Up&&!wayOut[d].alreadyMoved.Up)||
                         (wayOut[d].canMove.Down&&!wayOut[d].alreadyMoved.Down)||
                         (wayOut[d].canMove.Left&&!wayOut[d].alreadyMoved.Left)||
                         (wayOut[d].canMove.Right&&!wayOut[d].alreadyMoved.Right))){
                        current2[wayOut[d].coord.i][wayOut[d].coord.j]=6;
                        wayOut.erase(wayOut.begin()+d);
                        if(wayOut.size()<=0){err=true;return out;}
                        d=wayOut.size()-1;
                    }else                        
                    d=-1;
                }
            }
        }
        if((dirs||wayOut[wayOut.size()-1].isCheckPoint)&&cameFrom!=0){
            cameFrom=0;
            if(!wayOut[wayOut.size()-1].isCheckPoint){
                if(dirs&1)wayOut[wayOut.size()-1].canMove.Up=true;
                if(dirs&2)wayOut[wayOut.size()-1].canMove.Down=true;
                if(dirs&4)wayOut[wayOut.size()-1].canMove.Left=true;
                if(dirs&8)wayOut[wayOut.size()-1].canMove.Right=true;
                if(dirs!=1&&dirs!=1&&dirs!=2&&dirs!=4&&dirs!=8){
                    wayOut[wayOut.size()-1].isCheckPoint=true;
                    current2[wayOut[wayOut.size()-1].coord.i][wayOut[wayOut.size()-1].coord.j]=7;
                }
            }
                if(wayOut[wayOut.size()-1].canMove.Up&&!wayOut[wayOut.size()-1].alreadyMoved.Up){
                    wayOut[wayOut.size()-1].alreadyMoved.Up=true;
                    wayOut[wayOut.size()-1].footPrint=true;
                    cameFrom=2;
                    nTComplexCoord move;
                    move.coord=wayOut[wayOut.size()-1].coord;
                    if(!wayOut[wayOut.size()-1].isCheckPoint)current2[wayOut[wayOut.size()-1].coord.i][wayOut[wayOut.size()-1].coord.j]=1;                    move.coord.i--;
                    wayOut.push_back(move);
                }
                if(wayOut[wayOut.size()-1].canMove.Down&&!wayOut[wayOut.size()-1].alreadyMoved.Down){
                    wayOut[wayOut.size()-1].alreadyMoved.Down=true;
                    wayOut[wayOut.size()-1].footPrint=true;
                    cameFrom=1;
                    nTComplexCoord move;
                    move.coord=wayOut[wayOut.size()-1].coord;
                    if(!wayOut[wayOut.size()-1].isCheckPoint)current2[wayOut[wayOut.size()-1].coord.i][wayOut[wayOut.size()-1].coord.j]=1;                    move.coord.i++;
                    wayOut.push_back(move);
                }
                if(wayOut[wayOut.size()-1].canMove.Left&&!wayOut[wayOut.size()-1].alreadyMoved.Left){
                    wayOut[wayOut.size()-1].alreadyMoved.Left=true;
                    wayOut[wayOut.size()-1].footPrint=true;
                    cameFrom=8;
                    nTComplexCoord move;
                    move.coord=wayOut[wayOut.size()-1].coord;
                    if(!wayOut[wayOut.size()-1].isCheckPoint)current2[wayOut[wayOut.size()-1].coord.i][wayOut[wayOut.size()-1].coord.j]=1;                    move.coord.j--;
                    wayOut.push_back(move);
                }
                if(wayOut[wayOut.size()-1].canMove.Right&&!wayOut[wayOut.size()-1].alreadyMoved.Right){
                    wayOut[wayOut.size()-1].alreadyMoved.Right=true;
                    wayOut[wayOut.size()-1].footPrint=true;
                    cameFrom=4;
                    nTComplexCoord move;
                    move.coord=wayOut[wayOut.size()-1].coord;
                    if(!wayOut[wayOut.size()-1].isCheckPoint)current2[wayOut[wayOut.size()-1].coord.i][wayOut[wayOut.size()-1].coord.j]=1;
                    move.coord.j++;
                    wayOut.push_back(move);
                }
        }
    }
    out.clear();
    nTCoord convert;
    for(int t=0;t<wayOut.size();t++){
        convert.set(wayOut[t].coord.i,wayOut[t].coord.j);
        out.push_back(convert);
    }
    return out;
}