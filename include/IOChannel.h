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
#ifndef IOCHANNEL_H
#define IOCHANNEL_H
#include "IOEvent.h"
#include "DataSource.h"
#include "DataSink.h"

class CIOChannel{
    public:       
        virtual std::shared_ptr<CDataSource> CreateDataSouce() = 0;
        virtual std::shared_ptr<CDataSink> CreateDataSink() = 0;
        
        virtual void SetInEventCallback(TIOCalldata calldata, TIOCallback callback) = 0;
        virtual void SetOutEventCallback(TIOCalldata calldata, TIOCallback callback) = 0;
        virtual void SetPriorityEventCallback(TIOCalldata calldata, TIOCallback callback) = 0;
        virtual void SetErrorEventCallback(TIOCalldata calldata, TIOCallback callback) = 0;
        virtual void SetHangUpEventCallback(TIOCalldata calldata, TIOCallback callback) = 0;
        virtual void SetInvalidEventCallback(TIOCalldata calldata, TIOCallback callback) = 0;

};

#endif

