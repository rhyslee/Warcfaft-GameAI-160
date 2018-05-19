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
#include "MiniMapRenderer.h"
#include "GraphicFactory.h"

/**
*
* @class CMiniMapRenderer
*
* @brief This class maintains the minimap rendering information
*
* @author Hugo
* 
* @version 4.0
*
* @date 10/30/17
*
* Contact: huhuang@ucdavis.edu
*
*/


/**
* A CMiniMapRenderer object constructor
*
* @param[in] maprender
* @param[in] assetrender
* @param[in] fogrender
* @param[in] viewport
* @param[in] format
*
* @return none (Constructor)
*/
CMiniMapRenderer::CMiniMapRenderer(std::shared_ptr< CMapRenderer > maprender, std::shared_ptr< CAssetRenderer > assetrender, std::shared_ptr< CFogRenderer > fogrender, std::shared_ptr< CViewportRenderer > viewport, CGraphicSurface::ESurfaceFormat format){
    DMapRenderer = maprender;
    DAssetRenderer = assetrender;
    DFogRenderer = fogrender;
    DViewportRenderer = viewport;
    DViewportColor = 0xFFFFFF;

    DVisibleWidth = DMapRenderer->MapWidth();
    DVisibleHeight = DMapRenderer->MapHeight();

    DWorkingSurface = CGraphicFactory::CreateSurface(DMapRenderer->MapWidth(), DMapRenderer->MapHeight(), format);

    auto ResourceContext = DWorkingSurface->CreateResourceContext();
    ResourceContext->SetSourceRGB(0x000000);
    ResourceContext->Rectangle(0, 0, DMapRenderer->MapWidth(), DMapRenderer->MapHeight());
    ResourceContext->Fill();
}

/**
* A CMiniMapRenderer object destructor
*
* @return none (Destructor)
*/
CMiniMapRenderer::~CMiniMapRenderer(){

}

/**
* This function returns the ViewportColor data
*
* @return uint32_t
*/
uint32_t CMiniMapRenderer::ViewportColor() const{
    return DViewportColor;
}

/**
* This function sets and returns the ViewportColor data
*
* @param[in] color The new value for ViewportColor
*
* @return uint32_t
*/
uint32_t CMiniMapRenderer::ViewportColor(uint32_t color){
    return DViewportColor = color;
}

/**
*
* This function returns the VisibleWidth data
*
* @return int
*/
int CMiniMapRenderer::VisibleWidth() const{
    return DVisibleWidth;
}

/**
*
* This function returns the VisibleHeight data
*
* @return int
*/
int CMiniMapRenderer::VisibleHeight() const{
    return DVisibleHeight;
}

/**
*
* This function draws the minimap
*
* @param surface The shared pointer to a GraphicSurface object
*
* @return none
*/
void CMiniMapRenderer::DrawMiniMap(std::shared_ptr< CGraphicSurface > surface){
    auto ResourceContext = surface->CreateResourceContext();
    int MiniMapViewportX, MiniMapViewportY;
    int MiniMapViewportWidth, MiniMapViewportHeight;
    int MiniMapWidth, MiniMapHeight;
    int MMW_MH, MMH_MW;
    int DrawWidth, DrawHeight;
    double SX, SY;

    MiniMapWidth = surface->Width();
    MiniMapHeight = surface->Height();

    SX = (double)MiniMapWidth / (double)DMapRenderer->MapWidth();
    SY = (double)MiniMapHeight / (double)DMapRenderer->MapHeight();

    if(SX < SY){
        DrawWidth = MiniMapWidth;
        DrawHeight = SX * DMapRenderer->MapHeight();
        SY = SX;
    }
    else if(SX > SY){
        DrawWidth = SY * DMapRenderer->MapWidth();
        DrawHeight = MiniMapHeight;
        SX = SY;
    }
    else{
        DrawWidth = MiniMapWidth;
        DrawHeight = MiniMapHeight;
    }

    MMW_MH = MiniMapWidth * DMapRenderer->MapHeight();
    MMH_MW = MiniMapHeight * DMapRenderer->MapWidth();

    if(MMH_MW > MMW_MH){
        DVisibleWidth = MiniMapWidth;
        DVisibleHeight = (DMapRenderer->MapHeight() * MiniMapWidth) / DMapRenderer->MapWidth();
    }
    else if(MMH_MW < MMW_MH){
        DVisibleWidth = (DMapRenderer->MapWidth() * MiniMapHeight) / DMapRenderer->MapHeight();
        DVisibleHeight = MiniMapHeight;
    }
    else{
        DVisibleWidth = MiniMapWidth;
        DVisibleHeight = MiniMapHeight;
    }

    DMapRenderer->DrawMiniMap(DWorkingSurface);
    DAssetRenderer->DrawMiniAssets(DWorkingSurface);
    if(nullptr != DFogRenderer){
        DFogRenderer->DrawMiniMap(DWorkingSurface);
    }

    ResourceContext->Save();
    ResourceContext->Scale(SX, SY);
    ResourceContext->SetSourceSurface(DWorkingSurface, 0, 0);
    ResourceContext->Rectangle(0, 0, DrawWidth, DrawHeight);
    ResourceContext->Fill();
    ResourceContext->Restore();
    /*
    DWorkingPixbuf = gdk_pixbuf_get_from_drawable(DWorkingPixbuf, DWorkingPixmap, nullptr, 0, 0, 0, 0, -1, -1);

    ScaledPixbuf = gdk_pixbuf_scale_simple(DWorkingPixbuf, DVisibleWidth, DVisibleHeight, GDK_INTERP_BILINEAR);
    gdk_draw_pixbuf(drawable, TempGC, ScaledPixbuf, 0, 0, 0, 0, -1, -1, GDK_RGB_DITHER_NONE, 0, 0);
    g_object_unref(ScaledPixbuf);
    */
    if(nullptr != DViewportRenderer){
        ResourceContext->SetSourceRGB(DViewportColor);
        //gdk_gc_set_rgb_fg_color(TempGC, &DViewportColor);
        MiniMapViewportX = (DViewportRenderer->ViewportX() * DVisibleWidth) / DMapRenderer->DetailedMapWidth();
        MiniMapViewportY = (DViewportRenderer->ViewportY() * DVisibleHeight) / DMapRenderer->DetailedMapHeight();
        MiniMapViewportWidth = (DViewportRenderer->LastViewportWidth() * DVisibleWidth) / DMapRenderer->DetailedMapWidth();
        MiniMapViewportHeight = (DViewportRenderer->LastViewportHeight() * DVisibleHeight) / DMapRenderer->DetailedMapHeight();
        ResourceContext->Rectangle(MiniMapViewportX, MiniMapViewportY, MiniMapViewportWidth,  MiniMapViewportHeight);
        ResourceContext->Stroke();
        //gdk_draw_rectangle(drawable, TempGC, FALSE, MiniMapViewportX, MiniMapViewportY, MiniMapViewportWidth,  MiniMapViewportHeight);
    }


}
