#ifndef CUSTOMMATH_H
#define CUSTOMMATH_H

static bool IsBigEndian()
{
    int a = 1;
    return !((char*)&a)[0];
}

static int ConvertToInt(char* buffer, int len)
{
    int a = 0;
    if (!IsBigEndian())
        for (int i = 0; i<len; i++)
            ((char*)&a)[i] = buffer[i];
    else
        for (int i = 0; i<len; i++)
            ((char*)&a)[3 - i] = buffer[i];
    return a;
}

#endif