#ifndef NTCAMERA_H
#define NTCAMERA_H
#include "Util.hpp"
class nTCamera {
public:
    nTCamera();
    nTCamera(const nTCamera& orig);
    virtual ~nTCamera();
    nTPoint rotation;
    nTPoint pos;
    nTPoint direction;
    static float angleVar; 
    static float steepVar;
private:
    friend ostream& operator<<(ostream &strm, const nTCamera &cam);
};

#endif /* NTCAMERA_H */

