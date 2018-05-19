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
#ifndef FONTTILESET_H
#define FONTTILESET_H
#include "GraphicMulticolorTileset.h"
#include <vector>

class CFontTileset : public CGraphicMulticolorTileset{
    protected:
        std::vector< int > DCharacterWidths;
        std::vector< std::vector < int > > DDeltaWidths;
        std::vector< int > DCharacterTops;
        std::vector< int > DCharacterBottoms;
        int DCharacterBaseline;
        int DSearchCall;
        int DTopOpaque;
        int DBottomOpaque;
        
        static uint32_t TopBottomSearch(void *data, uint32_t pixel);
        
    public:
        CFontTileset();
        virtual ~CFontTileset();
        
        bool LoadFont(std::shared_ptr< CGraphicRecolorMap > colormap, std::shared_ptr< CDataSource > source); 
        
        int CharacterBaseline() const{
            return DCharacterBaseline;  
        };
        
        void DrawText(std::shared_ptr<CGraphicSurface> surface, int xpos, int ypos, const std::string &str);
        void DrawTextColor(std::shared_ptr<CGraphicSurface> surface, int xpos, int ypos, int colorindex, const std::string &str);
        void DrawTextWithShadow(std::shared_ptr<CGraphicSurface> surface, int xpos, int ypos, int color, int shadowcol, int shadowwidth, const std::string &str);
        void MeasureText(const std::string &str, int &width, int &height);
        void MeasureTextDetailed(const std::string &str, int &width, int &height, int &top, int &bottom);
};

#endif

