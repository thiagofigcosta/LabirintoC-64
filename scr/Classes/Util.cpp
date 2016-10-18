#include "Util.hpp"
Util::Util() {
};

Util::Util(const Util& orig) {
}

Util::~Util() {
}

bool Util::DEBUG=true;
string Util::resourcesFolder="res/";
int Util::left=13;
int Util::right=45;
int Util::up=9849526;
int Util::down=552626;
char* Util::mallocChar=(char*)malloc(sizeof(char));

char* Util::newPath(char* path){
    if(resourcesFolder!=""){
        string tmp(path);
        tmp=resourcesFolder+tmp;
        if(1){
            mallocChar=(char*)realloc(mallocChar,(tmp.size()+1)*sizeof(char));
            mallocChar[0]='\0';
            for(int i=0;i<tmp.size();i++)
                 mallocChar[i]=tmp[i];
            mallocChar[tmp.size()]='\0';
            return mallocChar;
        }
        char*bug=(char*)tmp.c_str();
        return bug;
    }
    return path;
}

char* Util::getDinamicPath(char* p1,int idx, char* p3){
    p1=newPath(p1);
    char buffer[5];
    snprintf(buffer,5,"%d",idx);
    string out(p1);
    string str1(buffer);
    string str2(p3);
    out+=str1+str2;
    if(1){
        mallocChar=(char*)realloc(mallocChar,(out.size()+1)*sizeof(char));
        mallocChar[0]='\0';
        for(int i=0;i<out.size();i++)
             mallocChar[i]=out[i];
        mallocChar[out.size()]='\0';
        return mallocChar;
    }
    char*bug=(char*)out.c_str();

    return bug;
}

nTColor Util::nTColorSet(float R,float G, float B, float A){
    nTColor tmp;
    tmp.setColor(R,G,B,A);
    return tmp;
}
nTPoint Util::nTPointSet(float x,float y,float z){
    nTPoint tmp;
    tmp.setPoint(x,y,z);
    return tmp;
}
nTRectangle Util::nTRectangleSet(float x0,float y0,float x1,float y1,float z0,float z1){
    nTRectangle tmp;
    tmp.setRectangle(x0,y0,x1,y1,z0,z1);
    return tmp;
}

nTRectangle Util::getCollisionRectangle(nTPoint pos,nTPoint size){
    nTPoint inf_left, up_right;
    nTRectangle collision;

    inf_left.setPoint(pos.x-(size.x/2) ,pos.y+(size.y/2), pos.z-(size.z/2)); //vértice inferior esquerdo
    up_right.setPoint(pos.x+(size.x/2) ,pos.y-(size.y/2), pos.z+(size.z/2)); //vértice superior direito
    collision.setRec(inf_left, up_right);
    return collision;
}

vector <nTPoint> Util::getRetangleVertexs(nTRectangle rectangle){
  int sizex, sizey;
  nTPoint sup_left, inf_right, inf_left, sup_right;
  vector <nTPoint> points; // inferior esquerdo, inferior direito, superior direito e superior esquerdo,;

  inf_left=rectangle.p0;
  sup_right=rectangle.p1;

  sizex = sup_right.x - inf_left.x;
  sizey = inf_left.y - sup_right.y;

  inf_right.setPoint(inf_left.x + sizex, inf_left.y, inf_left.z);
  sup_left.setPoint(sup_right.x - sizex, sup_right.y , sup_right.z); //conferir se z será o mesmo sempre
  points.push_back(inf_left);
  points.push_back(inf_right);
  points.push_back(sup_right);
  points.push_back(sup_left);
  return points;
}

vector<vector<float> >Util::multiplyMatrix(vector<vector<float> >m1,vector<vector<float> >m2){
    nTPoint outSize;
    outSize.setPoint(m1.size(),m2[0].size(),0);
    vector<vector<float> >out;
    out.resize(outSize.x,vector<float>(outSize.y ,0));
    if(m1[0].size() != m2.size()) return out; //n se aplica
    for(int i=0;i<outSize.x;i++)
        for(int j=0;j<outSize.y;j++)
            for(int k=0;k<m1[0].size();k++)
                out[i][j]+=m1[i][k]*m2[k][j];
    return out;
}

float Util::angleToRad(float RAD){
    return RAD*M_PI/180;
}
float Util::radToAngle(float angle){
    return angle*180/M_PI;
}

bool Util::analyseCollision(nTRectangle one,nTRectangle two){
    if(one.p0.x>one.p1.x){
        float tmp=one.p1.x;
        one.p1.x=one.p0.x;
        one.p0.x=tmp;
    }
    if(one.p0.y>one.p1.y){
        float tmp=one.p1.y;
        one.p1.y=one.p0.y;
        one.p0.y=tmp;
    }
    if(one.p0.z>one.p1.z){
        float tmp=one.p1.z;
        one.p1.z=one.p0.z;
        one.p0.z=tmp;
    }
    if(two.p0.x>two.p1.x){
        float tmp=two.p1.x;
        two.p1.x=two.p0.x;
        two.p0.x=tmp;
    }
    if(two.p0.y>two.p1.y){
        float tmp=two.p1.y;
        two.p1.y=two.p0.y;
        two.p0.y=tmp;
    }
    if(two.p0.z>two.p1.z){
        float tmp=two.p1.z;
        two.p1.z=two.p0.z;
        two.p0.z=tmp;
    }
    return (one.p0.x <= two.p1.x && one.p1.x >= two.p0.x) &&
         (one.p0.y <= two.p1.y && one.p1.y >= two.p0.y) &&
         (one.p0.z <= two.p1.z && one.p1.z >= two.p0.z);
}
bool Util::analysePointCollison(nTPoint one,nTRectangle two){
    if(two.p0.x>two.p1.x){
        float tmp=two.p1.x;
        two.p1.x=two.p0.x;
        two.p0.x=tmp;
    }
    if(two.p0.y>two.p1.y){
        float tmp=two.p1.y;
        two.p1.y=two.p0.y;
        two.p0.y=tmp;
    }
    if(two.p0.z>two.p1.z){
        float tmp=two.p1.z;
        two.p1.z=two.p0.z;
        two.p0.z=tmp;
    }
    return (one.x >= two.p0.x && one.x <= two.p1.x) &&
         (one.y >= two.p0.y && one.y <= two.p1.y) &&
         (one.z >= two.p0.z && one.z <= two.p1.z);
}