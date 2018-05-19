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
#ifndef CURSORSET_H
#define CURSORSET_H

#include "GraphicTileset.h"

class CCursorSet : public CGraphicTileset{
    protected:
        std::vector < int > DCursorXPoint;
        std::vector < int > DCursorYPoint;
        
    public:
        CCursorSet();
        virtual ~CCursorSet();

        int CursorCount() const{
            return DTileCount;
        };
        
        int CursorWidth() const{
            return DTileWidth;
        };
        
        int CursorHeight() const{
            return DTileHeight;
        };
        
        int FindCursor(const std::string &cursorname) const;
        
        bool LoadCursors(std::shared_ptr< CDataSource > source);
        
        void DrawCursor(std::shared_ptr<CGraphicSurface> surface, int xpos, int ypos, int cursorindex);
};

#endif

