/**
 * @file UnitActionRenderer.h
 * @brief Draws Unit Action Tiles
 * 
 * Loads the images from the bin/img folder to use as assets
 * Performs checks on the selected units capabilities to draw action tiles
 * Used by the main Render() function in Battle Mode
 */

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
#ifndef UNITDACTIONRENDERER_H
#define UNITDACTIONRENDERER_H
#include "Bevel.h"
#include "PlayerAsset.h"
#include "GameModel.h"
#include <list>
#include <vector>

class CUnitActionRenderer{
    protected:
        std::shared_ptr< CGraphicTileset > DIconTileset;
        std::shared_ptr< CBevel > DBevel;
        std::shared_ptr< CPlayerData > DPlayerData;
        std::vector< int > DCommandIndices;
        std::vector< EAssetCapabilityType > DDisplayedCommands;
        EPlayerColor DPlayerColor;
        int DFullIconWidth;
        int DFullIconHeight;
        int DDisabledIndex;
        
    public:        
        CUnitActionRenderer(std::shared_ptr< CBevel > bevel, std::shared_ptr< CGraphicTileset > icons, EPlayerColor color, std::shared_ptr< CPlayerData > player);
        ~CUnitActionRenderer();
        
        int MinimumWidth() const;
        int MinimumHeight() const;
        
        EAssetCapabilityType Selection(const CPosition &pos) const;
        
        void DrawUnitAction(std::shared_ptr< CGraphicSurface > surface, const std::list< std::weak_ptr< CPlayerAsset > > &selectionlist, EAssetCapabilityType currentaction);
};

#endif

