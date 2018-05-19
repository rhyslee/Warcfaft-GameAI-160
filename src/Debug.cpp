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
#include "Debug.h"

#ifdef DEBUG


int CDebug::DDebugLevel  = 0;
FILE *CDebug::DDebugFile = nullptr;
std::shared_ptr< CDebug > CDebug::DDebugPointer;

CDebug::CDebug(const SPrivateDebugType &key){
    
}

CDebug::~CDebug(){
    if(DDebugFile){
        fclose(DDebugFile);
        DDebugFile = nullptr;
        DDebugLevel = 0;
    }
}

bool CDebug::CreateDebugFile(const std::string &filename, int level){
    if(DDebugFile){
        return false;   
    }
    if(DDebugPointer){
        return false;   
    }
    if(0 > DDebugLevel){
        return false;   
    }
    DDebugFile = fopen(filename.c_str(), "w");
    if(nullptr == DDebugFile){
        return false;    
    }
    DDebugPointer = std::make_shared< CDebug > (SPrivateDebugType{});
    DDebugLevel = level + 1;
    
    return true;
}

#endif

