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
#ifndef IOFACTORY_H
#define IOFACTORY_H
#include "IOChannel.h"

class CIOFactory{         
    public:
        struct SFileOptions{
            uint32_t DValue = 0;
            static const uint32_t ReadOnly;
            static const uint32_t WriteOnly;
            static const uint32_t ReadWrite;
            static const uint32_t Append;
            static const uint32_t Truncate;
            static const uint32_t Create;
            static const uint32_t Exclusive;
            
            void SetReadOnly();
            void SetWriteOnly();
            void SetReadWrite();
            void SetAppend();
            void SetTruncate();
            void SetCreate();
            void SetExclusive();
            
            void ClearReadOnly();
            void ClearWriteOnly();
            void ClearReadWrite();
            void ClearAppend();
            void ClearTruncate();
            void ClearCreate();
            void ClearExclusive(); 
        };

        static std::shared_ptr<CIOChannel> NewFileIOChannel(const std::string &filename, const struct SFileOptions &options);
};

#endif

