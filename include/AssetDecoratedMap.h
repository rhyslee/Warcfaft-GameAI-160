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
#ifndef ASSETDECORATDMAP_H
#define ASSETDECORATDMAP_H
#include "TerrainMap.h"
#include "PlayerAsset.h"
#include "VisibilityMap.h"
#include <list>
#include <map>
#include <array>

class CAssetDecoratedMap : public CTerrainMap{
    public:
        typedef struct{
            std::string DType;
            EPlayerColor DColor;
            CTilePosition DTilePosition;
        } SAssetInitialization, *SAssetInitializationRef;
        
        typedef struct{
            EPlayerColor DColor;
            int DGold;
            int DLumber;
            int DStone;
        } SResourceInitialization, *SResourceInitializationRef;
        
    protected:
        std::list< std::shared_ptr< CPlayerAsset > > DAssets;
        std::list< SAssetInitialization > DAssetInitializationList;
        std::list< SResourceInitialization > DResourceInitializationList;
        std::vector< std::vector< int > > DSearchMap;
        std::vector< std::vector< int > > DLumberAvailable;
        std::vector< std::vector< int > > DStoneAvailable;
        
        static std::map< std::string, int > DMapNameTranslation;
        static std::vector< std::shared_ptr< CAssetDecoratedMap > > DAllMaps;
        
    public:        
        CAssetDecoratedMap();
        CAssetDecoratedMap(const CAssetDecoratedMap &map);
        CAssetDecoratedMap(const CAssetDecoratedMap &map, const std::array< EPlayerColor, to_underlying(EPlayerColor::Max)> &newcolors);
        ~CAssetDecoratedMap();
        
        CAssetDecoratedMap &operator=(const CAssetDecoratedMap &map);
        
        static bool LoadMaps(std::shared_ptr< CDataContainer > container);
        static int FindMapIndex(const std::string &name);
        static std::shared_ptr< const CAssetDecoratedMap > GetMap(int index);
        static std::shared_ptr< CAssetDecoratedMap > DuplicateMap(int index, const std::array< EPlayerColor, to_underlying(EPlayerColor::Max)> &newcolors);
        
        int PlayerCount() const{
             return DResourceInitializationList.size() - 1;
        };
        
        bool AddAsset(std::shared_ptr< CPlayerAsset > asset);
        bool RemoveAsset(std::shared_ptr< CPlayerAsset > asset);
        std::weak_ptr< CPlayerAsset > FindNearestAsset(const CPixelPosition &pos, EPlayerColor color, EAssetType type);
        bool CanPlaceAsset(const CTilePosition &pos, int size, std::shared_ptr< CPlayerAsset > ignoreasset);
        CTilePosition FindAssetPlacement(std::shared_ptr< CPlayerAsset > placeasset, std::shared_ptr< CPlayerAsset > fromasset, const CTilePosition &nexttiletarget);
        
        void RemoveLumber(const CTilePosition &pos, const CTilePosition &from, int amount);
        void RemoveStone(const CTilePosition &pos, const CTilePosition &from, int amount);
        
        bool LoadMap(std::shared_ptr< CDataSource > source);

        const std::list< std::shared_ptr< CPlayerAsset > > &Assets() const;
        const std::list< SAssetInitialization > &AssetInitializationList() const;
        const std::list< SResourceInitialization > &ResourceInitializationList() const;
        
        std::shared_ptr< CAssetDecoratedMap > CreateInitializeMap() const;
        std::shared_ptr< CVisibilityMap > CreateVisibilityMap() const;
        bool UpdateMap(const CVisibilityMap &vismap, const CAssetDecoratedMap &resmap);
        CTilePosition FindNearestReachableTileType(const CTilePosition &pos, ETileType type);
};

#endif

