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
#ifndef APPLICATIONMODE_H
#define APPLICATIONMODE_H

#include <memory>
class CApplicationData;

class CApplicationMode{
    protected:
        float DTime;
    public:
        virtual void InitializeChange(std::shared_ptr< CApplicationData > context){};
        virtual void Input(std::shared_ptr< CApplicationData > context) = 0;
        virtual void Calculate(std::shared_ptr< CApplicationData > context) = 0;
        virtual void Render(std::shared_ptr< CApplicationData > context) = 0;
        virtual void IncrementTimer() {};
        virtual float GetTime() {};
};

#endif

