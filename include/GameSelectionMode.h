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
#ifndef GAMESELECTIONMODE_H
#define GAMESELECTIONMODE_H

#include "ApplicationMode.h"
#include "Rectangle.h"
#include <vector>
#include <string>
                                   
using TGameSelectionCallbackFunction = void (*)(std::shared_ptr< CApplicationData >);

class CGameSelectionMode : public CApplicationMode{
    protected:
        struct SPrivateConstructorType{};
        static std::shared_ptr< CGameSelectionMode > DGameSelectionModePointer;
        std::string DTitle;
        std::vector< std::string > DButtonTexts;
        std::vector< TGameSelectionCallbackFunction > DButtonFunctions;
        std::vector< SRectangle > DButtonLocations;
        bool DButtonHovered;
        int DMapOffset;
        
        static void SelectMapButtonCallback(std::shared_ptr< CApplicationData > context);
        static void JoinPrivateButtonCallback(std::shared_ptr< CApplicationData > context);
        static void HostGameButtonCallback(std::shared_ptr< CApplicationData > context);
        static void BackButtonCallback(std::shared_ptr< CApplicationData > context);
        CGameSelectionMode(const CGameSelectionMode &) = delete;
        const CGameSelectionMode &operator =(const CGameSelectionMode &) = delete;

    public:
        explicit CGameSelectionMode(const SPrivateConstructorType & key);
        virtual void InitializeChange(std::shared_ptr< CApplicationData > context) override;
        virtual void Input(std::shared_ptr< CApplicationData > context) override;
        virtual void Calculate(std::shared_ptr< CApplicationData > context) override;
        virtual void Render(std::shared_ptr< CApplicationData > context) override;
        static std::shared_ptr< CApplicationMode > Instance();
};

#endif

