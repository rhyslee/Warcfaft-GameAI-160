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

/**
*
*@class CMainMenuMode
*
*@brief  This class shows the logic for the "Main Menu" mode.
*    It generates labelling for the buttons present in the main menu, as well
*    as functionality for said buttons.
*
*    List of buttons:
*
*    "Single Player Game" :- When pressed, the mode switches to Map Selection.
*
*    "Multi Player Game"  :- When pressed, the mode switches to the Multiplayer Options Menu.
*
*    "Options"            :- When pressed, the mode switches to the Options Menu Mode.
*
*    "Resume Game"        :- When pressed, the mode switches to the Battle Mode.
*
*    "Exit Game"          :- When pressed, the main window is closed (exiting the game).
*
*
*@author Greg
*
*@version 1.1
*
*@date 10/10/2017 19:21:59
*
*Contact: gjdost@ucdavis.edu
*
*/

#include "MainMenuMode.h"
#include "OptionsMenuMode.h"
#include "BattleMode.h"
#include "MultiPlayerOptionsMenuMode.h"
#include "ConnectionSelectionMenuMode.h"
#include "MapSelectionMode.h"
#include "ApplicationData.h"
#include "MemoryDataSource.h"

std::shared_ptr< CApplicationMode > CMainMenuMode::DMainMenuModePointer;

CMainMenuMode::CMainMenuMode(const SPrivateConstructorType &key){
    DTitle = "The Game";

//DButtonTexts.push_back creates a new button and gives it a label

    DButtonTexts.push_back("Single Player Game");

//DButtonFunctions.push_back gives the most recent button functionality
//In this case, when the "Single Player Game" button is pressed, the
//SinglePlayerGameButtonCallback function is activated

    DButtonFunctions.push_back(SinglePlayerGameButtonCallback);
    DButtonTexts.push_back("Multi Player Game");
    DButtonFunctions.push_back(MultiPlayerGameButtonCallback);
    DButtonTexts.push_back("Options");
    DButtonFunctions.push_back(OptionsButtonCallback);
    DButtonTexts.push_back("");
    DButtonFunctions.push_back(nullptr);
    DButtonTexts.push_back("Exit Game");
    DButtonFunctions.push_back(ExitGameButtonCallback);
}

/**
*When the "Single Player Game" button is pressed, change the gametype to Map 
*Selection mode (see CMapSelectionMode).
*
*@param[in] context The data for the game's current state.
*
*@return Nothing.
*
*/
void CMainMenuMode::SinglePlayerGameButtonCallback(std::shared_ptr< CApplicationData > context){
    context->DGameSessionType = CApplicationData::gstSinglePlayer;

    context->ChangeApplicationMode(CMapSelectionMode::Instance());
}
/**
*When the "Multi Player Game" button is pressed, change the gametype to the 
*Multiplayer Options Menu Mode (see CMultiPlayerOptionsMenuMode).
*
*@param[in] context The data for the game's current state.
*
*@return Nothing.

*
*/
void CMainMenuMode::MultiPlayerGameButtonCallback(std::shared_ptr< CApplicationData > context){
    context->ChangeApplicationMode(CConnectionSelectionMenuMode::Instance());
}
/**
*When the "Resume Game" button is pressed, change the gametype to the in game 
*battle mode (see CBattleMode).
*
*@param[in] context The data for the game's current state.
*
*@return Nothing.
*
*/

/**
*When the "Options" button is pressed, change the gametype to the Options Menu
* (see COptionsMenuMode).
*
*@param[in] context The data for the game's current state.
*
*@return Nothing.
*
*/

void CMainMenuMode::OptionsButtonCallback(std::shared_ptr< CApplicationData > context){
    context->ChangeApplicationMode(COptionsMenuMode::Instance());
}
/**
*When the "Exit Game" button is pressed, close the game window. 
*
*@param[in] context The data for the game's current state.
*
*@return Nothing.
*
*/

void CMainMenuMode::LoadButtonCallback(std::shared_ptr< CApplicationData > context){
    context->ChangeApplicationMode(CMultiPlayerOptionsMenuMode::Instance());
}

void CMainMenuMode::ExitGameButtonCallback(std::shared_ptr< CApplicationData > context){
    context->DMainWindow->Close();
}
/**
*Creates an instance of the CApplicationMode and returns a shared_ptr to it or
*just returns a shared_ptr to the current instance of the game.
*This function only allows for one instance of CApplicationMode unless
*constructor is called directly.
*
*@param[in] none.
*
*@return shared_ptr to application context.
*
*/
std::shared_ptr< CApplicationMode > CMainMenuMode::Instance(){
    if(DMainMenuModePointer == nullptr){
        DMainMenuModePointer = std::make_shared< CMainMenuMode >(SPrivateConstructorType());
    }
    return DMainMenuModePointer;
}
