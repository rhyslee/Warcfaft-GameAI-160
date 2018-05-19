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

/**
 * @class CPlayerCapabilityBuildingUpgrade
 *
 * @ brief This class was written to give player building upgrade capability \n
 *   <pre>
 *    The class contains 2 other clases: CRegistrant and CActivatedCapability.
 *    CRegistrant determines which building will be ugraded: Keep, Castle, GuardTower, KeepTower.
 *    CActivatedCapability calls several functions: PercentComplete, IncrementStep, Cancel that determine the status of applying the building upgrade.
 *   </pre>
 *     
 *     Public Member Functions:
 *     CanInitiate: to return true if player is eligible for building upgrade. 
 *     CanApply: to return true after calling CanInitiate function.
 *     ApplyCapability: to return true once building upgrade is applied. 
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
class CPlayerCapabilityBuildingUpgrade : public CPlayerCapability{
    protected:
        /**
         * @class CRegistrant
         *
         * @ brief This class determines which building will be ugraded: Keep, Castle, GuardTower, KeepTower \n
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
         * @ brief This calls several functions: PercentComplete, IncrementStep, and Cancel that determine the status of applying the building upgrade. \n
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
                std::shared_ptr< CPlayerAssetType > DOriginalType;
                std::shared_ptr< CPlayerAssetType > DUpgradeType;
                int DCurrentStep;
                int DTotalSteps;
                int DLumber;
                int DGold;
                int DStone;
                
            public:
                CActivatedCapability(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata, std::shared_ptr< CPlayerAsset > target, std::shared_ptr< CPlayerAssetType > origtype, std::shared_ptr< CPlayerAssetType > upgradetype, int lumber, int gold, int stone, int steps);
                virtual ~CActivatedCapability(){};
                
                int PercentComplete(int max);
                bool IncrementStep();
                void Cancel();
        };
        std::string DBuildingName;
        CPlayerCapabilityBuildingUpgrade(const std::string &buildingname);
        
    public:
        //destructor
        virtual ~CPlayerCapabilityBuildingUpgrade(){};
        
        bool CanInitiate(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata);
        bool CanApply(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata, std::shared_ptr< CPlayerAsset > target);
        bool ApplyCapability(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata, std::shared_ptr< CPlayerAsset > target);
};

// creating new instance from CRegistrant 
CPlayerCapabilityBuildingUpgrade::CRegistrant CPlayerCapabilityBuildingUpgrade::DRegistrant;

//constructor
CPlayerCapabilityBuildingUpgrade::CRegistrant::CRegistrant(){
    CPlayerCapability::Register(std::shared_ptr< CPlayerCapabilityBuildingUpgrade >(new CPlayerCapabilityBuildingUpgrade("Keep")));   
    CPlayerCapability::Register(std::shared_ptr< CPlayerCapabilityBuildingUpgrade >(new CPlayerCapabilityBuildingUpgrade("Castle")));
    CPlayerCapability::Register(std::shared_ptr< CPlayerCapabilityBuildingUpgrade >(new CPlayerCapabilityBuildingUpgrade("GuardTower")));   
    CPlayerCapability::Register(std::shared_ptr< CPlayerCapabilityBuildingUpgrade >(new CPlayerCapabilityBuildingUpgrade("CannonTower")));
}

//constructor
CPlayerCapabilityBuildingUpgrade::CPlayerCapabilityBuildingUpgrade(const std::string &buildingname) : CPlayerCapability(std::string("Build") + buildingname, ETargetType::None){
    DBuildingName = buildingname;
}

/**
* checks if the buiding upgrade can be intiated, under following conditions
*     <pre>
*     player's Lumber should be greater than lumbercost for building upgrade.
*     player's Gold should be greater than goldcost for building upgrade.
*     </pre>
*
* @param[in] actor shared pointer from class PlayerAsset.
* @param[in] playerdata shared pointer from class PlayerData
*
* @return boolean value
*
*/
bool CPlayerCapabilityBuildingUpgrade::CanInitiate(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata){
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
        if(!playerdata->AssetRequirementsMet(DBuildingName)){
            return false;
        }
    }
    
    return true;
}

/**
* calls CanInitiate function to see if building upgrade requirements met. 
*
* @param[in] actor shared pointer from class PlayerAsset.
* @param[in] playerdata shared pointer from class PlayerData
* @param[in] target shared pointer from class PlayerAsset
*
* @return boolean value
*
*/
bool CPlayerCapabilityBuildingUpgrade::CanApply(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata, std::shared_ptr< CPlayerAsset > target){
    return CanInitiate(actor, playerdata);
}

/**
* applies building upgrade capability 
*     <pre>
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
bool CPlayerCapabilityBuildingUpgrade::ApplyCapability(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata, std::shared_ptr< CPlayerAsset > target){
    auto Iterator = playerdata->AssetTypes()->find(DBuildingName);
    
    if(Iterator != playerdata->AssetTypes()->end()){
        SAssetCommand NewCommand;
        auto AssetType = Iterator->second;
        
        actor->ClearCommand();
        NewCommand.DAction = EAssetAction::Capability;
        NewCommand.DCapability = AssetCapabilityType();
        NewCommand.DAssetTarget = target;
        NewCommand.DActivatedCapability = std::make_shared< CActivatedCapability >(actor, playerdata, target, actor->AssetType(), AssetType, AssetType->LumberCost(), AssetType->GoldCost(), AssetType->StoneCost(), CPlayerAsset::UpdateFrequency() * AssetType->BuildTime());
        actor->PushCommand(NewCommand);
        
        return true;
    }
    return false;
}

CPlayerCapabilityBuildingUpgrade::CActivatedCapability::CActivatedCapability(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata, std::shared_ptr< CPlayerAsset > target, std::shared_ptr< CPlayerAssetType > origtype, std::shared_ptr< CPlayerAssetType > upgradetype, int lumber, int gold, int stone, int steps) :
CActivatedPlayerCapability(actor, playerdata, target){
    SAssetCommand AssetCommand;
    
    DOriginalType = origtype;
    DUpgradeType = upgradetype;
    DCurrentStep = 0;
    DTotalSteps = steps;
    DLumber = lumber;
    DGold = gold;
    DStone = stone;
    DPlayerData->DecrementLumber(DLumber);
    DPlayerData->DecrementGold(DGold);
    DPlayerData->DecrementStone(DStone);
}

/**
* completion of action (shown via the health bar in the game UI)
*
* @param[in] max health value 
*
* @return int value of health completion 
*
*/
int CPlayerCapabilityBuildingUpgrade::CActivatedCapability::PercentComplete(int max){
    return DCurrentStep * max / DTotalSteps;
}

/**
* Set/initilize the next steps until building upgrade is complete. 
*
* @param[in] nothing
*
* @return boolean value
*
*/
bool CPlayerCapabilityBuildingUpgrade::CActivatedCapability::IncrementStep(){
    int AddHitPoints = ((DUpgradeType->HitPoints() - DOriginalType->HitPoints()) * (DCurrentStep + 1) / DTotalSteps) - ((DUpgradeType->HitPoints() - DOriginalType->HitPoints()) * DCurrentStep / DTotalSteps);
    
    if(0 == DCurrentStep){
        SAssetCommand AssetCommand = DActor->CurrentCommand();
        AssetCommand.DAction = EAssetAction::Construct;
        DActor->PopCommand();
        DActor->PushCommand(AssetCommand);
        DActor->ChangeType(DUpgradeType);  
        DActor->ResetStep();
    }
    
    DActor->IncrementHitPoints(AddHitPoints);
    if(DActor->HitPoints() > DActor->MaxHitPoints()){
        DActor->HitPoints(DActor->MaxHitPoints());
    }
    DCurrentStep++;
    DActor->IncrementStep();
    if(DCurrentStep >= DTotalSteps){
        SGameEvent TempEvent;
        
        TempEvent.DType = EEventType::WorkComplete;
        TempEvent.DAsset = DActor;
        DPlayerData->AddGameEvent(TempEvent);
        
        DActor->PopCommand();
        if(DActor->Range()){
            SAssetCommand Command;
            Command.DAction = EAssetAction::StandGround;
            DActor->PushCommand(Command);   
        }
        return true;    
    }
    return false;
}

/**
* Cancels building upgrade capability and reincrements the assets for lumber and gold and pops command. 
*
* @param[in] nothing
*
* @return nothing
*
*/
void CPlayerCapabilityBuildingUpgrade::CActivatedCapability::Cancel(){
    DPlayerData->IncrementLumber(DLumber);
    DPlayerData->IncrementGold(DGold);
    DPlayerData->IncrementStone(DStone);
    DActor->ChangeType(DOriginalType);
    DActor->PopCommand();
}


