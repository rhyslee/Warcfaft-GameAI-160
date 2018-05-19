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
* @class BattleMode
*
* @brief This class shows the logic for the Battle Mode.
*   It calculates if the character is still alive,
*   and renders the Battle Mode. So the screen you see
*   after you start playing the game, is what is rendered by this file.
* 
* @note Menu button in BattleMode is checked on ln 531
* 
* @author Nishant
* 
* @version 1.0
* 
* @date 10/06/2017 14:48:15
* 
* Contact: nchandrashekar@ucdavis.edu
* 
*/

#include "BattleMode.h"
#include "EndOfBattleMode.h"
#include "ApplicationData.h"
#include "InGameMenuMode.h"
#include "PixelType.h"
#include "Debug.h"
#include <sstream>

#define PAN_SPEED_MAX           0x100
#define PAN_SPEED_SHIFT         1

template <typename T>
inline bool WeakPtrEquals(const std::weak_ptr<T>& t, const std::weak_ptr<T>& u){
    return !t.expired() && t.lock() == u.lock();
}

template <typename T>
inline bool WeakPtrCompare(const std::weak_ptr<T>& t, const std::weak_ptr<T>& u){
    return !t.expired() && t.lock() <= u.lock();
}

// static variable for battle status
bool CBattleMode::DBattleWon;
bool CBattleMode::DBattleOver;

std::shared_ptr< CBattleMode > CBattleMode::DBattleModePointer;

/**
* Set/initialize default values for the game.
*
* @param[in] key Empty struct, not used.
*
* @return Nothing.
*
*/
CBattleMode::CBattleMode(const SPrivateConstructorType & key){
    DTime = 0;
    DLeftClicked = false;
}

void CBattleMode::InitializeChange(std::shared_ptr< CApplicationData > context){
    context->LoadGameMap(context->DSelectedMapIndex);
    context->DSoundLibraryMixer->PlaySong(context->DSoundLibraryMixer->FindSong("game1"), context->DMusicVolume);
}

/**
*  Read input from the game to update Battle Mode
*  settings, including screen panning.
* 
* @param[in] context a shared pointer to Application Data
*
* @return Nothing.
*
*/
void CBattleMode::Input(std::shared_ptr< CApplicationData > context){
//    PrintDebug(DEBUG_LOW, "Started BattleMode::Input\n");
    int CurrentX, CurrentY;
    bool Panning = false;
    bool ShiftPressed = false;
    EDirection PanningDirection = EDirection::Max;

    CurrentX = context->DCurrentX;
    CurrentY = context->DCurrentY;

    context->DGameModel->ClearGameEvents();

    // handle screen panning
    for(auto Key : context->DPressedKeys){
        if(SGUIKeyType::UpArrow == Key){
            PanningDirection = EDirection::North;
            Panning = true;
        }
        else if(SGUIKeyType::DownArrow == Key){
            PanningDirection = EDirection::South;
            Panning = true;
        }
        else if(SGUIKeyType::LeftArrow == Key){
            PanningDirection = EDirection::West;
            Panning = true;
        }
        else if(SGUIKeyType::RightArrow == Key){
            PanningDirection = EDirection::East;
            Panning = true;
        }
        else if((SGUIKeyType::LeftShift == Key)||(SGUIKeyType::RightShift == Key)){
            ShiftPressed = true;
        }

        // handle unit grouping
        else if (SGUIKeyType::LeftControl == DPrevKey || SGUIKeyType::RightControl == DPrevKey){

            // check if key-combo is valid unit grouping hotkey
            auto KeyLookup = context->DGroupHotKeyMap.find(Key);
                if(KeyLookup != context->DGroupHotKeyMap.end()){

                    // if player has some units selected, map unit grouping
                    if(DLeftClicked && context->DSelectedPlayerAssets.size()){

                        // limit units to 9
                        if(context->DSelectedPlayerAssets.size() > 9){
                            context->DSelectedPlayerAssets.resize(9);
                        }

                        context->DGroupHotKeyMap[Key] = context->DSelectedPlayerAssets;
                        DLeftClicked = false;
                    }

                    // else switch unit groups
                    else{
                        context->DSelectedPlayerAssets = KeyLookup->second;
                    }
                }
        }

        // record previously pressed key
        DPrevKey = Key;
    }

    // Handle releases
    for(auto Key : context->DReleasedKeys){

        // make sure if the player selected asset
        if(context->DSelectedPlayerAssets.size()){
            bool CanMove = true;

            // the player can select multiple assets, such as a team of troops
            for(auto &Asset : context->DSelectedPlayerAssets){
                if(auto LockedAsset = Asset.lock()){
                    // check if the player's asset is selected, not the AI's asset
                    if(context->DPlayerColor != LockedAsset->Color()){
                        context->DReleasedKeys.clear();
                        return;
                    }
                    // check if the selected asset can move
                    if(0 == LockedAsset->Speed()){
                        CanMove = false;
                        break;
                    }
                }
            }
            if(SGUIKeyType::Escape == Key){
                context->DCurrentAssetCapability = EAssetCapabilityType::None;
            }

            // if player is trying to build something
            if(EAssetCapabilityType::BuildSimple == context->DCurrentAssetCapability){
                // check if hotkey is valid
                auto KeyLookup = context->DBuildHotKeyMap.find(Key);
                // if hotkey valid
                if(KeyLookup != context->DBuildHotKeyMap.end()){
                    // check if player has capability
                    auto PlayerCapability = CPlayerCapability::FindCapability(KeyLookup->second);

                    // if capable
                    if(PlayerCapability){
                        // select current asset
                        auto ActorTarget = context->DSelectedPlayerAssets.front().lock();

                        if(PlayerCapability->CanInitiate(ActorTarget, context->DGameModel->Player(context->DPlayerColor))){
                            SGameEvent TempEvent;
                            TempEvent.DType = EEventType::ButtonTick;
                            context->DGameModel->Player(context->DPlayerColor)->AddGameEvent(TempEvent);

                            context->DCurrentAssetCapability = KeyLookup->second;
                        }
                    }
                }
            }

            else if(CanMove){
                auto KeyLookup = context->DUnitHotKeyMap.find(Key);

                if(KeyLookup != context->DUnitHotKeyMap.end()){
                    bool HasCapability = true;
                    for(auto &Asset : context->DSelectedPlayerAssets){
                        if(auto LockedAsset = Asset.lock()){
                            if(!LockedAsset->HasCapability(KeyLookup->second)){
                                HasCapability = false;
                                break;
                            }
                        }
                    }
                    if(HasCapability){
                        auto PlayerCapability = CPlayerCapability::FindCapability(KeyLookup->second);
                        SGameEvent TempEvent;
                        TempEvent.DType = EEventType::ButtonTick;
                        context->DGameModel->Player(context->DPlayerColor)->AddGameEvent(TempEvent);

                        if(PlayerCapability){
                            if((CPlayerCapability::ETargetType::None == PlayerCapability->TargetType())||(CPlayerCapability::ETargetType::Player == PlayerCapability->TargetType())){
                                auto ActorTarget = context->DSelectedPlayerAssets.front().lock();

                                if(PlayerCapability->CanApply(ActorTarget, context->DGameModel->Player(context->DPlayerColor), ActorTarget)){

                                    context->DPlayerCommands[to_underlying(context->DPlayerColor)].DAction = KeyLookup->second;
                                    context->DPlayerCommands[to_underlying(context->DPlayerColor)].DActors = context->DSelectedPlayerAssets;
                                    context->DPlayerCommands[to_underlying(context->DPlayerColor)].DTargetColor = EPlayerColor::None;
                                    context->DPlayerCommands[to_underlying(context->DPlayerColor)].DTargetType = EAssetType::None;
                                    context->DPlayerCommands[to_underlying(context->DPlayerColor)].DTargetLocation = ActorTarget->Position();
                                    context->DCurrentAssetCapability = EAssetCapabilityType::None;
                                }
                            }
                            else{
                                context->DCurrentAssetCapability = KeyLookup->second;
                            }
                        }
                        else{
                            context->DCurrentAssetCapability = KeyLookup->second;
                        }
                    }
                }
            }
            else{
                auto KeyLookup = context->DTrainHotKeyMap.find(Key);

                if(KeyLookup != context->DTrainHotKeyMap.end()){
                    bool HasCapability = true;
                    for(auto &Asset : context->DSelectedPlayerAssets){
                        if(auto LockedAsset = Asset.lock()){
                            if(!LockedAsset->HasCapability(KeyLookup->second)){
                                HasCapability = false;
                                break;
                            }
                        }
                    }
                    if(HasCapability){
                        auto PlayerCapability = CPlayerCapability::FindCapability(KeyLookup->second);
                        SGameEvent TempEvent;
                        TempEvent.DType = EEventType::ButtonTick;
                        context->DGameModel->Player(context->DPlayerColor)->AddGameEvent(TempEvent);

                        if(PlayerCapability){
                            if((CPlayerCapability::ETargetType::None == PlayerCapability->TargetType())||(CPlayerCapability::ETargetType::Player == PlayerCapability->TargetType())){
                                auto ActorTarget = context->DSelectedPlayerAssets.front().lock();

                                if(PlayerCapability->CanApply(ActorTarget, context->DGameModel->Player(context->DPlayerColor), ActorTarget)){

                                    context->DPlayerCommands[to_underlying(context->DPlayerColor)].DAction = KeyLookup->second;
                                    context->DPlayerCommands[to_underlying(context->DPlayerColor)].DActors = context->DSelectedPlayerAssets;
                                    context->DPlayerCommands[to_underlying(context->DPlayerColor)].DTargetColor = EPlayerColor::None;
                                    context->DPlayerCommands[to_underlying(context->DPlayerColor)].DTargetType = EAssetType::None;
                                    context->DPlayerCommands[to_underlying(context->DPlayerColor)].DTargetLocation = ActorTarget->Position();
                                    context->DCurrentAssetCapability = EAssetCapabilityType::None;
                                }
                            }
                            else{
                                context->DCurrentAssetCapability = KeyLookup->second;
                            }
                        }
                        else{
                            context->DCurrentAssetCapability = KeyLookup->second;
                        }
                    }
                }
            }
        }
    }
    context->DReleasedKeys.clear();

    context->DMenuButtonState = CButtonRenderer::EButtonState::None;
    CApplicationData::EUIComponentType ComponentType = context->FindUIComponentType(CPixelPosition(CurrentX, CurrentY));
    if(CApplicationData::uictViewport == ComponentType){
        CPixelPosition TempPosition(context->ScreenToDetailedMap(CPixelPosition(CurrentX, CurrentY)));
        CPixelPosition ViewPortPosition = context->ScreenToViewport(CPixelPosition(CurrentX, CurrentY));
        CPixelType PixelType = CPixelType::GetPixelType(context->DViewportTypeSurface, ViewPortPosition);

        if(context->DRightClick && !context->DRightDown && context->DSelectedPlayerAssets.size()){
            bool CanMove = true;

            for(auto &Asset : context->DSelectedPlayerAssets){
                if(auto LockedAsset = Asset.lock()){
                    // check if the player's asset is selected, not the AI's asset
                    if(context->DPlayerColor != LockedAsset->Color())
                        return;
                    // Selected unit can be moved if its speed stat is not 0 or it has the same team color value as the player
                    if(0 == LockedAsset->Speed()){
                        CanMove = false;
                        break;
                    }
                }
            }

            if(CanMove){
                if(EPlayerColor::None != PixelType.Color()){
                    // Command is either walk/deliver, repair, or attack

                    context->DPlayerCommands[to_underlying(context->DPlayerColor)].DAction = EAssetCapabilityType::Move;
                    context->DPlayerCommands[to_underlying(context->DPlayerColor)].DTargetColor = PixelType.Color();
                    context->DPlayerCommands[to_underlying(context->DPlayerColor)].DTargetType = PixelType.AssetType();
                    context->DPlayerCommands[to_underlying(context->DPlayerColor)].DActors = context->DSelectedPlayerAssets;
                    context->DPlayerCommands[to_underlying(context->DPlayerColor)].DTargetLocation = TempPosition;

                    if(PixelType.Color() == context->DPlayerColor){
                        bool HaveLumber = false;
                        bool HaveGold = false;
                        bool HaveStone = false;

                        for(auto &Asset : context->DSelectedPlayerAssets){
                            if(auto LockedAsset = Asset.lock()){
                                if(LockedAsset->Lumber()){
                                    HaveLumber = true;
                                }
                                if(LockedAsset->Gold()){
                                    HaveGold = true;
                                }
                                if(LockedAsset->Stone()){
                                    HaveStone = true;
                                }
                            }
                        }
                        // context->DPlayerCommands[to_underlying(context->DPlayerColor)].DAction = EAssetCapabilityType::Move;
                        if(HaveGold){
                            if((EAssetType::TownHall == context->DPlayerCommands[to_underlying(context->DPlayerColor)].DTargetType)||(EAssetType::Keep == context->DPlayerCommands[to_underlying(context->DPlayerColor)].DTargetType)||(EAssetType::Castle == context->DPlayerCommands[to_underlying(context->DPlayerColor)].DTargetType)){
                                context->DPlayerCommands[to_underlying(context->DPlayerColor)].DAction = EAssetCapabilityType::Convey;
                            }
                        }
                        else if(HaveLumber){
                            if((EAssetType::TownHall == context->DPlayerCommands[to_underlying(context->DPlayerColor)].DTargetType)||(EAssetType::Keep == context->DPlayerCommands[to_underlying(context->DPlayerColor)].DTargetType)||(EAssetType::Castle == context->DPlayerCommands[to_underlying(context->DPlayerColor)].DTargetType)||(EAssetType::LumberMill == context->DPlayerCommands[to_underlying(context->DPlayerColor)].DTargetType)){
                                context->DPlayerCommands[to_underlying(context->DPlayerColor)].DAction = EAssetCapabilityType::Convey;
                            }
                        }
                        else if(HaveStone){
                            if((EAssetType::TownHall == context->DPlayerCommands[to_underlying(context->DPlayerColor)].DTargetType)||(EAssetType::Keep == context->DPlayerCommands[to_underlying(context->DPlayerColor)].DTargetType)||(EAssetType::Castle == context->DPlayerCommands[to_underlying(context->DPlayerColor)].DTargetType)||(EAssetType::Blacksmith == context->DPlayerCommands[to_underlying(context->DPlayerColor)].DTargetType)){
                                context->DPlayerCommands[to_underlying(context->DPlayerColor)].DAction = EAssetCapabilityType::Convey;
                            }
                        }
                        else{
                            auto TargetAsset = context->DGameModel->Player(context->DPlayerColor)->SelectAsset(TempPosition, PixelType.AssetType()).lock();
                            if((0 == TargetAsset->Speed())&&(TargetAsset->MaxHitPoints() > TargetAsset->HitPoints())){
                                context->DPlayerCommands[to_underlying(context->DPlayerColor)].DAction = EAssetCapabilityType::Repair;
                            }
                        }
                    }
                    else{
                            context->DPlayerCommands[to_underlying(context->DPlayerColor)].DAction = EAssetCapabilityType::Attack;
                    }
                    context->DCurrentAssetCapability = EAssetCapabilityType::None;
                }
                else{
                    // Command is either walk, mine, harvest, quarry
                    CPixelPosition TempPosition(context->ScreenToDetailedMap(CPixelPosition(CurrentX, CurrentY)));
                    bool CanHarvest = true;

                    context->DPlayerCommands[to_underlying(context->DPlayerColor)].DAction = EAssetCapabilityType::Move;
                    context->DPlayerCommands[to_underlying(context->DPlayerColor)].DTargetColor = EPlayerColor::None;
                    context->DPlayerCommands[to_underlying(context->DPlayerColor)].DTargetType = EAssetType::None;
                    context->DPlayerCommands[to_underlying(context->DPlayerColor)].DActors = context->DSelectedPlayerAssets;
                    context->DPlayerCommands[to_underlying(context->DPlayerColor)].DTargetLocation = TempPosition;

                    for(auto &Asset : context->DSelectedPlayerAssets){
                        if(auto LockedAsset = Asset.lock()){
                            if(!LockedAsset->HasCapability(EAssetCapabilityType::Mine)){
                                CanHarvest = false;
                                break;
                            }
                        }
                    }
                    if(CanHarvest){
                        if(CPixelType::EAssetTerrainType::Tree == PixelType.Type()){
                            CTilePosition TempTilePosition;

                            context->DPlayerCommands[to_underlying(context->DPlayerColor)].DAction = EAssetCapabilityType::Mine;
                            TempTilePosition.SetFromPixel(context->DPlayerCommands[to_underlying(context->DPlayerColor)].DTargetLocation);
                            if(CTerrainMap::ETileType::Forest != context->DGameModel->Player(context->DPlayerColor)->PlayerMap()->TileType(TempTilePosition)){
                                // Could be tree pixel, but tops of next row
                                TempTilePosition.IncrementY(1);
                                if(CTerrainMap::ETileType::Forest == context->DGameModel->Player(context->DPlayerColor)->PlayerMap()->TileType(TempTilePosition)){
                                    context->DPlayerCommands[to_underlying(context->DPlayerColor)].DTargetLocation.SetFromTile(TempTilePosition);
                                }
                            }
                        }
                        else if(CPixelType::EAssetTerrainType::Rock == PixelType.Type()){
                            CTilePosition TempTilePosition;

                            context->DPlayerCommands[to_underlying(context->DPlayerColor)].DAction = EAssetCapabilityType::Mine;
                            TempTilePosition.SetFromPixel(context->DPlayerCommands[to_underlying(context->DPlayerColor)].DTargetLocation);
                            if(CTerrainMap::ETileType::Rock != context->DGameModel->Player(context->DPlayerColor)->PlayerMap()->TileType(TempTilePosition)){
                                // Could be  pixel, but tops of next row
                                TempTilePosition.IncrementY(1);
                                if(CTerrainMap::ETileType::Rock == context->DGameModel->Player(context->DPlayerColor)->PlayerMap()->TileType(TempTilePosition)){
                                    context->DPlayerCommands[to_underlying(context->DPlayerColor)].DTargetLocation.SetFromTile(TempTilePosition);
                                }
                            }
                        }
                        else if(CPixelType::EAssetTerrainType::GoldMine == PixelType.Type()){
                            context->DPlayerCommands[to_underlying(context->DPlayerColor)].DAction = EAssetCapabilityType::Mine;
                            context->DPlayerCommands[to_underlying(context->DPlayerColor)].DTargetType = EAssetType::GoldMine;
                        }
                    }
                    context->DCurrentAssetCapability = EAssetCapabilityType::None;
                }
            }
        }

        // if left mouse button is clicked
        else if(context->DLeftClick){
            DLeftClicked = true;

            if((EAssetCapabilityType::None == context->DCurrentAssetCapability)||(EAssetCapabilityType::BuildSimple == context->DCurrentAssetCapability)){
                // record mouse clicked position
                if(context->DLeftDown){
                    context->DMouseDown = TempPosition;
                }

                // record mouse dragged area
                else{
                    SRectangle TempRectangle;
                    EPlayerColor SearchColor = context->DPlayerColor;
                    std::list< std::shared_ptr< CPlayerAsset > > PreviousSelections;

                    for(auto WeakAsset : context->DSelectedPlayerAssets){
                        if(auto LockedAsset = WeakAsset.lock()){
                            PreviousSelections.push_back(LockedAsset);
                        }
                    }

                    TempRectangle.DXPosition = std::min(context->DMouseDown.X(), TempPosition.X());
                    TempRectangle.DYPosition = std::min(context->DMouseDown.Y(), TempPosition.Y());
                    TempRectangle.DWidth = std::max(context->DMouseDown.X(), TempPosition.X()) - TempRectangle.DXPosition;
                    TempRectangle.DHeight = std::max(context->DMouseDown.Y(), TempPosition.Y()) - TempRectangle.DYPosition;

                    if((TempRectangle.DWidth < CPosition::TileWidth())||(TempRectangle.DHeight < CPosition::TileHeight())||(2 == context->DLeftClick)){
                        TempRectangle.DXPosition = TempPosition.X();
                        TempRectangle.DYPosition = TempPosition.Y();
                        TempRectangle.DWidth = 0;
                        TempRectangle.DHeight = 0;
                        SearchColor = PixelType.Color();
                    }
                    if(SearchColor != context->DPlayerColor){
                        context->DSelectedPlayerAssets.clear();
                    }
                    if(ShiftPressed){
                        if(!context->DSelectedPlayerAssets.empty()){
                            if(auto TempAsset = context->DSelectedPlayerAssets.front().lock()){
                                if(TempAsset->Color() != context->DPlayerColor){
                                    context->DSelectedPlayerAssets.clear();
                                }
                            }
                        }
                        context->DSelectedPlayerAssets.splice(context->DSelectedPlayerAssets.end(), context->DGameModel->Player(SearchColor)->SelectAssets(TempRectangle, PixelType.AssetType(), 2 == context->DLeftClick));
                        context->DSelectedPlayerAssets.sort(WeakPtrCompare<CPlayerAsset>);
                        context->DSelectedPlayerAssets.unique(WeakPtrEquals<CPlayerAsset>);
                    }
                    else{
                        PreviousSelections.clear();
                        context->DSelectedPlayerAssets = context->DGameModel->Player(SearchColor)->SelectAssets(TempRectangle, PixelType.AssetType(), 2 == context->DLeftClick);
                    }
                    for(auto WeakAsset : context->DSelectedPlayerAssets){
                        if(auto LockedAsset = WeakAsset.lock()){
                            bool FoundPrevious = false;
                            for(auto PrevAsset : PreviousSelections){
                                if(PrevAsset == LockedAsset){
                                    FoundPrevious = true;
                                    break;
                                }
                            }
                            if(!FoundPrevious){
                                SGameEvent TempEvent;
                                TempEvent.DType = EEventType::Selection;
                                TempEvent.DAsset = LockedAsset;
                                context->DGameModel->Player(context->DPlayerColor)->AddGameEvent(TempEvent);
                            }
                        }
                    }


                    context->DMouseDown = CPixelPosition(-1,-1);
                }
                context->DCurrentAssetCapability = EAssetCapabilityType::None;
            }
            else{
                auto PlayerCapability = CPlayerCapability::FindCapability(context->DCurrentAssetCapability);

                if(PlayerCapability && !context->DLeftDown){
                    if(((CPlayerCapability::ETargetType::Asset == PlayerCapability->TargetType())||(CPlayerCapability::ETargetType::TerrainOrAsset == PlayerCapability->TargetType()))&&(EAssetType::None != PixelType.AssetType())){
                        auto NewTarget = context->DGameModel->Player(PixelType.Color())->SelectAsset(TempPosition, PixelType.AssetType()).lock();

                        if(PlayerCapability->CanApply(context->DSelectedPlayerAssets.front().lock(), context->DGameModel->Player(context->DPlayerColor), NewTarget)){
                            SGameEvent TempEvent;
                            TempEvent.DType = EEventType::PlaceAction;
                            TempEvent.DAsset = NewTarget;
                            context->DGameModel->Player(context->DPlayerColor)->AddGameEvent(TempEvent);

                            context->DPlayerCommands[to_underlying(context->DPlayerColor)].DAction = context->DCurrentAssetCapability;
                            context->DPlayerCommands[to_underlying(context->DPlayerColor)].DActors = context->DSelectedPlayerAssets;
                            context->DPlayerCommands[to_underlying(context->DPlayerColor)].DTargetColor = PixelType.Color();
                            context->DPlayerCommands[to_underlying(context->DPlayerColor)].DTargetType = PixelType.AssetType();
                            context->DPlayerCommands[to_underlying(context->DPlayerColor)].DTargetLocation = TempPosition;
                            context->DCurrentAssetCapability = EAssetCapabilityType::None;
                        }
                    }
                    else if(((CPlayerCapability::ETargetType::Terrain == PlayerCapability->TargetType())||(CPlayerCapability::ETargetType::TerrainOrAsset == PlayerCapability->TargetType()))&&((EAssetType::None == PixelType.AssetType())&&(EPlayerColor::None == PixelType.Color()))){
                        auto NewTarget = context->DGameModel->Player(context->DPlayerColor)->CreateMarker(TempPosition, false);

                        if(PlayerCapability->CanApply(context->DSelectedPlayerAssets.front().lock(), context->DGameModel->Player(context->DPlayerColor), NewTarget)){
                            SGameEvent TempEvent;
                            TempEvent.DType = EEventType::PlaceAction;
                            TempEvent.DAsset = NewTarget;
                            context->DGameModel->Player(context->DPlayerColor)->AddGameEvent(TempEvent);

                            context->DPlayerCommands[to_underlying(context->DPlayerColor)].DAction = context->DCurrentAssetCapability;
                            context->DPlayerCommands[to_underlying(context->DPlayerColor)].DActors = context->DSelectedPlayerAssets;
                            context->DPlayerCommands[to_underlying(context->DPlayerColor)].DTargetColor = EPlayerColor::None;
                            context->DPlayerCommands[to_underlying(context->DPlayerColor)].DTargetType = EAssetType::None;
                            context->DPlayerCommands[to_underlying(context->DPlayerColor)].DTargetLocation = TempPosition;
                            context->DCurrentAssetCapability = EAssetCapabilityType::None;
                        }
                    }
                    else{

                    }

                }
            }
        }
    }
    else if(CApplicationData::uictViewportBevelN == ComponentType){
        PanningDirection = EDirection::North;
        Panning = true;
    }
    else if(CApplicationData::uictViewportBevelE == ComponentType){
        PanningDirection = EDirection::East;
        Panning = true;
    }
    else if(CApplicationData::uictViewportBevelS == ComponentType){
        PanningDirection = EDirection::South;
        Panning = true;
    }
    else if(CApplicationData::uictViewportBevelW == ComponentType){
        PanningDirection = EDirection::West;
        Panning = true;
    }
    else if(CApplicationData::uictMiniMap == ComponentType){
        if(context->DLeftClick && !context->DLeftDown){
            CPixelPosition TempPosition(context->ScreenToMiniMap(CPixelPosition(CurrentX, CurrentY)));
            TempPosition = context->MiniMapToDetailedMap(TempPosition);

            context->DViewportRenderer->CenterViewport(TempPosition);
        }
    }
    else if(CApplicationData::uictUserDescription == ComponentType){
        if(context->DLeftClick && !context->DLeftDown){
            int IconPressed = context->DUnitDescriptionRenderer->Selection(context->ScreenToUnitDescription(CPixelPosition(CurrentX, CurrentY)));

            if(1 == context->DSelectedPlayerAssets.size()){
                if(0 == IconPressed){
                    if(auto Asset = context->DSelectedPlayerAssets.front().lock()){
                        context->DViewportRenderer->CenterViewport(Asset->Position());
                    }
                }
            }
            else if(0 <= IconPressed){
                while(IconPressed){
                    IconPressed--;
                    context->DSelectedPlayerAssets.pop_front();
                }
                while(1 < context->DSelectedPlayerAssets.size()){
                    context->DSelectedPlayerAssets.pop_back();
                }
                SGameEvent TempEvent;
                TempEvent.DType = EEventType::Selection;
                TempEvent.DAsset = context->DSelectedPlayerAssets.front().lock();
                context->DGameModel->Player(context->DPlayerColor)->AddGameEvent(TempEvent);
            }
        }
    }
    else if(CApplicationData::uictUserAction == ComponentType){
        if(context->DLeftClick && !context->DLeftDown){
            EAssetCapabilityType CapabilityType = context->DUnitActionRenderer->Selection(context->ScreenToUnitAction(CPixelPosition(CurrentX, CurrentY)));
            auto PlayerCapability = CPlayerCapability::FindCapability(CapabilityType);

            if(EAssetCapabilityType::None != CapabilityType){
                SGameEvent TempEvent;
                TempEvent.DType = EEventType::ButtonTick;
                context->DGameModel->Player(context->DPlayerColor)->AddGameEvent(TempEvent);
            }
            if(PlayerCapability){
                if((CPlayerCapability::ETargetType::None == PlayerCapability->TargetType())||(CPlayerCapability::ETargetType::Player == PlayerCapability->TargetType())){
                    auto ActorTarget = context->DSelectedPlayerAssets.front().lock();

                    if(PlayerCapability->CanApply(ActorTarget, context->DGameModel->Player(context->DPlayerColor), ActorTarget)){

                        context->DPlayerCommands[to_underlying(context->DPlayerColor)].DAction = CapabilityType;
                        context->DPlayerCommands[to_underlying(context->DPlayerColor)].DActors = context->DSelectedPlayerAssets;
                        context->DPlayerCommands[to_underlying(context->DPlayerColor)].DTargetColor = EPlayerColor::None;
                        context->DPlayerCommands[to_underlying(context->DPlayerColor)].DTargetType = EAssetType::None;
                        context->DPlayerCommands[to_underlying(context->DPlayerColor)].DTargetLocation = ActorTarget->Position();
                        context->DCurrentAssetCapability = EAssetCapabilityType::None;
                    }
                }
                else{
                    context->DCurrentAssetCapability = CapabilityType;
                }
            }
            else{
                context->DCurrentAssetCapability = CapabilityType;
            }
        }
    }
    else if(CApplicationData::uictMenuButton == ComponentType){
        context->DMenuButtonState = context->DLeftDown ? CButtonRenderer::EButtonState::Pressed : CButtonRenderer::EButtonState::Hover;

        // if the menu button is clicked, bring up the in-game menu
        if(context->DMenuButtonState == CButtonRenderer::EButtonState::Pressed){
            context->DNextApplicationMode = CInGameMenuMode::Instance();
        }
    }
    if(!Panning){
        context->DPanningSpeed = 0;
    }
    else{
        if(EDirection::North == PanningDirection){
            context->DViewportRenderer->PanNorth(context->DPanningSpeed>>PAN_SPEED_SHIFT);
        }
        else if(EDirection::East == PanningDirection){
            context->DViewportRenderer->PanEast(context->DPanningSpeed>>PAN_SPEED_SHIFT);
        }
        else if(EDirection::South == PanningDirection){
            context->DViewportRenderer->PanSouth(context->DPanningSpeed>>PAN_SPEED_SHIFT);
        }
        else if(EDirection::West == PanningDirection){
            context->DViewportRenderer->PanWest(context->DPanningSpeed>>PAN_SPEED_SHIFT);
        }
        if(context->DPanningSpeed){
            context->DPanningSpeed++;
            if(PAN_SPEED_MAX <context->DPanningSpeed){
                context->DPanningSpeed = PAN_SPEED_MAX;
            }
        }
        else{
            context->DPanningSpeed = 1<<PAN_SPEED_SHIFT;
        }
    }

    //PrintDebug(DEBUG_LOW, "Finished CBattleMode::Input\n");
}

/**
* Calculate current player assets and process
* Input values
* 
* @param[in] context shared pointer to Application Data
*
* @return Nothing
*
*/
void CBattleMode::Calculate(std::shared_ptr< CApplicationData > context){
    context->DGameModel->GetTriggerHandler()->Resolve(ETriggerType::Time, false, EPlayerColor::None, -1, 1, new int[1]{(int)(GetTime() * 1000)});
    //PrintDebug(DEBUG_LOW, "Started CBattleMode::Calculate\n");

    // number of players left in the battle
    int PlayerLeft = 0;


    PrintDebug(DEBUG_LOW, "Started 1st for loop\n");
    for(int Index = 1; Index < to_underlying(EPlayerColor::Max); Index++){
        // calculate the total number of players left in the battle
        //if(context->DGameModel->Player(static_cast<EPlayerColor>(Index))->IsAlive()){
            //PlayerLeft++;

            // if there is any NPC left in the battle
            //if(context->DGameModel->Player(static_cast<EPlayerColor>(Index))->IsAI()){
            //    DBattleWon = false;
            //}
            // if there is no NPC left in the battle
            //else{
            //    DBattleWon = true;
            //}
        //}
        if(context->DGameModel->Player(static_cast<EPlayerColor>(Index))->IsAlive() && context->DGameModel->Player(static_cast<EPlayerColor>(Index))->IsAI()){
            context->DAIPlayers[Index]->CalculateCommand(context->DPlayerCommands[Index]);
        }
    }

    // if there is only one player left in battle, battle ends
    //if(PlayerLeft == 1){
    //    context->ChangeApplicationMode(CEndOfBattleMode::Instance());
    //}

    // if there has been an end game trigger
    if (DBattleOver){
        DBattleOver = false;
        context->ChangeApplicationMode(CEndOfBattleMode::Instance());
    }

    PrintDebug(DEBUG_LOW, "Finished 1st for loop and started 2nd for loop\n");
    for(int Index = 1; Index < to_underlying(EPlayerColor::Max); Index++){
        if(EAssetCapabilityType::None != context->DPlayerCommands[Index].DAction){
            auto PlayerCapability = CPlayerCapability::FindCapability(context->DPlayerCommands[Index].DAction);
            if(PlayerCapability){
                std::shared_ptr< CPlayerAsset > NewTarget;
                
                if((CPlayerCapability::ETargetType::None != PlayerCapability->TargetType())&&(CPlayerCapability::ETargetType::Player != PlayerCapability->TargetType())){
                    if(EAssetType::None == context->DPlayerCommands[Index].DTargetType){
                        NewTarget = context->DGameModel->Player(static_cast<EPlayerColor>(Index))->CreateMarker(context->DPlayerCommands[Index].DTargetLocation, true);
                    }
                    else{
                        NewTarget = context->DGameModel->Player(context->DPlayerCommands[Index].DTargetColor)->SelectAsset(context->DPlayerCommands[Index].DTargetLocation, context->DPlayerCommands[Index].DTargetType).lock();
                        //IL: If nullptr is returned will crash. Make sure to use the right player data.
                    }
                }

        PrintDebug(DEBUG_LOW, "Started 3rd for loop (nested)\n");

                for(auto &WeakActor : context->DPlayerCommands[Index].DActors){
                    if(auto Actor = WeakActor.lock()){
                        auto NewActor = FindAssetObj(Actor->AssetID());
                        if(PlayerCapability->CanApply(NewActor, context->DGameModel->Player(static_cast<EPlayerColor>(Index)), NewTarget) && (NewActor->Interruptible() || (EAssetCapabilityType::Cancel == context->DPlayerCommands[Index].DAction))){
                            PlayerCapability->ApplyCapability(NewActor, context->DGameModel->Player(static_cast<EPlayerColor>(Index)), NewTarget);
                        }
                    }
                }
		PrintDebug(DEBUG_LOW, "Finished 3rd for loop (nested)\n");
            }
            context->DPlayerCommands[Index].DAction = EAssetCapabilityType::None;
        }
    }

    PrintDebug(DEBUG_LOW,"Finished 2nd for loop(nested)\n");
    context->DGameModel->Timestep();
    auto WeakAsset = context->DSelectedPlayerAssets.begin();
    PrintDebug(DEBUG_LOW,"Started 1st while (4th loop)\n");
    while(WeakAsset != context->DSelectedPlayerAssets.end()){
        if(auto Asset = WeakAsset->lock()){
            if(context->DGameModel->ValidAsset(Asset) && Asset->Alive()){
                if(Asset->Speed() && (EAssetAction::Capability == Asset->Action())){
                    auto Command = Asset->CurrentCommand();

                    if(Command.DAssetTarget && (EAssetAction::Construct == Command.DAssetTarget->Action())){
                        SGameEvent TempEvent;

                        context->DSelectedPlayerAssets.clear();
                        context->DSelectedPlayerAssets.push_back(Command.DAssetTarget);

                        TempEvent.DType = EEventType::Selection;
                        TempEvent.DAsset = Command.DAssetTarget;
                        context->DGameModel->Player(context->DPlayerColor)->AddGameEvent(TempEvent);

                        break;
                    }
                }
                WeakAsset++;
            }
            else{
                WeakAsset = context->DSelectedPlayerAssets.erase(WeakAsset);
            }
        }
        else{
            WeakAsset = context->DSelectedPlayerAssets.erase(WeakAsset);
        }
    }
    PrintDebug(DEBUG_LOW, "Finished 1st while (4th loop)\n");
  //  PrintDebug(DEBUG_LOW, "Finished CBattleMode::Calculate\n");
}

/**
* Render Battle Mode graphics
* 
* @param[in] context shared pointer to Application Data
*
* @return Nothing
*
*/
void CBattleMode::Render(std::shared_ptr< CApplicationData > context){
    //PrintDebug(DEBUG_LOW, "Started CBatleMode::Render\n");
    SRectangle TempRectangle({0,0,0,0});
    int CurrentX, CurrentY;
    int BufferWidth, BufferHeight;
    int ViewWidth, ViewHeight;
    int MiniMapWidth, MiniMapHeight;
    int DescriptionWidth, DescriptionHeight;
    int ActionWidth, ActionHeight;
    int ResourceWidth, ResourceHeight;
    std::list< std::weak_ptr< CPlayerAsset > > SelectedAndMarkerAssets = context->DSelectedPlayerAssets;

    CurrentX = context->DCurrentX;
    CurrentY = context->DCurrentY;
    BufferWidth = context->DWorkingBufferSurface->Width();
    BufferHeight = context->DWorkingBufferSurface->Height();
    ViewWidth = context->DViewportSurface->Width();
    ViewHeight = context->DViewportSurface->Height();
    MiniMapWidth = context->DMiniMapSurface->Width();
    MiniMapHeight = context->DMiniMapSurface->Height();
    DescriptionWidth = context->DUnitDescriptionSurface->Width();
    DescriptionHeight = context->DUnitDescriptionSurface->Height();
    ActionWidth = context->DUnitActionSurface->Width();
    ActionHeight = context->DUnitActionSurface->Height();
    ResourceWidth = context->DResourceSurface->Width();
    ResourceHeight = context->DResourceSurface->Height();

    if(context->DLeftDown && 0 < context->DMouseDown.X()){
        CPixelPosition TempPosition(context->ScreenToDetailedMap(CPixelPosition(CurrentX, CurrentY)));
        TempRectangle.DXPosition = std::min(context->DMouseDown.X(), TempPosition.X());
        TempRectangle.DYPosition = std::min(context->DMouseDown.Y(), TempPosition.Y());
        TempRectangle.DWidth = std::max(context->DMouseDown.X(), TempPosition.X()) - TempRectangle.DXPosition;
        TempRectangle.DHeight = std::max(context->DMouseDown.Y(), TempPosition.Y()) - TempRectangle.DYPosition;
    }
    else{
        CPixelPosition TempPosition(context->ScreenToDetailedMap(CPixelPosition(CurrentX, CurrentY)));
        TempRectangle.DXPosition = TempPosition.X();
        TempRectangle.DYPosition = TempPosition.Y();
    }
    for(int YPos = 0; YPos < BufferHeight; YPos += context->DBackgroundTileset->TileHeight()){
        for(int XPos = 0; XPos < BufferWidth; XPos += context->DBackgroundTileset->TileWidth()){
            context->DBackgroundTileset->DrawTile(context->DWorkingBufferSurface, XPos, YPos, 0);
        }
    }

    context->DInnerBevel->DrawBevel(context->DWorkingBufferSurface, context->DViewportXOffset, context->DViewportYOffset, ViewWidth, ViewHeight);
    context->DInnerBevel->DrawBevel(context->DWorkingBufferSurface, context->DMiniMapXOffset, context->DMiniMapYOffset, MiniMapWidth, MiniMapHeight);

    context->DResourceSurface->Draw(context->DWorkingBufferSurface, 0, 0, ResourceWidth, ResourceHeight, context->DViewportXOffset, 0);
    context->DResourceRenderer->DrawResources(context->DResourceSurface);
    context->DWorkingBufferSurface->Draw(context->DResourceSurface, context->DViewportXOffset, 0, -1, -1, 0, 0);

    context->DOuterBevel->DrawBevel(context->DWorkingBufferSurface, context->DUnitDescriptionXOffset, context->DUnitDescriptionYOffset, DescriptionWidth, DescriptionHeight);

    context->DUnitDescriptionSurface->Draw(context->DWorkingBufferSurface, 0, 0, DescriptionWidth, DescriptionHeight, context->DUnitDescriptionXOffset, context->DUnitDescriptionYOffset);
    context->DUnitDescriptionRenderer->DrawUnitDescription(context->DUnitDescriptionSurface, context->DSelectedPlayerAssets);
    context->DWorkingBufferSurface->Draw(context->DUnitDescriptionSurface, context->DUnitDescriptionXOffset, context->DUnitDescriptionYOffset, -1, -1, 0, 0);

    context->DOuterBevel->DrawBevel(context->DWorkingBufferSurface, context->DUnitActionXOffset, context->DUnitActionYOffset, ActionWidth, ActionHeight);
    context->DUnitActionSurface->Draw(context->DWorkingBufferSurface, 0, 0, ActionWidth, ActionHeight, context->DUnitActionXOffset, context->DUnitActionYOffset);
    context->DUnitActionRenderer->DrawUnitAction(context->DUnitActionSurface, context->DSelectedPlayerAssets, context->DCurrentAssetCapability);
    context->DWorkingBufferSurface->Draw(context->DUnitActionSurface, context->DUnitActionXOffset, context->DUnitActionYOffset, -1, -1, 0, 0);

    for(auto Asset : context->DGameModel->Player(context->DPlayerColor)->PlayerMap()->Assets()){
        if(EAssetType::None == Asset->Type()){
            SelectedAndMarkerAssets.push_back(Asset);
        }
    }
    context->DViewportRenderer->DrawViewport(context->DViewportSurface, context->DViewportTypeSurface, SelectedAndMarkerAssets, TempRectangle, context->DCurrentAssetCapability);
    context->DMiniMapRenderer->DrawMiniMap(context->DMiniMapSurface);

    context->DWorkingBufferSurface->Draw(context->DMiniMapSurface, context->DMiniMapXOffset, context->DMiniMapYOffset, -1, -1, 0, 0);
    context->DWorkingBufferSurface->Draw(context->DViewportSurface, context->DViewportXOffset, context->DViewportYOffset, -1, -1, 0, 0);


    context->DMenuButtonRenderer->DrawButton(context->DWorkingBufferSurface, context->DMenuButtonXOffset, context->DMenuButtonYOffset,context->DMenuButtonState);

    switch(context->FindUIComponentType(CPixelPosition(CurrentX, CurrentY))){
        case CApplicationData::uictViewport:        {
                                                        CPixelPosition ViewportCursorLocation = context->ScreenToViewport(CPixelPosition(CurrentX, CurrentY));
                                                        CPixelType PixelType = CPixelType::GetPixelType(context->DViewportTypeSurface, ViewportCursorLocation.X(), ViewportCursorLocation.Y());
                                                        context->DCursorType = CApplicationData::ctPointer;
                                                        if(EAssetCapabilityType::None == context->DCurrentAssetCapability){
                                                            if(PixelType.Color() == context->DPlayerColor){
                                                                context->DCursorType = CApplicationData::ctInspect;
                                                            }
                                                        }
                                                        else{
                                                            auto PlayerCapability = CPlayerCapability::FindCapability(context->DCurrentAssetCapability);

                                                            if(PlayerCapability){
                                                                bool CanApply = false;

                                                                if(EAssetType::None == PixelType.AssetType()){
                                                                    if((CPlayerCapability::ETargetType::Terrain == PlayerCapability->TargetType())||(CPlayerCapability::ETargetType::TerrainOrAsset == PlayerCapability->TargetType())){
                                                                        auto NewTarget = context->DGameModel->Player(context->DPlayerColor)->CreateMarker(context->ViewportToDetailedMap(ViewportCursorLocation), false);

                                                                        CanApply = PlayerCapability->CanApply(context->DSelectedPlayerAssets.front().lock(), context->DGameModel->Player(context->DPlayerColor), NewTarget);
                                                                    }
                                                                }
                                                                else{
                                                                    if((CPlayerCapability::ETargetType::Asset == PlayerCapability->TargetType())||(CPlayerCapability::ETargetType::TerrainOrAsset == PlayerCapability->TargetType())){
                                                                        auto NewTarget = context->DGameModel->Player(PixelType.Color())->SelectAsset(context->ViewportToDetailedMap(ViewportCursorLocation), PixelType.AssetType()).lock();

                                                                        CanApply = PlayerCapability->CanApply(context->DSelectedPlayerAssets.front().lock(), context->DGameModel->Player(context->DPlayerColor), NewTarget);
                                                                    }
                                                                }

                                                                context->DCursorType = CanApply ? CApplicationData::ctTargetOn : CApplicationData::ctTargetOff;
                                                            }
                                                        }

                                                    }
                                                    break;
        case CApplicationData::uictViewportBevelN:  context->DCursorType = CApplicationData::ctArrowN;
                                                    break;
        case CApplicationData::uictViewportBevelE:  context->DCursorType = CApplicationData::ctArrowE;
                                                    break;
        case CApplicationData::uictViewportBevelS:  context->DCursorType = CApplicationData::ctArrowS;
                                                    break;
        case CApplicationData::uictViewportBevelW:  context->DCursorType = CApplicationData::ctArrowW;
                                                    break;
        default:                                    context->DCursorType = CApplicationData::ctPointer;
                                                    break;
    }
    SRectangle ViewportRectangle({context->DViewportRenderer->ViewportX(),context->DViewportRenderer->ViewportY(),context->DViewportRenderer->LastViewportWidth(),context->DViewportRenderer->LastViewportHeight()});

    context->DSoundEventRenderer->RenderEvents(ViewportRectangle);
   // PrintDebug(DEBUG_LOW, "Finished CBattleMode::Render\n");
}

float CBattleMode::GetTime(){
    return DTime * 50/1000;
}

std::shared_ptr< CApplicationMode > CBattleMode::Instance(){
    if(DBattleModePointer == nullptr){
        DBattleModePointer = std::make_shared< CBattleMode >(SPrivateConstructorType{});
    }

    return DBattleModePointer;
}
