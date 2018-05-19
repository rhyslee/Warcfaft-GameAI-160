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
#ifndef ASSETRENDERER_H
#define ASSETRENDERER_H
#include "GraphicMulticolorTileset.h"
#include "AssetDecoratedMap.h"
#include "Rectangle.h"
#include "Position.h"
#include "GameModel.h"
#include <vector>
#include <list>

class CAssetRenderer{
    protected:
        std::shared_ptr< CPlayerData > DPlayerData;
        std::shared_ptr< CAssetDecoratedMap > DPlayerMap;
        std::vector< std::shared_ptr< CGraphicMulticolorTileset > > DTilesets;
        std::shared_ptr< CGraphicTileset > DMarkerTileset;
        std::vector< std::shared_ptr< CGraphicTileset > > DFireTilesets;
        std::shared_ptr< CGraphicTileset > DBuildingDeathTileset;
        std::shared_ptr< CGraphicTileset > DCorpseTileset;
        std::shared_ptr< CGraphicTileset > DArrowTileset;
        std::vector< int > DMarkerIndices;
        std::vector< int > DCorpseIndices;
        std::vector< int > DArrowIndices;
        int DPlaceGoodIndex;
        int DPlaceBadIndex;
        std::vector< std::vector< int > > DNoneIndices;
        std::vector< std::vector< int > > DConstructIndices;
        std::vector< std::vector< int > > DBuildIndices;
        std::vector< std::vector< int > > DWalkIndices;
        std::vector< std::vector< int > > DAttackIndices;
        std::vector< std::vector< int > > DCarryGoldIndices;
        std::vector< std::vector< int > > DCarryLumberIndices;
        std::vector< std::vector< int > > DCarryStoneIndices;
        std::vector< std::vector< int > > DDeathIndices;
        std::vector< std::vector< int > > DPlaceIndices;
        
        std::vector< uint32_t > DPixelColors;
        static int DAnimationDownsample;
        
    public:
        CAssetRenderer(std::shared_ptr< CGraphicRecolorMap > colors, std::vector< std::shared_ptr< CGraphicMulticolorTileset > > tilesets, std::shared_ptr< CGraphicTileset > markertileset, std::shared_ptr< CGraphicTileset > corpsetileset, std::vector< std::shared_ptr< CGraphicTileset > > firetileset, std::shared_ptr< CGraphicTileset > buildingdeath, std::shared_ptr< CGraphicTileset > arrowtileset, std::shared_ptr< CPlayerData > player, std::shared_ptr< CAssetDecoratedMap > map);
        
        static int UpdateFrequency(int freq);
        
        void DrawAssets(std::shared_ptr<CGraphicSurface> surface, std::shared_ptr<CGraphicSurface> typesurface, const SRectangle &rect);
        void DrawSelections(std::shared_ptr<CGraphicSurface> surface, const SRectangle &rect, const std::list< std::weak_ptr< CPlayerAsset > > &selectionlist, const SRectangle &selectrect, bool highlightbuilding);
        void DrawOverlays(std::shared_ptr<CGraphicSurface> surface, const SRectangle &rect);
        void DrawPlacement(std::shared_ptr<CGraphicSurface> surface, const SRectangle &rect, const CPixelPosition &pos, EAssetType type, std::shared_ptr< CPlayerAsset > builder);
        void DrawMiniAssets(std::shared_ptr<CGraphicSurface> surface);
};

#endif

