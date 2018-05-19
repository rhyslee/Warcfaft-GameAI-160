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
#include "GraphicMulticolorTileset.h"
//#include "Debug.h"

CGraphicMulticolorTileset::CGraphicMulticolorTileset() : CGraphicTileset(){
    
}

CGraphicMulticolorTileset::~CGraphicMulticolorTileset(){

}

bool CGraphicMulticolorTileset::LoadTileset(std::shared_ptr< CGraphicRecolorMap > colormap, std::shared_ptr< CDataSource > source){
    DColorMap = colormap;
    if(!CGraphicTileset::LoadTileset(source)){
        return false;
    }
    
    DColoredTilesets.clear();
    DColoredTilesets.push_back(DSurfaceTileset);
    for(int ColIndex = 1; ColIndex < colormap->GroupCount(); ColIndex++){
        DColoredTilesets.push_back(colormap->RecolorSurface(ColIndex, DSurfaceTileset));
    }
    
    return true;
}

void CGraphicMulticolorTileset::DrawTile(std::shared_ptr<CGraphicSurface> surface, int xpos, int ypos, int tileindex, int colorindex){
    if((0 > tileindex)||(tileindex >= DTileCount)){
        return;
    }
    if((0 > colorindex)||(colorindex >= DColoredTilesets.size())){
        return;    
    }
    
    surface->Draw(DColoredTilesets[colorindex], xpos, ypos, DTileWidth, DTileHeight, 0, tileindex * DTileHeight);
}

