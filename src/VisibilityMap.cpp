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
#include "VisibilityMap.h"

/**
*
* @class VisibilityMap
*
* @brief This class keeps track of what portions of the map the user can see
*
* @author Jade
* 
* @version 1.0
*
* @date 10/12/17
*
* Contact: jamacdonnell@ucdavis.edu
*
*/

/**
* Constructor, builds visibility map based on input width and height
* Begins with all tiles unseen
*
* @param[in] width The width of the map
* @param[in] height The height of the map
* @param[in] maxvisibility The distance from an asset the user can see
*
* @return Nothing
*
*/

CVisibilityMap::CVisibilityMap(int width, int height, int maxvisibility){
    DMaxVisibility = maxvisibility;
    DMap.resize(height + 2 * DMaxVisibility);
    for(auto &Row : DMap){
        Row.resize(width + 2 * DMaxVisibility);
        for(auto &Cell : Row){
            Cell = ETileVisibility::None;   
        }
    }
    DTotalMapTiles = width * height;
    DUnseenTiles = DTotalMapTiles; 
}

/**
* Constructor, copies data members from input visibility map
*
* @param[in] map CVisibilityMap class object that you want to copy
*
* @return Nothing
*
*/

CVisibilityMap::CVisibilityMap(const CVisibilityMap &map){
    DMaxVisibility = map.DMaxVisibility;
    DMap = map.DMap;
    DTotalMapTiles = map.DTotalMapTiles;
    DUnseenTiles = map.DUnseenTiles;
}

/**
* Destructor
*
* @param[in] Nothing
*
* @return Nothing
*
*/

CVisibilityMap::~CVisibilityMap(){
    
}

/**
* = Assignment operator, copy all data member values from another CVisibilityMap class object
*
* @param[in] map CVisibilityMap class object on rhs of =
*
* @return CVisibilityMap class object on lhs of =
*
*/

CVisibilityMap &CVisibilityMap::operator=(const CVisibilityMap &map){
    if(this != &map){
        DMaxVisibility = map.DMaxVisibility;
        DMap = map.DMap;   
        DTotalMapTiles = map.DTotalMapTiles;
        DUnseenTiles = map.DUnseenTiles;
    }
    return *this;
}

/**
* Returns the width of the map, less the added visibility tiles 
*
* @param[in] Nothing
*
* @return int width of the map
*
*/

int CVisibilityMap::Width() const{
    if(DMap.size()){
        return DMap[0].size() - 2 * DMaxVisibility;
    }
    return 0;
}

/**
* Returns the height of the map, less the added visibility tiles
*
* @param[in] Nothing
*
* @return int height of the map
*
*/

int CVisibilityMap::Height() const{
    return DMap.size() - 2 * DMaxVisibility;
}

/**
* Returns the percentage of tiles the user has seen
*
* @param[in] max Usually 100, to convert decimal to percent
*
* @return int percent seen
*
*/

int CVisibilityMap::SeenPercent(int max) const{
    return (max * (DTotalMapTiles - DUnseenTiles)) / DTotalMapTiles;
}

/**
* Updates the portion of the map the user can see based on
* their assets
*
* @param[in] assets List of the players assets
*
* @return Nothing
*
*/
        
void CVisibilityMap::Update(const std::list< std::weak_ptr< CPlayerAsset > > &assets){
    for(auto &Row : DMap){
        for(auto &Cell : Row){
            if((ETileVisibility::Visible == Cell)||(ETileVisibility::Partial == Cell)){
                Cell = ETileVisibility::Seen;
            }
            else if(ETileVisibility::PartialPartial == Cell){
                Cell = ETileVisibility::SeenPartial;   
            }
        }
    }
    for(auto WeakAsset : assets){
        if(auto CurAsset = WeakAsset.lock()){
            CTilePosition Anchor = CurAsset->TilePosition();
            int Sight = CurAsset->EffectiveSight() + CurAsset->Size()/2;
            int SightSquared = Sight * Sight;
            Anchor.X(Anchor.X() + CurAsset->Size()/2);
            Anchor.Y(Anchor.Y() + CurAsset->Size()/2);
            for(int X = 0; X <= Sight; X++){
                int XSquared = X * X;
                int XSquared1 = X ? (X - 1) * (X - 1) : 0;
                
                for(int Y = 0; Y <= Sight; Y++){
                    int YSquared = Y * Y;
                    int YSquared1 = Y ? (Y - 1) * (Y - 1) : 0;
                    
                    if((XSquared + YSquared) < SightSquared){
                        // Visible
                        DMap[Anchor.Y() - Y + DMaxVisibility][Anchor.X() - X + DMaxVisibility] = ETileVisibility::Visible;
                        DMap[Anchor.Y() - Y + DMaxVisibility][Anchor.X() + X + DMaxVisibility] = ETileVisibility::Visible;
                        DMap[Anchor.Y() + Y + DMaxVisibility][Anchor.X() - X + DMaxVisibility] = ETileVisibility::Visible;
                        DMap[Anchor.Y() + Y + DMaxVisibility][Anchor.X() + X + DMaxVisibility] = ETileVisibility::Visible;
                    }
                    else if((XSquared1 + YSquared1) < SightSquared){
                        // Partial   
                        ETileVisibility CurVis = DMap[Anchor.Y() - Y + DMaxVisibility][Anchor.X() - X + DMaxVisibility];
                        if(ETileVisibility::Seen == CurVis){
                            DMap[Anchor.Y() - Y + DMaxVisibility][Anchor.X() - X + DMaxVisibility] = ETileVisibility::Partial;
                        }
                        else if((ETileVisibility::None == CurVis)||(ETileVisibility::SeenPartial == CurVis)){
                            DMap[Anchor.Y() - Y + DMaxVisibility][Anchor.X() - X + DMaxVisibility] = ETileVisibility::PartialPartial;
                        }
                        CurVis = DMap[Anchor.Y() - Y + DMaxVisibility][Anchor.X() + X + DMaxVisibility];
                        if(ETileVisibility::Seen == CurVis){
                            DMap[Anchor.Y() - Y + DMaxVisibility][Anchor.X() + X + DMaxVisibility] = ETileVisibility::Partial;
                        }
                        else if((ETileVisibility::None == CurVis)||(ETileVisibility::SeenPartial == CurVis)){
                            DMap[Anchor.Y() - Y + DMaxVisibility][Anchor.X() + X + DMaxVisibility] = ETileVisibility::PartialPartial;
                        }
                        CurVis = DMap[Anchor.Y() + Y + DMaxVisibility][Anchor.X() - X + DMaxVisibility];
                        if(ETileVisibility::Seen == CurVis){
                            DMap[Anchor.Y() + Y + DMaxVisibility][Anchor.X() - X + DMaxVisibility] = ETileVisibility::Partial;
                        }
                        else if((ETileVisibility::None == CurVis)||(ETileVisibility::SeenPartial == CurVis)){
                            DMap[Anchor.Y() + Y + DMaxVisibility][Anchor.X() - X + DMaxVisibility] = ETileVisibility::PartialPartial;
                        }
                        CurVis = DMap[Anchor.Y() + Y + DMaxVisibility][Anchor.X() + X + DMaxVisibility];
                        if(ETileVisibility::Seen == CurVis){
                            DMap[Anchor.Y() + Y + DMaxVisibility][Anchor.X() + X + DMaxVisibility] = ETileVisibility::Partial;
                        }
                        else if((ETileVisibility::None == CurVis)||(ETileVisibility::SeenPartial == CurVis)){
                            DMap[Anchor.Y() + Y + DMaxVisibility][Anchor.X() + X + DMaxVisibility] = ETileVisibility::PartialPartial;
                        }
                    }
                }
            }
        }
    }
    int MinX, MinY, MaxX, MaxY;
    MinY = DMaxVisibility;
    MaxY = DMap.size() - DMaxVisibility;
    MinX = DMaxVisibility;
    MaxX = DMap[0].size() - DMaxVisibility;
    DUnseenTiles = 0;
    for(int Y = MinY; Y < MaxY; Y++){
        for(int X = MinX; X < MaxX; X++){
            if(ETileVisibility::None == DMap[Y][X]){
                DUnseenTiles++;
            }
        }
    }
    
}

