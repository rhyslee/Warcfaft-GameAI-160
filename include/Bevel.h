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
#ifndef BEVEL_H
#define BEVEL_H
#include "GraphicTileset.h"
#include <vector>

class CBevel{
    protected:
        std::shared_ptr< CGraphicTileset > DTileset;
        std::vector< int > DTopIndices;
        std::vector< int > DBottomIndices;
        std::vector< int > DLeftIndices;
        std::vector< int > DRightIndices;
        std::vector< int > DCornerIndices;
        int DWidth;
        
    public:
        CBevel(std::shared_ptr< CGraphicTileset > tileset);
        ~CBevel();

        int Width() const{
            return DWidth;
        };
        
        void DrawBevel(std::shared_ptr<CGraphicSurface> surface, int xpos, int ypos, int width, int height);
};

#endif

