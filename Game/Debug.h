#ifndef DEBUG_H
#define DEBUG_H

const unsigned int MAXPATH = 256;

#include <string>
#include <chrono>
#include <iostream>

class Debug
{
public:
    // Singleton
	static Debug& GetInstance()
	{
		static Debug instance;
		return instance;
	};

    void StartTimer()
    {
        start = std::chrono::high_resolution_clock::now();
    }

    void EndTimer()
    {
        finish = std::chrono::high_resolution_clock::now();
    }

    void PrintTime()
    {
        std::chrono::duration<double> elapsed = finish - start;
        std::cout << "Elapsed time: " << elapsed.count() << std::endl;
    }

    static void Print(std::string text)
    {
        std::cout << text << std::endl;
    }

    // static char* ExePath() 
    // {
    //     char currentDirPath[MAX_PATH];
    //     int bytes = GetModuleFileNameW(NULL, currentDirPath, MAX_PATH);
    //     if(bytes == 0)
    //         return;
    //     else
    //         return currentDirPath;
    // }

    std::chrono::time_point<std::chrono::high_resolution_clock> start, finish;
};
  
#endif