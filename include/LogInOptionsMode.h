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
#ifndef LOGINOPTIONSMODE_H
#define LOGINOPTIONSMODE_H

#include "EditOptionsMode.h"

class CLogInOptionsMode : public CEditOptionsMode{
    protected:
        struct SPrivateConstructorType{};
        static std::shared_ptr< CLogInOptionsMode > DLogInOptionsModePointer;
        static std::string DHiddenPassword;

        static bool ValidPasswordCallback(const std::string &str);
        static bool ValidHostnameCallback(const std::string &str);
        static void LogInOptionsUpdateButtonCallback(std::shared_ptr< CApplicationData > context);
        static void BackButtonCallback(std::shared_ptr< CApplicationData > context);

        static void RegisterButtonCallback(std::shared_ptr< CApplicationData > context);
                                               
        CLogInOptionsMode(const CLogInOptionsMode &) = delete;
        const CLogInOptionsMode &operator =(const CLogInOptionsMode &) = delete;
        
    public:
        explicit CLogInOptionsMode(const SPrivateConstructorType &key);
        
        static std::shared_ptr< CApplicationMode > Instance();
        virtual void InitializeChange(std::shared_ptr< CApplicationData > context) override;
};

#endif
