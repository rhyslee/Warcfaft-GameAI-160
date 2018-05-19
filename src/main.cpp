/*
    Copyright (c) 2015, Christopher Nitta
    All rights reserved.

    All source material (source code, images, sounds, EEventType::c.) have been provided to
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
 * @brief 
 *      main.cpp , where the game will begin execution
 *   
 * @author Christopher Ta
 *
 * @version 4.0
 *
 * @date 10/29/2017
 *
 * Contact: cta@ucdavis.edu
 *
*/
#include "ApplicationData.h"
#include "Debug.h"

#ifndef DEBUG_LEVEL
#define DEBUG_LEVEL DEBUG_HIGH
#endif

/**
 * Main function where execution of the game will begin
 *
 * @param[in] argc Integer containing the number of command line arguments, indexed from 0
 * @param[in] argv Character pointer containing the command line arguments, indexed by argc
 *
 * @return ReturnValue Exit code integer based on if the game ran and terminated successfully
*/
int main(int argc, char *argv[]){
    std::shared_ptr< CApplicationData > AppInstance;
    int ReturnValue;    
    
    OpenDebug("Debug.out", DEBUG_LEVEL);
    
    AppInstance = CApplicationData::Instance("edu.ucdavis.cs.ecs160.game");
    
    ReturnValue = AppInstance->Run(argc, argv);
    PrintDebug(DEBUG_HIGH,"Run Returned\n");
    return ReturnValue;
}

