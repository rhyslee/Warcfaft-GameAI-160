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
 * @class CPlayerCapabilityTrainNormal 
 *
 * @ brief This class was written to create "train" different units for its associated building \n
 *   <pre>
 *    The class contains 2 other clases: CRegistrant and CActivatedCapability.
 *    CRegistrant determines which unit to create: Peasant, Footman, Archer. 
 *    CActivatedCapability calls several functions: PercentComplete, IncrementStep, and Cancel that determine the status of creating "training" the unit 
 *   </pre>
 *   
 *     Public Member Functions:
 *     CanInitiate: to return true if player is eligible to create "train" unit. 
 *     CanApply: to return true after calling CanInitiate function.
 *     ApplyCapability: to return true once unit is created. 
 *
 * @author Wonhee
 *
 * @version 1.0
 *
 * @date 10/14/2017 21:04:00
 *
 * Contact: wonpark@ucdavis.edu 
 *
 */
class CPlayerCapabilityTrainNormal : public CPlayerCapability{
    protected:

        /**
         * @class CRegistrant
         *
         * @ brief This class determines which unit will be created: Peasant, Footman, Archer \n
         *  
         * @author Wonhee
         *
         * @version 1.0
         *
         * @date 10/14/2017 21:04:00
         *
         * Contact: wonpark@ucdavis.edu 
         *
         */
        class CRegistrant{
            public:  
                CRegistrant();
        };
        static CRegistrant DRegistrant;
        
         /**
         * @class CActivatedCapability
         *
         * @ brief This calls several functions: PercentComplete, IncrementStep, and Cancel that determine the status of creating "training" the unit \n
         *  
         * @author Wonhee
         *
         * @version 1.0
         *
         * @date 10/14/2017 21:04:00
         *
         * Contact: wonpark@ucdavis.edu 
         *
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
        std::string DUnitName;
        CPlayerCapabilityTrainNormal(const std::string &unitname);
        
    public:
        //destructor
        virtual ~CPlayerCapabilityTrainNormal(){};
        
        bool CanInitiate(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata);
        bool CanApply(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata, std::shared_ptr< CPlayerAsset > target);
        bool ApplyCapability(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata, std::shared_ptr< CPlayerAsset > target);
};

//creating new instance from CRegistrant
CPlayerCapabilityTrainNormal::CRegistrant CPlayerCapabilityTrainNormal::DRegistrant;

//constructor 
CPlayerCapabilityTrainNormal::CRegistrant::CRegistrant(){
    CPlayerCapability::Register(std::shared_ptr< CPlayerCapabilityTrainNormal >(new CPlayerCapabilityTrainNormal("Peasant")));   
    CPlayerCapability::Register(std::shared_ptr< CPlayerCapabilityTrainNormal >(new CPlayerCapabilityTrainNormal("Footman")));
    CPlayerCapability::Register(std::shared_ptr< CPlayerCapabilityTrainNormal >(new CPlayerCapabilityTrainNormal("Archer")));   
}

//constructor 
CPlayerCapabilityTrainNormal::CPlayerCapabilityTrainNormal(const std::string &unitname) : CPlayerCapability(std::string("Build") + unitname, ETargetType::None){
    DUnitName = unitname;
}

/**
* checks if player is eligible to create "train" unit
*     <pre>
*     player's Lumber should be greater than lumbercost for building upgrade.
*     player's Gold should be greater than goldcost for building upgrade.
*     player's FoodProduction should be greater than player + unit's food consumption
*     </pre>
*
* @param[in] actor shared pointer from class PlayerAsset.
* @param[in] playerdata shared pointer from class PlayerData
*
* @return boolean value
*
*/
bool CPlayerCapabilityTrainNormal::CanInitiate(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata){
    auto Iterator = playerdata->AssetTypes()->find(DUnitName);
    
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
        if((AssetType->FoodConsumption() + playerdata->FoodConsumption()) > playerdata->FoodProduction()){
            return false;    
        }
        if(!playerdata->AssetRequirementsMet(DUnitName)){
            return false;
        }
    }
    
    return true;
}

/**
* calls CanInitiate function to see if training unit requirements met. 
*
* @param[in] actor shared pointer from class PlayerAsset.
* @param[in] playerdata shared pointer from class PlayerData
* @param[in] target shared pointer from class PlayerAsset
*
* @return boolean value
*
*/
bool CPlayerCapabilityTrainNormal::CanApply(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata, std::shared_ptr< CPlayerAsset > target){
    return CanInitiate(actor, playerdata);
}

/**
* applies creating "training" of unit
*     <pre>
*     create asset and place tileposition.
*     have actor apply building upgrade according to action, capability, asset target, activatedcapability. 
*     </pre>
*
* @param[in] actor shared pointer from class PlayerAsset.
* @param[in] playerdata shared pointer from class PlayerData
* @param[in] target shared pointer from class PlayerAsset
*
* @return boolean value
*
*/
bool CPlayerCapabilityTrainNormal::ApplyCapability(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata, std::shared_ptr< CPlayerAsset > target){
    auto Iterator = playerdata->AssetTypes()->find(DUnitName);
    
    if(Iterator != playerdata->AssetTypes()->end()){
        auto AssetType = Iterator->second;
        auto NewAsset = playerdata->CreateAsset(DUnitName);
        SAssetCommand NewCommand;
        CTilePosition TilePosition;
        TilePosition.SetFromPixel(actor->Position());
        NewAsset->TilePosition(TilePosition);
        NewAsset->HitPoints(1);
        
        NewCommand.DAction = EAssetAction::Capability;
        NewCommand.DCapability = AssetCapabilityType();
        NewCommand.DAssetTarget = NewAsset;
        NewCommand.DActivatedCapability = std::make_shared< CActivatedCapability >(actor, playerdata, NewAsset, AssetType->LumberCost(), AssetType->GoldCost(), AssetType->StoneCost(), CPlayerAsset::UpdateFrequency() * AssetType->BuildTime());
        actor->PushCommand(NewCommand);
        actor->ResetStep();
    }
    return false;
}

/**
* constructor 
*
* @param[in] actor shared pointer from class PlayerAsset
* @param[in] playerdata shared pointer from class PlayerData
* @param[in] target shared pointer from class PlayerAsset
* @param[in] origtype shared pointer from class PlayerAssetType
* @param[in] upgradetype shared pointer from class PlayerAssetType
* @param[in] int amt of lumber
* @param[in] int amt of gold
* @param[in] int amt of steps
*
*/
CPlayerCapabilityTrainNormal::CActivatedCapability::CActivatedCapability(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata, std::shared_ptr< CPlayerAsset > target, int lumber, int gold, int stone, int steps) :
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
* completion of action (shown via the health bar in the game UI)
*
* @param[in] max health value 
*
* @return int value of health completion 
*
*/
int CPlayerCapabilityTrainNormal::CActivatedCapability::PercentComplete(int max){
    return DCurrentStep * max / DTotalSteps;
}

/**
* Set/initilize the next steps until training unit is complete. 
*
* @param[in] nothing
*
* @return boolean value
*
*/
bool CPlayerCapabilityTrainNormal::CActivatedCapability::IncrementStep(){
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
        
        TempEvent.DType = EEventType::Ready;
        TempEvent.DAsset = DTarget;
        DPlayerData->AddGameEvent(TempEvent);
        
        DTarget->PopCommand();
        DActor->PopCommand();
        DTarget->TilePosition(DPlayerData->PlayerMap()->FindAssetPlacement(DTarget, DActor, CTilePosition(DPlayerData->PlayerMap()->Width()-1, DPlayerData->PlayerMap()->Height()-1)));
        return true;    
    }
    return false;
}

/**
* Cancels training capability and reincrements the assets for lumber and gold and pops command. 
*
* @param[in] nothing
*
* @return nothing
*
*/
void CPlayerCapabilityTrainNormal::CActivatedCapability::Cancel(){
    DPlayerData->IncrementLumber(DLumber);
    DPlayerData->IncrementGold(DGold);
    DPlayerData->DeleteAsset(DTarget);
    DActor->PopCommand();
}


