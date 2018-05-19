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
#ifndef AIPLAYER_H
#define AIPLAYER_H

#include "GameModel.h"
#include "PlayerCommand.h"

#include <string>
#include <queue>

/**
* Request from AI Team to include lua scripts and enumeration of AI Difficulties
* Oct 22, 2017
* Edited by: Christopher Ta
*/
//NN: Lua includes
extern "C" {
    #include "lua.h"
    #include "lauxlib.h"
    #include "lualib.h"
}

class CAIPlayer{
    protected:
        std::shared_ptr< CPlayerData > DPlayerData;
        int DCycle;
        int DDownSample;
        const char* DLuaFile;
        std::queue<SPlayerCommandRequest> DQueuedCommands;
        std::map<int,bool> DAssignedAssets;

        static EAssetType ResolveAssetTypeFromName(CAIPlayer* aiptr, const char* assetName);
        static EAssetCapabilityType ResolveAssetCapabilityFromName( const char* assetName);
    public:        
        CAIPlayer(std::shared_ptr< CPlayerData > playerdata, int downsample, std::string luaFile);
        
        //Lua Getters
        static int FindAssetPlacementWithConstraints(lua_State *L);

        static int GetFoundAssetCount(lua_State *L);
        static int GetPlayerAssetCount(lua_State *L);
        static int GetIdleAssets(lua_State *L);
        static int GetSingleAssetWithCapability(lua_State *L);
        static int GetSingleAssetWithOneOfTwoCapability(lua_State *L);
        static int GetNearestTileOfType(lua_State *L);
        static int GetNearestTileOfTypeFromAsset(lua_State *L);
        static int GetNearestAssetToOfType(lua_State *L);
        static int GetNearestEnemyLocation(lua_State *L);

        static int GetBestAssetPlacement(lua_State *L);
        static int GetAssetTilePosition(lua_State *L);
        static int CountAssetsWithAction(lua_State *L);
        static int GetInterruptableAssetWithCapability(lua_State *L);
        static int GetAssetLumber(lua_State *L);
        static int GetAssetGold(lua_State *L);
        static int GetAssetType(lua_State *L);
        static int GetPlayerGold(lua_State *L);
        static int GetPlayerLumber(lua_State *L);
        static int GetCanApplyCapability(lua_State *L);
        static int BuildBuilding(lua_State *L);
        static int GetAllIdleAssetsWithCapability(lua_State *L);
        static int GetAllAssetsWithCapability(lua_State *L);
        static int GetAllAssetOfOneType(lua_State *L);
        static int SearchMapForEnemies(lua_State *L);
        static int GetPlayerVisibilityPercentage(lua_State *L);
        static int GetPlayerFoodConsumption(lua_State *L);
        static int GetPlayerFoodProduction(lua_State *L);
        static int GetLocationsOfAllAssetsOfType(lua_State *L);
        static int GetNearestEnemy(lua_State *L);
        static int GetAssetColor(lua_State *L);
        static int CanInitiate(lua_State *L);
        
        //Lua Command Setters
        static int SetCommandAction(lua_State *L);
        static int AddCommandActor(lua_State *L);
        static int SetCommandTargetColor(lua_State *L);
        static int SetCommandTargetType(lua_State *L);
        static int SetCommandTargetPos(lua_State *L);
        static int PushBackCommand(lua_State *L);

        //Lua Registration
        void RegisterFunctions(lua_State *L);
        
        void CalculateCommand(SPlayerCommandRequest &command);
        void PushCommand(SPlayerCommandRequest &command);
        void ClearAssignments();
};

#endif
