#include "AL.hpp"

AL::AL() {
    ALCdevice *device;                                                          //Create an OpenAL Device
    ALCcontext *context;                                                        //And an OpenAL Context
    device = alcOpenDevice(NULL);                                               //Open the device
    context = alcCreateContext(device, NULL);                                   //Give the device a context
    alcMakeContextCurrent(context);                                             //Make the context the current

    alListenerfv(AL_POSITION,    ListenerPos);                                  //Set position of the listener
    alListenerfv(AL_VELOCITY,    ListenerVel);                                  //Set velocity of the listener
    alListenerfv(AL_ORIENTATION, ListenerOri);
};

AL::AL(const AL& orig) {
}

AL::~AL() {
}

vector<string> AL::soundName;
vector<ALuint> AL::sounds;
vector<ALuint> AL::buffers;
vector<bool> AL::isMusic;
bool AL::soundOn;
bool AL::musicOn;
ALfloat AL::ListenerPos[3] = { 0.0, 0.0, 0.0 };
ALfloat AL::ListenerVel[3] = { 0.0, 0.0, 0.0 };
ALfloat AL::ListenerOri[6] = { 0.0, 0.0, -1.0,  0.0, 1.0, 0.0 };
ALfloat AL::SourcePos[3] = { 0.0, 0.0, 0.0 };
ALfloat AL::SourceVel[3] = { 0.0, 0.0, 0.0 };

bool AL::checkIfIsPlaying(int sound){
    ALint state;
    alGetSourcei(sounds[sound],AL_SOURCE_STATE,&state);
      if(AL_PLAYING != state)
          return false;
    return true;
}
void AL::stopAllSoundsExcept(vector<int>&soundss){
    bool stop;
    for(int i=0;i<sounds.size();i++){
        stop=true;
        for(int j=0;j<soundss.size();j++)
            if(soundss[j]==sounds[i]){
                stop=false;
                break;
            }
        if(stop)alSourceStop(sounds[i]);
    }
}
bool AL::playSoundByName(string name){
    int i=0;
    for(string n:soundName)
        if(n==name){
            if((isMusic[i]&&musicOn)||(!isMusic[i]&&soundOn))
                if(!checkIfIsPlaying(i))
                    alSourcePlay(sounds[i]);
            return true;
        }else
            i++;
    if(Util::DEBUG) cout<<"Failed to play sound "+name<<endl;
    return false;
}

int AL::getSoundByName(string name){
    int i=0;
    for(string n:soundName)
        if(n==name){
            return i;
        }else
            i++;
    if(Util::DEBUG) cout<<"Failed to get sound "+name<<endl;
    return -1;
}

bool AL::loadSound(char* path,string name,float vol,bool Loop){
    bool out=true;
    int currentLoading=sounds.size();
    sounds.push_back(NULL);
    buffers.push_back(NULL);
    isMusic.push_back(Loop);
    ALboolean loop;
    if(Loop)
        loop=AL_TRUE;
    else
        loop=AL_FALSE;
    
    FILE *fp = NULL;
    fp=fopen(Util::newPath(path),"rb");
    if (!fp) return 0;

    char type[4];
    unsigned int size,chunkSize;
    short formatType,channels;
    unsigned int sampleRate,avgBytesPerSec;
    short bytesPerSample,bitsPerSample;
    unsigned int dataSize;

    fread(type,sizeof(char),4,fp);
    if(type[0]!='R' || type[1]!='I' || type[2]!='F' || type[3]!='F'){
        if(Util::DEBUG)printf("Error loading sound %d, err:1\n",currentLoading);
        out=false;
    }

    fread(&size, sizeof(unsigned int),1,fp);
    fread(type, sizeof(char),4,fp);
    if (type[0]!='W' || type[1]!='A' || type[2]!='V' || type[3]!='E'){
        if(Util::DEBUG)printf("Error loading sound %d, err:2\n",currentLoading);
        out=false;
    }

    fread(type,sizeof(char),4,fp);
    if (type[0]!='f' || type[1]!='m' || type[2]!='t' || type[3]!=' '){
        if(Util::DEBUG)printf("Error loading sound %d, err:3\n",currentLoading);
        out=false;
    }

    fread(&chunkSize,sizeof(unsigned int),1,fp);
    fread(&formatType,sizeof(short),1,fp);
    fread(&channels,sizeof(short),1,fp);
    fread(&sampleRate,sizeof(unsigned int),1,fp);
    fread(&avgBytesPerSec,sizeof(unsigned int),1,fp);
    fread(&bytesPerSample,sizeof(short),1,fp);
    fread(&bitsPerSample,sizeof(short),1,fp);

    fread(type,sizeof(char),4,fp);
    if (type[0]!='d' || type[1]!='a' || type[2]!='t' || type[3]!='a'){
        if(Util::DEBUG)printf("Error loading sound %d, err:4(reconvert the file)\n",currentLoading);
        out=false;
    }

    fread(&dataSize,sizeof(unsigned int),1,fp);

    unsigned char* buf= new unsigned char[dataSize];
    fread(buf,sizeof(unsigned char),dataSize,fp);

    ALuint frequency=sampleRate;
    ALenum format=0;
    alGenBuffers(1, &buffers[currentLoading]);
    alGenSources(1, &sounds[currentLoading]);
    if(bitsPerSample == 8){
        if(channels == 1)
            format = AL_FORMAT_MONO8;
        else if(channels == 2)
            format = AL_FORMAT_STEREO8;
    }
    else if(bitsPerSample == 16){
        if(channels == 1)
            format = AL_FORMAT_MONO16;
        else if(channels == 2)
            format = AL_FORMAT_STEREO16;
    }

    alBufferData(buffers[currentLoading], format, buf, dataSize, frequency);

    alSourcei (sounds[currentLoading], AL_BUFFER,buffers[currentLoading]);
    alSourcef (sounds[currentLoading], AL_PITCH,1.0f);
    alSourcef (sounds[currentLoading], AL_GAIN,vol);
    alSourcefv(sounds[currentLoading], AL_POSITION,SourcePos);
    alSourcefv(sounds[currentLoading], AL_VELOCITY,SourceVel);
    alSourcei (sounds[currentLoading], AL_LOOPING,loop );
    fclose(fp);
    soundName.push_back(name);
    delete[] buf;

    return out;
}

bool AL::playSound(int sound){
    if(sound>=sounds.size())
        return false;
    if((isMusic[sound]&&musicOn)||(!isMusic[sound]&&soundOn))
        if(!checkIfIsPlaying(sound))
            alSourcePlay(sounds[sound]);
    return true;
}
bool AL::stopSound(int sound){
    if(sound>=sounds.size())
        return false;
    alSourceStop(sounds[sound]);
    return true;
}
void AL::setMusicState(bool musicOn){
    AL::musicOn=musicOn;
    if(!musicOn){
        for(int i=0;i<sounds.size();i++)
            if(isMusic[i])
                stopSound(i);
    }
}
void AL::setSoundState(bool soundOn){
    AL::soundOn=soundOn;
    if(!soundOn){
        for(int i=0;i<sounds.size();i++)
            if(!isMusic[i])
                stopSound(i);
    }
}
bool AL::getSoundState(){
    return AL::soundOn;
}
bool AL::getMusicState(){
    return AL::musicOn;
}
ostream& operator<<(ostream &strm, const AL &al){
    if(Util::DEBUG)
        return strm <<"AL:["<<"Loaded Sounds("<<al.sounds.size()<<"),"<<
                "Sound On("<<al.soundOn<<"),"<<"Music On("<<al.musicOn<<"),"<<"]\n";
    return strm;
}