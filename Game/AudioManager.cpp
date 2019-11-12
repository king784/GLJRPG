#include "AudioManager.h"

Audiomanager::~Audiomanager()
{
    alDeleteSources(1, &source);
    alDeleteBuffers(1, &buffer);
    alcDestroyContext(context);
    alcCloseDevice(audioDevice);
    // delete[] bgMusic.data;
}

void Audiomanager::StartAudioManager()
{
    // Open a handle to a device.
    audioDevice = alcOpenDevice(NULL);
    if(!audioDevice)
    {
        std::cerr << "Device error!" << std::endl;
    }

    // Check whether OpenAL supports enumerating devices.
    ALboolean enumeration;
    enumeration = alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT");
    if(enumeration == AL_FALSE)
    {
        // Enumeration not supported. Means that listing the audio devices should only return default device.
        GetAudioDevices(alcGetString(NULL, ALC_DEVICE_SPECIFIER));
    }
    else
    {
        // Enumeration is supported.
        GetAudioDevices(alcGetString(NULL, ALC_DEVICE_SPECIFIER));
    }

    // Create and initialize a context.  
    context = alcCreateContext(audioDevice, NULL);
    if(!alcMakeContextCurrent(context))
    {
        std::cerr << "Failed to make audio context!" << std::endl;
    } 

    CheckForErrors();

    // Set listener
    ALfloat newListenPoint[] = {0.0f, 0.0f, 0.0f};
    SetListenerPoint(newListenPoint);

    // Create source
    CreateSource();
    
    char path[] = "D:/Projects/OpenGL/GLJRPG/Game/Sounds/Music/alenarag.wav";
    // Load wav data
    alutLoadWAVFile((ALbyte*)path, &bgMusic.format, &bgMusic.data, &bgMusic.size, &bgMusic.frequency, &bgMusic.loop);
    // buffer = alutCreateBufferFromFile(path);
    // bgMusic.data = LoadWav("C:/Users/teemu.turku/Documents/GitHub/GLJRPG/Game/Sounds/Music/alenarag.wav", 
    // bgMusic.channel, bgMusic.sampleRate, bgMusic.bps, bgMusic.size);

    // SetAudioFormat(bgMusic);

    // Generate buffer and buffer the data
    alGenBuffers((ALuint)1, &buffer);

    alBufferData(buffer, bgMusic.format, bgMusic.data, bgMusic.size, bgMusic.frequency);
    CheckForErrors();

    // free memory used by data
    alutUnloadWAV(bgMusic.format, bgMusic.data, bgMusic.size, bgMusic.frequency);

    alSourcei(source, AL_BUFFER, buffer);
    // Play the source
    alSourcePlay(source);
    CheckForErrors();

    // Get source state. This can be used to check if the source is playing.
    // ALint source_state;
    // alGetSourcei(source, AL_SOURCE_STATE, &source_state);
    // while(source_state == AL_PLAYING)
    // {
    //     alGetSourcei(source, AL_SOURCE_STATE, &source_state);
    //     CheckForErrors();
    // }
}

void Audiomanager::GetAudioDevices(const ALCchar* devices)
{
    const ALCchar* device = devices, *next = devices + 1;
    size_t len = 0;

    std::cout << "Devices list: " << std::endl;
    std::cout << "------------" << std::endl;
    while(device && *device != '\0' && next && *next != '\0')
    {
        std::cout << device << std::endl;
        len = strlen(device); 
        device += (len + 1);
        next += (len + 2);
    }
    std::cout << "------------" << std::endl;
}

ALint Audiomanager::GetSourceState()
{
    alGetSourcei(source, AL_SOURCE_STATE, &source_state);
    return source_state;
}

void Audiomanager::SetListenerPoint(const ALfloat* newPoint)
{
    for(int i = 0; i < 3; i++)
    {
        listenerPoint[i] = newPoint[i];
    }
    alListener3f(AL_POSITION, 0, 0, 1.0f);
    CheckForErrors();
}

void Audiomanager::CreateSource()
{
    alGenSources((ALuint)1, &source);
    CheckForErrors();
    alSourcef(source, AL_PITCH, 1);
    CheckForErrors();
    alSourcef(source, AL_GAIN, 1);
    CheckForErrors();
    alSource3f(source, AL_POSITION, 0, 0, 0);
    CheckForErrors();
    alSource3f(source, AL_VELOCITY, 0, 0, 0);
    CheckForErrors();
    alSourcei(source, AL_LOOPING, AL_FALSE);
    CheckForErrors();
}

char* Audiomanager::LoadWav(const char* fn, int& chan, int& samplerate, int& bps, int& size)
{
    char buffer[4];
    std::ifstream in(fn, std::ios::binary);
    in.read(buffer, 4);
    if (strncmp(buffer, "RIFF", 4) != 0)
    {
        std::cout << "this is not a valid WAVE file" << std::endl;
        return NULL;
    }
    in.read(buffer, 4);
    in.read(buffer, 4);      //WAVE
    in.read(buffer, 4);      //fmt
    in.read(buffer, 4);      //16
    in.read(buffer, 2);      //1
    in.read(buffer, 2);
    chan = ConvertToInt(buffer, 2);
    in.read(buffer, 4);
    samplerate = ConvertToInt(buffer, 4);
    in.read(buffer, 4);
    in.read(buffer, 2);
    in.read(buffer, 2);
    bps = ConvertToInt(buffer, 2);
    in.read(buffer, 4);      //data
    in.read(buffer, 4);
    size = ConvertToInt(buffer, 4);
    char* data = new char[size];
    in.read(data, size);
    return data;
}

void Audiomanager::SetAudioFormat(Sound& sound)
{
    if (sound.channel == 1)
    {
        if (sound.bps == 8)
        {
            sound.format = AL_FORMAT_MONO8;
        }
        else 
        {
            sound.format = AL_FORMAT_MONO16;
        }
    }
    else {
        if (sound.bps == 8)
        {
            sound.format = AL_FORMAT_STEREO8;
        }
        else 
        {
            sound.format = AL_FORMAT_STEREO16;
        }
    }
}

void Audiomanager::CheckForErrors()
{
    // Check error
    ALCenum error;
    error = alGetError();
    if (error != AL_NO_ERROR)
    {
        std::cerr << "Error! Here is what happened: " << error << std::endl;
    }
}