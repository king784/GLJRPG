#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <iostream>
#include <cstring>
#include <string>

#include <irrKlang.h>
using namespace irrklang;

class Audiomanager
{
public:
    Audiomanager();

private:
    ISoundEngine* soundEngine;
};

#endif