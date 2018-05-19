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
#ifndef PIXELTYPE_H
#define PIXELTYPE_H
#include "GraphicSurface.h"
#include "TerrainMap.h"
#include "PlayerAsset.h"

class CPixelType{
    public:
        enum class EAssetTerrainType{
            None = 0,
            Grass,
            Dirt,
            Rock,
            Tree,
            Stump,
            Water,
            Wall,
            WallDamaged,
            Rubble,
            Peasant,
            Footman,
            Archer,
            Ranger,
            GoldMine,
            TownHall,
            Keep,
            Castle,
            Farm,
            Barracks,
            LumberMill,
            Blacksmith,
            ScoutTower,
            GuardTower,
            CannonTower,
            Max
        };
    protected:
        EAssetTerrainType DType;
        EPlayerColor DColor;
        
    public:
        CPixelType(int red, int green, int blue);
        CPixelType(CTerrainMap::ETileType type);
        CPixelType(const CPlayerAsset &asset);
        CPixelType(const CPixelType &pixeltype);
        
        
        CPixelType &operator=(const CPixelType &pixeltype);
        
        EAssetTerrainType Type() const{
            return DType;  
        };
        
        EPlayerColor Color() const{
            return DColor;  
        };

        uint32_t ToPixelColor() const;
        
        EAssetType AssetType() const;
        
        static CPixelType GetPixelType(std::shared_ptr<CGraphicSurface> surface, const CPixelPosition &pos);
        static CPixelType GetPixelType(std::shared_ptr<CGraphicSurface> surface, int xpos, int ypos);
};

#endif

