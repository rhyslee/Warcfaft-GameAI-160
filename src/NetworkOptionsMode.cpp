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
#include "NetworkOptionsMode.h"
#include "MultiPlayerOptionsMenuMode.h"
#include "ApplicationData.h"
#include "OptionsMenuMode.h"
#include <fstream>
#include <iostream>
#include <regex>


static std::string DInput;
/**
*
*@class CNetworkOptionsMode
*
*@brief This class provides text and functionality for the buttons present in the Network Options mode.
*       These buttons include:
*
*                              OK:- Executes NetworkOptionsUpdateButtonCallBack, which applies any changes
*                                   to Network Options.
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
// regex pattern for valid IP address

std::string CNetworkOptionsMode::DHiddenPassword = "";
std::shared_ptr< CNetworkOptionsMode > CNetworkOptionsMode::DNetworkOptionsModePointer;

CNetworkOptionsMode::CNetworkOptionsMode(const SPrivateConstructorType &key){
    DTitle = "Network Options";
    DButtonTexts.push_back("OK");
    DButtonFunctions.push_back(NetworkOptionsUpdateButtonCallback);
    DButtonTexts.push_back("Cancel");
    DButtonFunctions.push_back(BackButtonCallback);

    DEditTitles.push_back("User Name:");
    DEditValidationFunctions.push_back(ValidHostnameCallback);
    DEditTitles.push_back("Password:");
    DEditValidationFunctions.push_back(ValidPasswordCallback);
    DEditTitles.push_back("IP Address:");
    DEditValidationFunctions.push_back(ValidIPAddressCallback);
    DEditTitles.push_back("Port:");
    DEditValidationFunctions.push_back(ValidPortNumberCallback);
}

bool CNetworkOptionsMode::ValidIPAddressCallback(const std::string &str){
    std::regex IPAddressTemplate("([0-9]?[0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])\\.([0-9]?[0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])\\.([0-9]?[0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])\\.([0-9]?[0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])");

    if(std::regex_match(str, IPAddressTemplate))
        return true;

    return false;
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
bool CNetworkOptionsMode::ValidPasswordCallback(const std::string &str){
    // add character
    while(DHiddenPassword.length() < DNetworkOptionsModePointer->DEditText[1].length() && DNetworkOptionsModePointer->DEditText[1].length() != 0)
        DHiddenPassword += DNetworkOptionsModePointer->DEditText[1].back();
    // remove character
    while(DHiddenPassword.length() > DNetworkOptionsModePointer->DEditText[1].length() && !DHiddenPassword.empty())
        DHiddenPassword.pop_back();

    // mask password
    for(int i = 0; i < DNetworkOptionsModePointer->DEditText[1].length(); i++)
        DNetworkOptionsModePointer->DEditText[1][i] = '*';

    // empty password is invalid
    if(DNetworkOptionsModePointer->DEditText[1].empty())
        return false;

    return true;
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
bool CNetworkOptionsMode::ValidHostnameCallback(const std::string &str){
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


bool CNetworkOptionsMode::ValidPortNumberCallback(const std::string &str){
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

void CNetworkOptionsMode::NetworkOptionsUpdateButtonCallback(std::shared_ptr< CApplicationData > context){
    for(int Index = 0; Index < DNetworkOptionsModePointer->DEditText.size(); Index++){
        if(!DNetworkOptionsModePointer->DEditValidationFunctions[Index](DNetworkOptionsModePointer->DEditText[Index])){
            return;
        }
    }
    context->DUsername = DNetworkOptionsModePointer->DEditText[0];
    context->DPassword = DHiddenPassword;
    DHiddenPassword = "";
    context->DLANIPAddress = DNetworkOptionsModePointer->DEditText[2];
    context->DLANPort = std::stoi(DNetworkOptionsModePointer->DEditText[3]);

    /*
     * Save Options
     */
    // std::ofstream output("./bin/data/opt/network.dat", std::ios::trunc);
    // output << "# Username\n" << context->DUsername << "\n";
    // output << "# RemoteHostName\n" << context->DRemoteHostname  << "\n";
    // output << "# Multiplayer Port\n" << context->DMultiplayerPort << "\n";
    // output << "# Password\n" << context->DPassword << "\n";
    // output.close();

    context->ChangeApplicationMode(COptionsMenuMode::Instance());
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
void CNetworkOptionsMode::BackButtonCallback(std::shared_ptr< CApplicationData > context){
    context->ChangeApplicationMode(CMultiPlayerOptionsMenuMode::Instance());
}

std::shared_ptr< CApplicationMode > CNetworkOptionsMode::Instance(){
    if(DNetworkOptionsModePointer == nullptr){
        DNetworkOptionsModePointer = std::make_shared< CNetworkOptionsMode >(SPrivateConstructorType());
    }
    return DNetworkOptionsModePointer;
}

void CNetworkOptionsMode::InitializeChange(std::shared_ptr< CApplicationData > context){
    DEditSelected = -1;
    DEditSelectedCharacter = -1;
    DButtonLocations.clear();
    DEditLocations.clear();
    DEditText.clear();

    DEditText.push_back(context->DUsername);
    DEditText.push_back(DHiddenPassword);
    DEditText.push_back(context->DLANIPAddress);
    DEditText.push_back(std::to_string(context->DLANPort));
}
