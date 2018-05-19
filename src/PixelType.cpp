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
#include "PixelType.h"

/**
*
* @class PixelType
*
* @brief This class maintains a pixel's terrain type and player color
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

/**
* Constructor
*
* param[in] red Integer to set player color
* param[in] green Integer to set terrain type
* param[in] blue Integer
*
* return Nothing
*
*/

CPixelType::CPixelType(int red, int green, int blue){
    DColor = static_cast<EPlayerColor>(red);
    DType = static_cast<EAssetTerrainType>(green);
}

/**
* Constructor, given a tile type, set the matching AssetTerrainType
*
* param[in] type ETileType to copy
*
* return Nothing
*
*/

CPixelType::CPixelType(CTerrainMap::ETileType type){
    DColor = EPlayerColor::None;
    switch(type){
        case CTerrainMap::ETileType::LightGrass:         
        case CTerrainMap::ETileType::DarkGrass:     DType = EAssetTerrainType::Grass;
                                                    break;  
        case CTerrainMap::ETileType::LightDirt:                                            
        case CTerrainMap::ETileType::DarkDirt:      DType = EAssetTerrainType::Dirt;
                                                    break;
        case CTerrainMap::ETileType::Rock:          DType = EAssetTerrainType::Rock;
                                                    break;
        case CTerrainMap::ETileType::Forest:        DType = EAssetTerrainType::Tree;
                                                    break;
        case CTerrainMap::ETileType::Stump:         DType = EAssetTerrainType::Stump;
                                                    break;
        case CTerrainMap::ETileType::ShallowWater:                                              
        case CTerrainMap::ETileType::DeepWater:     DType = EAssetTerrainType::Water;
                                                    break;
        case CTerrainMap::ETileType::Rubble:        DType = EAssetTerrainType::Rubble;
                                                    break;
        default:                                    DType = EAssetTerrainType::None;
                                                    break;
    }                                                                             
}

/**
* Constructor, given an asset type, set the matching AssetTerrainType
*
* param[in] asset The asset type to copy
*
* return Nothing
*
*/

CPixelType::CPixelType(const CPlayerAsset &asset){
    switch(asset.Type()){
        case EAssetType::Peasant:       DType = EAssetTerrainType::Peasant;
                                        break;
        case EAssetType::Footman:       DType = EAssetTerrainType::Footman;
                                        break;
        case EAssetType::Archer:        DType = EAssetTerrainType::Archer;
                                        break;
        case EAssetType::Ranger:        DType = EAssetTerrainType::Ranger;
                                        break;
        case EAssetType::GoldMine:      DType = EAssetTerrainType::GoldMine;
                                        break;
        case EAssetType::TownHall:      DType = EAssetTerrainType::TownHall;
                                        break;
        case EAssetType::Keep:          DType = EAssetTerrainType::Keep;
                                        break;
        case EAssetType::Castle:        DType = EAssetTerrainType::Castle;
                                        break;
        case EAssetType::Farm:          DType = EAssetTerrainType::Farm;
                                        break;
        case EAssetType::Barracks:      DType = EAssetTerrainType::Barracks;
                                        break;
        case EAssetType::LumberMill:    DType = EAssetTerrainType::LumberMill;
                                        break;
        case EAssetType::Blacksmith:    DType = EAssetTerrainType::Blacksmith;
                                        break;
        case EAssetType::ScoutTower:    DType = EAssetTerrainType::ScoutTower;
                                        break;
        case EAssetType::GuardTower:    DType = EAssetTerrainType::GuardTower;
                                        break;
        case EAssetType::CannonTower:   DType = EAssetTerrainType::CannonTower;
                                        break;
        case EAssetType::Wall:          DType = EAssetTerrainType::Wall;
                                        break;
        default:                        DType = EAssetTerrainType::None;
                                        break;
    }
    DColor = asset.Color();
}

/**
* Constructor, copy another CPixelType object's data members
*
* param[in] pixeltype The CPixelType object to copy
*
* return Nothing
*
*/

CPixelType::CPixelType(const CPixelType &pixeltype){
    DType = pixeltype.DType;
    DColor = pixeltype.DColor;
}

/**
* = Assignment operator, copy all the data member values from
* another CPixelType class object
*
* param[in] pixeltype CPixelType object on rhs of =
*
* return CPixelType object on lhs of =
*
*/

CPixelType &CPixelType::operator=(const CPixelType &pixeltype){
    if(this != &pixeltype){
        DType = pixeltype.DType;
        DColor = pixeltype.DColor;   
    }
    return *this;
}
 
/**
* Convert a pixel's color to a value for renderer
*
* param[in] Nothing
*
* return color value for renderer
*
*/

uint32_t CPixelType::ToPixelColor() const{
    uint32_t RetVal = static_cast<uint32_t>(DColor);
    
    RetVal <<= 16;
    RetVal |= (static_cast<uint32_t>(DType))<<8;
    return RetVal;
}

/**
* Find a pixel's corresponding EAssetType based on its
* EAssetTerrainType
*
* param[in] Nothing
*
* return corresponding EAssetType
*
*/

EAssetType CPixelType::AssetType() const{
    switch(DType){
        case EAssetTerrainType::Peasant:        return EAssetType::Peasant;
        case EAssetTerrainType::Footman:        return EAssetType::Footman;
        case EAssetTerrainType::Archer:         return EAssetType::Archer;
        case EAssetTerrainType::Ranger:         return EAssetType::Ranger;
        case EAssetTerrainType::GoldMine:       return EAssetType::GoldMine;
        case EAssetTerrainType::TownHall:       return EAssetType::TownHall;
        case EAssetTerrainType::Keep:           return EAssetType::Keep;
        case EAssetTerrainType::Castle:         return EAssetType::Castle;
        case EAssetTerrainType::Farm:           return EAssetType::Farm;
        case EAssetTerrainType::Barracks:       return EAssetType::Barracks;
        case EAssetTerrainType::LumberMill:     return EAssetType::LumberMill;
        case EAssetTerrainType::Blacksmith:     return EAssetType::Blacksmith;
        case EAssetTerrainType::ScoutTower:     return EAssetType::ScoutTower;
        case EAssetTerrainType::GuardTower:     return EAssetType::GuardTower;
        case EAssetTerrainType::CannonTower:    return EAssetType::CannonTower;                          
        case EAssetTerrainType::Wall:           return EAssetType::Wall;                          
        default:                                return EAssetType::None;
    }
}

/**
* Given a CPixelPosition and CGraphicSurface, create a new CPixelType
* object with colors matching the surface pixelcolor at that position 
*
* param[in] surface The CGraphicSurface to get color from
* param[in] pos The pixel position 
*
* return new CPixelType object
*
*/

CPixelType CPixelType::GetPixelType(std::shared_ptr<CGraphicSurface> surface, const CPixelPosition &pos){
    return GetPixelType(surface, pos.X(), pos.Y());
}

/**
* Given an xpos, ypos, and CGraphicSurface, create a new CPixelType
* object with colors matching the surface pixelcolor at that position 
*
* param[in] surface The CGraphicSurface to get color from
* param[in] xpos The x value pixel position 
* param[in] ypos The y value pixel position
*
* return new CPixelType object
*
*/

CPixelType CPixelType::GetPixelType(std::shared_ptr<CGraphicSurface> surface, int xpos, int ypos){
    uint32_t PixelColor = surface->PixelAt(xpos, ypos);
    return CPixelType((PixelColor>>16) & 0xFF, (PixelColor>>8) & 0xFF, PixelColor & 0xFF);
}

