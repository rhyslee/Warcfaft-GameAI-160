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
#ifndef MINIMAPRENDERER_H
#define MINIMAPRENDERER_H
#include "ViewportRenderer.h"

class CMiniMapRenderer{        
    protected:
        std::shared_ptr< CMapRenderer > DMapRenderer;
        std::shared_ptr< CAssetRenderer > DAssetRenderer;
        std::shared_ptr< CFogRenderer > DFogRenderer;
        std::shared_ptr< CViewportRenderer > DViewportRenderer;
        std::shared_ptr< CGraphicSurface > DWorkingSurface;
        uint32_t DViewportColor;
        int DVisibleWidth;
        int DVisibleHeight;
        
    public:        
        CMiniMapRenderer(std::shared_ptr< CMapRenderer > maprender, std::shared_ptr< CAssetRenderer > assetrender, std::shared_ptr< CFogRenderer > fogrender, std::shared_ptr< CViewportRenderer > viewport, CGraphicSurface::ESurfaceFormat format);
        ~CMiniMapRenderer();
        
        uint32_t ViewportColor() const;
        uint32_t ViewportColor(uint32_t color);
        
        int VisibleWidth() const;
        int VisibleHeight() const;
        
        void DrawMiniMap(std::shared_ptr< CGraphicSurface > surface);
};

#endif

