#include "AudioManager.h"

Audiomanager::Audiomanager()
{
    // Open a handle to a device.
    audioDevice = alcOpenDevice(0);
    if(!audioDevice)
    {
        std::cerr << "Device error!" << std::endl;
    }

    // Check whether OpenAL supports enumerating devices.
    ALboolean enumeration;
    enumeration = alcIsExtensionPresent(0, "ALC_ENUMERATION_EXT");
    if(enumeration == AL_FALSE)
    {
        // Enumeration not supported. Means that listing the audio devices should only return default device.
        GetAudioDevices(alcGetString(0, ALC_DEVICE_SPECIFIER));
    }
    else
    {
        // Enumeration is supported.
        GetAudioDevices(alcGetString(0, ALC_DEVICE_SPECIFIER));
    }   
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