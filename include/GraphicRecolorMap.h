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
#ifndef GRAPHICRECOLORMAP_H
#define GRAPHICRECOLORMAP_H
#include <vector>
#include <unordered_map>
#include "GraphicSurface.h"
#include "DataSource.h"

class CGraphicRecolorMap{
    protected:
        int DState;
        std::unordered_map< std::string, int > DMapping;
        std::vector< std::string > DColorNames;
        std::vector< std::vector< uint32_t > > DColors;
        std::vector< std::vector< uint32_t > > DOriginalColors;
        
        static uint32_t RecolorPixels(void *data, uint32_t pixel);
        static uint32_t ObservePixels(void *data, uint32_t pixel);
        
    public:
        CGraphicRecolorMap();
        ~CGraphicRecolorMap();

        int GroupCount() const{
            return DColors.size();
        };
        
        int ColorCount() const{
            if(DColors.size()){
                return DColors[0].size();
            }
            return 0;
        };

        int FindColor(const std::string &colorname) const;
        uint32_t ColorValue(int gindex, int cindex) const;
        
        bool Load(std::shared_ptr< CDataSource > source);
        
        std::shared_ptr<CGraphicSurface> RecolorSurface(int index, std::shared_ptr<CGraphicSurface> srcsurface);
};

#endif

