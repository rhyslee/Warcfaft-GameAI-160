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
    purposes only and this copyright notice does not EAssetType::tempt to claim any 
    ownership of this material.
*/






/**
 * @class CAIPlayer
 *
 * @brief CAIPlayer creates an instance of a player with the purpose of being controlled by the computer.
 *
 *   The CAIPlayer class effectively sets up a player object that
 *   takes automated commands from the computer, of type SPlayerCommandRequest.
 *   By passing SPlayerCommandRequest objects to an AIPlayer instance, a script
 *   can control the assets listed as controlled by that AIPlayer instance.
 *
 * @author Spencer Van Dyke
 *
 * @version 1.0
 *
 * @date 10/10/2017
 *
 * Contact: savandyke@ucdavis.edu
 *
 */
#include "AIPlayer.h"
#include "Debug.h"
#include <cmath>

//NN: Lua includes
extern "C" {
    #include "lua.h"
    #include "lauxlib.h"
    #include "lualib.h"
}
#include "StringAndTypeConversion.h"

//NN: For more immediate debugging *SHOULDN'T BE IN FINISHED PRODUCT
/**
 * Constructor to initialize the values for the AIPlayer instance.
 *
 * @param[in] playerdata An instance of PlayerData, the same dataset used by a human player, though specifically here the boolean value DIsAI is true
 * @param[in] downsample Used for controlling how frequently the AI player calculates a new command to issue
 * -param[in] luaFile Character pointer containing the location of the lua file difficulty for each respective map
 *
 * @return None (Constructor)
 */
//NN: For more immediate debugging
#include "stdio.h"
#include <string.h>

/**
 * Function to quickly resolve asset type from string.
 * @param aiptr - pointer to aiptr
 * @param assetName - name of asset, i.e. "TownHall"
 * 
 * @return EassetType - i.e. EAssetType::TownHall
 */
EAssetType CAIPlayer::ResolveAssetTypeFromName( CAIPlayer* aiptr, const char* assetName)
{
    return aiptr->DPlayerData->AssetTypes()->find(assetName)->second->Type();
}
/**
 * Function to quickly resolve asset capability from string.
 * @param aiptr - redundant variable for consistancy's sake
 * @param assetName - name of asset, i.e. "Mine"
 * 
 * @return EassetType - i.e. EAssetCapbilityType::TownHall
 */
EAssetCapabilityType CAIPlayer::ResolveAssetCapabilityFromName( const char* assetName)
{
    return CPlayerCapability::FindCapability(assetName)->AssetCapabilityType();
}
CAIPlayer::CAIPlayer(std::shared_ptr< CPlayerData > playerdata, int downsample, std::string luaFile){
//CAIPlayer::CAIPlayer(std::shared_ptr< CPlayerData > playerdata, int downsample){
    DPlayerData = playerdata;
    DCycle = 0;
    DDownSample = downsample;
    DDownSample = 100;
    DLuaFile = luaFile.c_str();
}
//Lua Getters

/**
 * Gets the number of found assets of a given type
 * 
 * ---Parameters and returns are documented as Lua Side
 *
 * @param[in] AIPointer Pointer to the CAIPlayer to reference
 * @param[in] AssetType The EAssetType to find
 *
 * @return The number of found assets of the passed in type
 */
int CAIPlayer::GetFoundAssetCount(lua_State *L){
    CAIPlayer* aiptr = (CAIPlayer*)lua_topointer(L, -2);
    EAssetType assetType = ResolveAssetTypeFromName( aiptr, lua_tostring(L, -1));
    lua_pushnumber(L, aiptr->DPlayerData->FoundAssetCount(assetType));
    return 1;
}

/**
 * Gets the number of assets of a given type the player has control of
 * 
 * ---Parameters and returns are documented as Lua Side
 *
 * @param[in] AIPointer Pointer to the CAIPlayer to reference
 * @param[in] AssetType The EAssetType to find
 *
 * @return The number of assets of the passed in type the player has control of
 */
int CAIPlayer::GetPlayerAssetCount(lua_State *L){
    CAIPlayer* aiptr = (CAIPlayer*)lua_topointer(L, -2);
    EAssetType assetType = ResolveAssetTypeFromName( aiptr, lua_tostring(L,-1));   
    lua_pushnumber(L, aiptr->DPlayerData->PlayerAssetCount(assetType));
    return 1;
}

/**
 * Gets a list of all idle asset IDs
 * 
 * ---Parameters and returns are documented as Lua Side
 *
 * @param[in] AIPointer Pointer to the CAIPlayer to reference
 *
 * @return Table of idle asset IDs
 */
int CAIPlayer::GetIdleAssets(lua_State *L){
    CAIPlayer* aiptr = (CAIPlayer*)lua_topointer(L, -1);
    auto IdleAssets = aiptr->DPlayerData->IdleAssets();
    
    lua_newtable(L);
    int index = 1;
    for(auto WeakAsset : IdleAssets){
        if(auto Asset = WeakAsset.lock()){
            if (aiptr->DAssignedAssets[Asset->AssetID()])
                continue;

            lua_pushnumber(L, Asset->AssetID());
            lua_rawseti(L, -2, index++);
        }
    }

    return 1;
}

/**
 * Gets an idle asset's ID that has a given capability
 * 
 * ---Parameters and returns are documented as Lua Side
 *
 * @param[in] AIPointer Pointer to the CAIPlayer to reference
 * @param[in] Capability The wanted capability
 *
 * @return The asset ID of the found asset with capability if found. -1 if not found.
 */
int CAIPlayer::GetSingleAssetWithCapability(lua_State *L){
    CAIPlayer* aiptr = (CAIPlayer*)lua_topointer(L, -2);
    auto capability = ResolveAssetCapabilityFromName( lua_tostring(L, -1));
    auto IdleAssets = aiptr->DPlayerData->IdleAssets();
    int found = 0;

    for(auto WeakAsset : IdleAssets){
        if(auto Asset = WeakAsset.lock()){
            if (aiptr->DAssignedAssets[Asset->AssetID()])
                continue;

            if(Asset->HasCapability(capability)){
                lua_pushnumber(L, Asset->AssetID());
                found = 1;
                break;
            }
        }
    }
    if (!found)
        lua_pushnumber(L, -1);
    return 1;
}

/**
 * Gets an idle asset's ID that has one of two given capability
 * 
 * ---Parameters and returns are documented as Lua Side
 *
 * @param[in] AIPointer Pointer to the CAIPlayer to reference
 * @param[in] Capability The wanted capability
 *
 * @return The asset ID of the found asset with capability if found. -1 if not found.
 */
int CAIPlayer::GetSingleAssetWithOneOfTwoCapability(lua_State *L){
    CAIPlayer* aiptr = (CAIPlayer*)lua_topointer(L, -3);
    auto capabilityB = ResolveAssetCapabilityFromName( lua_tostring(L, -2));
    auto capabilityA = ResolveAssetCapabilityFromName( lua_tostring(L, -1));
    auto IdleAssets = aiptr->DPlayerData->IdleAssets();
    int found = 0;

    for(auto WeakAsset : IdleAssets){
        if(auto Asset = WeakAsset.lock()){
            if (aiptr->DAssignedAssets[Asset->AssetID()])
                continue;
            
            if(Asset->HasCapability(capabilityA)){
                lua_pushnumber(L, Asset->AssetID());
                lua_pushstring(L, CPlayerCapability::TypeToName(capabilityA).c_str());
                found = 1;
                break;
            }
            if(Asset->HasCapability(capabilityB)){
                lua_pushnumber(L, Asset->AssetID());
                lua_pushstring(L, CPlayerCapability::TypeToName(capabilityB).c_str());
                found = 1;
                break;
            }
        }
    }
    if (!found){
        lua_pushnumber(L, -1);
        lua_pushnumber(L, -1);
    }   
    return 2;
}

/**
 * Gets the nearest tile to an asset of a given type
 * 
 * ---Parameters and returns are documented as Lua Side
 *
 * @param[in] AIPointer Pointer to the CAIPlayer to reference
 * @param[in] Asset The asset to serach around
 * @param[in] TileType The type of tile to search for
 *
 * @return (x, y) The coordinates of the found tile
 */
int CAIPlayer::GetNearestTileOfType(lua_State *L){
    CAIPlayer* aiptr = (CAIPlayer*)lua_topointer(L, -3);
    int assetID = lua_tointeger(L, -2);
    std::shared_ptr<CPlayerAsset> asset = FindAssetObj(assetID);
    CTerrainMap::ETileType tileType = TileNameToType(lua_tostring(L, -1)); //TODO: lua_tostring(L,-1) to CTerrainMap::ETileType
    CTilePosition unknownPosition = aiptr->DPlayerData->PlayerMap()->FindNearestReachableTileType(asset->TilePosition(), tileType);
    lua_pushnumber(L, unknownPosition.X());
    lua_pushnumber(L, unknownPosition.Y());
    return 2;
}


/**
 * Gets the nearest tile to an asset of a given type
 *
 * ---Parameters and returns are documented as Lua Side
 *
 * @param[in] AIPointer Pointer to the CAIPlayer to reference
 * @param[in] Asset The asset to serach around
 * @param[in] CenterAsset The ID of the Asset as the center of the search
 * @param[in] TileType The type of tile to search for
 *
 * @return (x, y) The coordinates of the found tile
 */
//WIP, implement algorithm to find the closest tile to both center and the movableasset
int CAIPlayer::GetNearestTileOfTypeFromAsset(lua_State *L){
    CAIPlayer* aiptr = (CAIPlayer*)lua_topointer(L, -4);
    int assetID = lua_tointeger(L, -3);
    int centerAssetID = lua_tointeger(L, -2);
    std::shared_ptr<CPlayerAsset> asset = FindAssetObj(assetID);
    std::shared_ptr<CPlayerAsset> centerAsset = FindAssetObj(centerAssetID);
    CTerrainMap::ETileType tileType = TileNameToType(lua_tostring(L, -1)); //TODO: lua_tostring(L,-1) to CTerrainMap::ETileType
    //CTilePosition unknownPosition = aiptr->DPlayerData->PlayerMap()->FindNearestReachableTileType(asset->TilePosition(), tileType);
    CTilePosition unknownPosition = aiptr->DPlayerData->PlayerMap()->FindNearestReachableTileType(centerAsset->TilePosition(), tileType);
    lua_pushnumber(L, unknownPosition.X());
    lua_pushnumber(L, unknownPosition.Y());
    return 2;
}



/**
 * Gets the nearest asset to an asset of a given type
 * 
 * ---Parameters and returns are documented as Lua Side
 *
 * @param[in] AIPointer Pointer to the CAIPlayer to reference
 * @param[in] Asset The asset to serach around
 * @param[in] TargetType The type of asset to search for
 *
 * @return The AssetID of the nearest asset of the given type
 */
int CAIPlayer::GetNearestAssetToOfType(lua_State *L){
    CAIPlayer* aiptr = (CAIPlayer*)lua_topointer(L, -3);
    int assetID = lua_tointeger(L, -2);
    std::shared_ptr<CPlayerAsset> builderAsset = FindAssetObj(assetID);
    EAssetType searchType = ResolveAssetTypeFromName( aiptr, lua_tostring(L,-1));
    auto SearchedAsset = aiptr->DPlayerData->FindNearestAsset(builderAsset->Position(), searchType);
    lua_pushnumber(L, SearchedAsset->AssetID());
    return 1;
}

/**
 * Gets the "best placement" for a structure to be built
 * 
 * ---Parameters and returns are documented as Lua Side
 *
 * @param[in] AIPointer Pointer to the CAIPlayer to reference
 * @param[in] PositionX The x of the coordinate to search near
 * @param[in] PositionY The y of the coordinate to search near
 * @param[in] BuilderID The AssetID of the builder
 * @param[in] AssetType The type of building that is wanted to be built
 * @param[in] Buffer ???
 *
 * @return (x, y) The coordinates of the "Best Placement"
 */
int CAIPlayer::GetBestAssetPlacement(lua_State *L){
    CAIPlayer* aiptr = (CAIPlayer*)lua_topointer(L, -6);
    int posX = lua_tointeger(L, -5);
    int posY = lua_tointeger(L, -4);
    CTilePosition* pos = new CTilePosition(posX, posY);
    int builderAssetID = lua_tointeger(L, -3);
    std::shared_ptr<CPlayerAsset> builderAsset = FindAssetObj(builderAssetID);
    EAssetType assetToBuild = ResolveAssetTypeFromName( aiptr, lua_tostring(L,-2));
    int buffer = lua_tointeger(L, -1);
    CTilePosition placement = aiptr->DPlayerData->FindBestAssetPlacement(*pos, builderAsset, assetToBuild, buffer);
    lua_pushnumber(L, placement.X());
    lua_pushnumber(L, placement.Y());
    return 2;
}



/**
 * Find the best placement of a given building with Constraint Parameters
 *
 * ---Parameters and returns are documented as Lua Side
 *
 * @param[in] AIPointer Pointer to the CAIPlayer to reference//-10
 * @param builderAsset Id of the builder to build the building//-9
 * @param HConstrants Horizontal search Constrants, 1-left,2-mid,4right,3-l&m,6-m&r,5-l&r,7-l&m&r//-8
 * @param VConstrants vertical search Constrants,  1-top,2-mid,4bottom,3-t&m,6-m&b,5-t&b,7-t&m&b//-7
 * @param x target's x//-6
 * @param y target's y//-5
 * @param offset Minimal offset between this building and the target//-4
 * @param padding extra padding added to the size of the building to be built//-3
 * @param assetType Type of the asset to be build //-2
 * @param farthest the fartherst distance to search within  //-1
 *
 * @return buildable,x,y buildable is true if found pos x y; otherwise return false, and arbitrary invalid x, y
*/

int CAIPlayer::FindAssetPlacementWithConstraints(lua_State *L){
    CAIPlayer* aiptr = (CAIPlayer*)lua_topointer(L, -10);
    int HConstrants = lua_tointeger(L, -8);
    int VConstrants = lua_tointeger(L, -7);

    std::shared_ptr<CPlayerAsset> builderAsset = FindAssetObj(lua_tointeger(L, -9));
    int targetX = lua_tointeger(L, -6);
    int targetY = lua_tointeger(L, -5);

    int offset = lua_tointeger(L, -4);
    int padding = lua_tointeger(L, -3);
    int assetTypeID = lua_tointeger(L, -2);
    int farthest = lua_tointeger(L, -1);


    CTilePosition* pos = new CTilePosition(targetX, targetY);
    EAssetType assetType = static_cast<EAssetType>(assetTypeID);
    printf("\nPassing targetX=%d,targetY=%d",pos->X(),pos->Y());
    CTilePosition placement = aiptr->DPlayerData->FindBestAssetPlacementWithConstraints(*pos, builderAsset, assetType, offset,padding,HConstrants,VConstrants,farthest);
    printf("get result pos x=%d,y=%d.",placement.X(),placement.Y());
    lua_pushnumber(L, placement.X());
    lua_pushnumber(L, placement.Y());

    return 2;
}


/**
 * Gets and assets tile position
 * 
 * ---Parameters and returns are documented as Lua Side
 *
 * @param[in] AssetID The ID of the asset to lookup
 *
 * @return (x, y) The coordinates of the assets tile position
 */
int CAIPlayer::GetAssetTilePosition(lua_State *L){
    int assetID = lua_tointeger(L, -1);
    std::shared_ptr<CPlayerAsset> asset = FindAssetObj(assetID);
    CTilePosition position = asset->TilePosition();
    lua_pushnumber(L, position.X());
    lua_pushnumber(L, position.Y());
    return 2;
}

/**
 * Gets the number of player assets that have a given action queued
 * 
 * ---Parameters and returns are documented as Lua Side
 *
 * @param[in] AIPointer Pointer to the CAIPlayer to reference
 * @param[in] Action The action that should be search for
 *
 * @return Count of player's assets with the given action queued
 */
int CAIPlayer::CountAssetsWithAction(lua_State *L){
    CAIPlayer* aiptr = (CAIPlayer*)lua_topointer(L, -2);
    EAssetAction action = ActionNameToType(lua_tostring(L, -1));
    int count = 0;
    for(auto WeakAsset : aiptr->DPlayerData->Assets()){
        if(auto Asset = WeakAsset.lock()){
            if(Asset->HasAction(action)){
                count++;
            }
        }
    }
    lua_pushnumber(L, count);
    return 1;
}

/**
 * Gets an asset ID of a non-idle, interruptable asset that has a given capability
 * 
 * ---Parameters and returns are documented as Lua Side
 *
 * @param[in] AIPointer Pointer to the CAIPlayer to reference
 * @param[in] Action The action we are allowing to be interrupted
 * @param[in] Capability The wanted capability of an asset
 *
 * @return The asset ID if found. -1 if no asset found.
 */
int CAIPlayer::GetInterruptableAssetWithCapability(lua_State *L){
    CAIPlayer* aiptr = (CAIPlayer*)lua_topointer(L, -3);
    EAssetAction action = ActionNameToType(lua_tostring(L, -2));
    EAssetCapabilityType capability = ResolveAssetCapabilityFromName( lua_tostring(L, -1));
    for(auto WeakAsset : aiptr->DPlayerData->Assets()){
        if(auto Asset = WeakAsset.lock()){
            if(Asset->HasCapability(capability)){
                if (Asset->Interruptible() && Asset->Action() == action){
                    lua_pushnumber(L, Asset->AssetID());
                    return 1;
                }
            }
        }
    }
    lua_pushnumber(L, -1);
    return 1;
}

/**
 * Gets the amount of lumber currently held by an asset
 * 
 * ---Parameters and returns are documented as Lua Side
 *
 * @param[in] AIPointer Pointer to the CAIPlayer to reference
 * @param[in] AssetID The ID of the asset to check
 *
 * @return The amount of lumber on said asset.
 */
int CAIPlayer::GetAssetLumber(lua_State *L){
    CAIPlayer* aiptr = (CAIPlayer*)lua_topointer(L, -2);
    int assetID = lua_tointeger(L, -1);
    std::shared_ptr<CPlayerAsset> asset = FindAssetObj(assetID);
    lua_pushnumber(L, asset->Lumber());
    return 1;
}

/**
 * Gets the amount of gold currently held by an asset
 * 
 * ---Parameters and returns are documented as Lua Side
 *
 * @param[in] AIPointer Pointer to the CAIPlayer to reference
 * @param[in] AssetID The ID of the asset to check
 *
 * @return The amount of gold on said asset.
 */
int CAIPlayer::GetAssetGold(lua_State *L){
    CAIPlayer* aiptr = (CAIPlayer*)lua_topointer(L, -2);
    int assetID = lua_tointeger(L, -1);
    std::shared_ptr<CPlayerAsset> asset = FindAssetObj(assetID);
    lua_pushnumber(L, asset->Gold());
    return 1;
}

/**
 * Gets the EAssetType of an asset
 * 
 * ---Parameters and returns are documented as Lua Side
 *
 * @param[in] AIPointer Pointer to the CAIPlayer to reference
 * @param[in] AssetID The ID of the asset to check
 *
 * @return The type of the passed asset.
 */
int CAIPlayer::GetAssetType(lua_State *L){
    CAIPlayer* aiptr = (CAIPlayer*)lua_topointer(L, -2);
    int assetID = lua_tointeger(L, -1);
    std::shared_ptr<CPlayerAsset> asset = FindAssetObj(assetID);
    lua_pushstring(L, CPlayerAssetType::TypeToName(asset->Type()).c_str());
    return 1;
}

/**
 * Gets the amount of stored gold by the CAIPlayer
 * 
 * ---Parameters and returns are documented as Lua Side
 *
 * @param[in] AIPointer Pointer to the CAIPlayer to reference
 *
 * @return The amount of stored gold.
 */
int CAIPlayer::GetPlayerGold(lua_State *L){
    CAIPlayer* aiptr = (CAIPlayer*)lua_topointer(L, -1);
    lua_pushnumber(L, aiptr->DPlayerData->Gold());
    return 1;
}

/**
 * Gets the amount of stored lumber by the CAIPlayer
 * 
 * ---Parameters and returns are documented as Lua Side
 *
 * @param[in] AIPointer Pointer to the CAIPlayer to reference
 *
 * @return The amount of stored lumber.
 */
int CAIPlayer::GetPlayerLumber(lua_State *L){
    CAIPlayer* aiptr = (CAIPlayer*)lua_topointer(L, -1);
    lua_pushnumber(L, aiptr->DPlayerData->Lumber());
    return 1;
}

/**
 * Gets whether or not a capability of an asset can be activated
 * 
 * ---Parameters and returns are documented as Lua Side
 *
 * @param[in] AIPointer Pointer to the CAIPlayer to reference
 * @param[in] AssetID The ID of the asset we are checking
 * @param[in] TargetID The ID of the asset the capability will be acting on
 * @param[in] Capability The capability to check
 *
 * @return True if the capability can be activated. False otherwise.
 */
int CAIPlayer::GetCanApplyCapability(lua_State *L){
    CAIPlayer* aiptr = (CAIPlayer*)lua_topointer(L, -4);
    int assetID = lua_tointeger(L, -3);
    std::shared_ptr<CPlayerAsset> asset = FindAssetObj(assetID);
    int targetID = lua_tointeger(L, -2);
    std::shared_ptr<CPlayerAsset> target = FindAssetObj(targetID);
    auto PlayerCapability = CPlayerCapability::FindCapability(lua_tostring(L,-1));
    if (PlayerCapability){
        lua_pushboolean(L, PlayerCapability->CanApply(asset, aiptr->DPlayerData, target));
        return 1;
    }
    lua_pushboolean(L, 0);
    return 1;
}



/**
 * Commands an unused builder unit to construct a building of type other than town hall. The
 * default structure is a farmhouse, though the type of structure passed in can be changed
 *
 * @param[in] aiptr A ptr to a struct containing AI player data
 * @param[in] buildingtype Enum value that dictates what kind of building is constructed
 * @param[in] neartype Enum value that dictates what type of asset this building must be constructed next to
 *
 * @return True if any of the AI's units can successfully complete this command
 */
int CAIPlayer::BuildBuilding(lua_State* L){
    std::shared_ptr<CPlayerAsset> BuilderAsset ;
    std::shared_ptr<CPlayerAsset> TownHallAsset;
    std::shared_ptr<CPlayerAsset> NearAsset;
    CAIPlayer* aiptr = (CAIPlayer*)lua_topointer(L, -4);    
    EAssetCapabilityType BuildAction = ResolveAssetCapabilityFromName( lua_tostring(L,-3));

    EAssetType buildingType = ResolveAssetTypeFromName( aiptr, lua_tostring(L,-2));

    EAssetType nearType = ResolveAssetTypeFromName( aiptr, lua_tostring(L,-1));

    
    bool AssetIsIdle = false;
    for(auto WeakAsset : aiptr->DPlayerData->Assets()){
        if(auto Asset = WeakAsset.lock()){
            if(Asset->HasCapability(BuildAction) && Asset->Interruptible() && !aiptr->DAssignedAssets[Asset->AssetID()]){
                if(!BuilderAsset || (!AssetIsIdle && (EAssetAction::None == Asset->Action()))){
                    BuilderAsset = Asset;
                    AssetIsIdle = EAssetAction::None == Asset->Action();
                }
            }
            if(Asset->HasCapability(EAssetCapabilityType::BuildPeasant)){
                TownHallAsset = Asset;
            }
            if(Asset->HasActiveCapability(BuildAction)){ 
                lua_pushnumber(L, -1);        
                lua_pushnumber(L, -1);
                lua_pushnumber(L, -1);
                return 3;    
            }
            if( ( nearType == Asset->Type() )&&(EAssetAction::Construct != Asset->Action())){
                NearAsset = Asset;
            }
            if(buildingType == Asset->Type()){
                if(EAssetAction::Construct == Asset->Action()){
                    lua_pushnumber(L, -1);      
                    lua_pushnumber(L, -1);
                    lua_pushnumber(L, -1);
                    return 3;   
                }
            }
        }
    }    
    if ((buildingType != nearType) && !NearAsset) 
    {
        lua_pushnumber(L, -1);
        lua_pushnumber(L, -1);
        lua_pushnumber(L, -1);
        return 3;
    }
    
    if(BuilderAsset)
    {
        CTilePosition SourcePosition = TownHallAsset->TilePosition();
        CTilePosition MapCenter(aiptr->DPlayerData->PlayerMap()->Width()/2, aiptr->DPlayerData->PlayerMap()->Height()/2);
        if(NearAsset){
            SourcePosition = NearAsset->TilePosition();
        }
        if(MapCenter.X() < SourcePosition.X()){
            SourcePosition.DecrementX(TownHallAsset->Size()/2);   
        }
        else if(MapCenter.X() > SourcePosition.X()){
            SourcePosition.IncrementX(TownHallAsset->Size()/2);
        }
        if(MapCenter.Y() < SourcePosition.Y()){
            SourcePosition.DecrementY(TownHallAsset->Size()/2);   
        }
        else if(MapCenter.Y() > SourcePosition.Y()){
            SourcePosition.IncrementY(TownHallAsset->Size()/2);
        }
        CTilePosition Placement = aiptr->DPlayerData->FindBestAssetPlacement(SourcePosition, BuilderAsset, buildingType, 1);
        lua_pushnumber(L, BuilderAsset->AssetID());
        lua_pushnumber(L, Placement.X());
        lua_pushnumber(L, Placement.Y());
    }
    else
    {
        lua_pushnumber(L, -1);
        lua_pushnumber(L, -1);
        lua_pushnumber(L, -1);
    }

    return 3;
}


/**
 * Pushes all idle units that can move and are not peasants (implied to be fighters) with the specified action and capabililty,
 * onto the command DActor vector.
 *
 * @param[in] cmdptr A pointer to a struct containing a list of units that should be issued this command
 * @param[in] aiptr A pointer to a struct containing aiplayer data
 * @param[in] action The integer representation of the desired EActionType
 * @param[in] capability the integer representation of the desired EAssetCapabilityType
 * 
 * @return True if any of the AI's units can successfully complete this command
 */
 int CAIPlayer::GetAllIdleAssetsWithCapability(lua_State *L){

    CAIPlayer* aiptr = (CAIPlayer*)lua_topointer(L, -4);
    SPlayerCommandRequest* cmdptr =  (SPlayerCommandRequest*)lua_topointer(L, -3);    
    EAssetAction action = ActionNameToType(lua_tostring(L, -2));  
    EAssetCapabilityType capability = ResolveAssetCapabilityFromName( lua_tostring(L,-1));
    
    auto IdleAssets = aiptr->DPlayerData->IdleAssets();
    for(auto WeakAsset : IdleAssets){
        if(auto Asset = WeakAsset.lock()){
            if(Asset->Speed()){
                if(!Asset->HasAction(action) && !Asset->HasActiveCapability(capability) && Asset->HasCapability(capability)){
                    cmdptr->DActors.push_back(Asset); //pushes all fighters who are able to stand ground
                }
            }
        }
    }
    lua_pushnumber(L,cmdptr->DActors.size());
    return 1;
}

/**
 * Pushes all units that can move with the specified action and capabililty,
 * onto the command DActor vector.
 *
 * @param[in] cmdptr A pointer to a struct containing a list of units that should be issued this command
 * @param[in] aiptr A pointer to a struct containing aiplayer data
 * @param[in] action The integer representation of the desired EActionType
 * @param[in] capability the integer representation of the desired EAssetCapabilityType
 * 
 * @return True if any of the AI's units can successfully complete this command
 */
 int CAIPlayer::GetAllAssetsWithCapability(lua_State *L){

    CAIPlayer* aiptr = (CAIPlayer*)lua_topointer(L, -4);
    SPlayerCommandRequest* cmdptr =  (SPlayerCommandRequest*)lua_topointer(L, -3);    
    EAssetAction action = ActionNameToType(lua_tostring(L, -2));  
    EAssetCapabilityType capability =  ResolveAssetCapabilityFromName( lua_tostring(L,-1));
    
    auto Assets = aiptr->DPlayerData->Assets();
    for(auto WeakAsset : Assets){
        if(auto Asset = WeakAsset.lock()){
            if(Asset->Speed()){
                if(!Asset->HasAction(action) && !Asset->HasActiveCapability(capability) && Asset->HasCapability(capability) ){
                    cmdptr->DActors.push_back(Asset); //pushes all fighters who are able to stand ground
                }
            }
        }
    }
    lua_pushnumber(L,cmdptr->DActors.size());
    return 1;
}



/**
 * Pushes all idle units of the type assetType onto the command DAactor vector
 *
 * @param[in] cmdptr A pointer to a struct containing a list of units that should be issued this command
 * @param[in] aiptr A pointer to a struct containing aiplayer data
 * @param[in] assetType Type of unit you wish to access
 *
 * @return True if any of the AI's units can successfully complete this command
 */
int CAIPlayer::GetAllAssetOfOneType(lua_State *L){
    CAIPlayer* aiptr = (CAIPlayer*)lua_topointer(L, -3);
    SPlayerCommandRequest* cmdptr =  (SPlayerCommandRequest*)lua_topointer(L, -2);

    EAssetType assetType = ResolveAssetTypeFromName( aiptr, lua_tostring(L,-1));
    auto IdleAssets = aiptr->DPlayerData->IdleAssets();
    for(auto WeakAsset : IdleAssets){
        if(auto Asset = WeakAsset.lock()){
            if(assetType == Asset->Type()){
                cmdptr->DActors.push_back(Asset);
            }
        }
    }
    lua_pushnumber(L,cmdptr->DActors.size());
    return 1;
}

/**
 * Commands unt to look through the map for the nearest enemy from your town hall.
 * 
 * @param aiptr pointer to player information
 * @param TownHallAsset ID to the townhall asset object
 * 
 * @return true if a previously called find nearest enemy call has expired, otherwise false.
 **/
int CAIPlayer::SearchMapForEnemies(lua_State* L){
    CAIPlayer* aiptr = (CAIPlayer*)lua_topointer(L, -2);
    auto TownHallAsset = FindAssetObj(lua_tointeger(L,-1));
    if(aiptr->DPlayerData->FindNearestEnemy(TownHallAsset->Position(), -1).expired()){
        lua_pushboolean(L, true);
        return 1;
    }
    lua_pushboolean(L, false);
    return 1;    
}



//Lua Set Commands

/**
 * Sets the action member of the command variable
 * 
 * ---Parameters and returns are documented as Lua Side
 *
 * @param[in] AIPointer Pointer to the CAIPlayer to reference
 * @param[in] CmdPointer Pointer to the SPlayerCommandRequest to set
 * @param[in] Action The value to set the command's action to
 */
int CAIPlayer::SetCommandAction(lua_State *L){
    CAIPlayer* aiptr = (CAIPlayer*)lua_topointer(L, -3);
    SPlayerCommandRequest* cmdptr = (SPlayerCommandRequest*)lua_topointer(L, -2);
    EAssetCapabilityType action = ResolveAssetCapabilityFromName( lua_tostring(L, -1));
    cmdptr->DAction = action;
    return 0;
}

/**
 * Adds an actor to the command variable
 * 
 * ---Parameters and returns are documented as Lua Side
 *
 * @param[in] AIPointer Pointer to the CAIPlayer to reference
 * @param[in] CmdPointer Pointer to the SPlayerCommandRequest to set
 * @param[in] ActorID The ID of the asset to add to the actor list
 */
int CAIPlayer::AddCommandActor(lua_State *L){
    CAIPlayer* aiptr = (CAIPlayer*)lua_topointer(L, -3);
    SPlayerCommandRequest* cmdptr = (SPlayerCommandRequest*)lua_topointer(L, -2);
    int actorID = lua_tointeger(L, -1);
    auto asset = FindAssetObj(actorID);
    if(!asset) return 0;
    cmdptr->DActors.push_back(asset);
    aiptr->DAssignedAssets[asset->AssetID()] = true;
    return 0;
}

/**
 * Sets the target color member of the command variable
 * 
 * ---Parameters and returns are documented as Lua Side
 *
 * @param[in] AIPointer Pointer to the CAIPlayer to reference
 * @param[in] CmdPointer Pointer to the SPlayerCommandRequest to set
 * @param[in] TargetColor The value to set the command's target color to
 */
int CAIPlayer::SetCommandTargetColor(lua_State *L){
    CAIPlayer* aiptr = (CAIPlayer*)lua_topointer(L, -3);
    SPlayerCommandRequest* cmdptr = (SPlayerCommandRequest*)lua_topointer(L, -2);
    EPlayerColor targetColor = ColorNameToType(lua_tostring(L, -1));
    cmdptr->DTargetColor = targetColor;
    return 0;
}

/**
 * Sets the target type member of the command variable
 * 
 * ---Parameters and returns are documented as Lua Side
 *
 * @param[in] AIPointer Pointer to the CAIPlayer to reference
 * @param[in] CmdPointer Pointer to the SPlayerCommandRequest to set
 * @param[in] Action The value to set the command's target type to
 */
int CAIPlayer::SetCommandTargetType(lua_State *L){
    CAIPlayer* aiptr = (CAIPlayer*)lua_topointer(L, -3);
    SPlayerCommandRequest* cmdptr = (SPlayerCommandRequest*)lua_topointer(L, -2);
    EAssetType targetType = ResolveAssetTypeFromName( aiptr, lua_tostring(L,-1));
    cmdptr->DTargetType = targetType;
    return 0;
}

/**
 * Sets the target position member of the command variable
 * 
 * ---Parameters and returns are documented as Lua Side
 *
 * @param[in] AIPointer Pointer to the CAIPlayer to reference
 * @param[in] CmdPointer Pointer to the SPlayerCommandRequest to set
 * @param[in] TargetX The x coordinate of the tile to target
 * @param[in] TargetY The y coordinate of the tile to target
 */
int CAIPlayer::SetCommandTargetPos(lua_State *L){
    CAIPlayer* aiptr = (CAIPlayer*)lua_topointer(L, -4);
    SPlayerCommandRequest* cmdptr = (SPlayerCommandRequest*)lua_topointer(L, -3);
    int x = lua_tonumber(L, -2);
    int y = lua_tonumber(L, -1);
    CTilePosition tilePosition = *(new CTilePosition(x, y));
    cmdptr->DTargetLocation.SetFromTile(tilePosition);;
    return 0;
}

/**
 * Finalizes Command and pushes it onto the vector
 *
 * ---Parameters and returns are documented as Lua Side
 *
 * @param[in] AIPointer Pointer to the CAIPlayer to reference
 */
int CAIPlayer::PushBackCommand(lua_State *L){
    //printf("PushBackCommand\n");
    CAIPlayer* aiptr = (CAIPlayer*)lua_topointer(L, -2);
    //printf("    Retrieved AIPointer\n");
    SPlayerCommandRequest* cmdptr = (SPlayerCommandRequest*)lua_topointer(L, -1);
    //printf("PushBackCommand\n");
    aiptr->PushCommand(*cmdptr);
    return 0;
}

/**
*   Gets the player visibility percentage from playerdata.
*   @param aiptr - pointer to the aiplayer
*   @param num - tbh no idea what this does. 
*   @return double which represents the percentage of map that is visible
*/
int CAIPlayer::GetPlayerVisibilityPercentage(lua_State* L){
    CAIPlayer* aiptr = (CAIPlayer*)lua_topointer(L, -2);
    int num = lua_tointeger(L,-1);
    lua_pushnumber(L, aiptr->DPlayerData->VisibilityMap()->SeenPercent(num));
    return 1;
}

/**
*   Gets the player food consumption
*   @param aiptr - pointer to the aiplayer
*   @return int which represents how much food is being used.
*/
int CAIPlayer::GetPlayerFoodConsumption(lua_State* L){
    CAIPlayer* aiptr = (CAIPlayer*)lua_topointer(L, -1);    
    lua_pushnumber(L,aiptr->DPlayerData->FoodConsumption());
    return 1;
}

/**
*   Gets the player food production
*   @param aiptr - pointer to the aiplayer
*   @return int which represents maximum food.
*/
int CAIPlayer::GetPlayerFoodProduction(lua_State* L){
    CAIPlayer* aiptr = (CAIPlayer*)lua_topointer(L, -1);
    lua_pushnumber(L,aiptr->DPlayerData->FoodProduction());
    return 1;
}

/**
*   Gets the assetID and X,Y coordinate position of assets of the desired type.
*   @param aiptr - pointer to the aiplayer
*   @param  assetType - the asset that you want to add
*   @return lua_table - containing all assetIDs, and asset possition (X,Y)
*/
int CAIPlayer::GetLocationsOfAllAssetsOfType(lua_State* L){
    CAIPlayer* aiptr = (CAIPlayer*)lua_topointer(L, -2);
//    EAssetType assetType = static_cast<EAssetType>(lua_tointeger(L,-1));
    EAssetType assetType = ResolveAssetTypeFromName( aiptr, lua_tostring(L,-1));
    lua_newtable(L);
    int index = 1;
    auto Assets = aiptr->DPlayerData->Assets();
    for(auto WeakAsset : Assets){
        if(auto Asset = WeakAsset.lock()){
            if(assetType == Asset->Type() && !Asset->HasAction(EAssetAction::Attack)){
                lua_pushnumber(L, Asset->AssetID());
                lua_rawseti(L, -2, index++);
                lua_pushnumber(L, Asset->TilePositionX());
                lua_rawseti(L, -2, index++);
                lua_pushnumber(L, Asset->TilePositionY());
                lua_rawseti(L, -2, index++);
            }
        }
    }
    return 1;
}


/**
*   Gets the nearest visible enemy to the position X,Y that does not exceed some range R.
*   @param aiptr - pointer to the aiplayer
*   @param X - x position
*   @param Y - y position
*   @param range - maximum range to look
*   @return targetID
*/



int CAIPlayer::GetNearestEnemy(lua_State* L){
    CAIPlayer* aiptr = (CAIPlayer*)lua_topointer(L, -4);
    int tileX = (int)std::round((double)lua_tonumber(L, -3));
    int tileY = (int)std::round((double)lua_tonumber(L, -2));
    CTilePosition tilePos(tileX, tileY);
    CPixelPosition pos;
    pos.SetFromTile(tilePos);
    int range = lua_tointeger(L, -1);
    auto TargetAsset = aiptr->DPlayerData->FindNearestEnemy(pos, range).lock();
    if (!TargetAsset)
        lua_pushnumber(L, -1);
    else
        lua_pushnumber(L, TargetAsset->AssetID());
        
    return 1;
}



/**
*   Gets the nearest visible enemy to the position X,Y that does not exceed some range R.
*   @param aiptr - pointer to the aiplayer
*   @param X - x position
*   @param Y - y position
*   @param range - maximum range to look
*   @return target position X Y
*/
int CAIPlayer::GetNearestEnemyLocation(lua_State* L){
    CAIPlayer* aiptr = (CAIPlayer*)lua_topointer(L, -4);
    int tileX = (int)std::round((double)lua_tonumber(L, -3));
    int tileY = (int)std::round((double)lua_tonumber(L, -2));
    CTilePosition tilePos(tileX, tileY);
    CPixelPosition pos;
    pos.SetFromTile(tilePos);
    int range = lua_tointeger(L, -1);
    auto TargetAsset = aiptr->DPlayerData->FindNearestEnemy(pos, range).lock();
    if (!TargetAsset) {
        lua_pushnumber(L, -1);
        lua_pushnumber(L, -1);
    }else {
        lua_pushnumber(L, TargetAsset->TilePositionX());
        lua_pushnumber(L, TargetAsset->TilePositionY());
    }
    return 2;
}





/**
*   Gets the color of the asset designated by assetID
*   @param aiptr - pointer to the aiplayer
*   @return string - color of the asset
*/
int CAIPlayer::GetAssetColor(lua_State* L){
    int assetID = lua_tointeger(L, -1);
    std::shared_ptr<CPlayerAsset> asset = FindAssetObj(assetID);
    lua_pushstring(L, ColorTypeToName(asset->Color()).c_str());
    return 1;
}

/**
*   Uses the CanInitiate function, which determines if a capability can be done. 
*   I.E. if a building can be built (enough resources, position is valid, etc)
*   @param aiptr - pointer to the aiplayer
*   @return boolean, true if can initiate, else false
*/
int CAIPlayer::CanInitiate(lua_State * L)
{
    CAIPlayer* aiptr = (CAIPlayer*)lua_topointer(L, -3);
    std::shared_ptr<CPlayerAsset> asset = FindAssetObj(lua_tointeger(L,-2));
    EAssetCapabilityType capability = ResolveAssetCapabilityFromName(lua_tostring(L, -1));
    std::shared_ptr< CPlayerCapability > playerCapability = CPlayerCapability::FindCapability(capability);
    lua_pushboolean(L, playerCapability->CanInitiate(asset, aiptr->DPlayerData));
    return 1;
}

//Lua Registration

/**
 * Registers all C++ side Lua Getter and Setter functions
 *
 * @param[in] L The lua_State the register the functions into
 */
void CAIPlayer::RegisterFunctions(lua_State *L){
    //Getters

    lua_register(L, "FindAssetPlacementWithConstraints", FindAssetPlacementWithConstraints);
    lua_register(L, "GetFoundAssetCount", GetFoundAssetCount);
    lua_register(L, "GetPlayerAssetCount", GetPlayerAssetCount);
    lua_register(L, "GetIdleAssets", GetIdleAssets);
    lua_register(L, "GetSingleAssetWithCapability", GetSingleAssetWithCapability);
    lua_register(L, "GetSingleAssetWithOneOfTwoCapability", GetSingleAssetWithOneOfTwoCapability);
    lua_register(L, "GetNearestTileOfTypeFromAsset", GetNearestTileOfTypeFromAsset);
    lua_register(L, "GetNearestTileOfType", GetNearestTileOfType);
    lua_register(L, "GetNearestEnemyLocation", GetNearestEnemyLocation);

    lua_register(L, "GetPlayerGold", GetPlayerGold);
    lua_register(L, "GetNearestAssetToOfType", GetNearestAssetToOfType);
    lua_register(L, "GetAssetTilePosition", GetAssetTilePosition);
    lua_register(L, "GetBestAssetPlacement", GetBestAssetPlacement);
    lua_register(L, "CountAssetsWithAction", CountAssetsWithAction);
    lua_register(L, "GetInterruptableAssetWithCapability", GetInterruptableAssetWithCapability);
    lua_register(L, "GetAssetLumber", GetAssetLumber);
    lua_register(L, "GetAssetGold", GetAssetGold);
    lua_register(L, "GetAssetType", GetAssetType);
    lua_register(L, "GetPlayerGold", GetPlayerGold);
    lua_register(L, "GetPlayerLumber", GetPlayerLumber);
    lua_register(L, "GetCanApplyCapability", GetCanApplyCapability);
    lua_register(L, "BuildBuilding", BuildBuilding);
    lua_register(L, "GetAllIdleAssetsWithCapability", GetAllIdleAssetsWithCapability);
    lua_register(L, "GetAllAssetsWithCapability", GetAllAssetsWithCapability);
    lua_register(L, "GetAllAssetOfOneType", GetAllAssetOfOneType);
    lua_register(L, "SearchMapForEnemies", SearchMapForEnemies);
    lua_register(L, "GetPlayerVisibilityPercentage", GetPlayerVisibilityPercentage);
    lua_register(L, "GetPlayerFoodConsumption", GetPlayerFoodConsumption);
    lua_register(L, "GetPlayerFoodProduction", GetPlayerFoodProduction);
    lua_register(L, "GetLocationsOfAllAssetsOfType",GetLocationsOfAllAssetsOfType);
    lua_register(L, "GetNearestEnemy", GetNearestEnemy);
    lua_register(L, "GetAssetColor", GetAssetColor);
    lua_register(L, "CanInitiate", CanInitiate);

    //Command Setters
    lua_register(L, "SetCommandAction", SetCommandAction);
    lua_register(L, "AddCommandActor", AddCommandActor);
    lua_register(L, "SetCommandTargetColor", SetCommandTargetColor);
    lua_register(L, "SetCommandTargetType", SetCommandTargetType);
    lua_register(L, "SetCommandTargetPos", SetCommandTargetPos);
    lua_register(L, "PushBackCommand", PushBackCommand);
}

/**
 * Calculates what command should be issued by the AIPlayer instance. This is based off of factors
 * such as how long the game has been running, how many commands the AI has already issued, what buildings
 * the AI currently owns, how much of the map the AI can see, how many peasants the AI has, and how many
 * footmen and archers the AI owns. Once the needs of the AI player are calculated, then then
 * appropriate command function is called from inside the calculate function
 *
 * @param[in] command A struct containing a list of units that should be issued this command
 *
 * @return None
 */
void CAIPlayer::CalculateCommand(SPlayerCommandRequest &command){
    command.DAction = EAssetCapabilityType::None;
    command.DActors.clear();    
    command.DTargetColor = EPlayerColor::None;
    command.DTargetType = EAssetType::None;   
    if((DCycle % DDownSample) == 0){
        printf("\n---CalculateCommand---\n");

        ClearAssignments();
        //Create a lua state unique to object
        lua_State *AIL = luaL_newstate();
        luaL_openlibs(AIL);
        //Register functions
        RegisterFunctions(AIL);
        //Load the brain
        //luaL_dofile(AIL, DLuaFile);
        luaL_dofile(AIL, "./scripts/brain.lua");
        //Set AI Pointer
        lua_pushlightuserdata(AIL, this);
        lua_setglobal(AIL, "AIPointer");
        //Set Command Pointer
        lua_pushlightuserdata(AIL, &command);
        lua_setglobal(AIL, "CmdPointer");
        //Set AI Color
        lua_pushstring(AIL, ColorTypeToName(DPlayerData->Color()).c_str());
        lua_setglobal(AIL, "AIColor");

        int response = 0;
        lua_getglobal(AIL, "CalculateCommand");
        if (response = lua_pcall(AIL, 0, 0, 0))
            printf("Could not execute function CalculateCommand in brain.lua: error code %d\n", response);

        lua_close(AIL);

        //Clear after calculating
        command.DAction = EAssetCapabilityType::None;
        command.DActors.clear();    
        command.DTargetColor = EPlayerColor::None;
        command.DTargetType = EAssetType::None; 

        printf("QueueSize: %d, At Timestep: %d\n", DQueuedCommands.size(), DCycle);
    }
    DCycle++;
    //printf("DCycle: %d\n", DCycle);

    if (!DQueuedCommands.empty()){
        command = DQueuedCommands.front();
        DQueuedCommands.pop();
        printf("    QueueSize: %d, At Timestep: %d\n", DQueuedCommands.size(), DCycle);
    }
}

void CAIPlayer::PushCommand(SPlayerCommandRequest &command){

    printf("PushCommand\n");

    SPlayerCommandRequest newCommand;

    newCommand.DAction = command.DAction;
    std::list< std::weak_ptr< CPlayerAsset > >::iterator it = command.DActors.begin();
    for (; it != command.DActors.end(); it++){
        std::weak_ptr< CPlayerAsset > newActor (*it);
        newCommand.DActors.push_back(newActor);
    }
    newCommand.DTargetColor = command.DTargetColor;
    newCommand.DTargetType = command.DTargetType;
    newCommand.DTargetLocation = command.DTargetLocation;
    
    DQueuedCommands.push(newCommand);

    //printf("%d\n", (int)newCommand.DAction);
}

void CAIPlayer::ClearAssignments(){
    for(auto WeakAsset : DPlayerData->Assets())
        if(auto Asset = WeakAsset.lock())
            DAssignedAssets[Asset->AssetID()] = false;
}