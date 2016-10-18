#ifndef MAP_HPP
#define MAP_HPP
#include "Util.hpp"
#include "GL.hpp"
class Map {
public:
    Map();
    Map(const Map& orig);
    virtual ~Map();
    static bool loadmap(string path);
    static int analyseColor(int R,int G,int B);
    static void draw();
    static vector<vector <int> > current;
    static bool fillWalls;
    static bool useWallTexture;
    static nTPoint scale;
private:
    static nTPoint size;
};

#endif /* MAP_HPP */

