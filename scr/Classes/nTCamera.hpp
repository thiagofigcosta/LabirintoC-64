#ifndef NTCAMERA_H
#define NTCAMERA_H
#include "Util.hpp"
class nTCamera {
public:
    nTCamera();
    nTCamera(const nTCamera& orig);
    virtual ~nTCamera();
    void moveYaxis(int dir);
    void rotateZaxis(int dir);
    void rotateYaxis(int dir);
    void behave();
    nTPoint rotation;
    nTPoint pos;
    nTPoint direction;
    static float angleVar; 
    static float steepVar;
    static float mouseX;
    static float mouseY;
    static bool putMouseOnMiddle;
private:
    friend ostream& operator<<(ostream &strm, const nTCamera &cam);
};

#endif /* NTCAMERA_H */

