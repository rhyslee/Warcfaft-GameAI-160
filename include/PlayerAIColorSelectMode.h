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
#ifndef PLAYERAICOLORSELECTMODE_H
#define PLAYERAICOLORSELECTMODE_H

#include "ApplicationMode.h"
#include "Rectangle.h"
#include "GameDataTypes.h"
#include <vector>
#include <string>
                                   
using TPlayerAIColorSelectCallbackFunction = void (*)(std::shared_ptr< CApplicationData >);

class CPlayerAIColorSelectMode : public CApplicationMode{
    protected:
        struct SPrivateConstructorType{};
        static std::shared_ptr< CPlayerAIColorSelectMode > DPlayerAIColorSelectModePointer;
        std::string DTitle;
        std::vector< std::string > DButtonTexts;
        std::vector< TPlayerAIColorSelectCallbackFunction > DButtonFunctions;
        std::vector< SRectangle > DButtonLocations;
        std::vector< SRectangle > DColorButtonLocations;
        std::vector< SRectangle > DPlayerTypeButtonLocations;
        EPlayerColor DPlayerColorRequestingChange;
        EPlayerColor DPlayerColorChangeRequest;
        EPlayerColor DPlayerColorRequesTypeChange;
        EPlayerColor DPlayerColorSelected;
        bool DButtonHovered;
        int DMapOffset;
        
        static void PlayGameButtonCallback(std::shared_ptr< CApplicationData > context);
        static void BackButtonCallback(std::shared_ptr< CApplicationData > context);
        static void SetPlayerColor(std::shared_ptr< CApplicationData > context, EPlayerColor color);
        CPlayerAIColorSelectMode(const CPlayerAIColorSelectMode &) = delete;
        const CPlayerAIColorSelectMode &operator =(const CPlayerAIColorSelectMode &) = delete;

    public:
        explicit CPlayerAIColorSelectMode(const SPrivateConstructorType & key);
        virtual void InitializeChange(std::shared_ptr< CApplicationData > context) override;
        virtual void Input(std::shared_ptr< CApplicationData > context) override;
        virtual void Calculate(std::shared_ptr< CApplicationData > context) override;
        virtual void Render(std::shared_ptr< CApplicationData > context) override;
        static std::shared_ptr< CApplicationMode > Instance();
        EPlayerColor PlayerColor(){
        	return DPlayerColorSelected;
        }
};

#endif

