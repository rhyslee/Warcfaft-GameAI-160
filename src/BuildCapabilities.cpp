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
#include "GameModel.h"
#include "Debug.h"

// Build normal buildings capability

/**
 *
 * @class CPlayerCapabilityMove
 *
 * @ brief This class was written to give player building capability \n
 *   <pre>
 *    The class contains two other classes which are CRegistrant and CActivatedCapability.
 *    They are responsible for determining the building which should build and activation of
 *    capability. The class also keep tracks of percentge compeltion of the capability
 *    the steps and cancellation of capability.
 *   </pre>
 * @author Seza
 *
 * @version 4.0
 *
 * @date 10/13/2017 21:04:00
 *
 * Contact: shabibi@ucdavis.edu
 *
 *     CanInitiate:<public> to return true if player can start to move
 *     CanApply:<public> to return true if player can accept capability
 *     ApplyCapability:<public> to apply move capability to target and return true
 */
class CPlayerCapabilityBuildNormal : public CPlayerCapability{
    protected:

      /**
       *
       *@class CRegistrant
       *
       * @brief the class was written to determine the building which should build
       *
       * @author Seza
       *
       * @version 1.0
       *
       * @date 10/13/2017 23:17:00
       *
       * Contact: shabibi@ucdavis.edu
       */
        class CRegistrant{
            public:
                CRegistrant();
        };
        static CRegistrant DRegistrant;

        /**
         *
         *@class CActivatedCapability
         *
         * @brief the class was written to activate building capability
         *     <pre>
         *      The class is responsible for showing the percentage completion until
         *      the player get the capability,following steps to get capability, and
         *      cancellation of capability.
         *     </pre>
         * @author Seza
         *
         * @version 1.0
         *
         * @date 10/13/2017 23:17:00
         *
         * Contact: shabibi@ucdavis.edu
         */
        class CActivatedCapability : public CActivatedPlayerCapability{
            protected:
                int DCurrentStep;
                int DTotalSteps;
                int DLumber;
                int DGold;
                int DStone;

            public:
                CActivatedCapability(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata, std::shared_ptr< CPlayerAsset > target, int lumber, int gold, int stone, int steps);
                virtual ~CActivatedCapability(){};

                int PercentComplete(int max);
                bool IncrementStep();
                void Cancel();
        };
        std::string DBuildingName;
        CPlayerCapabilityBuildNormal(const std::string &buildingname);

    public:
        virtual ~CPlayerCapabilityBuildNormal(){};

        bool CanInitiate(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata);
        bool CanApply(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata, std::shared_ptr< CPlayerAsset > target);
        bool ApplyCapability(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata, std::shared_ptr< CPlayerAsset > target);
};

CPlayerCapabilityBuildNormal::CRegistrant CPlayerCapabilityBuildNormal::DRegistrant;

CPlayerCapabilityBuildNormal::CRegistrant::CRegistrant(){
    CPlayerCapability::Register(std::shared_ptr< CPlayerCapabilityBuildNormal >(new CPlayerCapabilityBuildNormal("TownHall")));
    CPlayerCapability::Register(std::shared_ptr< CPlayerCapabilityBuildNormal >(new CPlayerCapabilityBuildNormal("Farm")));
    CPlayerCapability::Register(std::shared_ptr< CPlayerCapabilityBuildNormal >(new CPlayerCapabilityBuildNormal("Barracks")));
    CPlayerCapability::Register(std::shared_ptr< CPlayerCapabilityBuildNormal >(new CPlayerCapabilityBuildNormal("LumberMill")));
    CPlayerCapability::Register(std::shared_ptr< CPlayerCapabilityBuildNormal >(new CPlayerCapabilityBuildNormal("Blacksmith")));
    CPlayerCapability::Register(std::shared_ptr< CPlayerCapabilityBuildNormal >(new CPlayerCapabilityBuildNormal("ScoutTower")));
    CPlayerCapability::Register(std::shared_ptr< CPlayerCapabilityBuildNormal >(new CPlayerCapabilityBuildNormal("Wall")));

}

CPlayerCapabilityBuildNormal::CPlayerCapabilityBuildNormal(const std::string &buildingname) : CPlayerCapability(std::string("Build") + buildingname, ETargetType::TerrainOrAsset){
    DBuildingName = buildingname;
}

/**
* It will check if the building can be initiated. condition:
*     <pre>
*     LumberCost must be smaller than Lumber
*     GoldCost must be smaller than Gold
*     </pre>
*
* @param[in] actor share pointer to the class of PlayerAsset.
* @param[in] playerdata  share pointer to the class of PlayerData
*
* @return boolean function if the building should  intiated or not
*
*/
bool CPlayerCapabilityBuildNormal::CanInitiate(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata){
    auto Iterator = playerdata->AssetTypes()->find(DBuildingName);

    if(Iterator != playerdata->AssetTypes()->end()){
        auto AssetType = Iterator->second;
        if(AssetType->LumberCost() > playerdata->Lumber()){
            return false;
        }
        if(AssetType->GoldCost() > playerdata->Gold()){
            return false;
        }
        if(AssetType->StoneCost() > playerdata->Stone()){
            return false;
        }
    }

    return true;
}

/**
* It will check if the building capability can be applied. condition:
*     <pre>
*     actor must equal target and AssetTypes should equal Target type
*     LumberCost must be smaller than Lumber
*     GoldCost must be smaller than Gold
*     CanPlaceAsset function should return true
*     </pre>
*
* @param[in] actor share pointer to the class of PlayerAsset.
* @param[in] playerdata  share pointer to the class of PlayerData
* @param[in] playerdata  share pointer to the class of PlayerAsset
*
* @return boolian function if the building can be  applied or not
*
*/
bool CPlayerCapabilityBuildNormal::CanApply(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata, std::shared_ptr< CPlayerAsset > target){
    auto Iterator = playerdata->AssetTypes()->find(DBuildingName);

    if((actor != target)&&(EAssetType::None != target->Type())){
        return false;
    }
    if(Iterator != playerdata->AssetTypes()->end()){
        auto AssetType = Iterator->second;

        if(AssetType->LumberCost() > playerdata->Lumber()){
            return false;
        }
        if(AssetType->GoldCost() > playerdata->Gold()){
            return false;
        }
        if(AssetType->StoneCost() > playerdata->Stone()){
            return false;
        }
        if(!playerdata->PlayerMap()->CanPlaceAsset(target->TilePosition(), AssetType->Size(), actor)){
            return false;
        }
    }

    return true;
}

/**
* Set a new building capability. using a struct to define a new command and set it\n
* to NewAsset and get the position to put it in that Position
*
* @param[in] actor share pointer to the class of PlayerAsset.
* @param[in] playerdata  share pointer to the class of PlayerData
* @param[in] playerdata  share pointer to the class of PlayerAsset
*
* @return boolean function if the actor gotc apability or not.
*
*/
bool CPlayerCapabilityBuildNormal::ApplyCapability(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata, std::shared_ptr< CPlayerAsset > target){
    auto Iterator = playerdata->AssetTypes()->find(DBuildingName);

    if(Iterator != playerdata->AssetTypes()->end()){
        SAssetCommand NewCommand;

        actor->ClearCommand();
        if(actor->TilePosition() == target->TilePosition()){
            auto AssetType = Iterator->second;
            auto NewAsset = playerdata->CreateAsset(DBuildingName);
            CTilePosition TilePosition;
            TilePosition.SetFromPixel(target->Position());
            NewAsset->TilePosition(TilePosition);
            NewAsset->HitPoints(1);

            NewCommand.DAction = EAssetAction::Capability;
            NewCommand.DCapability = AssetCapabilityType();
            NewCommand.DAssetTarget = NewAsset;
            NewCommand.DActivatedCapability = std::make_shared< CActivatedCapability >(actor, playerdata, NewAsset, AssetType->LumberCost(), AssetType->GoldCost(), AssetType->StoneCost(), CPlayerAsset::UpdateFrequency() * AssetType->BuildTime());
            actor->PushCommand(NewCommand);
        }
        else{
            NewCommand.DAction = EAssetAction::Capability;
            NewCommand.DCapability = AssetCapabilityType();
            NewCommand.DAssetTarget = target;
            actor->PushCommand(NewCommand);

            NewCommand.DAction = EAssetAction::Walk;
            actor->PushCommand(NewCommand);
        }
        return true;
    }
    return false;
}

CPlayerCapabilityBuildNormal::CActivatedCapability::CActivatedCapability(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata, std::shared_ptr< CPlayerAsset > target, int lumber, int gold, int stone, int steps) :
CActivatedPlayerCapability(actor, playerdata, target){
    SAssetCommand AssetCommand;

    DCurrentStep = 0;
    DTotalSteps = steps;
    DLumber = lumber;
    DGold = gold;
    DStone = stone;
    DPlayerData->DecrementLumber(DLumber);
    DPlayerData->DecrementGold(DGold);
    DPlayerData->DecrementStone(DStone);
    AssetCommand.DAction = EAssetAction::Construct;
    AssetCommand.DAssetTarget = DActor;
    DTarget->PushCommand(AssetCommand);
}

/**
* Show the percentage completion of the capability.
*
* @param[in] max the number of max percent complete.
*
* @return number of percentage complete.
*
*/
int CPlayerCapabilityBuildNormal::CActivatedCapability::PercentComplete(int max){
    return DCurrentStep * max / DTotalSteps;
}

/**
* Set/Initialize the net steps until building is complete .
*
* @param[in] it does not have a parameter.
*
* @return boolean function if actor gain the capability or not.
*
*/
bool CPlayerCapabilityBuildNormal::CActivatedCapability::IncrementStep(){
    int AddHitPoints = (DTarget->MaxHitPoints() * (DCurrentStep + 1) / DTotalSteps) - (DTarget->MaxHitPoints() * DCurrentStep / DTotalSteps);

    DTarget->IncrementHitPoints(AddHitPoints);
    if(DTarget->HitPoints() > DTarget->MaxHitPoints()){
        DTarget->HitPoints(DTarget->MaxHitPoints());
    }
    DCurrentStep++;
    DActor->IncrementStep();
    DTarget->IncrementStep();
    if(DCurrentStep >= DTotalSteps){
        DPlayerData->ResolveNewAssetCounts();
        DPlayerData->IncrementCreated(DTarget);

        SGameEvent TempEvent;

        TempEvent.DType = EEventType::WorkComplete;
        TempEvent.DAsset = DActor;
        DPlayerData->AddGameEvent(TempEvent);

        DTarget->PopCommand();
        DActor->PopCommand();
        DActor->TilePosition(DPlayerData->PlayerMap()->FindAssetPlacement(DActor, DTarget, CTilePosition(DPlayerData->PlayerMap()->Width()-1, DPlayerData->PlayerMap()->Height()-1)));
        DActor->ResetStep();
        DTarget->ResetStep();

        return true;
    }
    return false;
}

/**
* Cancel the building capability.
*
* @param[in] it does not have a parameter.
*
* @return nothing.
*
*/
void CPlayerCapabilityBuildNormal::CActivatedCapability::Cancel(){
    DPlayerData->IncrementLumber(DLumber);
    DPlayerData->IncrementGold(DGold);
    DPlayerData->IncrementStone(DStone);
    DPlayerData->DeleteAsset(DTarget);
    DActor->PopCommand();
}
