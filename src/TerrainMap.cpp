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
#include "TerrainMap.h"
#include "CommentSkipLineDataSource.h"
#include "Tokenizer.h"
#include "Debug.h"
#include <cstdio>
#include <cstdlib>
                             
/**
*
* @class TerrainMap
*
* @brief This class maintains the tile attributes of the map
*
*   The map is represented by two 2D vectors, one is overlayed on top of the other. 
*   The ETerrainTileType (DTerrainMap) is the bottom layer and the ETileType (DMap) 
*   is the top layer. DTerrainMap is used to determine what tiles make up DMap and  
*   DMap is the map that is displayed. The DPartials map is used to keep track of
*   how much lumber can be acquired.
*
* @author Jade
* 
* @version 1.0
*
* @date 10/22/17
*
* Contact: jamacdonnell@ucdavis.edu
*
*/

const uint8_t CTerrainMap::DInvalidPartial = 0x1F; 

bool CTerrainMap::DAllowedAdjacent[to_underlying(ETerrainTileType::Max)][to_underlying(ETerrainTileType::Max)] = 
{
    {true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true },
    {true,  true,  true,  false, false, false, false, false, false, false, false},
    {true,  true,  true,  false, true,  false, false, true,  true,  false, false},
    {true,  false, false, true,  true,  false, false, false, false, false, false},
    {true,  false, true,  true,  true,  true,  true,  false, false, false, true },
    {true,  false, false, false, true,  true,  true,  false, false, false, false},
    {true,  false, false, false, true,  true,  true,  false, false, false, false},
    {true,  false, true,  false, false, false, false, true,  true,  false, false},
    {true,  false, true,  false, false, false, false, true,  true,  false, false},
    {true,  false, false, false, false, false, false, false, false, true,  true },
    {true,  false, false, false, true,  false, false, false, false, true,  true },
};

/**
* Constructor, sets rendered value to false
*
* @param[in] Nothing
*
* @return Nothing
*
*/

CTerrainMap::CTerrainMap(){
    DRendered = false;
}

/**
* Constructor, copies data members from input class
*
* @param[in] map CTerrainMap class object that you want to copy
*
* @return Nothing
*
*/

CTerrainMap::CTerrainMap(const CTerrainMap &map){
    DTerrainMap = map.DTerrainMap;
    DPartials = map.DPartials;
    DMapName = map.DMapName;
    DMap = map.DMap;
    DMapIndices = map.DMapIndices;
    DRendered = map.DRendered;
}

/**
* Destructor
*
* @param[in] Nothing
*
* @return Nothing
*
*/

CTerrainMap::~CTerrainMap(){

}

/**
* = Assignment operator, copy all data member values from another CTerrainMap class object 
*
* @param[in] map TerrainMap class object on rhs of =
*
* @return TerrainMap class object on lhs of =
*
*/

CTerrainMap &CTerrainMap::operator=(const CTerrainMap &map){
    if(this != &map){
        DTerrainMap = map.DTerrainMap;
        DPartials = map.DPartials;
        DMapName = map.DMapName; 
        DMap = map.DMap;
        DMapIndices = map.DMapIndices;
        DRendered = map.DRendered;        
    }
    return *this;
}

/**
* Returns string containing the name of the map
*
* @param[in] Nothing
*
* @return string map name
*
*/

std::string CTerrainMap::MapName() const{
    return DMapName;   
}

/**
* Returns the width of the map
*
* @note the map is represented by a vector of vectors of TileTypes
*
* @param[in] Nothing
*
* @return int width of the map
*
*/

int CTerrainMap::Width() const{
    if(DTerrainMap.size()){
        return DTerrainMap[0].size()-1;
    }
    return 0;
}

/**
* Returns the height of the map
*
* @note the map is represented by a vector of vectors of TileTypes
*
* @param[in] Nothing
*
* @return in height of the map
*
*/

int CTerrainMap::Height() const{
    return DTerrainMap.size()-1;
}

/**
* This function updates an index in the partials map to a new
* value and then recalculates the tile type of DMap at its
* corresponding index. 
*
* @param[in] xindex The x coordinate of the tile
* @param[in] yindex The y coordinate of the tile
* @param[in] val New value of partial at the coordinate
*
* @return Nothing
*
*/

void CTerrainMap::ChangeTerrainTilePartial(int xindex, int yindex, uint8_t val){
    if((0 > yindex)||(0 > xindex)){
        return;    
    }
    if(yindex >= DPartials.size()){
        return;    
    }
    if(xindex >= DPartials[0].size()){
        return;    
    }
    DPartials[yindex][xindex] = val;
    for(int YOff = 0; YOff < 2; YOff++){
        for(int XOff = 0; XOff < 2; XOff++){
            if(DRendered){
                ETileType Type;
                int Index;
                int XPos = xindex + XOff;
                int YPos = yindex + YOff;
                if((0 < XPos)&&(0 < YPos)){
                    if((YPos + 1 < DMap.size())&&(XPos + 1 < DMap[YPos].size())){
                        CalculateTileTypeAndIndex(XPos-1, YPos-1, Type, Index);
                        DMap[YPos][XPos] = Type;
                        DMapIndices[YPos][XPos] = Index;
                    }
                }
            }
        }
    }
}

/**
* Checks if a tile type can be traversed
*
* @param[in] type The ETileType you want to check
*
* @return true if the type can be traversed
*
*/

bool CTerrainMap::IsTraversable(ETileType type){
    switch(type){
        case ETileType::None:
        case ETileType::DarkGrass:
        case ETileType::LightGrass:
        case ETileType::DarkDirt:
        case ETileType::LightDirt:
        case ETileType::Rubble:
        case ETileType::Stump:      return true;
        default:                    return false;
    }
}

/**
* Checks if a tile type is the correct type to have an object 
* placed on it
*
* @param[in] type The ETileType you want to check
*
* @return true if the type can be placed on
*
*/

bool CTerrainMap::CanPlaceOn(ETileType type){
    switch(type){
        case ETileType::DarkGrass:
        case ETileType::LightGrass:
        case ETileType::DarkDirt:
        case ETileType::LightDirt:
        case ETileType::Rubble:
        case ETileType::Stump:      return true;
        default:                    return false;
    }
}

/**
* Given a map coordiante, determines the ETileType based on the ETerrainTileType
* and partial maps and calculates the index of that tile.
*
* @param[in] x The x coordinate
* @param[in] y The y coordinate
* @param[in] type The ETileType that will be returned
* @param[in] index The index that will be returned
*
* @return Nothing, but the two parameters type and index are passed
* in as a reference and are updated in the function
*
*/

void CTerrainMap::CalculateTileTypeAndIndex(int x, int y, ETileType &type, int &index){
    auto UL = DTerrainMap[y][x];
    auto UR = DTerrainMap[y][x+1];
    auto LL = DTerrainMap[y+1][x];
    auto LR = DTerrainMap[y+1][x+1];
    int TypeIndex = ((DPartials[y][x] & 0x8)>>3) | ((DPartials[y][x+1] & 0x4)>>1) | ((DPartials[y+1][x] & 0x2)<<1) | ((DPartials[y+1][x+1] & 0x1)<<3);
    
    
    if((ETerrainTileType::DarkGrass == UL)||(ETerrainTileType::DarkGrass == UR)||(ETerrainTileType::DarkGrass == LL)||(ETerrainTileType::DarkGrass == LR)){
        TypeIndex &= (ETerrainTileType::DarkGrass == UL) ? 0xF : 0xE;
        TypeIndex &= (ETerrainTileType::DarkGrass == UR) ? 0xF : 0xD;
        TypeIndex &= (ETerrainTileType::DarkGrass == LL) ? 0xF : 0xB;
        TypeIndex &= (ETerrainTileType::DarkGrass == LR) ? 0xF : 0x7;
        type = ETileType::DarkGrass;
        index = TypeIndex;
    }
    else if((ETerrainTileType::DarkDirt == UL)||(ETerrainTileType::DarkDirt == UR)||(ETerrainTileType::DarkDirt == LL)||(ETerrainTileType::DarkDirt == LR)){
        TypeIndex &= (ETerrainTileType::DarkDirt == UL) ? 0xF : 0xE;
        TypeIndex &= (ETerrainTileType::DarkDirt == UR) ? 0xF : 0xD;
        TypeIndex &= (ETerrainTileType::DarkDirt == LL) ? 0xF : 0xB;
        TypeIndex &= (ETerrainTileType::DarkDirt == LR) ? 0xF : 0x7;
        type = ETileType::DarkDirt;
        index = TypeIndex;
    }
    else if((ETerrainTileType::DeepWater == UL)||(ETerrainTileType::DeepWater == UR)||(ETerrainTileType::DeepWater == LL)||(ETerrainTileType::DeepWater == LR)){
        TypeIndex &= (ETerrainTileType::DeepWater == UL) ? 0xF : 0xE;
        TypeIndex &= (ETerrainTileType::DeepWater == UR) ? 0xF : 0xD;
        TypeIndex &= (ETerrainTileType::DeepWater == LL) ? 0xF : 0xB;
        TypeIndex &= (ETerrainTileType::DeepWater == LR) ? 0xF : 0x7;
        type = ETileType::DeepWater;
        index = TypeIndex;
    }
    else if((ETerrainTileType::ShallowWater == UL)||(ETerrainTileType::ShallowWater == UR)||(ETerrainTileType::ShallowWater == LL)||(ETerrainTileType::ShallowWater == LR)){
        TypeIndex &= (ETerrainTileType::ShallowWater == UL) ? 0xF : 0xE;
        TypeIndex &= (ETerrainTileType::ShallowWater == UR) ? 0xF : 0xD;
        TypeIndex &= (ETerrainTileType::ShallowWater == LL) ? 0xF : 0xB;
        TypeIndex &= (ETerrainTileType::ShallowWater == LR) ? 0xF : 0x7;
        type = ETileType::ShallowWater;
        index = TypeIndex;
    }
    else if((ETerrainTileType::Rock == UL)||(ETerrainTileType::Rock == UR)||(ETerrainTileType::Rock == LL)||(ETerrainTileType::Rock == LR)){
        TypeIndex &= (ETerrainTileType::Rock == UL) ? 0xF : 0xE;
        TypeIndex &= (ETerrainTileType::Rock == UR) ? 0xF : 0xD;
        TypeIndex &= (ETerrainTileType::Rock == LL) ? 0xF : 0xB;
        TypeIndex &= (ETerrainTileType::Rock == LR) ? 0xF : 0x7;
        if(TypeIndex){
            type = ETileType::Rock;
            index = TypeIndex;
        }
        else{
            type = ETileType::Rubble;
            index = ((ETerrainTileType::Rock == UL) ? 0x1 : 0x0) | ((ETerrainTileType::Rock == UR) ? 0x2 : 0x0) | ((ETerrainTileType::Rock == LL) ? 0x4 : 0x0) | ((ETerrainTileType::Rock == LR) ? 0x8 : 0x0);
        }
    }
    else if((ETerrainTileType::Forest == UL)||(ETerrainTileType::Forest == UR)||(ETerrainTileType::Forest == LL)||(ETerrainTileType::Forest == LR)){
        TypeIndex &= (ETerrainTileType::Forest == UL) ? 0xF : 0xE;
        TypeIndex &= (ETerrainTileType::Forest == UR) ? 0xF : 0xD;
        TypeIndex &= (ETerrainTileType::Forest == LL) ? 0xF : 0xB;
        TypeIndex &= (ETerrainTileType::Forest == LR) ? 0xF : 0x7;
        if(TypeIndex){
            type = ETileType::Forest;
            index = TypeIndex;
        }
        else{            
            type = ETileType::Stump;
            index = ((ETerrainTileType::Forest == UL) ? 0x1 : 0x0) | ((ETerrainTileType::Forest == UR) ? 0x2 : 0x0) | ((ETerrainTileType::Forest == LL) ? 0x4 : 0x0) | ((ETerrainTileType::Forest == LR) ? 0x8 : 0x0);
        }
    }
    else if((ETerrainTileType::LightDirt == UL)||(ETerrainTileType::LightDirt == UR)||(ETerrainTileType::LightDirt == LL)||(ETerrainTileType::LightDirt == LR)){
        TypeIndex &= (ETerrainTileType::LightDirt == UL) ? 0xF : 0xE;
        TypeIndex &= (ETerrainTileType::LightDirt == UR) ? 0xF : 0xD;
        TypeIndex &= (ETerrainTileType::LightDirt == LL) ? 0xF : 0xB;
        TypeIndex &= (ETerrainTileType::LightDirt == LR) ? 0xF : 0x7;
        type = ETileType::LightDirt;
        index = TypeIndex;
    }
    else{
        // Error?    
        type = ETileType::LightGrass;
        index = 0xF;
    }
}

/**
* Constructs the ETileType 2D vector map (DMap) based on the
* TerrainMap and adds a rock tile border around the map
*
* @param[in] Nothing
*
* @return Nothing
*
*/

void CTerrainMap::RenderTerrain(){
    DMap.resize(DTerrainMap.size()+1);
    DMapIndices.resize(DTerrainMap.size()+1);
    for(int YPos = 0; YPos < DMap.size(); YPos++){
        if((0 == YPos)||(DMap.size() - 1 == YPos)){
            for(int XPos = 0; XPos < DTerrainMap[0].size() + 1; XPos++){
                DMap[YPos].push_back(ETileType::Rock);
                DMapIndices[YPos].push_back(0xF);   
            }
        }
        else{
            for(int XPos = 0; XPos < DTerrainMap[YPos-1].size() + 1; XPos++){
                if((0 == XPos)||(DTerrainMap[YPos-1].size() == XPos)){
                    DMap[YPos].push_back(ETileType::Rock);
                    DMapIndices[YPos].push_back(0xF);   
                }
                else{
                    ETileType Type;
                    int Index;
                    CalculateTileTypeAndIndex(XPos-1, YPos-1, Type, Index);
                    DMap[YPos].push_back(Type);
                    DMapIndices[YPos].push_back(Index);
                }
            }
        }
    }
    DRendered = true;
}

/**
* Builds the DTerrainMap based on the data provided by source
*
* @param[in] source Map info is loaded from .map file and stored in source
*
* @return true or false if map successfully loaded or not
*
*/

bool CTerrainMap::LoadMap(std::shared_ptr< CDataSource > source){     
    CCommentSkipLineDataSource LineSource(source, '#');
    std::string TempString;
    std::vector< std::string > Tokens;
    int MapWidth, MapHeight;
    bool ReturnStatus = false;
    
    DTerrainMap.clear();
    
    if(!LineSource.Read(DMapName)){
        goto LoadMapExit;
    }
    if(!LineSource.Read(TempString)){
        goto LoadMapExit;
    }
    CTokenizer::Tokenize(Tokens, TempString);
    if(2 != Tokens.size()){
        goto LoadMapExit;
    }
    try{
        std::vector< std::string > StringMap;
        MapWidth = std::stoi(Tokens[0]);
        MapHeight = std::stoi(Tokens[1]);
            
        if((8 > MapWidth)||(8 > MapHeight)){
            goto LoadMapExit;        
        }
        while(StringMap.size() < MapHeight + 1){
            if(!LineSource.Read(TempString)){
                goto LoadMapExit;
            }                       
            StringMap.push_back(TempString);
            if(MapWidth + 1 > StringMap.back().length()){
                goto LoadMapExit;
            }
        }
        if(MapHeight + 1 > StringMap.size()){
            goto LoadMapExit;
        }
        DTerrainMap.resize(MapHeight+1);
        for(int Index = 0; Index < DTerrainMap.size(); Index++){
            DTerrainMap[Index].resize(MapWidth+1);
            for(int Inner = 0; Inner < MapWidth+1; Inner++){
                switch(StringMap[Index][Inner]){
                    case 'G':   DTerrainMap[Index][Inner] = ETerrainTileType::DarkGrass;
                                break;
                    case 'g':   DTerrainMap[Index][Inner] = ETerrainTileType::LightGrass;
                                break;
                    case 'D':   DTerrainMap[Index][Inner] = ETerrainTileType::DarkDirt;
                                break;
                    case 'd':   DTerrainMap[Index][Inner] = ETerrainTileType::LightDirt;
                                break;
                    case 'R':   DTerrainMap[Index][Inner] = ETerrainTileType::Rock;
                                break;
                    case 'r':   DTerrainMap[Index][Inner] = ETerrainTileType::RockPartial;
                                break;
                    case 'F':   DTerrainMap[Index][Inner] = ETerrainTileType::Forest;
                                break;
                    case 'f':   DTerrainMap[Index][Inner] = ETerrainTileType::ForestPartial;
                                break;
                    case 'W':   DTerrainMap[Index][Inner] = ETerrainTileType::DeepWater;
                                break;
                    case 'w':   DTerrainMap[Index][Inner] = ETerrainTileType::ShallowWater;
                                break;
                    default:    goto LoadMapExit;
                                break;
                }
                if(Inner){
                    if(!DAllowedAdjacent[to_underlying(DTerrainMap[Index][Inner])][to_underlying(DTerrainMap[Index][Inner-1])]){
                        goto LoadMapExit;
                    }
                }
                if(Index){
                    if(!DAllowedAdjacent[to_underlying(DTerrainMap[Index][Inner])][to_underlying(DTerrainMap[Index-1][Inner])]){
                        goto LoadMapExit;
                    }
                }
            }
            
        }   
        StringMap.clear();
        while(StringMap.size() < MapHeight + 1){
            if(!LineSource.Read(TempString)){
                goto LoadMapExit;
            }                                 
            StringMap.push_back(TempString);
            if(MapWidth + 1 > StringMap.back().length()){
                goto LoadMapExit;
            }
        }
        if(MapHeight + 1 > StringMap.size()){
            goto LoadMapExit;
        }
        DPartials.resize(MapHeight+1);
        for(int Index = 0; Index < DTerrainMap.size(); Index++){
            DPartials[Index].resize(MapWidth+1);
            for(int Inner = 0; Inner < MapWidth+1; Inner++){
                if(('0' <= StringMap[Index][Inner])&&('9' >= StringMap[Index][Inner])){
                    DPartials[Index][Inner] = StringMap[Index][Inner] - '0';
                }
                else if(('A' <= StringMap[Index][Inner])&&('F' >= StringMap[Index][Inner])){
                    DPartials[Index][Inner] = StringMap[Index][Inner] - 'A' + 0x0A;
                }
                else{
                    goto LoadMapExit;
                }
            }
        }
        ReturnStatus = true;
    }
    catch(std::exception &E){

    }
   
LoadMapExit:
    return ReturnStatus;
}

