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

#include "ServerConnectOptionMode.h"
#include "ConnectionSelectionMenuMode.h"
#include "MultiPlayerOptionsMenuMode.h"
#include "ApplicationData.h"
#include "OptionsMenuMode.h"


/**
*
*@class CServerConnectOptionMode
*
*@brief This class provides text and functionality for the buttons present in the Server Connect Options mode.
*       These buttons include:
*
*                              OK:- Executes ConnectionUpdateButtonCallBack, which applies any changes
*                                   to Network Options.
*
*                          Cancel:- Executes BackButtonCallBack, which changes the application mode to
*                                   the connection selection menu. (See CConnectionSelectionMenu)
*
*                     User Name: :- Executes ValidIPAddressCallback, which ensures that the given IP address is valid.
*
*
*@author Hugo
*
*@version 4.0
*
*@date 10/24/2017 01:17:08
*
*/

// regex pattern for valid IP address
std::shared_ptr< CServerConnectOptionMode > CServerConnectOptionMode::DServerConnectOptionModePointer;

CServerConnectOptionMode::CServerConnectOptionMode(const SPrivateConstructorType &key){
    DTitle = "Connect to A Server";
    DButtonTexts.push_back("OK");
    DButtonFunctions.push_back(ConnectionUpdateButtonCallback);
    DButtonTexts.push_back("Cancel");
    DButtonFunctions.push_back(BackButtonCallback);

    DEditTitles.push_back("Server Address:");
    DEditValidationFunctions.push_back(ValidIPAddressCallback);
}

bool CServerConnectOptionMode::ValidIPAddressCallback(const std::string &str){
    std::regex IPAddressTemplate("([0-9]?[0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])\\.([0-9]?[0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])\\.([0-9]?[0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])\\.([0-9]?[0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])");

    if(std::regex_match(str, IPAddressTemplate))
        return true;

    return false;
}

/**
* 
*This function is called after the "OK" button is pressed. Any changes made to settings are updated and saved, and the application mode is changed
*to the multiplayer options menu (See CMultiPlayerOptionsMenuMode).
*
*@param context A ptr with information about the current game state.
*
*
*@return Nothing.
*
*/
void CServerConnectOptionMode::ConnectionUpdateButtonCallback(std::shared_ptr< CApplicationData > context){
    context->ChangeApplicationMode(CMultiPlayerOptionsMenuMode::Instance());
}

/**
*
*This function is called after the "cancel" button is pressed. Similar functionality to the "OK" button, as the application
*mode is changed to the multiplayer options menu (See CMultiPlayerOptionsMenuMode), but no changes are performed or saved.
*
*@param context A ptr with information about the current game state.
*
*@return Nothing.
*
*/
void CServerConnectOptionMode::BackButtonCallback(std::shared_ptr< CApplicationData > context){
    context->ChangeApplicationMode(CConnectionSelectionMenuMode::Instance());
}

/**
*Creates an instance of the CServerConnectionOptionMode and returns a shared_ptr to it or
*just returns a shared_ptr to the current instance of the game. 
*This function only allows for one instance of CServerConnectionOptionMode unless
*constructor is called directly.
*
*@param[in] none.
*
*@return shared_ptr to application context.
*
*/
std::shared_ptr< CApplicationMode > CServerConnectOptionMode::Instance(){
    if(DServerConnectOptionModePointer == nullptr){
        DServerConnectOptionModePointer = std::make_shared< CServerConnectOptionMode >(SPrivateConstructorType());
    }
    return DServerConnectOptionModePointer;
}

/**
*
*This function gives initial values for all of the possible settings.
*
*@param context A ptr with information about the current game state.
*
*@return Nothing
*
*/
void CServerConnectOptionMode::InitializeChange(std::shared_ptr< CApplicationData > context){
    DEditSelected = -1;
    DEditSelectedCharacter = -1;
    DButtonLocations.clear();
    DEditLocations.clear();
    DEditText.clear();
    DEditText.push_back("");
}
