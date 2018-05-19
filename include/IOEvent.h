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
#ifndef IOEVENT_H
#define IOEVENT_H
#include <memory>
#include <cstdint>

class CIOChannel;

struct SIOEventType{
    uint32_t DValue = 0;
    static const uint32_t EventIn;
    static const uint32_t EventOut;
    static const uint32_t EventPriority;
    static const uint32_t EventError;
    static const uint32_t EventHangUp;
    static const uint32_t EventInvalid;
    
    void SetIn();
    void SetOut();
    void SetPriority();
    void SetError();
    void SetHangUp();  
    void SetInvalid();
    
    void ClearIn();
    void ClearOut();
    void ClearPriority();
    void ClearError();
    void ClearHangUp();  
    void ClearInvalid();
    
    bool IsIn() const;
    bool IsOut() const;
    bool IsPriority() const;
    bool IsError() const;
    bool IsHangUp() const;
    bool IsInvalid() const;
};

using TIOCalldata = void *;
using TIOCallback = bool (*)(std::shared_ptr<CIOChannel> channel, SIOEventType &event, TIOCalldata data);

#endif

