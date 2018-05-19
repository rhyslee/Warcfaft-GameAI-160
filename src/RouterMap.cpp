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
#include "RouterMap.h"
#include <cstdlib>
#include <queue>

#define SEARCH_STATUS_UNVISITED -1
#define SEARCH_STATUS_VISITED   -2
#define SEARCH_STATUS_OCCUPIED  -3

/**
*
* @class RouterMap
*
* @brief This class finds routes for assets to move to another location
*
* @author Jade
* 
* @version 1.0
*
* @date 10/22/17
*
* Contact: jamacdonnell@ucdavis.edu
*
*/

EDirection CRouterMap::DIdealSearchDirection = EDirection::North;
int CRouterMap::DMapWidth = 1;

/**
* Determine if two directions are away from each other
*
* param[in] dir1 The first direction
* param[in] dir2 The second direction
*
* return true if the directions are moving away from each other
*
*/

bool CRouterMap::MovingAway(EDirection dir1, EDirection dir2){
    int Value;
    if((0 > to_underlying(dir2))||(to_underlying(EDirection::Max) <= to_underlying(dir2))){
        return false;    
    }
    Value = ((to_underlying(EDirection::Max) + to_underlying(dir2)) - to_underlying(dir1)) % to_underlying(EDirection::Max);
    if((1 >= Value)||(to_underlying(EDirection::Max) - 1 <= Value)){
        return true;   
    }
    return false;
}

/**
* Find a route for an asset to get to a specified position
*
* param[in] resmap The map to find a route through
* param[in] asset The asset to move along the route
* param[in] target The destination position
*
* return a direction to move
*
*/

EDirection CRouterMap::FindRoute(const CAssetDecoratedMap &resmap, const CPlayerAsset &asset, const CPixelPosition &target){
    int MapWidth = resmap.Width();
    int MapHeight = resmap.Height();
    int StartX = asset.TilePositionX();
    int StartY = asset.TilePositionY();
    SSearchTarget CurrentSearch, BestSearch, TempSearch;
    CTilePosition CurrentTile, TargetTile, TempTile;
    EDirection SearchDirecitons[] = {EDirection::North,EDirection::East,EDirection::South,EDirection::West};
    int ResMapXOffsets[] = {0,1,0,-1};
    int ResMapYOffsets[] = {-1,0,1,0};                    
    int DiagCheckXOffset[] = {0,1,1,1,0,-1,-1,-1};
    int DiagCheckYOffset[] = {-1,-1,0,1,1,1,0,-1};
    int SearchDirectionCount = sizeof(SearchDirecitons) / sizeof(EDirection);
    EDirection LastInDirection, DirectionBeforeLast;
    std::queue< SSearchTarget > SearchQueue;
    
    TargetTile.SetFromPixel(target);
    if((DMap.size() != MapHeight + 2)||(DMap[0].size() != MapWidth + 2)){
        int LastYIndex = MapHeight + 1;
        int LastXIndex = MapWidth + 1;
        DMap.resize(MapHeight + 2);
        for(auto &Row : DMap){
            Row.resize(MapWidth + 2);   
        }
        for(int Index = 0; Index < DMap.size(); Index++){
            DMap[Index][0] = SEARCH_STATUS_VISITED;
            DMap[Index][LastXIndex] = SEARCH_STATUS_VISITED; 
        }
        for(int Index = 0; Index < MapWidth; Index++){
            DMap[0][Index+1] = SEARCH_STATUS_VISITED;
            DMap[LastYIndex][Index+1] = SEARCH_STATUS_VISITED;
        }
        DMapWidth = MapWidth + 2;
    }

    if(asset.TilePosition() == TargetTile){
        int DeltaX = target.X() - asset.PositionX();
        int DeltaY = target.Y() - asset.PositionY();
        
        if(0 < DeltaX){
            if(0 < DeltaY){
                return EDirection::NorthEast;
            }
            else if(0 > DeltaY){
                return EDirection::SouthEast;
            }
            return EDirection::East;
        }
        else if(0 > DeltaX){
            if(0 < DeltaY){
                return EDirection::NorthWest;
            }
            else if(0 > DeltaY){
                return EDirection::SouthWest;
            }
            return EDirection::West;
        }
        if(0 < DeltaY){
            return EDirection::North;
        }
        else if(0 > DeltaY){
            return EDirection::South;
        }
        
        return EDirection::Max;
    }

    for(int Y = 0; Y < MapHeight; Y++){
        for(int X = 0; X < MapWidth; X++){
            DMap[Y+1][X+1] = SEARCH_STATUS_UNVISITED;
        }
    }

    for(auto &Res : resmap.Assets()){
        if(&asset != Res.get()){
            if(EAssetType::None != Res->Type()){
                if((EAssetAction::Walk != Res->Action())||(asset.Color() != Res->Color())){
                    if((asset.Color() != Res->Color())||((EAssetAction::ConveyGold != Res->Action())&&(EAssetAction::ConveyLumber != Res->Action())&&(EAssetAction::MineGold != Res->Action())&&(EAssetAction::ConveyStone != Res->Action()))){
                        for(int YOff = 0; YOff < Res->Size(); YOff++){
                            for(int XOff = 0; XOff < Res->Size(); XOff++){
                                DMap[Res->TilePositionY() + YOff + 1][Res->TilePositionX() + XOff + 1] = SEARCH_STATUS_VISITED;
                            }
                        }
                    }
                }
                else{
                    DMap[Res->TilePositionY() + 1][Res->TilePositionX() + 1] = SEARCH_STATUS_OCCUPIED - to_underlying(Res->Direction());
                }
            }
        }
    }
    
    DIdealSearchDirection = asset.Direction();
    CurrentTile = asset.TilePosition();
    CurrentSearch.DX = BestSearch.DX = CurrentTile.X();
    CurrentSearch.DY = BestSearch.DY = CurrentTile.Y();
    CurrentSearch.DSteps = 0;
    CurrentSearch.DTargetDistanceSquared = BestSearch.DTargetDistanceSquared = CurrentTile.DistanceSquared(TargetTile);
    CurrentSearch.DInDirection = BestSearch.DInDirection = EDirection::Max;
    DMap[StartY+1][StartX+1] = SEARCH_STATUS_VISITED;
    while(true){
        if(CurrentTile == TargetTile){
            BestSearch = CurrentSearch;
            break;
        }
        if(CurrentSearch.DTargetDistanceSquared < BestSearch.DTargetDistanceSquared){
            BestSearch = CurrentSearch;
        }
        for(int Index = 0; Index < SearchDirectionCount; Index++){
            TempTile.X(CurrentSearch.DX + ResMapXOffsets[Index]);
            TempTile.Y(CurrentSearch.DY + ResMapYOffsets[Index]);
            if((SEARCH_STATUS_UNVISITED == DMap[TempTile.Y() + 1][TempTile.X() + 1])||MovingAway(SearchDirecitons[Index], (EDirection)(SEARCH_STATUS_OCCUPIED - DMap[TempTile.Y() + 1][TempTile.X() + 1]))){
                DMap[TempTile.Y() + 1][TempTile.X() + 1] = Index;
                CTerrainMap::ETileType CurTileType = resmap.TileType(TempTile.X(), TempTile.Y());
                //if((CTerrainMap::ETileType::Grass == CurTileType)||(CTerrainMap::ETileType::Dirt == CurTileType)||(CTerrainMap::ETileType::Stump == CurTileType)||(CTerrainMap::ETileType::Rubble == CurTileType)||(CTerrainMap::ETileType::None == CurTileType)){
                if(CTerrainMap::IsTraversable(CurTileType)){
                    TempSearch.DX = TempTile.X();
                    TempSearch.DY = TempTile.Y();
                    TempSearch.DSteps = CurrentSearch.DSteps + 1;
                    TempSearch.DTileType = CurTileType;
                    TempSearch.DTargetDistanceSquared = TempTile.DistanceSquared(TargetTile);
                    TempSearch.DInDirection = SearchDirecitons[Index];
                    SearchQueue.push(TempSearch);
                }
            }
        }
        if(SearchQueue.empty()){
            break;   
        }
        CurrentSearch = SearchQueue.front();
        SearchQueue.pop();
        CurrentTile.X(CurrentSearch.DX);
        CurrentTile.Y(CurrentSearch.DY);
    }
    DirectionBeforeLast = LastInDirection = BestSearch.DInDirection;
    CurrentTile.X(BestSearch.DX);
    CurrentTile.Y(BestSearch.DY);
    while((CurrentTile.X() != StartX)||(CurrentTile.Y() != StartY)){
        int Index = DMap[CurrentTile.Y()+1][CurrentTile.X()+1];
        
        if((0 > Index)||(SearchDirectionCount <= Index)){
            exit(0);   
        }
        DirectionBeforeLast = LastInDirection;
        LastInDirection = SearchDirecitons[Index];
        CurrentTile.DecrementX(ResMapXOffsets[Index]);
        CurrentTile.DecrementY(ResMapYOffsets[Index]);
    }
    if(DirectionBeforeLast != LastInDirection){
        CTerrainMap::ETileType CurTileType = resmap.TileType(StartX + DiagCheckXOffset[to_underlying(DirectionBeforeLast)], StartY + DiagCheckYOffset[to_underlying(DirectionBeforeLast)]);
        //if((CTerrainMap::ETileType::Grass == CurTileType)||(CTerrainMap::ETileType::Dirt == CurTileType)||(CTerrainMap::ETileType::Stump == CurTileType)||(CTerrainMap::ETileType::Rubble == CurTileType)||(CTerrainMap::ETileType::None == CurTileType)){
        if(CTerrainMap::IsTraversable(CurTileType)){
            int Sum = to_underlying(LastInDirection) + to_underlying(DirectionBeforeLast);
            if((6 == Sum)&&((EDirection::North == LastInDirection) || (EDirection::North == DirectionBeforeLast))){ // NW wrap around
                Sum += 8;
            }
            Sum /= 2;
            LastInDirection = static_cast<EDirection>(Sum);
        }
    }
    
    return LastInDirection;
}

