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
#include "GraphicRecolorMap.h"
#include "GraphicFactory.h"
#include "CommentSkipLineDataSource.h"
#include "Debug.h"
        
CGraphicRecolorMap::CGraphicRecolorMap(){
    
}

CGraphicRecolorMap::~CGraphicRecolorMap(){
    
}

uint32_t CGraphicRecolorMap::RecolorPixels(void *data, uint32_t pixel){
    CGraphicRecolorMap *RecolorMap = static_cast<CGraphicRecolorMap *>(data);
    uint32_t Alpha = pixel & 0xFF000000;

    pixel |= 0xFF000000;
    for(size_t Index = 0; Index < RecolorMap->DColors[0].size(); Index++){
        if(pixel == RecolorMap->DColors[0][Index]){
            pixel = RecolorMap->DColors[RecolorMap->DState][Index];
            break;
        }
    }
    if(Alpha){
        uint32_t AlphaMult = Alpha>>24;
        return ((((pixel & 0x00FF0000) * AlphaMult) / 255) & 0x00FF0000) | ((((pixel & 0x0000FF00) * AlphaMult) / 255) & 0x0000FF00) | ((((pixel & 0x000000FF) * AlphaMult) / 255) & 0x000000FF) | Alpha;
    }
    return 0x00000000;
}

uint32_t CGraphicRecolorMap::ObservePixels(void *data, uint32_t pixel){
    CGraphicRecolorMap *RecolorMap = static_cast<CGraphicRecolorMap *>(data);
    int Row = RecolorMap->DState / RecolorMap->DColors[0].size();
    int Col = RecolorMap->DState % RecolorMap->DColors[0].size();
    RecolorMap->DOriginalColors[Row][Col] = pixel;
    RecolorMap->DColors[Row][Col] = pixel | 0xFF000000;
    RecolorMap->DState++;
    return pixel;
}

int CGraphicRecolorMap::FindColor(const std::string &colorname) const{
    auto Iterator = DMapping.find(colorname);
    if(DMapping.end() != Iterator){
        return Iterator->second;
    }
    return -1;
}

uint32_t CGraphicRecolorMap::ColorValue(int gindex, int cindex) const{
    if((0 > gindex)||(0 > cindex)||(gindex >= DOriginalColors.size())){
        return 0x00000000;   
    }
    if(cindex >= DOriginalColors[gindex].size()){
        return 0x00000000;
    }
    return DOriginalColors[gindex][cindex];
}

bool CGraphicRecolorMap::Load(std::shared_ptr< CDataSource > source){
    CCommentSkipLineDataSource LineSource(source, '#');
    std::string PNGPath, TempString;
    std::vector< std::string > Tokens;
    
    if(nullptr == source){
        return false;   
    }
    if(!LineSource.Read(PNGPath)){
        return false;
    }
    auto ColorSurface = CGraphicFactory::LoadSurface(source->Container()->DataSource(PNGPath));
    if(nullptr == ColorSurface){
        return false;
    }

    DColors.resize(ColorSurface->Height());
    DOriginalColors.resize(ColorSurface->Height());
    for(auto &Row : DColors){
        Row.resize(ColorSurface->Width());
    }
    for(auto &Row : DOriginalColors){
        Row.resize(ColorSurface->Width());
    }
    
    DState = 0;
    
    ColorSurface->Transform(ColorSurface, 0, 0, -1, -1, 0, 0, (void *)this, ObservePixels);
    
    if(!LineSource.Read(TempString)){
        return false;
    }

    try{
        int ColorCount = std::stoi(TempString);
        if(ColorCount != DColors.size()){
            return false;   
        }
        DColorNames.resize(ColorCount);
        for(int Index = 0; Index < ColorCount; Index++){
            if(!LineSource.Read(TempString)){
                return false;
            } 
            DMapping[TempString] = Index;
            DColorNames[Index] = TempString;   
        }
    }
    catch(std::exception &E){
        return false;
    }
    
    return true;
}


std::shared_ptr<CGraphicSurface> CGraphicRecolorMap::RecolorSurface(int index, std::shared_ptr<CGraphicSurface> srcsurface){
    if((0 > index)||(index >= DColors.size())){
        return nullptr;
    }
    DState = index;
    auto RecoloredSurface = CGraphicFactory::CreateSurface(srcsurface->Width(), srcsurface->Height(), srcsurface->Format()); 
    RecoloredSurface->Transform(srcsurface, 0, 0, -1, -1, 0, 0, (void *)this, RecolorPixels);
    
    return RecoloredSurface;
}

