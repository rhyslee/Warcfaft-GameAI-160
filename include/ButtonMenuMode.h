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
#ifndef BUTTONMENUMODE_H
#define BUTTONMENUMODE_H

#include "ApplicationMode.h"
#include "Rectangle.h"
#include <vector>
#include <string>
                                   
using TButtonMenuCallbackFunction = void (*)(std::shared_ptr< CApplicationData >);

class CButtonMenuMode : public CApplicationMode{
    protected:
        std::string DTitle;
        std::vector< std::string > DButtonTexts;
        std::vector< TButtonMenuCallbackFunction > DButtonFunctions;
        std::vector< SRectangle > DButtonLocations;
        bool DButtonHovered;
        
        CButtonMenuMode(const CButtonMenuMode &) = delete;
        const CButtonMenuMode &operator =(const CButtonMenuMode &) = delete;
        CButtonMenuMode();
        
    public:
        virtual void Input(std::shared_ptr< CApplicationData > context);
        virtual void Calculate(std::shared_ptr< CApplicationData > context);
        virtual void Render(std::shared_ptr< CApplicationData > context);
};

#endif

