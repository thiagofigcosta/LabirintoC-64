#ifndef UTIL_H
#define UTIL_H
#define ABS(x) ((x)<0?-(x):(x))
#define M_PI 3.14159265359
#include <vector>
#include <string>
#include <cstdlib>
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

typedef struct map{
  int backgroundId;
  vector<vector<int> > map;
}nTMap;

typedef struct color{
        float R,G,B,A;
        void setColor(float R_,float G_, float B_, float A_){
            R=R_;G=G_;B=B_;A=A_;
        };
}nTColor;

typedef struct point{
        float x,y,z;
        void setPoint(float x_,float y_,float z_){
            x=x_;y=y_;z=z_;
        };
}nTPoint;

typedef struct rectangle{
        nTPoint p0,p1;
        void setRec(nTPoint p0_,nTPoint p1_){
            p0=p0_;p1=p1_;
        };
        void setRectangle(float x0_,float y0_,float x1_,float y1_,float z0_,float z1_){
            p0.x=x0_;p0.y=y0_;p1.x=x1_;p1.y=y1_;
            p0.z=z0_;p1.z=z1_;
        };
}nTRectangle;

typedef struct auxcollision{
  int firstObj;
  int secondObj;
}objCollision;

typedef struct collision{
    objCollision collision;
    int blockRef;
}mapCollision;

class Util {
public:
    Util();
    Util(const Util& orig);
    virtual ~Util();
    static char* newPath(char* path);
    static bool DEBUG;
    static char* getDinamicPath(char* p1,int idx, char* p3);
    static vector<vector<float> >multiplyMatrix(vector<vector<float> >m1,vector<vector<float> >m2);
    static nTColor nTColorSet(float R,float G, float B,float A);
    static nTPoint nTPointSet(float x,float y,float z);
    static nTRectangle nTRectangleSet(float x0,float y0,float x1,float y1,float z0,float z1);
    static nTRectangle getCollisionRectangle(nTPoint pos,nTPoint size);
    static vector <nTPoint> getRetangleVertexs(nTRectangle rectangle);
    static bool analyseCollision(nTRectangle one,nTRectangle two);
    static bool analysePointCollison(nTPoint one,nTRectangle two);
    static float angleToRad(float angle);
    static float radToAngle(float RAD);
    static int left;
    static int right;
    static int up;
    static int down;
    static char* mallocChar;
private:
    static string resourcesFolder;
};

#endif /* UTIL_H */
