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
#include "SoundEventRenderer.h"
#include <unordered_map>

/**
*
* @class CSoundEventRender
*
* @brief This class maintains the sound event rendering information
*
* @author Hugo
*
* @version 4.0
*
* @date 10/30/17
*
* Contact: huhuang@ucdavis.edu
*
*/


/**
* A CSoundEventRenderer object constructor
*
* @param[in] mixer
* @param[in] player
*
* @return none (Constructor)
*/
CSoundEventRenderer::CSoundEventRenderer(std::shared_ptr< CSoundLibraryMixer > mixer, std::shared_ptr< CPlayerData > player){
    std::string Names[to_underlying(EAssetType::Max)];

    Names[to_underlying(EAssetType::None)] = "basic";
    Names[to_underlying(EAssetType::Peasant)] = "peasant";
    Names[to_underlying(EAssetType::Footman)] = "footman";
    Names[to_underlying(EAssetType::Archer)] = "archer";
    Names[to_underlying(EAssetType::Ranger)] = "archer";
    Names[to_underlying(EAssetType::GoldMine)] = "gold-mine";
    Names[to_underlying(EAssetType::TownHall)] = "town-hall";
    Names[to_underlying(EAssetType::Keep)] = "keep";
    Names[to_underlying(EAssetType::Castle)] = "castle";
    Names[to_underlying(EAssetType::Farm)] = "farm";
    Names[to_underlying(EAssetType::Barracks)] = "barracks";
    Names[to_underlying(EAssetType::LumberMill)] = "lumber-mill";
    Names[to_underlying(EAssetType::Blacksmith)] = "blacksmith";
    Names[to_underlying(EAssetType::ScoutTower)] = "scout-tower";
    Names[to_underlying(EAssetType::GuardTower)] = "guard-tower";
    Names[to_underlying(EAssetType::CannonTower)] = "cannon-tower";
    Names[to_underlying(EAssetType::Wall)] = "wall";

    DSoundMixer = mixer;
    DPlayer = player;
    DVolume = 1.0;

    DDelayedSelectionIndices.resize(to_underlying(EAssetType::Max));
    for(auto &Index : DDelayedSelectionIndices){
        Index = -1;
    }
    DDelayedAcknowledgeIndices.resize(to_underlying(EAssetType::Max));
    for(auto &Index : DDelayedAcknowledgeIndices){
        Index = -1;
    }

    DWorkCompleteIndices.resize(to_underlying(EAssetType::Max));
    DSelectionIndices.resize(to_underlying(EAssetType::Max));
    DAcknowledgeIndices.resize(to_underlying(EAssetType::Max));
    DReadyIndices.resize(to_underlying(EAssetType::Max));
    DDeathIndices.resize(to_underlying(EAssetType::Max));
    DAttackedIndices.resize(to_underlying(EAssetType::Max));
    DMissleFireIndices.resize(to_underlying(EAssetType::Max));
    DMissleHitIndices.resize(to_underlying(EAssetType::Max));
    DHarvestIndices.resize(to_underlying(EAssetType::Max));
    DQuarryIndices.resize(to_underlying(EAssetType::Max));
    DMeleeHitIndices.resize(to_underlying(EAssetType::Max));

    DPlaceIndex = DSoundMixer->FindClip("place");
    DTickIndex = DSoundMixer->FindClip("tick");
    DTockIndex = DSoundMixer->FindClip("tock");
    {
        int ClipIndex = DSoundMixer->FindClip("construct");
        if(0 <= ClipIndex){
            DConstructIndices.push_back(ClipIndex);
        }
        else{
            int SoundIndex = 1;
            while(true){
                ClipIndex = DSoundMixer->FindClip("construct" + std::to_string(SoundIndex));
                if(0 > ClipIndex){
                    break;
                }
                DConstructIndices.push_back(ClipIndex);
                SoundIndex++;
            }
        }
    }

    for(int TypeIndex = 0; TypeIndex < to_underlying(EAssetType::Max); TypeIndex++){
        auto AssetType = DPlayer->AssetTypes()->find(CPlayerAssetType::TypeToName((EAssetType)TypeIndex))->second;
        int ClipIndex;
        ClipIndex = DSoundMixer->FindClip(Names[TypeIndex] + "-work-completed");
        if(0 <= ClipIndex){
            DWorkCompleteIndices[TypeIndex].push_back(ClipIndex);
        }
        else if(DWorkCompleteIndices[to_underlying(EAssetType::None)].size()){
            DWorkCompleteIndices[TypeIndex].push_back(DWorkCompleteIndices[to_underlying(EAssetType::None)].front());
        }
        ClipIndex = DSoundMixer->FindClip(Names[TypeIndex] + "-selected");
        if(0 <= ClipIndex){
            DSelectionIndices[TypeIndex].push_back(ClipIndex);
        }
        else{
            int SoundIndex = 1;
            while(true){
                ClipIndex = DSoundMixer->FindClip(Names[TypeIndex] + "-selected" + std::to_string(SoundIndex));
                if(0 > ClipIndex){
                    break;
                }
                DSelectionIndices[TypeIndex].push_back(ClipIndex);
                SoundIndex++;
            }
            if(!DSelectionIndices[TypeIndex].size() && AssetType->Speed() && DSelectionIndices[to_underlying(EAssetType::None)].size()){
                DSelectionIndices[TypeIndex] = DSelectionIndices[to_underlying(EAssetType::None)];
            }
        }
        ClipIndex = DSoundMixer->FindClip(Names[TypeIndex] + "-acknowledge");
        if(0 <= ClipIndex){
            DAcknowledgeIndices[TypeIndex].push_back(ClipIndex);
        }
        else{
            int SoundIndex = 1;
            while(true){
                ClipIndex = DSoundMixer->FindClip(Names[TypeIndex] + "-acknowledge" + std::to_string(SoundIndex));
                if(0 > ClipIndex){
                    break;
                }
                DAcknowledgeIndices[TypeIndex].push_back(ClipIndex);
                SoundIndex++;
            }
            if(!DAcknowledgeIndices[TypeIndex].size() && DAcknowledgeIndices[to_underlying(EAssetType::None)].size()){
                DAcknowledgeIndices[TypeIndex] = DAcknowledgeIndices[to_underlying(EAssetType::None)];
            }
        }
        ClipIndex = DSoundMixer->FindClip(Names[TypeIndex] + "-ready");
        if(0 <= ClipIndex){
            DReadyIndices[TypeIndex].push_back(ClipIndex);
        }
        else if(to_underlying(EAssetType::Footman) == TypeIndex){
            ClipIndex = DSoundMixer->FindClip(Names[to_underlying(EAssetType::None)] + "-ready");
            if(0 <= ClipIndex){
                DReadyIndices[TypeIndex].push_back(ClipIndex);
            }
        }
        std::string UnitBuildingName = AssetType->Speed() ? "unit" : "building";
        ClipIndex = DSoundMixer->FindClip(UnitBuildingName + "-death");
        if(0 <= ClipIndex){
            DDeathIndices[TypeIndex].push_back(ClipIndex);
        }
        else{
            int SoundIndex = 1;
            while(true){
                ClipIndex = DSoundMixer->FindClip(UnitBuildingName + "-death" + std::to_string(SoundIndex));
                if(0 > ClipIndex){
                    break;
                }
                DDeathIndices[TypeIndex].push_back(ClipIndex);
                SoundIndex++;
            }
            if(!DDeathIndices[TypeIndex].size() && DDeathIndices[to_underlying(EAssetType::None)].size()){
                DDeathIndices[TypeIndex] = DDeathIndices[to_underlying(EAssetType::None)];
            }
        }

        ClipIndex = DSoundMixer->FindClip(UnitBuildingName + "-help");
        if(0 <= ClipIndex){
            DAttackedIndices[TypeIndex].push_back(ClipIndex);
        }
        else{
            int SoundIndex = 1;
            while(true){
                ClipIndex = DSoundMixer->FindClip(UnitBuildingName + "-help" + std::to_string(SoundIndex));
                if(0 > ClipIndex){
                    break;
                }
                DAttackedIndices[TypeIndex].push_back(ClipIndex);
                SoundIndex++;
            }
            if(!DAttackedIndices[TypeIndex].size() && DAttackedIndices[to_underlying(EAssetType::None)].size()){
                DAttackedIndices[TypeIndex] = DAttackedIndices[to_underlying(EAssetType::None)];
            }
        }
        if((to_underlying(EAssetType::Archer) == TypeIndex)||(to_underlying(EAssetType::Ranger) == TypeIndex)||(to_underlying(EAssetType::GuardTower) == TypeIndex)){
            DMissleFireIndices[TypeIndex].push_back(DSoundMixer->FindClip("bowfire"));
            DMissleHitIndices[TypeIndex].push_back(DSoundMixer->FindClip("bowhit"));
        }
        else if(to_underlying(EAssetType::CannonTower) == TypeIndex){
            DMissleFireIndices[TypeIndex].push_back(DSoundMixer->FindClip("cannonfire"));
            DMissleHitIndices[TypeIndex].push_back(DSoundMixer->FindClip("cannonhit"));
        }
        ClipIndex = DSoundMixer->FindClip("harvest");
        if(0 <= ClipIndex){
            DAttackedIndices[TypeIndex].push_back(ClipIndex);
        }
        else{
            int SoundIndex = 1;
            while(true){
                ClipIndex = DSoundMixer->FindClip("harvest" + std::to_string(SoundIndex));
                if(0 > ClipIndex){
                    break;
                }
                DHarvestIndices[TypeIndex].push_back(ClipIndex);
                SoundIndex++;
            }
            if(!DHarvestIndices[TypeIndex].size() && DHarvestIndices[to_underlying(EAssetType::None)].size()){
                DHarvestIndices[TypeIndex] = DHarvestIndices[to_underlying(EAssetType::None)];
            }
        }
        ClipIndex = DSoundMixer->FindClip("quarry");
        if(0 <= ClipIndex){
            DAttackedIndices[TypeIndex].push_back(ClipIndex);
        }
        else{
            int SoundIndex = 1;
            while(true){
                ClipIndex = DSoundMixer->FindClip("quarry" + std::to_string(SoundIndex));
                if(0 > ClipIndex){
                    break;
                }
                DQuarryIndices[TypeIndex].push_back(ClipIndex);
                SoundIndex++;
            }
            if(!DQuarryIndices[TypeIndex].size() && DQuarryIndices[to_underlying(EAssetType::None)].size()){
                DQuarryIndices[TypeIndex] = DQuarryIndices[to_underlying(EAssetType::None)];
            }
        }
        if(1 == AssetType->Range()){
            ClipIndex = DSoundMixer->FindClip("melee-hit");
            if(0 <= ClipIndex){
                DMeleeHitIndices[TypeIndex].push_back(ClipIndex);
            }
            else{
                int SoundIndex = 1;
                while(true){
                    ClipIndex = DSoundMixer->FindClip("melee-hit" + std::to_string(SoundIndex));
                    if(0 > ClipIndex){
                        break;
                    }
                    DMeleeHitIndices[TypeIndex].push_back(ClipIndex);
                    SoundIndex++;
                }
                if(!DMeleeHitIndices[TypeIndex].size() && DMeleeHitIndices[to_underlying(EAssetType::None)].size()){
                    DMeleeHitIndices[TypeIndex] = DMeleeHitIndices[to_underlying(EAssetType::None)];
                }
            }
        }
    }

}

/**
* RightBias function
*
* @param[in] viewportrect
* @param[in] position
*
* @return float
*/
float CSoundEventRenderer::RightBias(const SRectangle &viewportrect, const CPosition &position){
    int LeftX, RightX, CenterX;

    LeftX = viewportrect.DXPosition;
    RightX = viewportrect.DXPosition + viewportrect.DWidth - 1;
    CenterX = (LeftX + RightX) / 2;

    if(position.X() <= LeftX){
        return -1.0;
    }
    if(position.X() >= RightX){
        return 1.0;
    }
    if(LeftX == CenterX){
        return 0.0;
    }
    if(position.X() < CenterX){
        return 1.0 - ((float)(position.X() - LeftX)) / (CenterX - LeftX);
    }
    return ((float)(position.X() - CenterX)) / (RightX - CenterX);
}

/**
* A CSoundEventRenderer object constructor
*
* @param[in] viewportrect
* @param[in] position
*
* @return bool
*/
bool CSoundEventRenderer::OnScreen(const SRectangle &viewportrect, const CPosition &position){
    int LeftX, RightX, TopY, BottomY;

    LeftX = viewportrect.DXPosition - CPosition::TileWidth();
    if(position.X() < LeftX){
        return false;
    }
    RightX = viewportrect.DXPosition + viewportrect.DWidth + CPosition::TileWidth()- 1;
    if(position.X() > RightX){
        return false;
    }
    TopY = viewportrect.DYPosition - CPosition::TileHeight();
    if(position.Y() < TopY){
        return false;
    }
    BottomY = viewportrect.DYPosition + viewportrect.DHeight + CPosition::TileHeight()- 1;
    if(position.Y() > BottomY){
        return false;
    }
    return true;
}

/**
* A function to render all sound clips
*
* @param[in] viewportrect
*
* @return none
*/
void CSoundEventRenderer::RenderEvents(const SRectangle &viewportrect){
    int MainRandomNumber = DRandomNumberGenerator.Random();
    std::vector< SGameEvent > AllEvents = DDelayedEvents;
    std::vector< int > Selections, Acknowledges;

    DDelayedEvents.clear();
    Selections.resize(to_underlying(EAssetType::Max));
    Acknowledges.resize(to_underlying(EAssetType::Max));

    AllEvents.insert(AllEvents.end(), DPlayer->GameEvents().begin(), DPlayer->GameEvents().end());
    for(auto Event : AllEvents){
        if(EEventType::Selection == Event.DType){
            if(Event.DAsset){
                if((EPlayerColor::None == Event.DAsset->Color())||(DPlayer->Color() == Event.DAsset->Color())){
                    if(EAssetAction::Construct == Event.DAsset->Action()){
                        if(DConstructIndices.size()){
                            int RandomClip = MainRandomNumber % DConstructIndices.size();

                            DSoundMixer->PlayClip(DConstructIndices[RandomClip], DVolume, RightBias(viewportrect, Event.DAsset->Position()));
                        }
                    }
                    else if(DSelectionIndices[to_underlying(Event.DAsset->Type())].size()){
                        if(1 > Selections[to_underlying(Event.DAsset->Type())]){
                            int RandomClip = DRandomNumberGenerator.Random() % DSelectionIndices[to_underlying(Event.DAsset->Type())].size();
                            if(0 > DDelayedSelectionIndices[to_underlying(Event.DAsset->Type())]){
                                DDelayedSelectionIndices[to_underlying(Event.DAsset->Type())] = RandomClip;
                            }
                            else{
                                RandomClip = DDelayedSelectionIndices[to_underlying(Event.DAsset->Type())];
                            }
                            DSoundMixer->PlayClip(DSelectionIndices[to_underlying(Event.DAsset->Type())][RandomClip], DVolume, RightBias(viewportrect, Event.DAsset->Position()));
                            Selections[to_underlying(Event.DAsset->Type())]++;
                        }
                        else if(0 == (DRandomNumberGenerator.Random() & 0x3)){
                            DDelayedEvents.push_back(Event);
                        }
                    }
                }
            }
        }
        else if(EEventType::Acknowledge == Event.DType){
            if(Event.DAsset){
                if((EPlayerColor::None == Event.DAsset->Color())||(DPlayer->Color() == Event.DAsset->Color())){
                    if(DAcknowledgeIndices[to_underlying(Event.DAsset->Type())].size()){
                        if(1 > Acknowledges[to_underlying(Event.DAsset->Type())]){
                            int RandomClip = MainRandomNumber % DAcknowledgeIndices[to_underlying(Event.DAsset->Type())].size();

                            if(0 > DDelayedAcknowledgeIndices[to_underlying(Event.DAsset->Type())]){
                                DDelayedAcknowledgeIndices[to_underlying(Event.DAsset->Type())] = RandomClip;
                            }
                            else{
                                RandomClip = DDelayedAcknowledgeIndices[to_underlying(Event.DAsset->Type())];
                            }
                            DSoundMixer->PlayClip(DAcknowledgeIndices[to_underlying(Event.DAsset->Type())][RandomClip], DVolume, RightBias(viewportrect, Event.DAsset->Position()));
                            Acknowledges[to_underlying(Event.DAsset->Type())]++;
                        }
                        else if(0 == (DRandomNumberGenerator.Random() & 0x3)){
                            DDelayedEvents.push_back(Event);
                        }
                    }
                }
            }
        }
        else if(EEventType::WorkComplete == Event.DType){
            if(Event.DAsset){
                if(DPlayer->Color() == Event.DAsset->Color()){
                    if(DWorkCompleteIndices[to_underlying(Event.DAsset->Type())].size()){
                        int RandomClip = DRandomNumberGenerator.Random() % DWorkCompleteIndices[to_underlying(Event.DAsset->Type())].size();

                        DSoundMixer->PlayClip(DWorkCompleteIndices[to_underlying(Event.DAsset->Type())][RandomClip], DVolume, RightBias(viewportrect, Event.DAsset->Position()));
                    }
                }
            }
        }
        else if(EEventType::Ready == Event.DType){
            if(Event.DAsset){
                if((EPlayerColor::None == Event.DAsset->Color())||(DPlayer->Color() == Event.DAsset->Color())){
                    if(DReadyIndices[to_underlying(Event.DAsset->Type())].size()){
                        int RandomClip = DRandomNumberGenerator.Random() % DReadyIndices[to_underlying(Event.DAsset->Type())].size();

                        DSoundMixer->PlayClip(DReadyIndices[to_underlying(Event.DAsset->Type())][RandomClip], DVolume, RightBias(viewportrect, Event.DAsset->Position()));
                    }
                }
            }
        }
        else if(EEventType::Death == Event.DType){
            if(Event.DAsset){
                if(OnScreen(viewportrect, Event.DAsset->Position())){
                    if(DDeathIndices[to_underlying(Event.DAsset->Type())].size()){
                        int RandomClip = DRandomNumberGenerator.Random() % DDeathIndices[to_underlying(Event.DAsset->Type())].size();

                        DSoundMixer->PlayClip(DDeathIndices[to_underlying(Event.DAsset->Type())][RandomClip], DVolume, RightBias(viewportrect, Event.DAsset->Position()));
                    }
                }
            }
        }
        else if(EEventType::Attacked == Event.DType){
            if(Event.DAsset){
                if(!OnScreen(viewportrect, Event.DAsset->Position())){
                    if(DAttackedIndices[to_underlying(Event.DAsset->Type())].size()){
                        int RandomClip = DRandomNumberGenerator.Random() % DAttackedIndices[to_underlying(Event.DAsset->Type())].size();

                        DSoundMixer->PlayClip(DAttackedIndices[to_underlying(Event.DAsset->Type())][RandomClip], DVolume, RightBias(viewportrect, Event.DAsset->Position()));
                    }
                }
            }
        }
        else if(EEventType::MissleFire == Event.DType){
            if(Event.DAsset){
                if(OnScreen(viewportrect, Event.DAsset->Position())){
                    if(DMissleFireIndices[to_underlying(Event.DAsset->Type())].size()){
                        int RandomClip = DRandomNumberGenerator.Random() % DMissleFireIndices[to_underlying(Event.DAsset->Type())].size();

                        DSoundMixer->PlayClip(DMissleFireIndices[to_underlying(Event.DAsset->Type())][RandomClip], DVolume, RightBias(viewportrect, Event.DAsset->Position()));
                    }
                }
            }
        }
        else if(EEventType::MissleHit == Event.DType){
            if(Event.DAsset){
                if(OnScreen(viewportrect, Event.DAsset->Position())){
                    auto CreationCommand = Event.DAsset->NextCommand(); // Find out type of missle

                    if((EAssetAction::Construct == CreationCommand.DAction) && CreationCommand.DAssetTarget){
                        if(DMissleHitIndices[to_underlying(CreationCommand.DAssetTarget->Type())].size()){
                            int RandomClip = DRandomNumberGenerator.Random() % DMissleHitIndices[to_underlying(CreationCommand.DAssetTarget->Type())].size();

                            DSoundMixer->PlayClip(DMissleHitIndices[to_underlying(CreationCommand.DAssetTarget->Type())][RandomClip], DVolume, RightBias(viewportrect, Event.DAsset->Position()));
                        }
                    }
                }
            }
        }
        else if(EEventType::Harvest == Event.DType){
            if(Event.DAsset){
                if(OnScreen(viewportrect, Event.DAsset->Position()) && (Event.DAsset->AttackSteps()-1 == (Event.DAsset->Step() % Event.DAsset->AttackSteps()))){
                    if(DHarvestIndices[to_underlying(Event.DAsset->Type())].size()){
                        int RandomClip = DRandomNumberGenerator.Random() % DHarvestIndices[to_underlying(Event.DAsset->Type())].size();

                        DSoundMixer->PlayClip(DHarvestIndices[to_underlying(Event.DAsset->Type())][RandomClip], DVolume, RightBias(viewportrect, Event.DAsset->Position()));
                    }
                }
            }
        }
        else if(EEventType::Quarry == Event.DType){
            if(Event.DAsset){
                if(OnScreen(viewportrect, Event.DAsset->Position()) && (Event.DAsset->AttackSteps()-1 == (Event.DAsset->Step() % Event.DAsset->AttackSteps()))){
                    if(DQuarryIndices[to_underlying(Event.DAsset->Type())].size()){
                        int RandomClip = DRandomNumberGenerator.Random() % DQuarryIndices[to_underlying(Event.DAsset->Type())].size();

                        DSoundMixer->PlayClip(DQuarryIndices[to_underlying(Event.DAsset->Type())][RandomClip], DVolume, RightBias(viewportrect, Event.DAsset->Position()));
                    }
                }
            }
        }
        else if(EEventType::MeleeHit == Event.DType){
            if(Event.DAsset){
                if(OnScreen(viewportrect, Event.DAsset->Position())){
                    if(DMeleeHitIndices[to_underlying(Event.DAsset->Type())].size()){
                        int RandomClip = DRandomNumberGenerator.Random() % DMeleeHitIndices[to_underlying(Event.DAsset->Type())].size();

                        DSoundMixer->PlayClip(DMeleeHitIndices[to_underlying(Event.DAsset->Type())][RandomClip], DVolume, RightBias(viewportrect, Event.DAsset->Position()));
                    }
                }
            }
        }
        else if(EEventType::PlaceAction == Event.DType){
            if(Event.DAsset){
                if(0 <= DPlaceIndex){
                    DSoundMixer->PlayClip(DPlaceIndex, DVolume, RightBias(viewportrect, Event.DAsset->Position()));
                }
            }

        }
        else if(EEventType::ButtonTick == Event.DType){
            if(0 <= DTickIndex){
                DSoundMixer->PlayClip(DTickIndex, DVolume, 0.0);
            }
        }
    }

    for(int Index = 0; Index < to_underlying(EAssetType::Max); Index++){
        if(0 == Selections[Index]){
            DDelayedSelectionIndices[Index] = -1;
        }
        if(0 == Acknowledges[Index]){
            DDelayedAcknowledgeIndices[Index] = -1;
        }
    }

}
