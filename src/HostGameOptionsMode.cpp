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

#include "HostGameOptionsMode.h"
#include "MapSelectionMode.h"
#include "ApplicationData.h"
#include "ConnectionSelectionMenuMode.h"
#include "MultiPlayerOptionsMenuMode.h"
#include <fstream>
#include <iostream>


/**
*
*@class CHostGameOptionsMode
*
*@brief This class provides text and functionality for the buttons present in the HostGame Options mode.
*       These buttons include:
*
*                              OK:- Executes HostGameOptionsUpdateButtonCallBack, which applies any changes
*                                   to HostGame Options.
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

std::string CHostGameOptionsMode::DHiddenPassword = "";
std::shared_ptr< CHostGameOptionsMode > CHostGameOptionsMode::DHostGameOptionsModePointer;

CHostGameOptionsMode::CHostGameOptionsMode(const SPrivateConstructorType &key){
    DTitle = "Host Game Options";
    DButtonTexts.push_back("Host Game");
    DButtonFunctions.push_back(HostGameOptionsUpdateButtonCallback);
    DButtonTexts.push_back("Select Map");
    DButtonFunctions.push_back(MapSelectionButtonCallback);
    DButtonTexts.push_back("Cancel");
    DButtonFunctions.push_back(BackButtonCallback);

    DEditTitles.push_back("Game Name:");
    DEditValidationFunctions.push_back(ValidHostnameCallback);
    DEditTitles.push_back("Password:");
    DEditValidationFunctions.push_back(ValidPasswordCallback);
    DEditTitles.push_back("Port:");
    DEditValidationFunctions.push_back(ValidPortNumberCallback);
    DEditTitles.push_back("Map:");
    DEditValidationFunctions.push_back(ValidSelectedMapCallback);
}


bool CHostGameOptionsMode::ValidSelectedMapCallback(const std::string &str){
    // DSelectedMapIndex
    bool ValidMap = false;
    std::vector< std::string > MapNames;  

    int i = 0;
    
    while(CAssetDecoratedMap::GetMap(i)){
        if(DHostGameOptionsModePointer->DEditText[3] == CAssetDecoratedMap::GetMap(i++)->MapName())
            ValidMap = true;
    }

    return ValidMap;
}



/**
*
*This function is called after the "Remote Hostname :" button is pressed. It checks a given port number for validity, and returns
*a value of true if the port number is valid.
*
*@param str The port number, represented by a string.
*
*@return bool True if the port number is valid, otherwise false.
*
*/
bool CHostGameOptionsMode::ValidPortNumberCallback(const std::string &str){
    try{
        int Port = std::stoi(str);
        if((1024 < Port)&&(65535 >= Port)){
            return str == std::to_string(Port);
        }
    }
    catch(std::exception &E){
        return false;
    }
    return false;
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
bool CHostGameOptionsMode::ValidHostnameCallback(const std::string &str){
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
bool CHostGameOptionsMode::ValidPasswordCallback(const std::string &str){
    // add character
    while(DHiddenPassword.length() < DHostGameOptionsModePointer->DEditText[1].length() && DHostGameOptionsModePointer->DEditText[1].length() != 0)
        DHiddenPassword += DHostGameOptionsModePointer->DEditText[1].back();
    // remove character
    while(DHiddenPassword.length() > DHostGameOptionsModePointer->DEditText[1].length() && !DHiddenPassword.empty())
        DHiddenPassword.pop_back();

    // mask password
    for(int i = 0; i < DHostGameOptionsModePointer->DEditText[1].length(); i++)
        DHostGameOptionsModePointer->DEditText[1][i] = '*';

    // empty password is invalid
    if(DHostGameOptionsModePointer->DEditText[1].empty())
        return false;

    return true;
}


void CHostGameOptionsMode::MapSelectionButtonCallback(std::shared_ptr< CApplicationData > context){
    context->ChangeApplicationMode(CMapSelectionMode::Instance());
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
void CHostGameOptionsMode::HostGameOptionsUpdateButtonCallback(std::shared_ptr< CApplicationData > context){
    // go to game lobby
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
void CHostGameOptionsMode::BackButtonCallback(std::shared_ptr< CApplicationData > context){
    context->ChangeApplicationMode(CMultiPlayerOptionsMenuMode::Instance());
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
std::shared_ptr< CApplicationMode > CHostGameOptionsMode::Instance(){
    if(DHostGameOptionsModePointer == nullptr){
        DHostGameOptionsModePointer = std::make_shared< CHostGameOptionsMode >(SPrivateConstructorType());
    }
    return DHostGameOptionsModePointer;
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
void CHostGameOptionsMode::InitializeChange(std::shared_ptr< CApplicationData > context){
    DHiddenPassword = context->DPassword;
    DEditSelected = -1;
    DEditSelectedCharacter = -1;
    DButtonLocations.clear();
    DEditLocations.clear();
    DEditText.clear();
    // DEditText.push_back(context->DUsername);
    DEditText.push_back("");
    DEditText.push_back("");

    DEditText.push_back("");
    // DEditText.push_back(DHiddenPassword);
    if(context->DMapConfirmed)
        DEditText.push_back(context->DSelectedMap->MapName());
    else
        DEditText.push_back("None");

}


void CHostGameOptionsMode::Input(std::shared_ptr< CApplicationData > context){
    int CurrentX, CurrentY;
    CurrentX = context->DCurrentX;
    CurrentY = context->DCurrentY;
    if(context->DLeftClick && !context->DLeftDown){
        bool ClickedEdit = false;
        for(int Index = 0; Index < DButtonLocations.size(); Index++){
            if(DButtonLocations[Index].PointInside(CurrentX, CurrentY)){
                DButtonFunctions[Index](context);
            }
        }
        for(int Index = 0; Index < DEditLocations.size() - 1; Index++){
            if(DEditLocations[Index].PointInside(CurrentX, CurrentY)){
                if(Index != DEditSelected){
                    DEditSelected = Index;
                    DEditSelectedCharacter = DEditText[Index].size();
                    ClickedEdit = true;
                }
            } 
        }
        if(!ClickedEdit){
            DEditSelected = -1;     
        }
    }
    for(auto Key : context->DReleasedKeys){
        if(SGUIKeyType::Escape == Key){
            DEditSelected = -1;
        }
        else if(0 <= DEditSelected){
            SGUIKeyType TempKey;
            TempKey.DValue = Key;
            if((SGUIKeyType::Delete == Key)||(SGUIKeyType::BackSpace == Key)){
                if(DEditSelectedCharacter){             
                    DEditText[DEditSelected] = DEditText[DEditSelected].substr(0,DEditSelectedCharacter-1) + DEditText[DEditSelected].substr(DEditSelectedCharacter, DEditText[DEditSelected].length()-DEditSelectedCharacter);
                    DEditSelectedCharacter--;
                }
                else if(DEditText[DEditSelected].length()){
                    DEditText[DEditSelected] = DEditText[DEditSelected].substr(1);
                }
            }
            else if(SGUIKeyType::LeftArrow == Key){
                if(DEditSelectedCharacter){
                   DEditSelectedCharacter--; 
                }
            }
            else if(SGUIKeyType::RightArrow == Key){
                if(DEditSelectedCharacter < DEditText[DEditSelected].length()){
                   DEditSelectedCharacter++; 
                }
            }
            else if(TempKey.IsAlphaNumeric()||(SGUIKeyType::Period == Key)){
                DEditText[DEditSelected] = DEditText[DEditSelected].substr(0,DEditSelectedCharacter) + std::string(1, (char)Key) + DEditText[DEditSelected].substr(DEditSelectedCharacter,DEditText[DEditSelected].length()-DEditSelectedCharacter);
                DEditSelectedCharacter++;
            }
        }
    }
    context->DReleasedKeys.clear();
}