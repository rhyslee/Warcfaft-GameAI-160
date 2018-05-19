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
 * @class CPlayerCapabilityUnitUpgrade
 *
 * @ brief This class was written to give unit's an upgrade capability \n
 *   <pre>
 *    The class contains 2 other clases: CRegistrant and CActivatedCapability.
 *    CRegistrant determines which unit will be ugraded: WeaponUpgrade2, WeaponUpgrade3, ArmorUpgrade2, ArmorUpgrade3, ArrowUpgrade2, ArrowUpgrade3, Longbow, RangerScouting, Marksmanship.
 *    CActivatedCapability calls several functions: PercentComplete, IncrementStep, Cancel that determine the status of applying the unit upgrade.
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
class CPlayerCapabilityUnitUpgrade : public CPlayerCapability{
    protected:

        /**
         * @class CRegistrant
         *
         * @ brief This class determines which unit will be ugraded: Weapon 2/3, Armor 2/3, Arrow 2/3, Longbow, RangerScouting, Marksmanship  \n
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
                std::shared_ptr< CPlayerAssetType > DUpgradingType;
                std::string DUpgradeName;
                int DCurrentStep;
                int DTotalSteps;
                int DLumber;
                int DGold;
                int DStone;
                
            public:
                CActivatedCapability(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata, std::shared_ptr< CPlayerAsset > target, std::shared_ptr< CPlayerAssetType > upgradingtype, const std::string &upgradename, int lumber, int gold, int stone, int steps);
                virtual ~CActivatedCapability(){};
                
                int PercentComplete(int max);
                bool IncrementStep();
                void Cancel();
        };
        std::string DUpgradeName;
        CPlayerCapabilityUnitUpgrade(const std::string &upgradename);
        
    public:
        //destructor 
        virtual ~CPlayerCapabilityUnitUpgrade(){};
        
        bool CanInitiate(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata);
        bool CanApply(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata, std::shared_ptr< CPlayerAsset > target);
        bool ApplyCapability(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata, std::shared_ptr< CPlayerAsset > target);
};

// creating new instance from CRegistrant 
CPlayerCapabilityUnitUpgrade::CRegistrant CPlayerCapabilityUnitUpgrade::DRegistrant;

//constructor 
CPlayerCapabilityUnitUpgrade::CRegistrant::CRegistrant(){
    CPlayerCapability::Register(std::shared_ptr< CPlayerCapabilityUnitUpgrade >(new CPlayerCapabilityUnitUpgrade("WeaponUpgrade2")));   
    CPlayerCapability::Register(std::shared_ptr< CPlayerCapabilityUnitUpgrade >(new CPlayerCapabilityUnitUpgrade("WeaponUpgrade3")));
    CPlayerCapability::Register(std::shared_ptr< CPlayerCapabilityUnitUpgrade >(new CPlayerCapabilityUnitUpgrade("ArmorUpgrade2")));   
    CPlayerCapability::Register(std::shared_ptr< CPlayerCapabilityUnitUpgrade >(new CPlayerCapabilityUnitUpgrade("ArmorUpgrade3")));
    CPlayerCapability::Register(std::shared_ptr< CPlayerCapabilityUnitUpgrade >(new CPlayerCapabilityUnitUpgrade("ArrowUpgrade2")));
    CPlayerCapability::Register(std::shared_ptr< CPlayerCapabilityUnitUpgrade >(new CPlayerCapabilityUnitUpgrade("ArrowUpgrade3")));
    CPlayerCapability::Register(std::shared_ptr< CPlayerCapabilityUnitUpgrade >(new CPlayerCapabilityUnitUpgrade("Longbow")));
    CPlayerCapability::Register(std::shared_ptr< CPlayerCapabilityUnitUpgrade >(new CPlayerCapabilityUnitUpgrade("RangerScouting")));
    CPlayerCapability::Register(std::shared_ptr< CPlayerCapabilityUnitUpgrade >(new CPlayerCapabilityUnitUpgrade("Marksmanship")));
}

//constructor 
CPlayerCapabilityUnitUpgrade::CPlayerCapabilityUnitUpgrade(const std::string &upgradename) : CPlayerCapability(upgradename, ETargetType::None){
    DUpgradeName = upgradename;
}

/**
* checks if the unit upgrade can be intiated, under following conditions
*     <pre>
*     player's Lumber should be greater than lumbercost for unit upgrade.
*     player's Gold should be greater than goldcost for unit upgrade.
*     </pre>
*
* @param[in] actor shared pointer from class PlayerAsset.
* @param[in] playerdata shared pointer from class PlayerData
*
* @return boolean value
*
*/
bool CPlayerCapabilityUnitUpgrade::CanInitiate(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata){
    auto Upgrade = CPlayerUpgrade::FindUpgradeFromName(DUpgradeName);
    
    if(Upgrade){

        if(Upgrade->LumberCost() > playerdata->Lumber()){
            return false;   
        }
        if(Upgrade->GoldCost() > playerdata->Gold()){
            return false;   
        }
        if(Upgrade->StoneCost() > playerdata->Stone()){
            return false;
        }
        /*
        if(!playerdata->AssetRequirementsMet(DUpgradeName)){
            return false;
        }
        */
    }
    
    return true;
}

/**
* calls CanInitiate function to see if unit upgrade requirements met. 
*
* @param[in] actor shared pointer from class PlayerAsset.
* @param[in] playerdata shared pointer from class PlayerData
* @param[in] target shared pointer from class PlayerAsset
*
* @return boolean value
*
*/
bool CPlayerCapabilityUnitUpgrade::CanApply(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata, std::shared_ptr< CPlayerAsset > target){
    return CanInitiate(actor, playerdata);
}

/**
* applies unit upgrade capability 
*     <pre>
*     have actor apply unit upgrade according to action, capability, asset target, activatedcapability. 
*     </pre>
*
* @param[in] actor shared pointer from class PlayerAsset.
* @param[in] playerdata shared pointer from class PlayerData
* @param[in] target shared pointer from class PlayerAsset
*
* @return boolean value
*
*/
bool CPlayerCapabilityUnitUpgrade::ApplyCapability(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata, std::shared_ptr< CPlayerAsset > target){
    auto Upgrade = CPlayerUpgrade::FindUpgradeFromName(DUpgradeName);
    
    if(Upgrade){
        SAssetCommand NewCommand;
        
        actor->ClearCommand();
        NewCommand.DAction = EAssetAction::Capability;
        NewCommand.DCapability = AssetCapabilityType();
        NewCommand.DAssetTarget = target;
        NewCommand.DActivatedCapability = std::make_shared< CActivatedCapability >(actor, playerdata, target, actor->AssetType(), DUpgradeName, Upgrade->LumberCost(), Upgrade->GoldCost(), Upgrade->StoneCost(), CPlayerAsset::UpdateFrequency() * Upgrade->ResearchTime());
        actor->PushCommand(NewCommand);
        
        return true;
    }
    return false;
}

/**
* constructor 
*
* @param[in] actor shared pointer from class PlayerAsset
* @param[in] playerdata shared pointer from class PlayerData
* @param[in] target shared pointer from class PlayerAsset
* @param[in] upgradingtype shared pointer from class PlayerAssetType
* @param[in] const upgradename
* @param[in] int amt of lumber
* @param[in] int amt of gold
* @param[in] int amt of steps
*
*/
CPlayerCapabilityUnitUpgrade::CActivatedCapability::CActivatedCapability(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata, std::shared_ptr< CPlayerAsset > target, std::shared_ptr< CPlayerAssetType > upgradingtype, const std::string &upgradename, int lumber, int gold, int stone, int steps) :
CActivatedPlayerCapability(actor, playerdata, target){
    SAssetCommand AssetCommand;
    
    DUpgradingType = upgradingtype;
    DUpgradeName = upgradename;
    DCurrentStep = 0;
    DTotalSteps = steps;
    DLumber = lumber;
    DGold = gold;
    DStone = stone;
    DPlayerData->DecrementLumber(DLumber);
    DPlayerData->DecrementGold(DGold);
    DPlayerData->DecrementStone(DStone);
    DUpgradingType->RemoveCapability(CPlayerCapability::NameToType(DUpgradeName));
}

/**
* completion of action (shown via the health bar in the game UI)
*
* @param[in] max health value 
*
* @return int value of health completion 
*
*/
int CPlayerCapabilityUnitUpgrade::CActivatedCapability::PercentComplete(int max){
    return DCurrentStep * max / DTotalSteps;
}

/**
* Set/initilize the next steps until unit upgrade is complete. 
*
* @param[in] nothing
*
* @return boolean value
*
*/
bool CPlayerCapabilityUnitUpgrade::CActivatedCapability::IncrementStep(){
    DCurrentStep++;
    DActor->IncrementStep();
    if(DCurrentStep >= DTotalSteps){
        DPlayerData->AddUpgrade(DUpgradeName);
        DActor->PopCommand();
        if(DUpgradeName.rfind("2") == (DUpgradeName.length()-1)){
            DUpgradingType->AddCapability(CPlayerCapability::NameToType(DUpgradeName.substr(0, DUpgradeName.length()-1) + "3"));
        }
        
        return true;    
    }
    return false;
}

/**
* Cancels unit upgrade capability and reincrements the assets for lumber and gold and pops command. 
*
* @param[in] nothing
*
* @return nothing
*
*/
void CPlayerCapabilityUnitUpgrade::CActivatedCapability::Cancel(){
    DPlayerData->IncrementLumber(DLumber);
    DPlayerData->IncrementGold(DGold);
    DPlayerData->IncrementStone(DStone);
    DUpgradingType->AddCapability(CPlayerCapability::NameToType(DUpgradeName));
    DActor->PopCommand();
}

/**
 * @class CPlayerCapabilityBuildRanger
 *
 * @ brief This class was written to build a ranger capability \n
 *   <pre>
 *    The class contains 2 other clases: CRegistrant and CActivatedCapability.
 *    CRegistrant determines which unit to build: ranger.
 *    CActivatedCapability calls several functions: PercentComplete, IncrementStep, Cancel that determine the status of building ranger.
 *   </pre>
 *     
 *     Public Member Functions:
 *     CanInitiate: to return true if player is eligible for building ranger. 
 *     CanApply: to return true after calling CanInitiate function.
 *     ApplyCapability: to return true once building ranger is applied. 
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
class CPlayerCapabilityBuildRanger : public CPlayerCapability{
    protected:
        /**
         * @class CRegistrant
         *
         * @ brief This class determines which unit to build: ranger \n
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
         * @ brief This calls several functions: PercentComplete, IncrementStep, and Cancel that determine the status of building ranger. \n
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
                std::shared_ptr< CPlayerAssetType > DUpgradingType;
                std::string DUnitName;
                int DCurrentStep;
                int DTotalSteps;
                int DLumber;
                int DGold;
                int DStone;
                
            public:
                //destructor
                CActivatedCapability(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata, std::shared_ptr< CPlayerAsset > target, std::shared_ptr< CPlayerAssetType > upgradingtype, const std::string &unitname, int lumber, int gold, int stone, int steps);
                virtual ~CActivatedCapability(){};
                
                int PercentComplete(int max);
                bool IncrementStep();
                void Cancel();
        };
        std::string DUnitName;
        CPlayerCapabilityBuildRanger(const std::string &unitname);
        
    public:
        virtual ~CPlayerCapabilityBuildRanger(){};
        
        bool CanInitiate(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata);
        bool CanApply(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata, std::shared_ptr< CPlayerAsset > target);
        bool ApplyCapability(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata, std::shared_ptr< CPlayerAsset > target);
};

// creating new instance from CRegistrant 
CPlayerCapabilityBuildRanger::CRegistrant CPlayerCapabilityBuildRanger::DRegistrant;

//constructor 
CPlayerCapabilityBuildRanger::CRegistrant::CRegistrant(){
    CPlayerCapability::Register(std::shared_ptr< CPlayerCapabilityBuildRanger >(new CPlayerCapabilityBuildRanger("Ranger")));   
}

//constructor 
CPlayerCapabilityBuildRanger::CPlayerCapabilityBuildRanger(const std::string &unitname) : CPlayerCapability(std::string("Build") + unitname, ETargetType::None){
    DUnitName = unitname;
}

/**
* checks if the buiding ranger can be intiated, under following conditions
*     <pre>
*     if asset type is LumberMill then build ranger 
*       player's Lumber should be greater than lumbercost for building upgrade.
*       player's Gold should be greater than goldcost for building upgrade.
*       player's asset requirements are met
*     if asset type is Barracks then iterate through player's assettypes until ranger found
*       player's Lumber should be greater than lumbercost for building upgrade.
*       player's Gold should be greater than goldcost for building upgrade.
*       player's food production should be greater than ranger's foodconsumption and player's food consumption   
*     </pre>
*
* @param[in] actor shared pointer from class PlayerAsset.
* @param[in] playerdata shared pointer from class PlayerData
*
* @return boolean value
*
*/
bool CPlayerCapabilityBuildRanger::CanInitiate(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata){
    
    if(EAssetType::LumberMill == actor->Type()){
        auto Upgrade = CPlayerUpgrade::FindUpgradeFromName(std::string("Build") + DUnitName);
        
        if(Upgrade){
    
            if(Upgrade->LumberCost() > playerdata->Lumber()){
                return false;   
            }
            if(Upgrade->GoldCost() > playerdata->Gold()){
                return false;   
            }
            if(Upgrade->StoneCost() > playerdata->Stone()){
                return false;
            }
            if(!playerdata->AssetRequirementsMet(DUnitName)){
                return false;
            }
        }
    }
    else if(EAssetType::Barracks == actor->Type()){
        auto AssetIterator = playerdata->AssetTypes()->find(DUnitName);
        
        if(AssetIterator != playerdata->AssetTypes()->end()){
            auto AssetType = AssetIterator->second;
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
        }
    }
    
    return true;
}

/**
* calls CanInitiate function to see if building of ranger requirements met. 
*
* @param[in] actor shared pointer from class PlayerAsset.
* @param[in] playerdata shared pointer from class PlayerData
* @param[in] target shared pointer from class PlayerAsset
*
* @return boolean value
*
*/
bool CPlayerCapabilityBuildRanger::CanApply(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata, std::shared_ptr< CPlayerAsset > target){
    return CanInitiate(actor, playerdata);
}

/**
* applies building ranger capability based on whether asset type is lumbermill or barracks (for barracks - iterate through assettypes until ranger)
*     <pre>
*     have actor apply building ranger according to action, capability, asset target, activatedcapability. 
*     </pre>
*
* @param[in] actor shared pointer from class PlayerAsset.
* @param[in] playerdata shared pointer from class PlayerData
* @param[in] target shared pointer from class PlayerAsset
*
* @return boolean value
*
*/
bool CPlayerCapabilityBuildRanger::ApplyCapability(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata, std::shared_ptr< CPlayerAsset > target){
    if(EAssetType::LumberMill == actor->Type()){
        auto Upgrade = CPlayerUpgrade::FindUpgradeFromName(std::string("Build") + DUnitName);
        
        if(Upgrade){
            SAssetCommand NewCommand;
            
            actor->ClearCommand();
            NewCommand.DAction = EAssetAction::Capability;
            NewCommand.DCapability = AssetCapabilityType();
            NewCommand.DAssetTarget = target;
            NewCommand.DActivatedCapability = std::make_shared< CActivatedCapability >(actor, playerdata, target, actor->AssetType(), DUnitName, Upgrade->LumberCost(), Upgrade->GoldCost(), Upgrade->StoneCost(), CPlayerAsset::UpdateFrequency() * Upgrade->ResearchTime());
            actor->PushCommand(NewCommand);
            
            return true;
        }
    }
    else if(EAssetType::Barracks == actor->Type()){
        auto AssetIterator = playerdata->AssetTypes()->find(DUnitName);
        
        if(AssetIterator != playerdata->AssetTypes()->end()){
            auto AssetType = AssetIterator->second;
            auto NewAsset = playerdata->CreateAsset(DUnitName);
            SAssetCommand NewCommand;
            CTilePosition TilePosition;
            TilePosition.SetFromPixel(actor->Position());
            NewAsset->TilePosition(TilePosition);
            NewAsset->HitPoints(1);
            
            NewCommand.DAction = EAssetAction::Capability;
            NewCommand.DCapability = AssetCapabilityType();
            NewCommand.DAssetTarget = NewAsset;
            NewCommand.DActivatedCapability = std::make_shared< CActivatedCapability >(actor, playerdata, NewAsset, actor->AssetType(), DUnitName, AssetType->LumberCost(), AssetType->GoldCost(), AssetType->StoneCost(), CPlayerAsset::UpdateFrequency() * AssetType->BuildTime());
            actor->PushCommand(NewCommand);
        }
    }
    return false;
}

/**
* constructor 
*
* @param[in] actor shared pointer from class PlayerAsset
* @param[in] playerdata shared pointer from class PlayerData
* @param[in] target shared pointer from class PlayerAsset
* @param[in] upgradingtype shared pointer from class PlayerAssetType
* @param[in] const unitname
* @param[in] int amt of lumber
* @param[in] int amt of gold
* @param[in] int amt of steps
*
*/
CPlayerCapabilityBuildRanger::CActivatedCapability::CActivatedCapability(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata, std::shared_ptr< CPlayerAsset > target, std::shared_ptr< CPlayerAssetType > upgradingtype, const std::string &unitname, int lumber, int gold, int stone, int steps) :
CActivatedPlayerCapability(actor, playerdata, target){
    DUnitName = unitname;
    DCurrentStep = 0;
    DTotalSteps = steps;
    DLumber = lumber;
    DGold = gold;
    DStone = stone;
    DPlayerData->DecrementLumber(DLumber);
    DPlayerData->DecrementGold(DGold);
    DPlayerData->DecrementStone(DStone);
    if(EAssetType::LumberMill == actor->Type()){
        DUpgradingType = upgradingtype;
        DUpgradingType->RemoveCapability(CPlayerCapability::NameToType(std::string("Build") + DUnitName));
    }
    else if(EAssetType::Barracks == actor->Type()){
        SAssetCommand AssetCommand;

        AssetCommand.DAction = EAssetAction::Construct;
        AssetCommand.DAssetTarget = DActor;
        DTarget->PushCommand(AssetCommand);
    }
}

/**
* completion of action (shown via the health bar in the game UI)
*
* @param[in] max health value 
*
* @return int value of health completion 
*
*/
int CPlayerCapabilityBuildRanger::CActivatedCapability::PercentComplete(int max){
    return DCurrentStep * max / DTotalSteps;
}

/**
* Set/initilize the next steps until building ranger is complete. 
*
* @param[in] nothing
*
* @return boolean value
*
*/
bool CPlayerCapabilityBuildRanger::CActivatedCapability::IncrementStep(){
    if(EAssetType::Barracks == DActor->Type()){
        int AddHitPoints = (DTarget->MaxHitPoints() * (DCurrentStep + 1) / DTotalSteps) - (DTarget->MaxHitPoints() * DCurrentStep / DTotalSteps);
        
        DTarget->IncrementHitPoints(AddHitPoints);
        if(DTarget->HitPoints() > DTarget->MaxHitPoints()){
            DTarget->HitPoints(DTarget->MaxHitPoints());
        }   
    }
    
    DCurrentStep++;
    DActor->IncrementStep();
    if(DCurrentStep >= DTotalSteps){
        SGameEvent TempEvent;
        
        if(EAssetType::LumberMill == DActor->Type()){
            auto BarracksIterator = DPlayerData->AssetTypes()->find("Barracks");
            auto RangerIterator = DPlayerData->AssetTypes()->find("Ranger");
            auto LumberMillIterator = DPlayerData->AssetTypes()->find("LumberMill");
            
            TempEvent.DType = EEventType::WorkComplete;
            TempEvent.DAsset = DActor;
            
            BarracksIterator->second->AddCapability(EAssetCapabilityType::BuildRanger);
            BarracksIterator->second->RemoveCapability(EAssetCapabilityType::BuildArcher);
            LumberMillIterator->second->AddCapability(EAssetCapabilityType::Longbow);
            LumberMillIterator->second->AddCapability(EAssetCapabilityType::RangerScouting);
            LumberMillIterator->second->AddCapability(EAssetCapabilityType::Marksmanship);
            // Upgrade all Archers
            for(auto WeakAsset : DPlayerData->Assets()){
                if(auto Asset = WeakAsset.lock()){
                    if(EAssetType::Archer == Asset->Type()){
                        int HitPointIncrement = RangerIterator->second->HitPoints() - Asset->MaxHitPoints();
                        
                        Asset->ChangeType(RangerIterator->second);
                        Asset->IncrementHitPoints(HitPointIncrement);
                    }   
                }
            }
        }
        else if(EAssetType::Barracks == DActor->Type()){
            TempEvent.DType = EEventType::Ready;
            TempEvent.DAsset = DTarget;
        
            DTarget->PopCommand();
            DTarget->TilePosition(DPlayerData->PlayerMap()->FindAssetPlacement(DTarget, DActor, CTilePosition(DPlayerData->PlayerMap()->Width()-1, DPlayerData->PlayerMap()->Height()-1)));
        }
        DPlayerData->AddGameEvent(TempEvent);
        DActor->PopCommand();
        return true;    
    }
    return false;
}

/**
* Cancels building ranger capability and reincrements the assets for lumber and gold and pops command, dependent on whether asset type is LumberMill or Barracks. 
*
* @param[in] nothing
*
* @return nothing
*
*/
void CPlayerCapabilityBuildRanger::CActivatedCapability::Cancel(){
    DPlayerData->IncrementLumber(DLumber);
    DPlayerData->IncrementGold(DGold);
    DPlayerData->IncrementStone(DStone);
    if(EAssetType::LumberMill == DActor->Type()){
        DUpgradingType->AddCapability(CPlayerCapability::NameToType(std::string("Build") + DUnitName));
    }
    else if(EAssetType::Barracks == DActor->Type()){
        DPlayerData->DeleteAsset(DTarget);
    }
    DActor->PopCommand();
}


