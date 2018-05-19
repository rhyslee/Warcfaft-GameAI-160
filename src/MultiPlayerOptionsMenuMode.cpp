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
*@class CMultiPlayerOptionsMenuMode
*
*@brief This class provides text and functionality for the buttons present in the multiplayer
*       options menu.
*
*       The buttons include:
*
*                            Host Multi Player Game:- Calls the function HostMultiPlayerButtonCallBack,
*                                                     which sets the user to be the host, and switches 
*                                                     to map selection mode. (See CMapSelectionMode)
*                            
*                            Join Multi Player Game:- Calls the function JoinMultiPlayerButtonCallBack
*                                                     (no currently implemented functionality).
*      
*                            Back :-                   Calls the function MainMenuButtonCallBack, which 
*                                                      switches to main menu mode. (See CMainMenuMode)
*
*@author Greg
*
*@version 1.1
*
*@date 10/11/2017 00:58:34
*                         
*Contact: gjdost@ucdavis.edu                       
*/


#include "MultiPlayerOptionsMenuMode.h"
#include "HostGameOptionsMode.h"
#include "ConnectionSelectionMenuMode.h"
#include "MainMenuMode.h"
#include "ApplicationData.h"
#include "MapSelectionMode.h"
#include "NetworkOptionsMode.h"

std::shared_ptr< CApplicationMode > CMultiPlayerOptionsMenuMode::DMultiPlayerOptionsMenuModePointer;
        

CMultiPlayerOptionsMenuMode::CMultiPlayerOptionsMenuMode(const SPrivateConstructorType &key){
    DTitle = "Multi Player Game Options";    
    DButtonTexts.push_back("Host Multi Player Game");
    DButtonFunctions.push_back(HostMultiPlayerButtonCallback);
    DButtonTexts.push_back("Join Multi Player Game");
    DButtonFunctions.push_back(JoinMultiPlayerButtonCallback);
    DButtonTexts.push_back("");
    DButtonFunctions.push_back(nullptr);
    DButtonTexts.push_back("Back");
    DButtonFunctions.push_back(BackButtonCallback);
}
/**
*
*Gives functionality for the "Host Multi Player Game" button. Sets the player as a multiplayer host, and changes the mode to map selection.
*
*@param[in] context The current state of the game.
*
*@return Nothing.
*
*/
void CMultiPlayerOptionsMenuMode::HostMultiPlayerButtonCallback(std::shared_ptr< CApplicationData > context){
    context->DGameSessionType = CApplicationData::gstMultiPlayerHost;
    
    context->ChangeApplicationMode(CHostGameOptionsMode::Instance());
}

/**
*
*Gives functionality for the "Join Multi Player Game" button. Currently has no written implementation.
*
*@param[in] context The current state of the game.
*
*@return Nothing.
*
*/
void CMultiPlayerOptionsMenuMode::JoinMultiPlayerButtonCallback(std::shared_ptr< CApplicationData > context){
    context->ChangeApplicationMode(CNetworkOptionsMode::Instance());
}

/**
*
*Gives functionality for the "Back" button, by changing the mode to the Main Menu.
*
*@param[in] context The current state of the game.
*
*@return Nothing.
*
*/
void CMultiPlayerOptionsMenuMode::BackButtonCallback(std::shared_ptr< CApplicationData > context){
    context->ChangeApplicationMode(CConnectionSelectionMenuMode::Instance());
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
        
std::shared_ptr< CApplicationMode > CMultiPlayerOptionsMenuMode::Instance(){
    if(DMultiPlayerOptionsMenuModePointer == nullptr){
        DMultiPlayerOptionsMenuModePointer = std::make_shared< CMultiPlayerOptionsMenuMode >(SPrivateConstructorType());
    }
    return DMultiPlayerOptionsMenuModePointer;
}

