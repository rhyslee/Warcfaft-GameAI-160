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
#ifndef CONNECTIONSELECTIONMODE_H
#define CONNECTIONSELECTIONMODE_H

#include "ButtonMenuMode.h"

class CConnectionSelectionMenuMode : public CButtonMenuMode{
    protected:
        struct SPrivateConstructorType{};
        static std::shared_ptr< CApplicationMode > DConnectionSelectionMenuModePointer;

        static void DirectConnectButtonCallback(std::shared_ptr< CApplicationData > context);
        static void ServerConnectButtonCallback(std::shared_ptr< CApplicationData > context);
        static void BackButtonCallback(std::shared_ptr< CApplicationData > context);

        CConnectionSelectionMenuMode(const CConnectionSelectionMenuMode &) = delete;
        const CConnectionSelectionMenuMode &operator =(const CConnectionSelectionMenuMode &) = delete;

    public:
        explicit CConnectionSelectionMenuMode(const SPrivateConstructorType &key);

        static std::shared_ptr< CApplicationMode > Instance();
};

#endif
