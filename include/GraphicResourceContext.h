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
#ifndef GRAPHICRESOURCECONTEXT_H
#define GRAPHICRESOURCECONTEXT_H
#include <memory>

class CGraphicSurface;

class CGraphicResourceContext{
    public:
        enum class ELineCap{Butt, Round, Square};
        enum class ELineJoin{Miter, Round, Bevel};
        
        CGraphicResourceContext(){};
        virtual ~CGraphicResourceContext(){};

        virtual void SetSourceRGB(uint32_t rgb) = 0;
        virtual void SetSourceRGB(double r, double g, double b) = 0;
        virtual void SetSourceRGBA(uint32_t rgb) = 0;
        virtual void SetSourceRGBA(double r, double g, double b, double a) = 0;
        virtual void SetSourceSurface(std::shared_ptr<CGraphicSurface> srcsurface, int xpos, int ypos) = 0;
        virtual void SetLineWidth(double width) = 0;
        virtual void SetLineCap(ELineCap cap) = 0;
        virtual void SetLineJoin(ELineJoin join) = 0;
        virtual void Scale(double sx, double sy) = 0;
        virtual void Paint() = 0;
        virtual void PaintWithAlpha(double alpha) = 0;
        virtual void Fill() = 0;
        virtual void Stroke() = 0;
        virtual void Rectangle(int xpos, int ypos, int width, int height) = 0;
        virtual void MoveTo(int xpos, int ypos) = 0;
        virtual void LineTo(int xpos, int ypos) = 0;
        virtual void Clip() = 0;
        virtual void MaskSurface(std::shared_ptr<CGraphicSurface> srcsurface, int xpos, int ypos) = 0;
        
        virtual std::shared_ptr<CGraphicSurface> GetTarget() = 0;
        virtual void Save() = 0;
        virtual void Restore() = 0;
        virtual void DrawSurface(std::shared_ptr<CGraphicSurface> srcsurface, int dxpos, int dypos, int width, int height, int sxpos, int sypos) = 0;
        virtual void CopySurface(std::shared_ptr<CGraphicSurface> srcsurface, int dxpos, int dypos, int width, int height, int sxpos, int sypos) = 0;
};

#endif

