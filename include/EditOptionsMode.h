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
#ifndef EDITOPTIONSMODE_H
#define EDITOPTIONSMODE_H

#include "ApplicationMode.h"
#include "Rectangle.h"
#include <vector>
#include <string>
                                   
using TEditOptionsCallbackFunction = void (*)(std::shared_ptr< CApplicationData >);
using TEditOptionsTextValidationCallbackFunction = bool (*)(const std::string &); 

class CEditOptionsMode : public CApplicationMode{
    protected:
        std::string DTitle;
        std::vector< std::string > DButtonTexts;
        std::vector< TEditOptionsCallbackFunction > DButtonFunctions;
        std::vector< SRectangle > DButtonLocations;
        bool DButtonHovered;
        
        int DEditSelected;
        int DEditSelectedCharacter;
        std::vector< SRectangle > DEditLocations;
        std::vector< std::string > DEditTitles;
        std::vector< std::string > DEditText;
        std::vector< TEditOptionsTextValidationCallbackFunction > DEditValidationFunctions;  
        
        CEditOptionsMode(const CEditOptionsMode &) = delete;
        const CEditOptionsMode &operator =(const CEditOptionsMode &) = delete;
        CEditOptionsMode();

    public:         
        virtual void Input(std::shared_ptr< CApplicationData > context);
        virtual void Calculate(std::shared_ptr< CApplicationData > context);
        virtual void Render(std::shared_ptr< CApplicationData > context);
};

#endif

