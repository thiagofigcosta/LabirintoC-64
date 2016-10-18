#ifndef MAP_HPP
#define MAP_HPP
#include "Util.hpp"
#include "GL.hpp"
#include "Player.hpp"
typedef struct coord0{;
    int i;
    int j;
    void set(int i_,int j_){
        i=i_;
        j=j_;
    };
}nTCoord;

typedef struct coord1{
    bool Up=false;
    bool Down=false;
    bool Left=false;
    bool Right=false;
}nT2Ddirections;

typedef struct coord2{
    nTCoord coord;
    bool footPrint=false;
    bool isCheckPoint=false;
    nT2Ddirections canMove;
    nT2Ddirections alreadyMoved;
}nTComplexCoord;

typedef struct coord3{
    nTCoord coord;
    nTByte paths=0;
    nT2Ddirections moved;
}nTGenCoord;

class nTMap {
public:
    nTMap();
    nTMap(const nTMap& orig);
    virtual ~nTMap();
    static bool loadmap(string path);
    static int analyseColor(int R,int G,int B);
    static vector<nTCoord> findExitOfCurrent();
    static nTByte waysToMove(int i,int j,int cameFrom);
    static void draw();
    static void genMap();
    static vector<vector <int> > current;
    static bool fillWalls;
    static bool useWallTexture;
    static nTPoint scale;
    static nTPoint begin;
    static float offset;
private:
    static nTPoint size;
    static vector<vector <int> > current2;
};

#endif /* MAP_HPP */
