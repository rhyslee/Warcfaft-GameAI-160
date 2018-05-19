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
#include "InGameMenuMode.h"
#include "MainMenuMode.h"
#include "BattleMode.h"
#include "OptionsMenuMode.h"
#include "ApplicationData.h"

// static shared pointer to hold reference to an CInGameMenuMode object
std::shared_ptr< CApplicationMode > CInGameMenuMode::DInGameMenuModePointer;

/**
* A CInGameMenuMode object constructor
*
* @param[in] key
*
* @return Nothing
*
*/
CInGameMenuMode::CInGameMenuMode(const SPrivateConstructorType &key){
    DTitle = "In-Game Menu";
    DButtonTexts.push_back("Resume");
    DButtonFunctions.push_back(BackButtonCallback);
    DButtonTexts.push_back("");
    DButtonFunctions.push_back(nullptr);
    DButtonTexts.push_back("");
    DButtonFunctions.push_back(nullptr);
    DButtonTexts.push_back("Options");
    DButtonFunctions.push_back(OptionsMenuButtonCallback);
    DButtonTexts.push_back("Main Menu");
    DButtonFunctions.push_back(MainMenuButtonCallback);

}

/**
* A callback function for bringing up the OptionsMenu
*
* @param context
*
* @return Nothing
*
*/
void CInGameMenuMode::OptionsMenuButtonCallback(std::shared_ptr< CApplicationData > context){
    context->ChangeApplicationMode(COptionsMenuMode::Instance());
}

// This function will be implemented
void CInGameMenuMode::LoadButtonCallback(std::shared_ptr< CApplicationData > context){
    // context->ChangeApplicationMode(COptionsMenuMode::Instance());
}

// This function will be implemented
void CInGameMenuMode::SaveButtonCallback(std::shared_ptr< CApplicationData > context){
    // context->ChangeApplicationMode(COptionsMenuMode::Instance());
}

/**
* A callback function for going back to the MainMenu
*
* @param context
*
* @return Nothing
*
*/
void CInGameMenuMode::MainMenuButtonCallback(std::shared_ptr< CApplicationData > context){
    // deletes the current battle
    CBattleMode::EndBattle();
    context->ChangeApplicationMode(CMainMenuMode::Instance());
}

/**
* A callback function for resuming a paused battle
*
* @param context
*
* @return Nothing
*
*/
void CInGameMenuMode::BackButtonCallback(std::shared_ptr< CApplicationData > context){
    // NOTE: context->ChangeApplicationMode(CBattleMode::Instance()) will restart the battle
    context->DNextApplicationMode = CBattleMode::Instance();
}

/**
* Allows only one instance of the InGameMenuMode object throughout the program
*
* @param context
*
* @return CApplicationMode shared pointer
*
*/
std::shared_ptr< CApplicationMode > CInGameMenuMode::Instance(){
    if(DInGameMenuModePointer == nullptr){
        DInGameMenuModePointer = std::make_shared< CInGameMenuMode >(SPrivateConstructorType());
    }
    return DInGameMenuModePointer;
}
