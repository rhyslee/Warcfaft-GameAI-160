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
#ifndef INGAMEMENUMODE_H
#define INGAMEMENUMODE_H

#include "ButtonMenuMode.h"

class CInGameMenuMode : public CButtonMenuMode{
    protected:
        struct SPrivateConstructorType{};
        static std::shared_ptr< CApplicationMode > DInGameMenuModePointer;

        static void OptionsMenuButtonCallback(std::shared_ptr< CApplicationData > context);
        static void MainMenuButtonCallback(std::shared_ptr< CApplicationData > context);
        static void BackButtonCallback(std::shared_ptr< CApplicationData > context);
        static void LoadButtonCallback(std::shared_ptr< CApplicationData > context);
        static void SaveButtonCallback(std::shared_ptr< CApplicationData > context);

        CInGameMenuMode(const CInGameMenuMode &) = delete;
        const CInGameMenuMode &operator =(const CInGameMenuMode &) = delete;

    public:
        explicit CInGameMenuMode(const SPrivateConstructorType &key);

        static std::shared_ptr< CApplicationMode > Instance();
};

#endif
