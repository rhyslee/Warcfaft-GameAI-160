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
#ifndef TERRAINMAP_H
#define TERRAINMAP_H
#include "DataSource.h"
#include "GameDataTypes.h"
#include "Position.h"
#include <vector>

class CTerrainMap{
    public:
        enum class ETerrainTileType{
            None = 0,
            DarkGrass,
            LightGrass,
            DarkDirt,
            LightDirt,
            Rock,
            RockPartial,
            Forest,
            ForestPartial,
            DeepWater,
            ShallowWater,
            Max
        };
        enum class ETileType{
            None = 0,
            DarkGrass,
            LightGrass,
            DarkDirt,
            LightDirt,
            Rock,
            Rubble,
            Forest,
            Stump,
            DeepWater,
            ShallowWater,
            Max
        };
        
        static const uint8_t DInvalidPartial;
        
    protected:
        static bool DAllowedAdjacent[to_underlying(ETerrainTileType::Max)][to_underlying(ETerrainTileType::Max)];
        
        std::vector< std::vector< ETerrainTileType > > DTerrainMap;
        std::vector< std::vector< uint8_t > > DPartials;
        std::vector< std::vector< ETileType > > DMap;
        std::vector< std::vector< int > > DMapIndices;
        std::string DMapName;
        bool DRendered;
        
        void CalculateTileTypeAndIndex(int x, int y, ETileType &type, int &index);
        
    public:        
        CTerrainMap();
        CTerrainMap(const CTerrainMap &map);
        ~CTerrainMap();
        
        CTerrainMap &operator=(const CTerrainMap &map);
        
        std::string MapName() const;
        
        int Width() const;
        int Height() const;
        
        ETileType TileType(int xindex, int yindex) const{
            if((-1 > xindex)||(-1 > yindex)){
                return ETileType::None;    
            }
            if(DMap.size() <= yindex+1){
                return ETileType::None;   
            }
            if(DMap[yindex+1].size() <= xindex+1){
                return ETileType::None;   
            }
            return DMap[yindex+1][xindex+1];
        };
        
        ETileType TileType(const CTilePosition &pos) const{
            return TileType(pos.X(), pos.Y());
        };
        
        int TileTypeIndex(int xindex, int yindex) const{
            if((-1 > xindex)||(-1 > yindex)){
                return -1;    
            }
            if(DMapIndices.size() <= yindex+1){
                return -1;
            }
            if(DMapIndices[yindex+1].size() <= xindex+1){
                return -1;
            }
            return DMapIndices[yindex+1][xindex+1];
        };
        
        int TileTypeIndex(const CTilePosition &pos) const{
            return TileTypeIndex(pos.X(), pos.Y());
        };
        
        ETerrainTileType TerrainTileType(int xindex, int yindex) const{
            if((0 > xindex)||(0 > yindex)){
                return ETerrainTileType::None;    
            }
            if(DTerrainMap.size() <= yindex){
                return ETerrainTileType::None;   
            }
            if(DTerrainMap[yindex].size() <= xindex){
                return ETerrainTileType::None;   
            }
            return DTerrainMap[yindex][xindex];
        };
        ETerrainTileType TerrainTileType(const CTilePosition &pos) const{
            return TerrainTileType(pos.X(), pos.Y());
        };
        
        uint8_t TilePartial(int xindex, int yindex) const{
            if((0 > xindex)||(0 > yindex)){
                return DInvalidPartial;
            }
            if(DPartials.size() <= yindex){
                return DInvalidPartial;
            }
            if(DPartials[yindex].size() <= xindex){
                return DInvalidPartial;
            }
            return DPartials[yindex][xindex];
        };
        uint8_t TilePartial(const CTilePosition &pos) const{
            return TilePartial(pos.X(), pos.Y());   
        };
        
        void ChangeTerrainTilePartial(int xindex, int yindex, uint8_t val);
        
        void InitializeLumber(int lumber);
        
        static bool IsTraversable(ETileType type);
        static bool CanPlaceOn(ETileType type);
        
        void RenderTerrain();
        
        virtual bool LoadMap(std::shared_ptr< CDataSource > source);
};

#endif

