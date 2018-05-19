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

#include "ConnectionSelectionMenuMode.h"
#include "ServerConnectOptionMode.h"
#include "LogInOptionsMode.h"
#include "MultiPlayerOptionsMenuMode.h"
#include "MainMenuMode.h"
#include "ApplicationData.h"
#include "OptionsMenuMode.h"


/**
*
*@class CConnectionSelectionMenuMode
*
*@brief This class provides text and functionality for the buttons present in the Network Options mode.
*       These buttons include:
*
*                  Direct Connect:- Executes DirectConnectButtonCallBack, which changes the application mode to
*                                   the multiplayer options menu.
*
*             Connect to a Server:- Executes ServerConnectButtonCallBack, which changes the application mode to
                                    the server connect option mode.
*
*                            Back:- Executes BackButtonCallBack, which changes the application mode to
*                                   the main menu.
*
*
*
*@author Hugo
*
*@version 4.0
*
*@date 10/24/2017 01:17:08
*
*/

std::shared_ptr< CApplicationMode > CConnectionSelectionMenuMode::DConnectionSelectionMenuModePointer;

CConnectionSelectionMenuMode::CConnectionSelectionMenuMode(const SPrivateConstructorType &key){
    DTitle = "Connection Selection";
    DButtonTexts.push_back("Direct Connect");
    DButtonFunctions.push_back(DirectConnectButtonCallback);
    DButtonTexts.push_back("Connect to a Server");
    DButtonFunctions.push_back(ServerConnectButtonCallback);
    DButtonTexts.push_back("Back");
    DButtonFunctions.push_back(BackButtonCallback);
}

/**
*
*This function is called after the "direct connect" button is pressed. The application
*mode is changed to the multiplayer options menu (See CMultiPlayerOptionsMenuMode).
*
*@param context A ptr with information about the current game state.
*
*@return Nothing.
*
*/
void CConnectionSelectionMenuMode::DirectConnectButtonCallback(std::shared_ptr< CApplicationData > context){
    context->ChangeApplicationMode(CMultiPlayerOptionsMenuMode::Instance());
}

/**
*
*This function is called after the "connect to a server" button is pressed. The application
*mode is changed to the server connect options menu (See CServerConnectOptionMode).
*
*@param context A ptr with information about the current game state.
*
*@return Nothing.
*
*/
void CConnectionSelectionMenuMode::ServerConnectButtonCallback(std::shared_ptr< CApplicationData > context){
    context->ChangeApplicationMode(CLogInOptionsMode::Instance());
}

/**
*
*This function is called after the "cancel" button is pressed. The application
*mode is changed to the main menu (See CMainMenuMode).
*
*@param context A ptr with information about the current game state.
*
*@return Nothing.
*
*/
void CConnectionSelectionMenuMode::BackButtonCallback(std::shared_ptr< CApplicationData > context){
    context->ChangeApplicationMode(CMainMenuMode::Instance());
}

/**
*Creates an instance of the CConnectionSelectionMenuMode and returns a shared_ptr to it or
*just returns a shared_ptr to the current instance of the game. 
*This function only allows for one instance of CConnectionSelectionMenuMode unless
*constructor is called directly.
*
*@param[in] none.
*
*@return shared_ptr to application context.
*
*/
std::shared_ptr< CApplicationMode > CConnectionSelectionMenuMode::Instance(){
    if(DConnectionSelectionMenuModePointer == nullptr){
        DConnectionSelectionMenuModePointer = std::make_shared< CConnectionSelectionMenuMode >(SPrivateConstructorType());
    }
    return DConnectionSelectionMenuModePointer;
}