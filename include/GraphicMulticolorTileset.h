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
#ifndef GRAPHICMULTICOLORTILESET_H
#define GRAPHICMULTICOLORTILESET_H
#include <string>
#include <map>
#include "DataSource.h"
#include "GraphicTileset.h"
#include "GraphicRecolorMap.h" 

class CGraphicMulticolorTileset : public CGraphicTileset{
    protected:
        std::vector< std::shared_ptr< CGraphicSurface > > DColoredTilesets;
        std::shared_ptr< CGraphicRecolorMap > DColorMap;
        
    public:
        CGraphicMulticolorTileset();
        virtual ~CGraphicMulticolorTileset();

        int ColorCount() const{
            return DColoredTilesets.size();  
        };
        
        int FindColor(const std::string &colorname) const{
            return DColorMap->FindColor(colorname);
        };
        
        virtual bool LoadTileset(std::shared_ptr< CGraphicRecolorMap > colormap, std::shared_ptr< CDataSource > source); 
        
        void DrawTile(std::shared_ptr<CGraphicSurface> surface, int xpos, int ypos, int tileindex, int colorindex);
};

#endif

