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

#include "LogInOptionsMode.h"
#include "ApplicationData.h"
#include "ConnectionSelectionMenuMode.h"
#include "GameSelectionMode.h"
#include <fstream>
#include <iostream>


/**
*
*@class CLogInOptionsMode
*
*@brief This class provides text and functionality for the buttons present in the LogIn Options mode.
*       These buttons include:
*
*                              OK:- Executes LogInOptionsUpdateButtonCallBack, which applies any changes
*                                   to LogIn Options.
*
*                          Cancel:- Executes OptionsButtonCallBack, which changes the application mode to
*                                   the options menu. (See COptionsMenu)
*
*                     User Name: :- Executes ValidHostnameCallback, which ensures that the given username is valid.
*
*              Remote Hostname:  :- Executes ValidHostnameCallback, which ensures that the given username is valid.
*                                   Also executes ValidPortNumberCallback, which ensures that a given port number is
*                                   valid.
*
*@author Greg
*
*@version 1.1
*
*@date 10/10/2017 01:17:08
*
*/

std::string CLogInOptionsMode::DHiddenPassword = "";
std::shared_ptr< CLogInOptionsMode > CLogInOptionsMode::DLogInOptionsModePointer;

CLogInOptionsMode::CLogInOptionsMode(const SPrivateConstructorType &key){
    DTitle = "Login Options";
    DButtonTexts.push_back("Login");
    DButtonFunctions.push_back(LogInOptionsUpdateButtonCallback);
    DButtonTexts.push_back("Register");
    DButtonFunctions.push_back(RegisterButtonCallback);
    DButtonTexts.push_back("Cancel");
    DButtonFunctions.push_back(BackButtonCallback);

    DEditTitles.push_back("User Name:");
    DEditValidationFunctions.push_back(ValidHostnameCallback);
    DEditTitles.push_back("Password:");
    DEditValidationFunctions.push_back(ValidPasswordCallback);
}



/**
*
*This function is called after the "User Name:" or "Remote Hostname:" buttons are pressed. It
*checks the given hostname, makes sure it has a valid length and uses valid characters, and
*returns true if the name is valid.
*
*@param str The hostname, represented by a string.
*
*@return bool True if the hostname is valid, otherwise false.
*
*/
bool CLogInOptionsMode::ValidHostnameCallback(const std::string &str){
    int CharSinceDot = 0;

    if(253 < str.length()){
        return false;
    }
    if(0 == str.length()){
        return false;
    }
    for(auto Char : str){
        if('.' == Char){
            if(0 == CharSinceDot){
                return false;
            }
            CharSinceDot = 0;
        }
        else{
            CharSinceDot++;
            if(63 < CharSinceDot){
                return false;
            }
            if(('-' != Char)&&(!(('0' <= Char)&&('9' >= Char)))&&(!(('a' <= Char)&&('z' >= Char)))&&(!(('A' <= Char)&&('Z' >= Char)))){
                return false;
            }
        }
    }
    return true;
}


/**
*
*This function is called after the "Password:" buttons are pressed. It checks
*the given password, makes sure it has a valid length, and returns true if the
*password is valid.
*
*@param str The password, represented by a string.
*
*@return bool True if the password is valid, otherwise false.
*
*/
bool CLogInOptionsMode::ValidPasswordCallback(const std::string &str){
    // add character
    while(DHiddenPassword.length() < DLogInOptionsModePointer->DEditText[1].length() && DLogInOptionsModePointer->DEditText[1].length() != 0)
        DHiddenPassword += DLogInOptionsModePointer->DEditText[1].back();
    // remove character
    while(DHiddenPassword.length() > DLogInOptionsModePointer->DEditText[1].length() && !DHiddenPassword.empty())
        DHiddenPassword.pop_back();

    // mask password
    for(int i = 0; i < DLogInOptionsModePointer->DEditText[1].length(); i++)
        DLogInOptionsModePointer->DEditText[1][i] = '*';

    // empty password is invalid
    if(DLogInOptionsModePointer->DEditText[1].empty())
        return false;

    return true;
}


/**
*
*This function is called after the "OK" button is pressed. Any changes made to settings are updated and saved, and the application mode is changed
*to the options menu (See COptionsMenuMode).
*
*@param context A ptr with information about the current game state.
*
*
*@return Nothing.
*
*/
void CLogInOptionsMode::LogInOptionsUpdateButtonCallback(std::shared_ptr< CApplicationData > context){
    // int ErrorCode = user_auth(context->DUsername, context->DPassword);

    // if(ErrorCode == 0)
        context->ChangeApplicationMode(CGameSelectionMode::Instance());
    // else
        // Error(ErrorCode);
    // error screen to be added
}


void CLogInOptionsMode::RegisterButtonCallback(std::shared_ptr< CApplicationData > context){
    system("xdg-open 'http://34.214.129.0/signup/signup.html'");
}
/**
*
*This function is called after the "cancel" button is pressed. Similar functionality to the "OK" button, as the application
*mode is changed to the options menu (see COptionsMenuMode), but no changes are performed or saved.
*
*@param context A ptr with information about the current game state.
*
*@return Nothing.
*
*/
void CLogInOptionsMode::BackButtonCallback(std::shared_ptr< CApplicationData > context){
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
std::shared_ptr< CApplicationMode > CLogInOptionsMode::Instance(){
    if(DLogInOptionsModePointer == nullptr){
        DLogInOptionsModePointer = std::make_shared< CLogInOptionsMode >(SPrivateConstructorType());
    }
    return DLogInOptionsModePointer;
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
void CLogInOptionsMode::InitializeChange(std::shared_ptr< CApplicationData > context){
    DHiddenPassword = context->DPassword;
    DEditSelected = -1;
    DEditSelectedCharacter = -1;
    DButtonLocations.clear();
    DEditLocations.clear();
    DEditText.clear();
    // DEditText.push_back(context->DUsername);
    DEditText.push_back("");
    // DEditText.push_back(DHiddenPassword);
    DEditText.push_back("");
}
