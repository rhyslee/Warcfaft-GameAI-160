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
#ifndef MAINMENUMODE_H
#define MAINMENUMODE_H

#include "ButtonMenuMode.h"

class CMainMenuMode : public CButtonMenuMode{
    protected:
        struct SPrivateConstructorType{};
        static std::shared_ptr< CApplicationMode > DMainMenuModePointer;

        static void SinglePlayerGameButtonCallback(std::shared_ptr< CApplicationData > context);
        static void MultiPlayerGameButtonCallback(std::shared_ptr< CApplicationData > context);
        static void ResumeBattleModeCallback(std::shared_ptr< CApplicationData > context);
        static void OptionsButtonCallback(std::shared_ptr< CApplicationData > context);
        static void ExitGameButtonCallback(std::shared_ptr< CApplicationData > context);
        static void LoadButtonCallback(std::shared_ptr< CApplicationData > context);

        CMainMenuMode(const CMainMenuMode &) = delete;
        const CMainMenuMode &operator =(const CMainMenuMode &) = delete;

    public:
        explicit CMainMenuMode(const SPrivateConstructorType &key);

        static std::shared_ptr< CApplicationMode > Instance();
};

#endif
