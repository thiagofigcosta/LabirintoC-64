#ifndef AL_H
#define AL_H
#include <AL/al.h>
#include <AL/alc.h>
#include "Util.hpp"

class AL {
public:
    AL();
    AL(const AL& orig);
    virtual ~AL();
    static bool checkIfIsPlaying(int sound);
    static void stopAllSoundsExcept(vector<int>&sounds);
    static bool playSoundByName(string name);
    static int  getSoundByName(string name);
    static bool playSound(int sound);
    static bool stopSound(int sound);
    static bool loadSound(char* path,string name,float vol,bool Loop);
    static void setMusicState(bool musicOn);
    static void setSoundState(bool soundOn);
    static bool getSoundState();
    static bool getMusicState();
private:
    friend ostream& operator<<(ostream &strm, const AL &al);
    static vector<string> soundName;
    static vector<ALuint> sounds;
    static vector<ALuint> buffers;
    static vector<bool> isMusic;
    static bool soundOn;
    static bool musicOn;
    static ALfloat ListenerPos[3];
    static ALfloat ListenerVel[3];
    static ALfloat ListenerOri[6];
    static ALfloat SourcePos[3];
    static ALfloat SourceVel[3];
};

#endif /* AL_H */
