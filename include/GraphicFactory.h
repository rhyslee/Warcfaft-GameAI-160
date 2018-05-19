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
#ifndef GRAPHICFACTORY_H
#define GRAPHICFACTORY_H
#include "GraphicSurface.h"
#include "DataSource.h"
#include "DataSink.h"

class CGraphicFactory{
    public:
        static std::shared_ptr<CGraphicSurface> CreateSurface(int width, int height, CGraphicSurface::ESurfaceFormat format);
        static std::shared_ptr<CGraphicSurface> LoadSurface(std::shared_ptr< CDataSource > source);
        static bool StoreSurface(std::shared_ptr< CDataSink > sink, std::shared_ptr<CGraphicSurface> surface);
};

#endif

