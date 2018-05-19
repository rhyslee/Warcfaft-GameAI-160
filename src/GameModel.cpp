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

/**
*
* @class GameModel
*
* @brief This class deals with basic Character Data
*   It creates and manages the Character's Assets, finding assets, selecting assets, requirements of assets, etc.
*   It also tracks Food consumption/production, asset behavior, and what occurs each timestep.
*
* @version 1.0
*
* @author Kitan $
*
* @date 10/20/2017 $
*
* Contact: kitgarcia@ucdavis.edu
*
*/

#include "GameModel.h"
#include "ApplicationData.h"
#include "Debug.h"
#include <algorithm>
#include <stdlib.h>

int GAssetIDCount = 0;
std::map< int, std::shared_ptr< CPlayerAsset > > GAssetIDMap;

std::shared_ptr< CPlayerAsset > FindAssetObj(int AssetID){
    auto Iterator = GAssetIDMap.find(AssetID);

    if(Iterator != GAssetIDMap.end())
        return Iterator->second;
    return nullptr;
}

void MapNewAssetObj(std::shared_ptr< CPlayerAsset > CreatedAsset){
    GAssetIDMap.insert(std::pair < int, std::shared_ptr< CPlayerAsset > >(GAssetIDCount, CreatedAsset));
    GAssetIDCount++;
}

int GetAssetIDCount(){
    return GAssetIDCount;
}


/**
*  Calculates the range in terms of tile width using Pythagorean Theorem
*
*  @param[in] range An int that stores the tile width
*
*  @return int returns the distance squared
*
*/
int RangeToDistanceSquared(int range){
    range *= CPosition::TileWidth();
    range *= range;
    range += CPosition::TileWidth() * CPosition::TileWidth();
    return range;
}

/**
*  Initializes resources from the map and player assets with the correct tile position, color, and type.
*
*  @param[in] map A shared pointer to CAssetDecoratedMap
*  @param[in] color A variable that stores the player's color
*
*/
CPlayerData::CPlayerData(std::shared_ptr< CAssetDecoratedMap > map, std::shared_ptr< CTriggerHandler > handler, EPlayerColor color){
    DIsAI = true;
    DGameCycle = 0;
    DColor = color;
    DTriggerHandler = handler;
    DActualMap = map;
    DAssetTypes = CPlayerAssetType::DuplicateRegistry(color);
    DPlayerMap = DActualMap->CreateInitializeMap();
    DVisibilityMap = DActualMap->CreateVisibilityMap();
    DGold = 0;
    DLumber = 0;
    DStone = 0;
    DAssetsCreated = (int*)calloc((int)EAssetType::Max * sizeof(int), sizeof(int));
    DAssetsLost = (int*)calloc((int)EAssetType::Max * sizeof(int), sizeof(int));
    DAssetsDestroyed = (int*)calloc((int)EAssetType::Max * sizeof(int), sizeof(int));

    DUpgrades.resize(to_underlying(EAssetCapabilityType::Max));
    for(int Index = 0; Index < DUpgrades.size(); Index++){
        DUpgrades[Index] = false;
    }
    for(auto &ResourceInit : DActualMap->ResourceInitializationList()){
        if(ResourceInit.DColor == color){
            DGold = ResourceInit.DGold;
            DLumber = ResourceInit.DLumber;
            DStone = ResourceInit.DStone;
        }
    }
    for(auto &AssetInit : DActualMap->AssetInitializationList()){
        if(AssetInit.DColor == color){
            PrintDebug(DEBUG_LOW, "Init %s %d (%d, %d)\n", AssetInit.DType.c_str(), AssetInit.DColor, AssetInit.DTilePosition.X(), AssetInit.DTilePosition.Y());
            std::shared_ptr< CPlayerAsset > InitAsset = CreateAsset(AssetInit.DType);
            InitAsset->TilePosition(AssetInit.DTilePosition);
            if(EAssetType::GoldMine == CPlayerAssetType::NameToType(AssetInit.DType)){
                InitAsset->Gold(DGold);
            }
        }
    }
}

/**
*  Calculates food cost of an asset
*
*  @return int The total combined food cost of every player asset
*
*/
int CPlayerData::FoodConsumption() const{
    int TotalConsumption = 0;

    // for every player asset calculate how much food for the asset
    for(auto &WeakAsset : DAssets){
        if(auto Asset = WeakAsset.lock()){
            int AssetConsumption = Asset->FoodConsumption();

            // if Asset does consume Food add to total food consumption
            if(0 < AssetConsumption){
                TotalConsumption += AssetConsumption;
            }
        }
    }
    return TotalConsumption;
}

/**
*  Calculates food remaining after it is consumed by an asset
*
*  @return int The amount of food remaining after food is consumed by an asset
*
*/
int CPlayerData::FoodProduction() const{
    int TotalProduction = 0;

    for(auto &WeakAsset : DAssets){
        if(auto Asset = WeakAsset.lock()){
            int AssetConsumption = Asset->FoodConsumption();
            if((0 > AssetConsumption)&&((EAssetAction::Construct != Asset->Action())||(!Asset->CurrentCommand().DAssetTarget))){
                TotalProduction += -AssetConsumption;
            }
        }
    }
    return TotalProduction;
}

/**
*  Adds a new marker to the map
*
*  @param[in] pos A variable storing pixel position (reference to CPixlePosition object)
*  @param[in] addtomap A boolean used for determining whether something should be added to the map or not
*
*  @return shared pointer to CPlayerAsset which tracks markers that are added to the map
*
*/
std::shared_ptr< CPlayerAsset > CPlayerData::CreateMarker(const CPixelPosition &pos, bool addtomap){
    std::shared_ptr< CPlayerAsset > NewMarker = (*DAssetTypes)["None"]->Construct();
    CTilePosition TilePosition;

    TilePosition.SetFromPixel(pos);
    NewMarker->TilePosition(TilePosition);
    if(addtomap){
        DPlayerMap->AddAsset(NewMarker);
    }

    return NewMarker;
}

/**
*  Creates an asset and adds it to the map
*
*  @param[in] assettypename a string reference containing the name of the type of an asset
*
*  @return shared pointer to CPlayerAsset A variable storing an asset that has just been created
*
*/
std::shared_ptr< CPlayerAsset > CPlayerData::CreateAsset(const std::string &assettypename){
    std::shared_ptr< CPlayerAsset > CreatedAsset = (*DAssetTypes)[assettypename]->Construct();

    MapNewAssetObj(CreatedAsset);

    CreatedAsset->CreationCycle(DGameCycle);
    DAssets.push_back(CreatedAsset);
    DActualMap->AddAsset(CreatedAsset);
    
    ResolveNewAssetCounts();

    return CreatedAsset;
}

/**
*  Deletes an asset and removes it from the map
*
*  @param[in] asset A shared pointer to CPlayerAsset storing an asset
*
*/
void CPlayerData::DeleteAsset(std::shared_ptr< CPlayerAsset > asset){
    auto Iterator = DAssets.begin();
    while(Iterator != DAssets.end()){
        if(Iterator->lock() == asset){
            DAssets.erase(Iterator);
            break;
        }
        Iterator++;
    }
    DActualMap->RemoveAsset(asset);

    ResolveNewAssetCounts();
}

void CPlayerData::ResolveNewAssetCounts(){
    int counts[(int)EAssetType::Max];
    for (int i = 0; i < (int)EAssetType::Max; i++)
        counts[i] = PlayerAssetCount(static_cast<EAssetType>(i));
    DTriggerHandler->Resolve(ETriggerType::AssetCount, DIsAI, DColor, -1, (int)EAssetType::Max, counts);
}

void CPlayerData::IncrementCreated(std::shared_ptr< CPlayerAsset > asset){
    EAssetType assetType = asset->Type();
    DAssetsCreated[(int)assetType]++;
    DTriggerHandler->Resolve(ETriggerType::AssetsCreated, DIsAI, DColor, asset->AssetID(), (int)EAssetType::Max, DAssetsCreated);
}

void CPlayerData::IncrementLost(std::shared_ptr< CPlayerAsset > asset){
    //printf("Lost AI?%d Lost%d\n", (int)DIsAI, (int)asset->Type());
    EAssetType assetType = asset->Type();
    DAssetsLost[(int)assetType]++;
    DTriggerHandler->Resolve(ETriggerType::AssetsLost, DIsAI, DColor, asset->AssetID(), (int)EAssetType::Max, DAssetsLost);
}

void CPlayerData::IncrementDestroyed(std::shared_ptr< CPlayerAsset > asset, std::shared_ptr< CPlayerAsset > destroyer){
    //printf("Destroyed AI?%d Destroyed%d\n", (int)DIsAI, (int)asset->Type());
    EAssetType assetType = asset->Type();
    DAssetsDestroyed[(int)assetType]++;
    DTriggerHandler->Resolve(ETriggerType::AssetsDestroyed, DIsAI, DColor, destroyer->AssetID(), (int)EAssetType::Max, DAssetsDestroyed);
}

void CPlayerData::CheckAssetLocations(){
    for (auto WeakAsset : DAssets){
        if (auto Asset = WeakAsset.lock()){
            DTriggerHandler->Resolve(ETriggerType::AssetLocation, DIsAI, DColor, Asset->AssetID(), 3, new int[3]{(int)Asset->Type(), Asset->TilePositionX(), Asset->TilePositionY()});
        }
    }
}

/**
*  Checks if requirements to add an asset are met
*
*  @param[in] assettypename A string reference containing the name of the type of an asset
*
*  @return true if asset is a castle and keep is not a requirement or asset is a keep or castle and town hall is not a requirement
*
*/
bool CPlayerData::AssetRequirementsMet(const std::string &assettypename){
    std::vector< int > AssetCount;

    AssetCount.resize(to_underlying(EAssetType::Max));

    for(auto WeakAsset : DAssets){
        if(auto Asset = WeakAsset.lock()){
            if(EAssetAction::Construct != Asset->Action()){
                AssetCount[to_underlying(Asset->Type())]++;
            }
        }
    }
    for(auto Requirement : (*DAssetTypes)[assettypename]->AssetRequirements()){
        if(0 == AssetCount[to_underlying(Requirement)]){
            if((EAssetType::Keep == Requirement)&&(AssetCount[to_underlying(EAssetType::Castle)])){
                continue;
            }
            if((EAssetType::TownHall == Requirement)&&(AssetCount[to_underlying(EAssetType::Keep)]||AssetCount[to_underlying(EAssetType::Castle)])){
                continue;
            }
            return false;
        }
    }
    return true;
}

/**
*  Updates what is visible on the map
*
*  @return Nothing
*
*/
void CPlayerData::UpdateVisibility(){
    std::list< std::shared_ptr< CPlayerAsset > > RemoveList;

    DVisibilityMap->Update(DAssets);
    DPlayerMap->UpdateMap(*DVisibilityMap, *DActualMap);
    for(auto &Asset : DPlayerMap->Assets()){
        if((EAssetType::None == Asset->Type())&&(EAssetAction::None == Asset->Action())){
            Asset->IncrementStep();
            if(CPlayerAsset::UpdateFrequency() < Asset->Step() * 2){
                RemoveList.push_back(Asset);
            }
        }
    }
    for(auto &Asset : RemoveList){
        DPlayerMap->RemoveAsset(Asset);
    }

}

/**
*  Handles selecting assets. If the select area has no width or height (ie. single click), the asset at the position of the click
*  is selected. If there are assets within the select area (ie. click and drag box), those assets are selected
*
*  @param[in] selectarea A reference to SRectangle which holds the rectangular area for selecting assetes
*  @param[in] assettype The type of asset (EAssetType)
*  @param[in] selectidentical A bool determining if identical assets should be selected
*
*  @return list < weak_ptr< CPlayerAsset > >  A list of weak pointers to CPlayerAsset
*
*/
std::list< std::weak_ptr< CPlayerAsset > > CPlayerData::SelectAssets(const SRectangle &selectarea, EAssetType assettype, bool selectidentical){
    std::list< std::weak_ptr< CPlayerAsset > > ReturnList;

    if((!selectarea.DWidth)||(!selectarea.DHeight)){
        std::weak_ptr< CPlayerAsset > BestAsset = SelectAsset(CPixelPosition(selectarea.DXPosition, selectarea.DYPosition), assettype);
        if(auto LockedAsset = BestAsset.lock()){
            ReturnList.push_back(BestAsset);
            if(selectidentical && LockedAsset->Speed()){
                for(auto &WeakAsset : DAssets){
                    if(auto Asset = WeakAsset.lock()){
                        if((LockedAsset != Asset)&&(Asset->Type() == assettype)){
                            ReturnList.push_back(Asset);
                        }
                    }
                }
            }
        }
    }
    else{
        bool AnyMovable = false;
        for(auto &WeakAsset : DAssets){
            if(auto Asset = WeakAsset.lock()){
                if((selectarea.DXPosition <= Asset->PositionX())&&(Asset->PositionX() < selectarea.DXPosition + selectarea.DWidth)&&(selectarea.DYPosition <= Asset->PositionY())&&(Asset->PositionY() < selectarea.DYPosition + selectarea.DHeight)){
                    if(AnyMovable){
                        if(Asset->Speed()){
                            ReturnList.push_back(Asset);
                        }
                    }
                    else{
                        if(Asset->Speed()){
                            ReturnList.clear();
                            ReturnList.push_back(Asset);
                            AnyMovable = true;
                        }
                        else{
                            if(ReturnList.empty()){
                                ReturnList.push_back(Asset);
                            }
                        }
                    }
                }
            }
        }
    }
    return ReturnList;
}

/**
*  Computes which asset is the best to be selected
*
*  @param[in] pos The position of a pixel (reference to CPixelPosition)
*  @param[in] assettype The type of asset
*
*  @return shared pointer to CPlayerAsset to A variable holding the best asset to be selected
*
*/
std::weak_ptr< CPlayerAsset > CPlayerData::SelectAsset(const CPixelPosition &pos, EAssetType assettype){
    std::shared_ptr< CPlayerAsset > BestAsset;
    int BestDistanceSquared = -1;

    if(EAssetType::None != assettype){
        for(auto &WeakAsset : DAssets){
            if(auto Asset = WeakAsset.lock()){
                if(Asset->Type() == assettype){
                    int CurrentDistance = Asset->Position().DistanceSquared(pos);

                    if((-1 == BestDistanceSquared)||(CurrentDistance < BestDistanceSquared)){
                        BestDistanceSquared = CurrentDistance;
                        BestAsset = Asset;
                    }
                }
            }
        }
    }
    return BestAsset;
}

/**
*  Finds the nearest owned asset
*
*  @param[in] pos A variable containing the pixel position (reference to CPixelPosition)
*  @param[in] assettypes A vector of asset types 
*
*  @return shared pointer to CPlayerAsset A variable containing the nearest owned asset
*
*/
std::weak_ptr< CPlayerAsset > CPlayerData::FindNearestOwnedAsset(const CPixelPosition &pos, const std::vector< EAssetType > assettypes){
    std::shared_ptr< CPlayerAsset > BestAsset;
    int BestDistanceSquared = -1;

    for(auto &WeakAsset : DAssets){
        if(auto Asset = WeakAsset.lock()){
            for(auto &AssetType : assettypes){
                if((Asset->Type() == AssetType)&&((EAssetAction::Construct != Asset->Action())||(EAssetType::Keep == AssetType)||(EAssetType::Castle == AssetType))){
                    int CurrentDistance = Asset->Position().DistanceSquared(pos);

                    if((-1 == BestDistanceSquared)||(CurrentDistance < BestDistanceSquared)){
                        BestDistanceSquared = CurrentDistance;
                        BestAsset = Asset;
                    }
                    break;
                }
            }
        }
    }
    return BestAsset;
}

/**
*  Finds the nearest asset
*
*  @param[in] pos A reference to CPixelPosition containing the pixel position
*  @param[in] assettypes A vector of asset types
*
*  @return shared pointer to CPlayerAsset A variable holding the nearest asset
*
*/
std::shared_ptr< CPlayerAsset > CPlayerData::FindNearestAsset(const CPixelPosition &pos, EAssetType assettype){
    std::shared_ptr< CPlayerAsset > BestAsset;
    int BestDistanceSquared = -1;

    for(auto &Asset : DPlayerMap->Assets()){
        if(Asset->Type() == assettype){
            int CurrentDistance = Asset->Position().DistanceSquared(pos);

            if((-1 == BestDistanceSquared)||(CurrentDistance < BestDistanceSquared)){
                BestDistanceSquared = CurrentDistance;
                BestAsset = Asset;
            }
        }
    }
    return BestAsset;
}

/**
*  Finds the nearest enemy that is not a villager by color and by checking if it is still alive
*
*  @param[in] pos A variable containing pixel position (reference to CPixelPosition)
*  @param[in] range A variable storing tile width to be passed into the RangeToDistanceSquared function above
*
*  @return shared pointer to CPlayerAsset A variable holding the nearest enemy
*
*/
std::weak_ptr< CPlayerAsset > CPlayerData::FindNearestEnemy(const CPixelPosition &pos, int range){
    std::shared_ptr< CPlayerAsset > BestAsset;
    int BestDistanceSquared = -1;

    // Assume tile width == tile height
    if(0 < range){
        range = RangeToDistanceSquared(range);
    }
    for(auto Asset : DPlayerMap->Assets()){
        if((Asset->Color() != DColor)&&(Asset->Color() != EPlayerColor::None)&&(Asset->Alive())){
            auto Command = Asset->CurrentCommand();
            if(EAssetAction::Capability == Command.DAction){
                if((Command.DAssetTarget)&&(EAssetAction::Construct == Command.DAssetTarget->Action())){
                    continue;
                }
            }
            if((EAssetAction::ConveyGold != Command.DAction)&&(EAssetAction::ConveyLumber != Command.DAction)&&(EAssetAction::MineGold != Command.DAction)&&(EAssetAction::ConveyStone != Command.DAction)){
                int CurrentDistance = Asset->ClosestPosition(pos).DistanceSquared( pos );//Asset->Position().DistanceSquared(pos);

                if((0 > range)||(CurrentDistance <= range)){
                    if((-1 == BestDistanceSquared)||(CurrentDistance < BestDistanceSquared)){
                        BestDistanceSquared = CurrentDistance;
                        BestAsset = Asset;
                    }
                }
            }
        }
    }
    return BestAsset;
}



/**
*  Finds the nearest enemy that is not a villager by color and by checking if it is still alive
*
*  @param[in] pos A variable containing pixel position (reference to CPixelPosition)
*  @param[in] range A variable storing tile width to be passed into the RangeToDistanceSquared function above
*
*  @return shared pointer to CPlayerAsset A variable holding the nearest enemy
*
*/
int CPlayerData::FindAssetSizeOnPosition(const CTilePosition &pos){
    std::shared_ptr< CPlayerAsset > BestAsset;
    // Assume tile width == tile height
    CTilePosition tempPosition;
//    tempPosition.SetFromPixel(pos);

    for(auto Asset : DPlayerMap->Assets()){
        tempPosition.SetFromPixel(Asset->Position());
        if((tempPosition == pos)){
            BestAsset = Asset;

            int centerAssetSize;
            int centerAssetSize_Offset;
            EAssetType centerAssetType = BestAsset->Type();
            switch(centerAssetType){
                case EAssetType::TownHall:
                    centerAssetSize = 4;
                    break;
                case EAssetType::Keep:
                    centerAssetSize = 4;
                    break;
                case EAssetType::Castle:
                    centerAssetSize = 4;
                    break;
                case EAssetType::Farm:
                    centerAssetSize = 2;
                    break;
                case EAssetType::Barracks:
                    centerAssetSize = 3;
                    break;
                case EAssetType::LumberMill:
                    centerAssetSize = 3;
                    break;
                case EAssetType::Blacksmith:
                    centerAssetSize = 3;
                    break;
                case EAssetType::ScoutTower:
                    centerAssetSize = 2;
                    break;
                case EAssetType::GuardTower:
                    centerAssetSize = 2;
                    break;
                case EAssetType::CannonTower:
                    centerAssetSize = 2;
                    break;
                case EAssetType::Wall:
                    centerAssetSize = 1;
                    break;
                default:
                    centerAssetSize = 0;
                    break;
            }
            return centerAssetSize;
        }
    }
    return -1;
}






/**
*  Finds the best tile position to place an asset
*
*  @param[in] pos A variable containing tile position (reference to CTilePosition)
*  @param[in] builder A shared pointer to CPlayerAsset to make sure that builders only are concerned with asset placement in
*   this case
*  @param[in] assettype A variable storing the name of the type of an asset
*  @param[in] buffer A integer to determine PlacementSize
*
*  @return CTilePosition A variable containing a tile position(x,y)
*
*/
CTilePosition CPlayerData::FindBestAssetPlacement(const CTilePosition &pos, std::shared_ptr< CPlayerAsset > builder, EAssetType assettype, int buffer){
    auto AssetType = (*DAssetTypes)[CPlayerAssetType::TypeToName(assettype)];
    int PlacementSize = AssetType->Size() + 2 * buffer;
    int MaxDistance = std::max(DPlayerMap->Width(), DPlayerMap->Height());
    for(int Distance = 1; Distance < MaxDistance; Distance++){
        CTilePosition BestPosition;
        int BestDistance = -1;
        int LeftX = pos.X() - Distance;
        int TopY = pos.Y() - Distance;
        int RightX = pos.X() + Distance;
        int BottomY = pos.Y() + Distance;
        bool LeftValid = true;
        bool RightValid = true;
        bool TopValid = true;
        bool BottomValid = true;

        if(0 > LeftX){
            LeftValid = false;
            LeftX = 0;
        }
        if(0 > TopY){
            TopValid = false;
            TopY = 0;
        }
        if(DPlayerMap->Width() <= RightX){
            RightValid = false;
            RightX = DPlayerMap->Width() - 1;
        }
        if(DPlayerMap->Height() <= BottomY){
            BottomValid = false;
            BottomY = DPlayerMap->Height() - 1;
        }
        if(TopValid){
           for(int Index = LeftX; Index <= RightX; Index++){
                CTilePosition TempPosition(Index, TopY);
                if(DPlayerMap->CanPlaceAsset(TempPosition, PlacementSize, builder)){
                    int CurrentDistance = builder->TilePosition().DistanceSquared(TempPosition);
                    if((-1 == BestDistance)||(CurrentDistance < BestDistance)){
                        BestDistance = CurrentDistance;
                        BestPosition = TempPosition;
                    }
                }
            }
        }
        if(RightValid){
           for(int Index = TopY; Index <= BottomY; Index++){
                CTilePosition TempPosition(RightX, Index);
                if(DPlayerMap->CanPlaceAsset(TempPosition, PlacementSize, builder)){
                    int CurrentDistance = builder->TilePosition().DistanceSquared(TempPosition);
                    if((-1 == BestDistance)||(CurrentDistance < BestDistance)){
                        BestDistance = CurrentDistance;
                        BestPosition = TempPosition;
                    }
                }
            }
        }
        if(BottomValid){
           for(int Index = LeftX; Index <= RightX; Index++){
                CTilePosition TempPosition(Index, BottomY);
                if(DPlayerMap->CanPlaceAsset(TempPosition, PlacementSize, builder)){
                    int CurrentDistance = builder->TilePosition().DistanceSquared(TempPosition);
                    if((-1 == BestDistance)||(CurrentDistance < BestDistance)){
                        BestDistance = CurrentDistance;
                        BestPosition = TempPosition;
                    }
                }
            }
        }
        if(LeftValid){
           for(int Index = TopY; Index <= BottomY; Index++){
                CTilePosition TempPosition(LeftX, Index);
                if(DPlayerMap->CanPlaceAsset(TempPosition, PlacementSize, builder)){
                    int CurrentDistance = builder->TilePosition().DistanceSquared(TempPosition);
                    if((-1 == BestDistance)||(CurrentDistance < BestDistance)){
                        BestDistance = CurrentDistance;
                        BestPosition = TempPosition;
                    }
                }
            }
        }
        if(-1 != BestDistance){
            return CTilePosition(BestPosition.X() + buffer, BestPosition.Y() + buffer);
        }
    }

    return CTilePosition(-1, -1);
}

/**
*  Finds the best tile position to place an asset
*
*  @param[in] pos A variable containing tile position (reference to CTilePosition)
*  @param[in] builder A shared pointer to CPlayerAsset to make sure that builders only are concerned with asset placement in
*   this case
*  @param[in] assettype A variable storing the name of the type of an asset
*  @param[in] buffer A integer to determine buffer distance from the center
*  @param[in] padding A integer to increase extra PlacementSize
*
*  @return CTilePosition A variable containing a tile position(x,y)
*
*/
CTilePosition CPlayerData::FindBestAssetPlacementWithConstraints(const CTilePosition &Pos, std::shared_ptr< CPlayerAsset > builder, EAssetType assettype, int buffer,int padding,int H ,int V, int F){
    int FUN_DEBUG = 0;
    auto AssetType = (*DAssetTypes)[CPlayerAssetType::TypeToName(assettype)];
    int PlacementSize = AssetType->Size();
//    int PlacementSize = AssetType->Size() + 2 * buffer;
    int MaxDistance = std::max(DPlayerMap->Width(), DPlayerMap->Height());


    int centerAssetSize_Offset;
    int centerAssetSize = FindAssetSizeOnPosition(Pos);
//    if(centerAssetSize ==-1 ){
//        centerAssetSize_Offset = 0;
//    }
    switch (centerAssetSize){
        case 1:
            centerAssetSize_Offset=0;
            break;
        case 2:
            centerAssetSize_Offset=0;
            break;
        case 3:
            centerAssetSize_Offset=1;
            break;
        case 4:
            centerAssetSize_Offset=1;
            break;
        default:
            centerAssetSize_Offset=0;
            break;
    }

    if(padding >0 ){
        centerAssetSize_Offset=0;
    }


//    centerAssetSize_Offset=centerAssetSize/2;

    CTilePosition targetPosition(Pos.X()+centerAssetSize_Offset,Pos.Y()+centerAssetSize_Offset);

    CTilePosition pos(targetPosition.X(),targetPosition.Y());
    if(FUN_DEBUG)printf("\n \ncenter pos is x=%d  y=%d",targetPosition.X(),targetPosition.Y());
    if(FUN_DEBUG)printf("\nin Find Best Asset Placement: targetX=%d,targetY=%d,buffer=%d,padding=%d,H=%d,V=%d,F=%d.",pos.X(),pos.Y(),buffer,padding,H,V,F);
    for(int Distance = buffer; Distance < F+1; Distance++){

        CTilePosition BestPosition;
        bool skipMiddle=false;
        int BestDistance = -1;
        int LeftX = pos.X() - Distance;
        int RightX = pos.X() + Distance;
        int TopY = pos.Y() - Distance;
        int BottomY = pos.Y() + Distance;
        if(H==1){//l
            LeftX = pos.X() - Distance;
            RightX = pos.X();
        }else if(H==2){//m
            LeftX = pos.X();
            RightX = pos.X() + 1;
        }else if(H==4){//r
            LeftX = pos.X()+1;
            RightX = pos.X() + Distance+1;
        }else if(H==3){//lm
            LeftX = pos.X() - Distance;
            RightX = pos.X()+1;
        }else if(H==5){//lr
            LeftX = pos.X() - Distance;
            RightX = pos.X()+Distance;
            skipMiddle=true;
        }else if(H==6){//mr
            LeftX = pos.X();
            RightX = pos.X()+Distance;
        }else if(H==7){//lmr
            LeftX = pos.X() - Distance;
            RightX = pos.X()+Distance;
        }
        if(V==1){//t
            TopY = pos.Y() - Distance;
            BottomY = pos.Y();
        }else if(V==2){//m
            TopY = pos.Y();
            BottomY = pos.Y() + 1;
        }else if(V==4){//b
            TopY = pos.Y()+1;
            BottomY = pos.Y() + Distance+1;
        }else if(V==3){//tm
            TopY = pos.Y() - Distance;
            BottomY = pos.Y()+1;
        }else if(V==5){//tb
            TopY = pos.Y() - Distance;
            BottomY = pos.Y()+Distance;
            skipMiddle=true;
        }else if(V==6){//mb
            TopY = pos.Y();
            BottomY = pos.Y()+Distance;
        }else if(V==7){//tmb
            TopY = pos.Y() - Distance;
            BottomY = pos.Y()+Distance;
        }


        bool LeftValid = true;
        bool RightValid = true;
        bool TopValid = true;
        bool BottomValid = true;

        if(0 > LeftX){
            LeftValid = false;
            LeftX = 0;
        }
        if(0 > TopY){
            TopValid = false;
            TopY = 0;
        }
        if(DPlayerMap->Width() <= RightX){
            RightValid = false;
            RightX = DPlayerMap->Width() - 1;
        }
        if(DPlayerMap->Height() <= BottomY){
            BottomValid = false;
            BottomY = DPlayerMap->Height() - 1;
        }
        if(FUN_DEBUG)printf("\nl=%d,r=%d,t=%d,b=%d",LeftX,RightX,TopY,BottomY);
        if(TopValid){
            for(int Index = LeftX; Index <= RightX; Index++){
                CTilePosition TempPosition(Index, TopY);
                if(DPlayerMap->CanPlaceAsset(TempPosition, PlacementSize, builder)){
                    //int CurrentDistance = builder->TilePosition().DistanceSquared(TempPosition);
                    if(FUN_DEBUG)printf("\ncan build at:x%d  y:%d ",Index, TopY);
                    int CurrentDistance = targetPosition.DistanceSquared(TempPosition);

                    if((-1 == BestDistance)||(CurrentDistance < BestDistance)){
                        BestDistance = CurrentDistance;
                        BestPosition = TempPosition;
                        if(FUN_DEBUG)printf("\nnew best position at:x%d  y:%d ",Index, TopY);
                    }
                    if(FUN_DEBUG)printf("\nDistance:tempd%d  bestd:%d ",CurrentDistance, BestDistance);
                }
                if(FUN_DEBUG)printf("\nIn Top: PlacementCandidate: x=%d,y=%d,bestd=%d",TempPosition.X(),TempPosition.Y(),BestDistance);

            }
        }
        if(RightValid){
            for(int Index = TopY; Index <= BottomY; Index++){
                CTilePosition TempPosition(RightX, Index);
                if(DPlayerMap->CanPlaceAsset(TempPosition, PlacementSize, builder)){
                    if(FUN_DEBUG)printf("\ncan build at:x%d  y:%d ",RightX, Index);
                    int CurrentDistance = targetPosition.DistanceSquared(TempPosition);
                    if((-1 == BestDistance)||(CurrentDistance < BestDistance)){
                        BestDistance = CurrentDistance;
                        BestPosition = TempPosition;
                        if(FUN_DEBUG)printf("\nnew best position at:x%d  y:%d ",RightX, Index);
                    }
                    if(FUN_DEBUG)printf("\nDistance:tempd%d  bestd:%d ",CurrentDistance, BestDistance);
                }
                if(FUN_DEBUG)printf("\nIn Right: PlacementCandidate: x=%d,y=%d,bestd=%d",TempPosition.X(),TempPosition.Y(),BestDistance);
            }
        }
        if(BottomValid){
            for(int Index = LeftX; Index <= RightX; Index++){
                CTilePosition TempPosition(Index, BottomY);
                if(DPlayerMap->CanPlaceAsset(TempPosition, PlacementSize, builder)){
                    if(FUN_DEBUG)printf("\ncan build at:x%d  y:%d ",Index, BottomY);
                    int CurrentDistance = targetPosition.DistanceSquared(TempPosition);
                    if((-1 == BestDistance)||(CurrentDistance < BestDistance)){
                        BestDistance = CurrentDistance;
                        BestPosition = TempPosition;
                        if(FUN_DEBUG)printf("\nnew best position at:x%d  y:%d ",Index, BottomY);
                    }
                    if(FUN_DEBUG)printf("\nDistance:tempd%d  bestd:%d ",CurrentDistance, BestDistance);
                }
                if(FUN_DEBUG)printf("\nIn Bottom: PlacementCandidate: x=%d,y=%d,bestd=%d",TempPosition.X(),TempPosition.Y(),BestDistance);
            }
        }
        if(LeftValid){
            for(int Index = TopY; Index <= BottomY; Index++){
                CTilePosition TempPosition(LeftX, Index);
                if(DPlayerMap->CanPlaceAsset(TempPosition, PlacementSize, builder)){
                    if(FUN_DEBUG)printf("\ncan build at:x%d  y:%d ",LeftX, Index);
                    int CurrentDistance = targetPosition.DistanceSquared(TempPosition);
                    if((-1 == BestDistance)||(CurrentDistance < BestDistance)){
                        BestDistance = CurrentDistance;
                        BestPosition = TempPosition;
                        if(FUN_DEBUG)printf("\nnew best position at:x%d  y:%d ",LeftX, Index);
                    }
                    if(FUN_DEBUG)printf("\nDistance:tempd%d  bestd:%d ",CurrentDistance, BestDistance);
                }
                if(FUN_DEBUG)printf("\nIn Left: PlacementCandidate: x=%d,y=%d,bestd=%d",TempPosition.X(),TempPosition.Y(),BestDistance);
            }
        }
        if(-1 != BestDistance){
//            return CTilePosition(BestPosition.X() + buffer, BestPosition.Y() + buffer);
            return CTilePosition(BestPosition.X(), BestPosition.Y() );
        }
    }

    return CTilePosition(-1, -1);
}


/**
*  Calculates the number of assets of a specific type a player has
*
*  @param[in] type The type of an asset
*
*  @return int An integer holding the number of assets of a specific type that a player has
*
*/
int CPlayerData::PlayerAssetCount(EAssetType type){
    int Count = 0;

    for(auto Asset : DPlayerMap->Assets()){
        if((Asset->Color() == DColor)&&(type == Asset->Type())){
            Count++;
        }
    }

    return Count;
}

/**
*  Calculates the number of assets of a certain type on the map
*
*  @param[in] type The type of an asset
*
*  @return int An integer storing the number of a certain type on the map
*
*/
int CPlayerData::FoundAssetCount(EAssetType type){
    int Count = 0;

    for(auto Asset : DPlayerMap->Assets()){
        if(type == Asset->Type()){
            Count++;
        }
    }

    return Count;
}

/**
*  Creates and populates a list of all a player's idle assets (assets not performing any action)
*
*  @return list< weak_ptr< CPlayerAsset > >  A list of weak pointer to player assets
*
*/
std::list< std::weak_ptr< CPlayerAsset > > CPlayerData::IdleAssets() const{
    std::list< std::weak_ptr< CPlayerAsset > > AssetList;

    for(auto WeakAsset : DAssets){
        if(auto Asset = WeakAsset.lock()){
            if((EAssetAction::None == Asset->Action())&&(EAssetType::None != Asset->Type())){
                AssetList.push_back(Asset);
            }
        }
    }

    return AssetList;
}

/**
*  Upgrades assets
*
*  @param[in] upgradename A string reference storing the name of an upgrade
*
*/
void CPlayerData::AddUpgrade(const std::string &upgradename){
    auto Upgrade = CPlayerUpgrade::FindUpgradeFromName(upgradename);

    if(Upgrade){
        for(auto AssetType : Upgrade->AffectedAssets()){
            std::string AssetName = CPlayerAssetType::TypeToName(AssetType);
            auto AssetIterator = DAssetTypes->find(AssetName);

            if(AssetIterator != DAssetTypes->end()){
                AssetIterator->second->AddUpgrade(Upgrade);
            }

        }
        DUpgrades[to_underlying(CPlayerCapability::NameToType(upgradename))] = true;
    }
}

/**
*  Upgrades assets
*
*  @param[in] upgradename A string reference storing the name of an upgrade
*
*/
void CPlayerData::RemoveUpgrade(const std::string &upgradename){
    auto Upgrade = CPlayerUpgrade::FindUpgradeFromName(upgradename);

    if(Upgrade){
        for(auto AssetType : Upgrade->AffectedAssets()){
            std::string AssetName = CPlayerAssetType::TypeToName(AssetType);
            auto AssetIterator = DAssetTypes->find(AssetName);

            if(AssetIterator != DAssetTypes->end()){
                AssetIterator->second->RemoveUpgrade(upgradename);
            }

        }
        DUpgrades[to_underlying(CPlayerCapability::NameToType(upgradename))] = true;
    }}

/**
*  Sets harvest mine, convey, death, and corpse decay times and steps. Also sets gold and lumber capacities and resizes occupancy maps
*
*  @param[in] mapindex A variable storing the index of a map
*  @param[in] seed A uint64_t seed used to generate random numbers
*  @param[in] newcolors An array containing player colors and the maximum
*   number of them
*
*/
CGameModel::CGameModel(int mapindex, uint64_t seed, const std::array< EPlayerColor, to_underlying(EPlayerColor::Max)> &newcolors){
    DHarvestTime = 5;
    DHarvestSteps = CPlayerAsset::UpdateFrequency() * DHarvestTime;
    DMineTime = 5;
    DMineSteps = CPlayerAsset::UpdateFrequency() * DMineTime;
    DQuarryTime = 5;
    DQuarrySteps = CPlayerAsset::UpdateFrequency() * DQuarryTime;
    DConveyTime = 1;
    DConveySteps = CPlayerAsset::UpdateFrequency() * DConveyTime;
    DDeathTime = 1;
    DDeathSteps = CPlayerAsset::UpdateFrequency() * DDeathTime;
    DDecayTime = 4;
    DDecaySteps = CPlayerAsset::UpdateFrequency() * DDecayTime;
    DLumberPerHarvest = 100;
    DGoldPerMining = 100;
    DStonePerQuarry = 100;

    DRandomNumberGenerator.Seed(seed);
    DActualMap = CAssetDecoratedMap::DuplicateMap(mapindex, newcolors);
    DTriggerHandler = CTriggerHandler::DuplicateHandler(mapindex);
    DTriggerHandler->ActivateTriggers();

    for(int PlayerIndex = 0; PlayerIndex < to_underlying(EPlayerColor::Max); PlayerIndex++){
        DPlayers[PlayerIndex] = std::make_shared< CPlayerData > (DActualMap, DTriggerHandler, static_cast<EPlayerColor>(PlayerIndex));
    }
    DAssetOccupancyMap.resize(DActualMap->Height());
    for(auto &Row : DAssetOccupancyMap){
        Row.resize(DActualMap->Width());
    }
    DDiagonalOccupancyMap.resize(DActualMap->Height());
    for(auto &Row : DDiagonalOccupancyMap){
        Row.resize(DActualMap->Width());
    }
}

/**
*  Checks to see if an asset is on the map
*
*  @param[in] asset A shared pointer to CPlayerAsset
*
*  @return true if the asset is on the map
*
*/
bool CGameModel::ValidAsset(std::shared_ptr< CPlayerAsset > asset){
    for(auto &Asset : DActualMap->Assets()){
        if(asset == Asset){
            return true;
        }
    }
    return false;
}

/**
*  Checks to see if player color is in range. If it has a higher value than the max of EPlayerColor, then return nullptr
*
*  @param[in] color A variable holding the color of a player
*
*  @return DPlayers if player color in range
*
*/
std::shared_ptr< CPlayerData > CGameModel::Player(EPlayerColor color) const{
    if((0 > to_underlying(color))||(EPlayerColor::Max <= color)){
        return nullptr;
    }
    return DPlayers[to_underlying(color)];
}

/**
* Compares two elements of Asset list for sort function 
*
* @param[in] a First element
* @param[in] b Second element
*
* @return true if first element greater than second element
*/

bool CompareTurnOrder(std::shared_ptr< CPlayerAsset > a, std::shared_ptr< CPlayerAsset > b){
    return a->GetTurnOrder() > b->GetTurnOrder();
}

/**
*  Handles what occurs each timestep and each of the following. Creates vector of current events. Places assets on occupancy map if not
*  mining or conveying gold or lumber. Updates visibility for all players that are alive. Assigns assets  a pseudo-random turn order and
*  sorts them by immobile or mobile. Peasants can find new lumber on their own and drop it off at the nearest repository and find a next
*  mine or give up. Soldiers will attack the nearest enemy on sight. Peasants can repair. Deaths occur and corpses appear. Travel
*
*  @return Nothing
*
*/
void CGameModel::Timestep(){
    std::vector< SGameEvent > CurrentEvents;
    SGameEvent TempEvent;

    for(auto &Row : DAssetOccupancyMap){
        for(auto &Cell : Row){
            Cell = nullptr;
        }
    }
    for(auto &Row : DDiagonalOccupancyMap){
        for(int Index = 0; Index < Row.size(); Index++){
            Row[Index] = false;
        }
    }

    //places asset on occupancy map if not mining or conveying gold or lumber
    for(auto &Asset : DActualMap->Assets()){
        if((EAssetAction::ConveyGold != Asset->Action())&&(EAssetAction::ConveyLumber != Asset->Action())&&(EAssetAction::MineGold != Asset->Action())&&(EAssetAction::ConveyStone != Asset->Action())){
            DAssetOccupancyMap[Asset->TilePositionY()][Asset->TilePositionX()] = Asset;
        }
    }

    //updates visibility for all players that are alive
    for(int PlayerIndex = 1; PlayerIndex < to_underlying(EPlayerColor::Max); PlayerIndex++){
        if(DPlayers[PlayerIndex]->IsAlive()){
            DPlayers[PlayerIndex]->UpdateVisibility();
            DPlayers[PlayerIndex]->CheckAssetLocations();
        }
    }

    auto AllAssets = DActualMap->Assets();
    std::list< std::shared_ptr< CPlayerAsset > > MobileAssets;
    std::list< std::shared_ptr< CPlayerAsset > > ImmobileAssets;

    // assign each asset a pseudo-random turn order
    for(auto &Asset : AllAssets){
        Asset->AssignTurnOrder();
        if(Asset->Speed()){
            MobileAssets.push_back(Asset);
        }
        else{
            ImmobileAssets.push_back(Asset);
        }
    }

    // sort turn order by mobile and immobile
    MobileAssets.sort(CompareTurnOrder);
    ImmobileAssets.sort(CompareTurnOrder);
    AllAssets = MobileAssets;
    AllAssets.splice(AllAssets.end(),ImmobileAssets);

    for(auto &Asset : AllAssets){
        // show that assets are ordered and sorted in Debug.out
        //PrintDebug(DEBUG_LOW, "%u\n", Asset->GetTurnOrder());
        //if(Asset->Speed()){
        //    PrintDebug(DEBUG_LOW, "asset is mobile\n");
        //}
        //else{
        //    PrintDebug(DEBUG_LOW, "asset is immobile\n");
        //}

        if(EAssetAction::None == Asset->Action()){
            Asset->PopCommand();
        }

        if(EAssetAction::Capability == Asset->Action()){
            SAssetCommand Command = Asset->CurrentCommand();
            if(Command.DActivatedCapability){
                if(Command.DActivatedCapability->IncrementStep()){
                    // All Done
                }
            }
            else{
                auto PlayerCapability = CPlayerCapability::FindCapability(Command.DCapability);

                Asset->PopCommand();
                if(PlayerCapability->CanApply(Asset, DPlayers[to_underlying(Asset->Color())], Command.DAssetTarget)){
                    PlayerCapability->ApplyCapability(Asset, DPlayers[to_underlying(Asset->Color())], Command.DAssetTarget);
                }
                else{
                    // Can't apply notify problem
                }
            }
        }
        else if(EAssetAction::HarvestLumber == Asset->Action()){
            SAssetCommand Command = Asset->CurrentCommand();
            CTilePosition TilePosition = Command.DAssetTarget->TilePosition();
            EDirection HarvestDirection = Asset->TilePosition().AdjacentTileDirection(TilePosition);

            if(CTerrainMap::ETileType::Forest != DActualMap->TileType(TilePosition)){
                HarvestDirection = EDirection::Max;
                TilePosition = Asset->TilePosition();
            }
            if(EDirection::Max == HarvestDirection){
                if(TilePosition == Asset->TilePosition()){
                    CTilePosition TilePosition = DPlayers[to_underlying(Asset->Color())]->PlayerMap()->FindNearestReachableTileType(Asset->TilePosition(), CTerrainMap::ETileType::Forest);
                    // Find new lumber
                    Asset->PopCommand();
                    if(0 <= TilePosition.X()){
                        CPixelPosition NewPosition;
                        NewPosition.SetFromTile(TilePosition);
                        Command.DAssetTarget = DPlayers[to_underlying(Asset->Color())]->CreateMarker(NewPosition, false);
                        Asset->PushCommand(Command);
                        Command.DAction = EAssetAction::Walk;
                        Asset->PushCommand(Command);
                        Asset->ResetStep();
                    }
                }
                else{
                    SAssetCommand NewCommand = Command;

                    NewCommand.DAction = EAssetAction::Walk;
                    Asset->PushCommand(NewCommand);
                    Asset->ResetStep();
                }
            }
            else{
                TempEvent.DType = EEventType::Harvest;
                TempEvent.DAsset = Asset;
                CurrentEvents.push_back(TempEvent);
                Asset->Direction(HarvestDirection);
                Asset->IncrementStep();
                if(DHarvestSteps <= Asset->Step()){
                    std::weak_ptr< CPlayerAsset > NearestRepository = DPlayers[to_underlying(Asset->Color())]->FindNearestOwnedAsset(Asset->Position(), {EAssetType::TownHall, EAssetType::Keep, EAssetType::Castle, EAssetType::LumberMill});

                    DActualMap->RemoveLumber(TilePosition, Asset->TilePosition(), DLumberPerHarvest);

                    if(!NearestRepository.expired()){
                        Command.DAction = EAssetAction::ConveyLumber;
                        Command.DAssetTarget = NearestRepository.lock();
                        Asset->PushCommand(Command);
                        Command.DAction = EAssetAction::Walk;
                        Asset->PushCommand(Command);
                        Asset->Lumber(DLumberPerHarvest);
                        Asset->ResetStep();
                    }
                    else{
                        Asset->PopCommand();
                        Asset->Lumber(DLumberPerHarvest);
                        Asset->ResetStep();
                    }
                }
            }
        }

        // added stone code
        else if(EAssetAction::QuarryStone == Asset->Action()){
            SAssetCommand Command = Asset->CurrentCommand();
            CTilePosition TilePosition = Command.DAssetTarget->TilePosition();
            EDirection QuarryDirection = Asset->TilePosition().AdjacentTileDirection(TilePosition);

            if(CTerrainMap::ETileType::Rock != DActualMap->TileType(TilePosition)){
                QuarryDirection = EDirection::Max;
                TilePosition = Asset->TilePosition();
            }
            if(EDirection::Max == QuarryDirection){
                if(TilePosition == Asset->TilePosition()){
                    CTilePosition TilePosition = DPlayers[to_underlying(Asset->Color())]->PlayerMap()->FindNearestReachableTileType(Asset->TilePosition(), CTerrainMap::ETileType::Rock);
                    // Find new stone
                    Asset->PopCommand();
                    if(0 <= TilePosition.X()){
                        CPixelPosition NewPosition;
                        NewPosition.SetFromTile(TilePosition);
                        Command.DAssetTarget = DPlayers[to_underlying(Asset->Color())]->CreateMarker(NewPosition, false);
                        Asset->PushCommand(Command);
                        Command.DAction = EAssetAction::Walk;
                        Asset->PushCommand(Command);
                        Asset->ResetStep();
                    }
                }
                else{
                    SAssetCommand NewCommand = Command;

                    NewCommand.DAction = EAssetAction::Walk;
                    Asset->PushCommand(NewCommand);
                    Asset->ResetStep();
                }
            }
            else{
                TempEvent.DType = EEventType::Quarry;
                TempEvent.DAsset = Asset;
                CurrentEvents.push_back(TempEvent);
                Asset->Direction(QuarryDirection);
                Asset->IncrementStep();
                if(DQuarrySteps <= Asset->Step()){
                    std::weak_ptr< CPlayerAsset > NearestRepository = DPlayers[to_underlying(Asset->Color())]->FindNearestOwnedAsset(Asset->Position(), {EAssetType::TownHall, EAssetType::Keep, EAssetType::Castle});
             
                    DActualMap->RemoveStone(TilePosition, Asset->TilePosition(), DStonePerQuarry);

                    if(!NearestRepository.expired()){
                        Command.DAction = EAssetAction::ConveyStone;
                        Command.DAssetTarget = NearestRepository.lock();
                        Asset->PushCommand(Command);
                        Command.DAction = EAssetAction::Walk;
                        Asset->PushCommand(Command);
                        Asset->Stone(DStonePerQuarry);
                        Asset->ResetStep();
                    }
                    else{
                        Asset->PopCommand();
                        Asset->Stone(DStonePerQuarry);
                        Asset->ResetStep();
                    }
                }
            }
        }
        // end of added stone code
        else if(EAssetAction::MineGold == Asset->Action()){
            SAssetCommand Command = Asset->CurrentCommand();
            CPixelPosition ClosestPosition = Command.DAssetTarget->ClosestPosition(Asset->Position());
            CTilePosition TilePosition;
            EDirection MineDirection;

            TilePosition.SetFromPixel(ClosestPosition);
            MineDirection = Asset->TilePosition().AdjacentTileDirection(TilePosition);
            if((EDirection::Max == MineDirection)&&(TilePosition != Asset->TilePosition())){
                SAssetCommand NewCommand = Command;

                NewCommand.DAction = EAssetAction::Walk;
                Asset->PushCommand(NewCommand);
                Asset->ResetStep();
            }
            else{
                if(0 == Asset->Step()){
                    if((Command.DAssetTarget->CommandCount() + 1) * DGoldPerMining <=  Command.DAssetTarget->Gold()){
                        SAssetCommand NewCommand;
                        NewCommand.DAction = EAssetAction::Build; // Pretend active
                        NewCommand.DAssetTarget = Asset;

                        Command.DAssetTarget->EnqueueCommand(NewCommand);
                        Asset->IncrementStep();
                        Asset->TilePosition(Command.DAssetTarget->TilePosition());
                    }
                    else{
                        // Look for new mine or give up?
                        Asset->PopCommand();
                    }
                }
                else{
                    Asset->IncrementStep();
                    if(DMineSteps <= Asset->Step()){
                        std::shared_ptr< CPlayerAsset > OldTarget = Command.DAssetTarget;
                        std::weak_ptr< CPlayerAsset > NearestRepository = DPlayers[to_underlying(Asset->Color())]->FindNearestOwnedAsset(Asset->Position(), {EAssetType::TownHall, EAssetType::Keep, EAssetType::Castle});
                        CTilePosition NextTarget(DPlayers[to_underlying(Asset->Color())]->PlayerMap()->Width()-1,DPlayers[to_underlying(Asset->Color())]->PlayerMap()->Height()-1);

                        Command.DAssetTarget->DecrementGold(DGoldPerMining);
                        Command.DAssetTarget->PopCommand();
                        if(0 >= Command.DAssetTarget->Gold()){
                            SAssetCommand NewCommand;

                            NewCommand.DAction = EAssetAction::Death;
                            Command.DAssetTarget->ClearCommand();
                            Command.DAssetTarget->PushCommand(NewCommand);
                            Command.DAssetTarget->ResetStep();
                        }
                        Asset->Gold(DGoldPerMining);
                        if(!NearestRepository.expired()){
                            Command.DAction = EAssetAction::ConveyGold;
                            Command.DAssetTarget = NearestRepository.lock();
                            Asset->PushCommand(Command);
                            Command.DAction = EAssetAction::Walk;
                            Asset->PushCommand(Command);
                            Asset->ResetStep();
                            NextTarget = Command.DAssetTarget->TilePosition();
                        }
                        else{
                            Asset->PopCommand();
                        }
                        Asset->TilePosition(DPlayers[to_underlying(Asset->Color())]->PlayerMap()->FindAssetPlacement(Asset, OldTarget, NextTarget));
                    }
                }
            }
        }
        else if(EAssetAction::StandGround == Asset->Action()){
            SAssetCommand Command = Asset->CurrentCommand();
            auto NewTarget = DPlayers[to_underlying(Asset->Color())]->FindNearestEnemy(Asset->Position(), Asset->EffectiveRange());

            if(NewTarget.expired()){
                Command.DAction = EAssetAction::None;
            }
            else{
                Command.DAction = EAssetAction::Attack;
                Command.DAssetTarget = NewTarget.lock();
            }
            Asset->PushCommand(Command);
            Asset->ResetStep();
        }
        else if(EAssetAction::Repair == Asset->Action()){
            SAssetCommand CurrentCommand = Asset->CurrentCommand();
            if(CurrentCommand.DAssetTarget->Alive()){
                EDirection RepairDirection = Asset->TilePosition().AdjacentTileDirection(CurrentCommand.DAssetTarget->TilePosition(), CurrentCommand.DAssetTarget->Size());
                if(EDirection::Max == RepairDirection){
                    SAssetCommand NextCommand = Asset->NextCommand();

                    CurrentCommand.DAction = EAssetAction::Walk;
                    Asset->PushCommand(CurrentCommand);
                    Asset->ResetStep();
                }
                else{
                    Asset->Direction(RepairDirection);
                    Asset->IncrementStep();
                    // Assume same movement as attack
                    if(Asset->Step() == Asset->AttackSteps()){
                        if(DPlayers[to_underlying(Asset->Color())]->Gold() && DPlayers[to_underlying(Asset->Color())]->Lumber() && DPlayers[to_underlying(Asset->Color())]->Stone()){
                            int RepairPoints = (CurrentCommand.DAssetTarget->MaxHitPoints() * (Asset->AttackSteps() + Asset->ReloadSteps())) / (CPlayerAsset::UpdateFrequency() * CurrentCommand.DAssetTarget->BuildTime());

                            if(0 == RepairPoints){
                                RepairPoints = 1;
                            }
                            DPlayers[to_underlying(Asset->Color())]->DecrementGold(1);
                            DPlayers[to_underlying(Asset->Color())]->DecrementLumber(1);
                            DPlayers[to_underlying(Asset->Color())]->DecrementStone(1);
                            CurrentCommand.DAssetTarget->IncrementHitPoints(RepairPoints);
                            if(CurrentCommand.DAssetTarget->HitPoints() == CurrentCommand.DAssetTarget->MaxHitPoints()){
                                TempEvent.DType = EEventType::WorkComplete;
                                TempEvent.DAsset = Asset;
                                DPlayers[to_underlying(Asset->Color())]->AddGameEvent(TempEvent);
                                Asset->PopCommand();
                            }
                        }
                        else{
                            // Stop repair
                            Asset->PopCommand();
                        }
                    }
                    if(Asset->Step() >= (Asset->AttackSteps() + Asset->ReloadSteps())){
                       Asset->ResetStep();
                    }
                }
            }
            else{
                Asset->PopCommand();
            }
        }
        else if(EAssetAction::Attack == Asset->Action()){
            SAssetCommand CurrentCommand = Asset->CurrentCommand();
            if(EAssetType::None == Asset->Type()){
                CPixelPosition ClosestTargetPosition = CurrentCommand.DAssetTarget->ClosestPosition(Asset->Position());
                CPixelPosition DeltaPosition(ClosestTargetPosition.X() - Asset->PositionX(), ClosestTargetPosition.Y() - Asset->PositionY());
                int Movement = CPosition::TileWidth() * 5 / CPlayerAsset::UpdateFrequency();
                int TargetDistance = Asset->Position().Distance(ClosestTargetPosition);
                int Divisor = (TargetDistance + Movement - 1)/ Movement;

                if(Divisor){
                    DeltaPosition.X( DeltaPosition.X() / Divisor);
                    DeltaPosition.Y( DeltaPosition.Y() / Divisor);
                }
                printf("hi from asset %d\n", Asset->AssetID());
                Asset->PositionX(Asset->PositionX() + DeltaPosition.X());
                Asset->PositionY(Asset->PositionY() + DeltaPosition.Y());
                Asset->Direction(Asset->Position().DirectionTo(ClosestTargetPosition));
                if(CPosition::HalfTileWidth() * CPosition::HalfTileHeight() > Asset->Position().DistanceSquared(ClosestTargetPosition)){
                    TempEvent.DType = EEventType::MissleHit;
                    TempEvent.DAsset = Asset;
                    CurrentEvents.push_back(TempEvent);

                    if(CurrentCommand.DAssetTarget->Alive()){
                        SAssetCommand TargetCommand = CurrentCommand.DAssetTarget->CurrentCommand();
                        TempEvent.DType = EEventType::Attacked;
                        TempEvent.DAsset = CurrentCommand.DAssetTarget;
                        DPlayers[to_underlying(CurrentCommand.DAssetTarget->Color())]->AddGameEvent(TempEvent);
                        if(EAssetAction::MineGold != TargetCommand.DAction){

                            if((EAssetAction::ConveyGold == TargetCommand.DAction)||(EAssetAction::ConveyLumber == TargetCommand.DAction)||(EAssetAction::ConveyStone == TargetCommand.DAction)){
                                // Damage the target
                                CurrentCommand.DAssetTarget = TargetCommand.DAssetTarget;
                            }
                            else if((EAssetAction::Capability == TargetCommand.DAction) && TargetCommand.DAssetTarget){
                                if(CurrentCommand.DAssetTarget->Speed() && (EAssetAction::Construct == TargetCommand.DAssetTarget->Action())){
                                    CurrentCommand.DAssetTarget = TargetCommand.DAssetTarget;
                                }
                            }
                            CurrentCommand.DAssetTarget->DecrementHitPoints(Asset->HitPoints());
                            if(!CurrentCommand.DAssetTarget->Alive()){
                                SAssetCommand Command = CurrentCommand.DAssetTarget->CurrentCommand();

                                TempEvent.DType = EEventType::Death;
                                TempEvent.DAsset = CurrentCommand.DAssetTarget;
                                CurrentEvents.push_back(TempEvent);
                                // Remove constructing
                                if((EAssetAction::Capability == Command.DAction)&&(Command.DAssetTarget)){
                                    if(EAssetAction::Construct == Command.DAssetTarget->Action()){
                                        DPlayers[to_underlying(Command.DAssetTarget->Color())]->DeleteAsset(Command.DAssetTarget);
                                    }
                                }
                                else if(EAssetAction::Construct == Command.DAction){
                                    if(Command.DAssetTarget){
                                        Command.DAssetTarget->ClearCommand();
                                    }
                                }
                                CurrentCommand.DAssetTarget->Direction(DirectionOpposite(Asset->Direction()));
                                Command.DAction = EAssetAction::Death;
                                CurrentCommand.DAssetTarget->ClearCommand();
                                CurrentCommand.DAssetTarget->PushCommand(Command);
                                CurrentCommand.DAssetTarget->ResetStep();
                            }
                        }
                    }
                    DPlayers[to_underlying(Asset->Color())]->DeleteAsset(Asset);
                }
            }
            else if(CurrentCommand.DAssetTarget->Alive()){
                if(1 == Asset->EffectiveRange()){
                    EDirection AttackDirection = Asset->TilePosition().AdjacentTileDirection(CurrentCommand.DAssetTarget->TilePosition(), CurrentCommand.DAssetTarget->Size());
                    if(EDirection::Max == AttackDirection){
                        SAssetCommand NextCommand = Asset->NextCommand();

                        if(EAssetAction::StandGround != NextCommand.DAction){
                            CurrentCommand.DAction = EAssetAction::Walk;
                            Asset->PushCommand(CurrentCommand);
                            Asset->ResetStep();
                        }
                        else{
                            Asset->PopCommand();
                        }
                    }
                    else{
                        Asset->Direction(AttackDirection);
                        Asset->IncrementStep();
                        if(Asset->Step() == Asset->AttackSteps()){
                            int Damage = Asset->EffectiveBasicDamage() - CurrentCommand.DAssetTarget->EffectiveArmor();
                            Damage = 0 > Damage ? 0 : Damage;
                            Damage += Asset->EffectivePiercingDamage();
                            if(DRandomNumberGenerator.Random() & 0x1){ // 50% chance half damage
                                Damage /= 2;
                            }
                            CurrentCommand.DAssetTarget->DecrementHitPoints(Damage);
                            TempEvent.DType = EEventType::MeleeHit;
                            TempEvent.DAsset = Asset;
                            CurrentEvents.push_back(TempEvent);
                            TempEvent.DType = EEventType::Attacked;
                            TempEvent.DAsset = CurrentCommand.DAssetTarget;
                            DPlayers[to_underlying(CurrentCommand.DAssetTarget->Color())]->AddGameEvent(TempEvent);
                            if(!CurrentCommand.DAssetTarget->Alive()){
                                DPlayers[to_underlying(Asset->Color())]->IncrementDestroyed(CurrentCommand.DAssetTarget, Asset);
                                DPlayers[to_underlying(CurrentCommand.DAssetTarget->Color())]->IncrementLost(CurrentCommand.DAssetTarget);
                                SAssetCommand Command = CurrentCommand.DAssetTarget->CurrentCommand();

                                TempEvent.DType = EEventType::Death;
                                TempEvent.DAsset = CurrentCommand.DAssetTarget;
                                CurrentEvents.push_back(TempEvent);
                                // Remove constructing
                                if((EAssetAction::Capability == Command.DAction)&&(Command.DAssetTarget)){
                                    if(EAssetAction::Construct == Command.DAssetTarget->Action()){
                                        DPlayers[to_underlying(Command.DAssetTarget->Color())]->DeleteAsset(Command.DAssetTarget);
                                    }
                                }
                                else if(EAssetAction::Construct == Command.DAction){
                                    if(Command.DAssetTarget){
                                        Command.DAssetTarget->ClearCommand();
                                    }
                                }
                                Command.DCapability = EAssetCapabilityType::None;
                                Command.DAssetTarget = nullptr;
                                Command.DActivatedCapability = nullptr;
                                CurrentCommand.DAssetTarget->Direction(DirectionOpposite(AttackDirection));
                                Command.DAction = EAssetAction::Death;
                                CurrentCommand.DAssetTarget->ClearCommand();
                                CurrentCommand.DAssetTarget->PushCommand(Command);
                                CurrentCommand.DAssetTarget->ResetStep();
                            }
                        }
                        if(Asset->Step() >= (Asset->AttackSteps() + Asset->ReloadSteps())){
                           Asset->ResetStep();
                        }
                    }
                }
                else{ // EffectiveRanged
                    CPixelPosition ClosestTargetPosition = CurrentCommand.DAssetTarget->ClosestPosition(Asset->Position());
                    if(ClosestTargetPosition.DistanceSquared( Asset->Position() ) > RangeToDistanceSquared(Asset->EffectiveRange())){
                        SAssetCommand NextCommand = Asset->NextCommand();

                        if(EAssetAction::StandGround != NextCommand.DAction){
                            CurrentCommand.DAction = EAssetAction::Walk;
                            Asset->PushCommand(CurrentCommand);
                            Asset->ResetStep();
                        }
                        else{
                            Asset->PopCommand();
                        }
                    }
                    else{
                        /*
                        CPosition DeltaPosition(ClosestTargetPosition.X() - Asset->PositionX(), ClosestTargetPosition.Y() - Asset->PositionY());
                        int DivX = DeltaPosition.X() / CPosition::HalfTileWidth();
                        int DivY = DeltaPosition.Y() / CPosition::HalfTileHeight();
                        int Div;
                        EDirection AttackDirection;
                        DivX = 0 > DivX ? -DivX : DivX;
                        DivY = 0 > DivY ? -DivY : DivY;
                        Div = DivX > DivY ? DivX : DivY;

                        if(Div){
                            DeltaPosition.X(DeltaPosition.X() / Div);
                            DeltaPosition.Y(DeltaPosition.Y() / Div);
                        }
                        DeltaPosition.IncrementX(CPosition::HalfTileWidth());
                        DeltaPosition.IncrementY(CPosition::HalfTileHeight());
                        if(0 > DeltaPosition.X()){
                            DeltaPosition.X(0);
                        }
                        if(0 > DeltaPosition.Y()){
                            DeltaPosition.Y(0);
                        }
                        if(CPosition::TileWidth() <= DeltaPosition.X()){
                            DeltaPosition.X(CPosition::TileWidth() - 1);
                        }
                        if(CPosition::TileHeight() <= DeltaPosition.Y()){
                            DeltaPosition.Y(CPosition::TileHeight() - 1);
                        }
                        AttackDirection = DeltaPosition.TileOctant();
                        */
                        EDirection AttackDirection = Asset->Position().DirectionTo(ClosestTargetPosition);
                        Asset->Direction(AttackDirection);
                        Asset->IncrementStep();
                        if(Asset->Step() == Asset->AttackSteps()){
                            SAssetCommand AttackCommand; // Create missle
                            auto ArrowAsset = DPlayers[to_underlying(EPlayerColor::None)]->CreateAsset("None");
                            int Damage = Asset->EffectiveBasicDamage() - CurrentCommand.DAssetTarget->EffectiveArmor();
                            Damage = 0 > Damage ? 0 : Damage;
                            Damage += Asset->EffectivePiercingDamage();
                            if(DRandomNumberGenerator.Random() & 0x1){ // 50% chance half damage
                                Damage /= 2;
                            }

                            TempEvent.DType = EEventType::MissleFire;
                            TempEvent.DAsset = Asset;
                            CurrentEvents.push_back(TempEvent);

                            ArrowAsset->HitPoints(Damage);
                            ArrowAsset->Position(Asset->Position());
                            if(ArrowAsset->PositionX() < ClosestTargetPosition.X()){
                                ArrowAsset->PositionX(ArrowAsset->PositionX() + CPosition::HalfTileWidth());
                            }
                            else if(ArrowAsset->PositionX() > ClosestTargetPosition.X()){
                                ArrowAsset->PositionX(ArrowAsset->PositionX() - CPosition::HalfTileWidth());
                            }

                            if(ArrowAsset->PositionY() < ClosestTargetPosition.Y()){
                                ArrowAsset->PositionY(ArrowAsset->PositionY() + CPosition::HalfTileHeight());
                            }
                            else if(ArrowAsset->PositionY() > ClosestTargetPosition.Y()){
                                ArrowAsset->PositionY(ArrowAsset->PositionY() - CPosition::HalfTileHeight());
                            }
                            ArrowAsset->Direction(AttackDirection);
                            AttackCommand.DAction = EAssetAction::Construct;
                            AttackCommand.DAssetTarget = Asset;
                            ArrowAsset->PushCommand(AttackCommand);
                            AttackCommand.DAction = EAssetAction::Attack;
                            AttackCommand.DAssetTarget = CurrentCommand.DAssetTarget;
                            ArrowAsset->PushCommand(AttackCommand);
                        }
                        if(Asset->Step() >= (Asset->AttackSteps() + Asset->ReloadSteps())){
                           Asset->ResetStep();
                        }
                    }
                }
            }
            else{
                SAssetCommand NextCommand = Asset->NextCommand();

                Asset->PopCommand();
                if(EAssetAction::StandGround != NextCommand.DAction){
                    auto NewTarget = DPlayers[to_underlying(Asset->Color())]->FindNearestEnemy(Asset->Position(), Asset->EffectiveSight());

                    if(!NewTarget.expired()){
                        CurrentCommand.DAssetTarget = NewTarget.lock();
                        Asset->PushCommand(CurrentCommand);
                        Asset->ResetStep();
                    }
                }
            }
        }
        else if((EAssetAction::ConveyLumber == Asset->Action())||(EAssetAction::ConveyGold == Asset->Action())||(EAssetAction::ConveyStone == Asset->Action())){
            Asset->IncrementStep();
            if(DConveySteps <= Asset->Step()){
                SAssetCommand Command = Asset->CurrentCommand();
                CTilePosition NextTarget(DPlayers[to_underlying(Asset->Color())]->PlayerMap()->Width()-1,DPlayers[to_underlying(Asset->Color())]->PlayerMap()->Height()-1);

                DPlayers[to_underlying(Asset->Color())]->IncrementGold(Asset->Gold());
                DPlayers[to_underlying(Asset->Color())]->IncrementLumber(Asset->Lumber());
                DPlayers[to_underlying(Asset->Color())]->IncrementStone(Asset->Stone());
                Asset->Gold(0);
                Asset->Lumber(0);
                Asset->Stone(0);
                Asset->PopCommand();
                Asset->ResetStep();
                if(EAssetAction::None != Asset->Action()){
                    NextTarget = Asset->CurrentCommand().DAssetTarget->TilePosition();
                }
                Asset->TilePosition(DPlayers[to_underlying(Asset->Color())]->PlayerMap()->FindAssetPlacement(Asset, Command.DAssetTarget, NextTarget));
            }
        }
        else if(EAssetAction::Construct == Asset->Action()){
            SAssetCommand Command = Asset->CurrentCommand();
            if(Command.DActivatedCapability){
                if(Command.DActivatedCapability->IncrementStep()){
                    // All Done
                }
            }
        }
        else if(EAssetAction::Death == Asset->Action()){
            Asset->IncrementStep();
            if(Asset->Step() > DDeathSteps){
                if(Asset->Speed()){
                    SAssetCommand DecayCommand;
                    // Create corpse
                    auto CorpseAsset = DPlayers[to_underlying(EPlayerColor::None)]->CreateAsset("None");

                    DecayCommand.DAction = EAssetAction::Decay;
                    CorpseAsset->Position(Asset->Position());
                    CorpseAsset->Direction(Asset->Direction());
                    CorpseAsset->PushCommand(DecayCommand);

                }
                DPlayers[to_underlying(Asset->Color())]->DeleteAsset(Asset);
            }
        }
        else if(EAssetAction::Decay == Asset->Action()){
            Asset->IncrementStep();
            if(Asset->Step() > DDecaySteps){
                DPlayers[to_underlying(Asset->Color())]->DeleteAsset(Asset);
            }
        }

        if(EAssetAction::Walk == Asset->Action()){
            if(Asset->TileAligned()){
                SAssetCommand Command = Asset->CurrentCommand();
                SAssetCommand NextCommand = Asset->NextCommand();
                EDirection TravelDirection;
                CPixelPosition MapTarget = Command.DAssetTarget->ClosestPosition(Asset->Position());

                if(EAssetAction::Attack == NextCommand.DAction){
                    // Check to see if can attack now
                    if(NextCommand.DAssetTarget->ClosestPosition(Asset->Position()).DistanceSquared( Asset->Position() ) <= RangeToDistanceSquared(Asset->EffectiveRange())){
                        Asset->PopCommand();
                        Asset->ResetStep();
                        continue;
                    }
                }
                TravelDirection = DRouterMap.FindRoute(*DPlayers[to_underlying(Asset->Color())]->PlayerMap(), *Asset, MapTarget);
                if(EDirection::Max != TravelDirection){
                    Asset->Direction(TravelDirection);
                }
                else{
                    CTilePosition TilePosition;
                    TilePosition.SetFromPixel(MapTarget);
                    if((TilePosition == Asset->TilePosition())||(EDirection::Max != Asset->TilePosition().AdjacentTileDirection(TilePosition))){
                        Asset->PopCommand();
                        Asset->ResetStep();
                        continue;
                    }
                    else if(EAssetAction::HarvestLumber == NextCommand.DAction){
                        TilePosition = DPlayers[to_underlying(Asset->Color())]->PlayerMap()->FindNearestReachableTileType(Asset->TilePosition(), CTerrainMap::ETileType::Forest);
                        // Find new lumber
                        Asset->PopCommand();
                        Asset->PopCommand();
                        if(0 <= TilePosition.X()){
                            CPixelPosition NewPosition;
                            NewPosition.SetFromTile(TilePosition);
                            Command.DAction = EAssetAction::HarvestLumber;
                            Command.DAssetTarget = DPlayers[to_underlying(Asset->Color())]->CreateMarker(NewPosition, false);
                            Asset->PushCommand(Command);
                            Command.DAction = EAssetAction::Walk;
                            Asset->PushCommand(Command);
                            Asset->ResetStep();
                            continue;
                        }
                    }
                    else if(EAssetAction::QuarryStone == NextCommand.DAction){
                        TilePosition = DPlayers[to_underlying(Asset->Color())]->PlayerMap()->FindNearestReachableTileType(Asset->TilePosition(), CTerrainMap::ETileType::Rock);
                        // Find new stone
                        Asset->PopCommand();
                        Asset->PopCommand();
                        if(0 <= TilePosition.X()){
                            CPixelPosition NewPosition;
                            NewPosition.SetFromTile(TilePosition);
                            Command.DAction = EAssetAction::QuarryStone;
                            Command.DAssetTarget = DPlayers[to_underlying(Asset->Color())]->CreateMarker(NewPosition, false);
                            Asset->PushCommand(Command);
                            Command.DAction = EAssetAction::Walk;
                            Asset->PushCommand(Command);
                            Asset->ResetStep();
                            continue;
                        }
                    }
                    else{
                        Command.DAction = EAssetAction::None;
                        Asset->PushCommand(Command);
                        Asset->ResetStep();
                        continue;
                    }
                }
            }

            if(!Asset->MoveStep(DAssetOccupancyMap, DDiagonalOccupancyMap)){
                Asset->Direction(DirectionOpposite(Asset->Position().TileOctant()));

            }
        }
    }
    DGameCycle++;
    for(int PlayerIndex = 0; PlayerIndex < to_underlying(EPlayerColor::Max); PlayerIndex++){
        DPlayers[PlayerIndex]->IncrementCycle();
        DPlayers[PlayerIndex]->AppendGameEvents(CurrentEvents);
    }
}

/**
*  Clears game events of all players
*
*  @return Nothing
*
*/
void CGameModel::ClearGameEvents(){
    for(int PlayerIndex = 0; PlayerIndex < to_underlying(EPlayerColor::Max); PlayerIndex++){
        DPlayers[PlayerIndex]->ClearGameEvents();
    }
}
