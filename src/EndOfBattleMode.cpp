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
#include "EndOfBattleMode.h"
#include "MainMenuMode.h"
#include "BattleMode.h"
#include "ApplicationData.h"

// static shared pointer to hold reference to an CEndOfBattleMode object
std::shared_ptr< CApplicationMode > CEndOfBattleMode::DEndOfBattleModePointer;

/**
* A CEndOfBattleMode object constructor
*
* @param key
*
* @return Nothing
*
*/
CEndOfBattleMode::CEndOfBattleMode(const SPrivateConstructorType &key){
    // edit end of battle message according to battle result
    if(CBattleMode::IsVictory())
        DTitle = "Congratulations! You won!";
    else
        DTitle = "Sorry, you lost!";

    // maps text and callback function
    DButtonTexts.push_back("Main Menu");
    DButtonFunctions.push_back(MainMenuButtonCallback);

}

/**
* A callback function for going back to the MainMenu
*
* @param context
*
* @return Nothing
*
*/
void CEndOfBattleMode::MainMenuButtonCallback(std::shared_ptr< CApplicationData > context){
    // deletes the current battle
    CBattleMode::EndBattle();
    context->ChangeApplicationMode(CMainMenuMode::Instance());
}

/**
* Allows only one instance of the EndOfBattleMode object throughout the program
*/
std::shared_ptr< CApplicationMode > CEndOfBattleMode::Instance(){
    if(DEndOfBattleModePointer == nullptr){
        DEndOfBattleModePointer = std::make_shared< CEndOfBattleMode >(SPrivateConstructorType());
    }
    return DEndOfBattleModePointer;
}
