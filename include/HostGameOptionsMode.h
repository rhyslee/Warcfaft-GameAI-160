/*
    Copyright (c) 2015, Christopher Nitta
    All rights reserved.

    All source material (source code, images, sounds, etc.) have been provided to
    University of California, Davis students of course ECS 160 for educational
    purposes. It may not be distributed beyond those enrloled in the course without
    prior permission from the copyright holder.

    All sound files, sound fonts, midi files, and images that have been included 
    that were extracted from original Warcraft II by Blizzard Entertainment 
    were found freely available via internet sources and have been labeld as 
    abandonware. They have been included in this distribution for educational 
    purposes only and this copyright notice does not attempt to claim any 
    ownership of this material.
*/
#ifndef HOSTGAMEOPTIONSMODE_H
#define HOSTGAMEOPTIONSMODE_H

#include "EditOptionsMode.h"

class CHostGameOptionsMode : public CEditOptionsMode{
    protected:
        struct SPrivateConstructorType{};
        static std::shared_ptr< CHostGameOptionsMode > DHostGameOptionsModePointer;
        static std::string DHiddenPassword;

        static bool ValidPasswordCallback(const std::string &str);
        static bool ValidHostnameCallback(const std::string &str);
        static bool ValidPortNumberCallback(const std::string &str);
        static bool ValidSelectedMapCallback(const std::string &str);

        static void HostGameOptionsUpdateButtonCallback(std::shared_ptr< CApplicationData > context);
        static void MapSelectionButtonCallback(std::shared_ptr< CApplicationData > context);
        static void BackButtonCallback(std::shared_ptr< CApplicationData > context);
        void Input(std::shared_ptr< CApplicationData > context);
                                               
        CHostGameOptionsMode(const CHostGameOptionsMode &) = delete;
        const CHostGameOptionsMode &operator =(const CHostGameOptionsMode &) = delete;
        
    public:
        explicit CHostGameOptionsMode(const SPrivateConstructorType &key);
        
        static std::shared_ptr< CApplicationMode > Instance();
        virtual void InitializeChange(std::shared_ptr< CApplicationData > context) override;
};

#endif
