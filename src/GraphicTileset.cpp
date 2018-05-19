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
#include "GraphicTileset.h"
#include "GraphicFactory.h"
#include "CommentSkipLineDataSource.h"
#include "Tokenizer.h"
#include "Debug.h"
#include <vector>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <iostream>

CGraphicTileset::CGraphicTileset(){
    DSurfaceTileset = nullptr;
    DTileCount = 0;
    DTileWidth = 0;
    DTileHeight = 0;
    DTileHalfWidth = 0;
    DTileHalfHeight = 0;
}

CGraphicTileset::~CGraphicTileset(){

}

bool CGraphicTileset::ParseGroupName(const std::string &tilename, std::string &aniname, int &anistep){
    size_t LastIndex = tilename.length();
    
    if(!LastIndex){
        return false;   
    }
    do{
        LastIndex--;
        if(!isdigit(tilename[LastIndex])){
            if(LastIndex + 1 == tilename.length()){
                return false;
            }
            aniname = tilename.substr(0,LastIndex+1);
            anistep = std::stoi(tilename.substr(LastIndex+1));
            return true;
        }
    }while(LastIndex);

    return false;
}

void CGraphicTileset::UpdateGroupNames(){
    DGroupSteps.clear();
    DGroupNames.clear();
    for(int Index = 0; Index < DTileCount; Index++){
        std::string GroupName;
        int GroupStep;

        if(ParseGroupName(DTileNames[Index], GroupName, GroupStep)){
            if(DGroupSteps.find(GroupName) != DGroupSteps.end()){
                if(DGroupSteps.at(GroupName) <= GroupStep){
                    DGroupSteps[GroupName] = GroupStep + 1;
                }
            }
            else{
                DGroupSteps[GroupName] = GroupStep + 1;
                DGroupNames.push_back(GroupName);
            }
        }
    }
}

int CGraphicTileset::TileCount(int count){

    if(0 > count){
        return DTileCount;   
    }
    if(!DTileWidth || !DTileHeight){
        return DTileCount;
    }
    if(count < DTileCount){
        auto Iterator = DMapping.begin();
        DTileCount = count;
        
        while(DMapping.end() != Iterator){
            if(Iterator->second >= DTileCount){
                Iterator = DMapping.erase(Iterator);
            }
            else{
                Iterator++;   
            }
        }
        DTileNames.resize(DTileCount);
        UpdateGroupNames();
        return DTileCount;
    }
    
    auto TempSurface = CGraphicFactory::CreateSurface(DTileWidth, count * DTileHeight, DSurfaceTileset->Format());
    
    if(!TempSurface){
        return DTileCount;   
    }
    
    DTileNames.resize(count);
    TempSurface->Copy(DSurfaceTileset, 0, 0, -1, -1, 0, 0);
    DSurfaceTileset = TempSurface;
    DTileCount = count;
    return DTileCount;
}

bool CGraphicTileset::ClearTile(int index){
    
    if((0 > index)||(index >= DTileCount)){
        return false;
    }
    if(!DSurfaceTileset){
        return false;
    }
    
    DSurfaceTileset->Clear(0, index * DTileHeight, DTileWidth, DTileHeight);
    return true;
}

bool CGraphicTileset::DuplicateTile(int destindex, const std::string &tilename, int srcindex){
    if((0 > srcindex)||(0 > destindex)||(srcindex >= DTileCount)||(destindex >= DTileCount)){
        return false;
    }
    if(tilename.empty()){
        return false;
    }
    ClearTile(destindex);
    
    DSurfaceTileset->Copy(DSurfaceTileset, 0, destindex * DTileHeight, DTileWidth, DTileHeight, 0, srcindex * DTileHeight);
    
    auto OldMapping = DMapping.find(DTileNames[destindex]);
    if(DMapping.end() != OldMapping){
        DMapping.erase(OldMapping);   
    }
    DTileNames[destindex] = tilename;
    DMapping[tilename] = destindex;

    return true;
}

bool CGraphicTileset::DuplicateClippedTile(int destindex, const std::string &tilename, int srcindex, int clipindex){
    if((0 > srcindex)||(0 > destindex)||(0 > clipindex)||(srcindex >= DTileCount)||(destindex >= DTileCount)||(clipindex >= DClippingMasks.size())){
        return false;
    }
    if(tilename.empty()){
        return false;
    }
    ClearTile(destindex);
    DSurfaceTileset->CopyMaskSurface(DSurfaceTileset, 0, destindex * DTileHeight, DClippingMasks[clipindex], 0, srcindex * DTileHeight);
    auto OldMapping = DMapping.find(DTileNames[destindex]);
    if(DMapping.end() != OldMapping){
        DMapping.erase(OldMapping);   
    }
    DTileNames[destindex] = tilename;
    DMapping[tilename] = destindex;
    if(destindex >= DClippingMasks.size()){
        DClippingMasks.resize(destindex + 1);
    }
    DClippingMasks[destindex] = CGraphicFactory::CreateSurface(DTileWidth, DTileHeight, CGraphicSurface::ESurfaceFormat::A1);
    DClippingMasks[destindex]->Copy(DSurfaceTileset, 0, 0, DTileWidth, DTileHeight, 0, destindex * DTileHeight);

    return true;
}

int CGraphicTileset::FindTile(const std::string &tilename) const{
    auto Iterator = DMapping.find(tilename);
    if(DMapping.end() != Iterator){
        return Iterator->second;
    }
    return -1;
}

int CGraphicTileset::GroupCount() const{
    return DGroupNames.size();
}

std::string CGraphicTileset::GroupName(int index) const{
    if(0 > index){
        return "";   
    }
    if(DGroupNames.size() <= index){
        return "";   
    }
    
    return DGroupNames[index];
}

int CGraphicTileset::GroupSteps(int index) const{
    if(0 > index){
        return 0;   
    }
    if(DGroupNames.size() <= index){
        return 0;   
    }
    return DGroupSteps.at(DGroupNames[index]);
}

int CGraphicTileset::GroupSteps(const std::string &Groupname) const{
    if(DGroupSteps.find(Groupname) != DGroupSteps.end()){
        return DGroupSteps.at(Groupname);
    }
    return 0;
}

void CGraphicTileset::CreateClippingMasks(){
    if(DSurfaceTileset){
        DClippingMasks.resize(DTileCount);
        for(int Index = 0; Index < DTileCount; Index++){
            DClippingMasks[Index] = CGraphicFactory::CreateSurface(DTileWidth, DTileHeight, CGraphicSurface::ESurfaceFormat::A1);
            DClippingMasks[Index]->Copy(DSurfaceTileset, 0, 0, DTileWidth, DTileHeight, 0, Index * DTileHeight);
        }
    }
}


bool CGraphicTileset::LoadTileset(std::shared_ptr< CDataSource > source){
    CCommentSkipLineDataSource LineSource(source, '#');
    std::string PNGPath, TempString;
    std::vector< std::string > Tokens;
    bool ReturnStatus = false;
    
    if(nullptr == source){
        return false;   
    }
    if(!LineSource.Read(PNGPath)){
        PrintError("Failed to get path.\n");
        goto LoadTilesetExit;
    }
    DSurfaceTileset = CGraphicFactory::LoadSurface(source->Container()->DataSource(PNGPath));
    if(nullptr == DSurfaceTileset){
        PrintError("Failed to load file %s.\n", PNGPath.c_str());
        goto LoadTilesetExit;        
    }
    DTileWidth = DSurfaceTileset->Width();
    DTileHeight = DSurfaceTileset->Height();
    if(!LineSource.Read(TempString)){
        goto LoadTilesetExit;
    }

    DTileCount = atoi(TempString.c_str());
    DTileHeight /= DTileCount;
    DTileNames.resize(DTileCount);
    for(int Index = 0; Index < DTileCount; Index++){
        if(!LineSource.Read(TempString)){
            goto LoadTilesetExit;
        }
        DMapping[TempString] = Index;
        DTileNames[Index] = TempString;
    }
    UpdateGroupNames();
    DTileHalfWidth = DTileWidth / 2;
    DTileHalfHeight = DTileHeight / 2;

    ReturnStatus = true;
LoadTilesetExit:
    return ReturnStatus;
}

void CGraphicTileset::DrawTile(std::shared_ptr<CGraphicSurface> surface, int xpos, int ypos, int tileindex){
    if((0 > tileindex)||(tileindex >= DTileCount)){
        return;
    }
    
    surface->Draw(DSurfaceTileset, xpos, ypos, DTileWidth, DTileHeight, 0, tileindex * DTileHeight);    
}


void CGraphicTileset::DrawClipped(std::shared_ptr<CGraphicSurface> surface, int xpos, int ypos, int tileindex, uint32_t rgb){
    if((0 > tileindex)||(tileindex >= DClippingMasks.size())){
        return;
    }
    auto ResourceContext = surface->CreateResourceContext();
    
    ResourceContext->SetSourceRGB(rgb);
    ResourceContext->MaskSurface(DClippingMasks[tileindex], xpos, ypos);
    ResourceContext->Fill();
}
