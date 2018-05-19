/**
 * @file UnitDescriptionRenderer.h
 * 
 * 
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
#ifndef UNITDESCRIPTIONRENDERER_H
#define UNITDESCRIPTIONRENDERER_H
#include "Bevel.h"
#include "FontTileset.h"
#include "GraphicMulticolorTileset.h"
#include "PlayerAsset.h"
#include <list>
#include <vector>
#include <array>

class CUnitDescriptionRenderer{
    public:
        enum class EFontSize{
            Small = 0,
            Medium,
            Large,
            Giant,
            Max
        };
        
    protected:
        std::shared_ptr< CGraphicMulticolorTileset > DIconTileset;
        std::shared_ptr< CBevel > DBevel;
        std::array< std::shared_ptr< CFontTileset >, to_underlying(EFontSize::Max) > DFonts;
        std::vector< int > DAssetIndices;
        std::vector< int > DResearchIndices;
        std::array< std::vector< int >, to_underlying(EFontSize::Max) > DFontColorIndices;
        std::vector< uint32_t > DHealthColors;
        EPlayerColor DPlayerColor;
        uint32_t DHealthRectangleFG;
        uint32_t DHealthRectangleBG;
        uint32_t DConstructionRectangleFG;
        uint32_t DConstructionRectangleBG;
        uint32_t DConstructionRectangleCompletion;
        uint32_t DConstructionRectangleShadow;
        int DFullIconWidth;
        int DFullIconHeight;
        int DDisplayedWidth;
        int DDisplayedHeight;
        int DDisplayedIcons;
        
        static std::string AddAssetNameSpaces(const std::string &name);
        void DrawCompletionBar(std::shared_ptr< CGraphicSurface > surface, int percent);
        
    public:        
        CUnitDescriptionRenderer(std::shared_ptr< CBevel > bevel, std::shared_ptr< CGraphicMulticolorTileset > icons, std::array< std::shared_ptr< CFontTileset >, to_underlying(EFontSize::Max) > fonts, EPlayerColor color);
        ~CUnitDescriptionRenderer();
        
        int MinimumWidth() const;
        int MinimumHeight(int width, int count) const;
        
        int MaxSelection(int width, int height) const;
        int Selection(const CPosition &pos) const;
        
        void DrawUnitDescription(std::shared_ptr< CGraphicSurface > surface, const std::list< std::weak_ptr< CPlayerAsset > > &selectionlist);
};

#endif

