#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <iostream>
#include <fstream>
#include <cstring>
#include <string>

#include <AL/al.h>
#include <AL/alc.h>

#include "CustomMath.h"

struct Sound
{
    int channel;
    int sampleRate;
    int bps;
    int size;
    int format;
    char* pathToSound;
    char* data;
};

class Audiomanager
{
public:
    Audiomanager();
    ~Audiomanager();
    void GetAudioDevices(const ALCchar* devices);
    void SetListenerPoint(const ALfloat* newPoint);
    void CreateSource();
    char* LoadWav(const char* fn, int& chan, int& samplerate, int& bps, int& size);
    void SetAudioFormat(Sound& sound);
    void CheckForErrors();

private:
    ALCdevice* audioDevice;
    ALCcontext* context;
    ALfloat listenerPoint[3];
    ALuint source;
    ALuint buffer;
    Sound bgMusic;
};

#endif