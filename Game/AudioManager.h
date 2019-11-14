#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <iostream>
#include <fstream>
#include <cstring>
#include <string>

#include <AL/alut.h>

#include "CustomMath.h"

struct Sound
{
    int channel = 0;
    int sampleRate = 0;
    int bps = 0;
	ALsizei size = 0;
	ALfloat frequency = 0.0f;
    ALenum format = AL_FORMAT_MONO8;
    char* pathToSound;
    ALvoid* data = NULL;
    ALboolean loop = AL_FALSE;
};

// struct Sound
// {
//     int channel;
//     int sampleRate;
//     int bps;
//     int size;
//     int format;
//     char* pathToSound;
//     char* data;
// };

class Audiomanager
{
public:
    //Audiomanager();
    ~Audiomanager();
    // Singleton
	static Audiomanager& GetInstance()
	{
		static Audiomanager instance;
		return instance;
	};
    void StartAudioManager();
    void GetAudioDevices(const ALCchar* devices);
    ALint GetSourceState();
    void SetListenerPoint(const ALfloat* newPoint);
    void CreateSource();
    char* LoadWav(const char* fn, int& chan, int& samplerate, int& bps, int& size);
    void SetAudioFormat(Sound& sound);
    void CheckForErrors();

private:
    ALCdevice* audioDevice = NULL;
    ALCcontext* context = NULL;
    ALfloat listenerPoint[3];
    ALuint source = 0;
    ALuint buffer = 0;
    Sound bgMusic;
    ALint source_state = 0;
};

#endif