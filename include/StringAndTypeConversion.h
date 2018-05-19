
#ifndef STRINGANDTYPECONVERSTION_H
#define STRINGANDTYPECONVERSTION_H


#include "GameDataTypes.h"
#include "TerrainMap.h"

static EAssetAction ActionNameToType(const std::string &name){
    static std::unordered_map< std::string, EAssetAction > NameTypeTranslation({
        {"None", EAssetAction::None },       
        {"Construct", EAssetAction::Construct},
        {"Build", EAssetAction::Build},
        {"Repair", EAssetAction::Repair},
        {"Walk", EAssetAction::Walk},
        {"StandGround", EAssetAction::StandGround},
        {"Attack", EAssetAction::Attack},
	    {"HarvestLumber", EAssetAction::HarvestLumber},
	    {"MineGold", EAssetAction::MineGold},
	    {"QuarryStone", EAssetAction::QuarryStone},
        {"ConveyLumber", EAssetAction::ConveyLumber},
        {"ConveyGold", EAssetAction::ConveyGold},
        {"ConveyStone", EAssetAction::ConveyStone},
        {"Death", EAssetAction::Death},
        {"Decay", EAssetAction::Decay},
        {"Capability", EAssetAction::Capability}
        });
    auto Iterator = NameTypeTranslation.find(name);

    if(Iterator != NameTypeTranslation.end()){
        return Iterator->second;
    }
    PrintError("Unknown action name \"%s\"\n",name.c_str());
    return EAssetAction::None;
}

static EPlayerColor ColorNameToType(const std::string &name){
    static std::unordered_map< std::string, EPlayerColor > NameTypeTranslation({
    {"None", EPlayerColor::None},
    {"Red", EPlayerColor::Red},
    {"Blue", EPlayerColor::Black},
    {"Green", EPlayerColor::Green},
    {"Purple", EPlayerColor::Purple},
    {"Orange", EPlayerColor::Orange},
    {"Yellow", EPlayerColor::Yellow},
    {"Black", EPlayerColor::Black},
    {"White", EPlayerColor::White}});
    auto Iterator = NameTypeTranslation.find(name);

    if(Iterator != NameTypeTranslation.end()){
        return Iterator->second;
    }
    PrintError("Unknown capability name \"%s\"\n",name.c_str());
    return EPlayerColor::None;
}

static  CTerrainMap::ETerrainTileType TerrainTypeToName(const std::string &name){
    static std::unordered_map< std::string,  CTerrainMap::ETerrainTileType > NameTypeTranslation({
    {"None",  CTerrainMap::ETerrainTileType::None},
    {"DarkGrass",  CTerrainMap::ETerrainTileType::DarkGrass},
    {"LightGrass",  CTerrainMap::ETerrainTileType::LightGrass},
    {"DarkDirt",  CTerrainMap::ETerrainTileType::DarkDirt},
    {"LightDirt",  CTerrainMap::ETerrainTileType::LightDirt},
    {"Rock",  CTerrainMap::ETerrainTileType::Rock},
    {"RockPartial",  CTerrainMap::ETerrainTileType::RockPartial},
    {"Forest",  CTerrainMap::ETerrainTileType::Forest},
    {"ForestPartial",  CTerrainMap::ETerrainTileType::ForestPartial},
    {"DeepWater",  CTerrainMap::ETerrainTileType::DeepWater},
    {"ShallowWater",  CTerrainMap::ETerrainTileType::ShallowWater},
    });
    auto Iterator = NameTypeTranslation.find(name);
    if(Iterator != NameTypeTranslation.end()){
        return Iterator->second;
    }
    PrintError("Unknown terrain name \"%s\"\n",name.c_str());
    return  CTerrainMap::ETerrainTileType::None;
}
static CTerrainMap::ETileType TileNameToType(const std::string &name){
    static std::unordered_map< std::string, CTerrainMap::ETileType > NameTypeTranslation({
    {"None", CTerrainMap::ETileType::None},
    {"DarkGrass", CTerrainMap::ETileType::DarkGrass},
    {"LightGrass", CTerrainMap::ETileType::LightGrass},
    {"DarkDirt", CTerrainMap::ETileType::DarkDirt},
    {"LightDirt", CTerrainMap::ETileType::LightDirt},
    {"Rock", CTerrainMap::ETileType::Rock},
    {"Rubble", CTerrainMap::ETileType::Rubble},
    {"Forest", CTerrainMap::ETileType::Forest},
    {"Stump", CTerrainMap::ETileType::Stump},
    {"DeepWater", CTerrainMap::ETileType::DeepWater},
    {"ShallowWater", CTerrainMap::ETileType::ShallowWater},
    });
    auto Iterator = NameTypeTranslation.find(name);

    if(Iterator != NameTypeTranslation.end()){
        return Iterator->second;
    }
    PrintError("Unknown tile name \"%s\"\n",name.c_str());
    return CTerrainMap::ETileType::None;
}
    
static std::string TerrainTypeToName( CTerrainMap::ETerrainTileType type){
    static std::vector< std::string > TypeStrings({
        "None", "DarkGrass", "LightGrass", "DarkDirt", "LightDirt", "Rock", "RockPartial", "Forest",
         "ForestPartial", "DeepWater", "ShallowWater"});
    if((0 > to_underlying(type))||(to_underlying(type) >= TypeStrings.size())){
        return "";
    }
    return TypeStrings[to_underlying(type)];
}

static std::string TileTypeToName(CTerrainMap::ETileType type){
    static std::vector< std::string > TypeStrings({
        "None", "DarkGrass", "LightGrass", "DarkDirt", "LightDirt", "Rock", "Rubble", "Forest",
         "Stump", "DeepWater", "ShallowWater"});
    if((0 > to_underlying(type))||(to_underlying(type) >= TypeStrings.size())){
        return "";
    }
    return TypeStrings[to_underlying(type)];
}


static std::string ActionTypeToName(EAssetAction type){
    static std::vector< std::string > TypeStrings({
        "None", "Construct", "Build", "Repair", "Walk", "StandGround", "Attack", "HarvestLumber", "MineGold", "QuarryStone", "ConveyLumber", "ConveyGold", "ConveyStone", "Death", "Decay", "Capability"});
    if((0 > to_underlying(type))||(to_underlying(type) >= TypeStrings.size())){
        return "";
    }
    return TypeStrings[to_underlying(type)];
}
static std::string ColorTypeToName(EPlayerColor type){
    static std::vector< std::string > TypeStrings({ "None", "Red", "Blue", "Green", "Purple", "Orange", "Yellow", "Black", "White" });
    if((0 > to_underlying(type))||(to_underlying(type) >= TypeStrings.size())){
        return "";
    }
    return TypeStrings[to_underlying(type)];
}
#endif 