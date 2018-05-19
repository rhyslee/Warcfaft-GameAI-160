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
#ifndef EDITRENDERER_H
#define EDITRENDERER_H
#include "Bevel.h"
#include "FontTileset.h"
#include "GameDataTypes.h"
#include <vector>

class CEditRenderer{
    protected:
        std::shared_ptr< CGraphicRecolorMap > DColorMap;
        std::shared_ptr< CBevel > DInnerBevel;
        std::vector< int > DLightIndices;
        std::vector< int > DDarkIndices;
        std::shared_ptr< CFontTileset > DFont;
        EPlayerColor DBackgroundColor;
        std::string DText;
        bool DTextValid;
        int DMinimumCharacters;
        int DMinimumWidth;
        int DMinimumHeight;
        int DWidth;
        int DHeight;
        int DWhiteIndex;
        int DGoldIndex;
        int DRedIndex;
        int DBlackIndex;
        
    public:        
        CEditRenderer(std::shared_ptr< CGraphicRecolorMap > colors, std::shared_ptr< CBevel > innerbevel, std::shared_ptr< CFontTileset > font);
        
        EPlayerColor BackgroundColor() const{
            return DBackgroundColor;   
        };
        
        EPlayerColor BackgroundColor(EPlayerColor color){
            return DBackgroundColor = color;
        };
        
        std::string Text() const{
            return DText;   
        };
        
        std::string Text(const std::string &text, bool valid);
        
        int MinimumCharacters() const{
            return DMinimumCharacters;  
        };
        
        int MinimumCharacters(int minchar);
        
        int Width() const{
            return DWidth;
        };
        
        int Width(int width);
        
        int Height() const{
            return DHeight;    
        };
        
        int Height(int height);
        
        void DrawEdit(std::shared_ptr< CGraphicSurface > surface, int x, int y, int cursorpos);
};

#endif

