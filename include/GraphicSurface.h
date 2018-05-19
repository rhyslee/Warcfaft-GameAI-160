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
#ifndef GRAPHICSURFACE_H
#define GRAPHICSURFACE_H
#include <cstdint>
#include "GraphicResourceContext.h"

using TGraphicSurfaceTransformCallback = uint32_t (*)(void *calldata, uint32_t src);

class CGraphicSurface{
    public:
        enum class ESurfaceFormat{ARGB32, RGB24, A8, A1};
        
        CGraphicSurface(){};
        virtual ~CGraphicSurface(){};
        
        virtual int Width() const = 0;
        virtual int Height() const = 0;
        
        virtual ESurfaceFormat Format() const = 0;
        
        virtual uint32_t PixelAt(int xpos, int ypos) = 0;
        
        virtual void Clear(int xpos = 0, int ypos = 0, int width = -1, int height = -1) = 0;
        virtual std::shared_ptr<CGraphicSurface> Duplicate() = 0;
        
        virtual std::shared_ptr<CGraphicResourceContext> CreateResourceContext() = 0;
        
        virtual void Draw(std::shared_ptr<CGraphicSurface> srcsurface, int dxpos, int dypos, int width, int height, int sxpos, int sypos) = 0;
        virtual void Copy(std::shared_ptr<CGraphicSurface> srcsurface, int dxpos, int dypos, int width, int height, int sxpos, int sypos) = 0;
        virtual void CopyMaskSurface(std::shared_ptr<CGraphicSurface> srcsurface, int dxpos, int dypos, std::shared_ptr<CGraphicSurface> masksurface, int sxpos, int sypos) = 0;
        virtual void Transform(std::shared_ptr<CGraphicSurface> srcsurface, int dxpos, int dypos, int width, int height, int sxpos, int sypos, void *calldata, TGraphicSurfaceTransformCallback callback) = 0;
};

#endif

