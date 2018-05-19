/*
    Copyright (c) 2015, Christopher Nitta
    All rights reserved.

    All source material (source code, images, sounds, etc.) have been provided to
    University of California, Davis students of course ECS 160 for educational
    purposes. It may not be distributed beyond those enrolled in the course without
    prior permission from the copyright holder.

    All sound files, sound fonts, midi files, and images that have been included 
    that were extracted from original Warcraft II by Blizzard Entertainment 
    were found freely available via internet sources and have been labeld as 
    abandonware. They have been included in this distribution for educational 
    purposes only and this copyright notice does not attempt to claim any 
    ownership of this material.
*/
#ifndef DEBUG_H
#define DEBUG_H
#include <cstdio>
#include <string>
#include <memory>

#ifdef DEBUG
class CDebug{
    struct SPrivateDebugType{};
    protected:
        static int DDebugLevel;
        static FILE *DDebugFile;
        static std::shared_ptr< CDebug > DDebugPointer;
        
        CDebug(const CDebug &) = delete;
        const CDebug &operator =(const CDebug &) = delete;
        
    public:
        explicit CDebug(const SPrivateDebugType &key);
        ~CDebug();
        
        static int DebugLevel(){
            return DDebugLevel;
        };
        
        static FILE *DebugFile(){
            return DDebugFile;  
        };

        static bool CreateDebugFile(const std::string &filename, int level);
};
#define PrintDebug(level, format, ...)  (CDebug::DebugLevel() > (level) ? fprintf(CDebug::DebugFile(), (format), ##__VA_ARGS__),fflush(CDebug::DebugFile()) : 0)
#define OpenDebug(filename, level)      (CDebug::CreateDebugFile(filename, level))
#else
#define PrintDebug(level, format, ...)  (0)
#define OpenDebug(filename, level)      (true)
#endif

#define DEBUG_LOW       0
#define DEBUG_MEDIUM    1
#define DEBUG_HIGH      2

#define PrintError(format, ...)         fprintf(stderr, format, ##__VA_ARGS__)

#endif

