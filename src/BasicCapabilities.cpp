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
#include "TerrainMap.h"

// Normal capabilities

/**
 *
 * @class CPlayerCapabilityMove
 *
 * @ brief This class was written to give player capability of move \n
 *   <pre>
 *    The class contains two other classes which are CRegistrant and CActivatedCapability.
 *    They are responsible for determining the player who should move and activation of
 *    capability. The class also keep tracks of percentage completion of the capability
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
 */
class CPlayerCapabilityMove : public CPlayerCapability{
    protected:

        /**
         *
         *@class CRegistrant
         *
         * @brief the class was written to determine the player who should get the capability
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
         * @brief the class was written to activate the player capability
         *     <pre>
         *      The class is responsible for showing the percentage completion until
         *      the player get the capability,following steps to get capability, and
         *      cancellation
             of capability.
         *     </pre>
         * @author Seza
         *
         * @version 4.0
         *
         * @date 10/13/2017 23:17:00
         *
         * Contact: shabibi@ucdavis.edu
         */
        class CActivatedCapability : public CActivatedPlayerCapability{
            protected:

            public:
                CActivatedCapability(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata, std::shared_ptr< CPlayerAsset > target);
                virtual ~CActivatedCapability(){};

                int PercentComplete(int max);
                bool IncrementStep();
                void Cancel();
        };
        CPlayerCapabilityMove();

    public:
        virtual ~CPlayerCapabilityMove(){};

        bool CanInitiate(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata);
        bool CanApply(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata, std::shared_ptr< CPlayerAsset > target);
        bool ApplyCapability(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata, std::shared_ptr< CPlayerAsset > target);
};

CPlayerCapabilityMove::CRegistrant CPlayerCapabilityMove::DRegistrant;

CPlayerCapabilityMove::CRegistrant::CRegistrant(){
    CPlayerCapability::Register(std::shared_ptr< CPlayerCapabilityMove >(new CPlayerCapabilityMove()));
}

CPlayerCapabilityMove::CPlayerCapabilityMove() : CPlayerCapability(std::string("Move"), ETargetType::TerrainOrAsset){

}

/**
* It will check if the actor can gain/ initiate capability.
*
* @param[in] actor share pointer to the class of PlayerAsset.
* @param[in] playerdata  share pointer to the class of PlayerData
*
* @return boolean function if the actor can initiate capability or not
*
*/
bool CPlayerCapabilityMove::CanInitiate(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata){
    return actor->Speed() > 0;
}

/**
* It will check if capability can be apply to the actor. condition in this case actor's speed > 0.
*
* @param[in] actor share pointer to the class of PlayerAsset.
* @param[in] playerdata  share pointer to the class of PlayerData
* @param[in] playerdata  share pointer to the class of PlayerAsset
*
* @return boolean function if the actor can be applied for capability
*
*/
bool CPlayerCapabilityMove::CanApply(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata, std::shared_ptr< CPlayerAsset > target){
    return actor->Speed() > 0;
}

/**
* Set actor a new capability. using a struct and PushCommand function
*
* @param[in] actor share pointer to the class of PlayerAsset.
* @param[in] playerdata  share pointer to the class of PlayerData
* @param[in] playerdata  share pointer to the class of PlayerAsset
*
* @return boolean function if the actor got capability or not.
*
*/
bool CPlayerCapabilityMove::ApplyCapability(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata, std::shared_ptr< CPlayerAsset > target){
    if(actor->TilePosition() != target->TilePosition()){
        SAssetCommand NewCommand;

        NewCommand.DAction = EAssetAction::Capability;
        NewCommand.DCapability = AssetCapabilityType();
        NewCommand.DAssetTarget = target;
        NewCommand.DActivatedCapability = std::make_shared< CActivatedCapability >(actor, playerdata, target);
        actor->ClearCommand();
        actor->PushCommand(NewCommand);
        return true;
    }

    return false;
}

CPlayerCapabilityMove::CActivatedCapability::CActivatedCapability(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata, std::shared_ptr< CPlayerAsset > target) :
CActivatedPlayerCapability(actor, playerdata, target){

}

/**
* Show the percentage completion of the capability.
*
* @param[in] max the number of max percent complete.
*
* @return number of percentage complete.
*
*/
int CPlayerCapabilityMove::CActivatedCapability::PercentComplete(int max){
    return 0;
}

/**
* Set/Initialize the next step.
*
* @brief it uses two struct to Acknowledge the actor and make the command to gain
*         capability.
*
* @param[in] it does not have a parameter.
*
* @return boolean function if actor gain the capability or not.
*
*/
bool CPlayerCapabilityMove::CActivatedCapability::IncrementStep(){
    SAssetCommand AssetCommand;
    SGameEvent TempEvent;

    TempEvent.DType = EEventType::Acknowledge;
    TempEvent.DAsset = DActor;
    DPlayerData->AddGameEvent(TempEvent);

    AssetCommand.DAction = EAssetAction::Walk;
    AssetCommand.DAssetTarget = DTarget;
    if(!DActor->TileAligned()){
        DActor->Direction(DirectionOpposite(DActor->Position().TileOctant()));
    }
    DActor->ClearCommand();
    DActor->PushCommand(AssetCommand);
    return true;
}

/**
* Cancel the capability of the actor.
*
* @param[in] it does not have a parameter.
*
* @return None.
*
*/
void CPlayerCapabilityMove::CActivatedCapability::Cancel(){

    DActor->PopCommand();
}

/**
 *
 * @class CPlayerCapabilityMineHarvest
 *
 * @ brief This class was written to give player capability of MineHarvest \n
 *     <pre>
 *    The class contains two other classes which are CRegistrant and CActivatedCapability.
 *    They are responsible for determining the player who should do Mine Harvest and activation of
 *    capability. The class also keep tracks of percentage completion of the capability
 *    the steps and cancellation
             of capability.
 *     </pre>
 * @author Seza
 *
 * @version 4.0
 *
 * @date 10/13/2017 21:04:00
 *
 * Contact: shabibi@ucdavis.edu
 *
 */
class CPlayerCapabilityMineHarvestQuarry : public CPlayerCapability{
    protected:

      /**
       *
       *@class CRegistrant
       *
       * @brief the class was written to determine the player who should get the capability
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
         * @brief the class was written to activate the player capability
         *     <pre>
         *      The class is responsible for showing the percentage completion until
         *      the player get the capability,following steps to get capability, and
         *      cancellation
             of capability.
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

            public:
                CActivatedCapability(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata, std::shared_ptr< CPlayerAsset > target);
                virtual ~CActivatedCapability(){};

                int PercentComplete(int max);
                bool IncrementStep();
                void Cancel();
        };
        CPlayerCapabilityMineHarvestQuarry();

    public:
        virtual ~CPlayerCapabilityMineHarvestQuarry(){};

        bool CanInitiate(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata);
        bool CanApply(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata, std::shared_ptr< CPlayerAsset > target);
        bool ApplyCapability(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata, std::shared_ptr< CPlayerAsset > target);
};

CPlayerCapabilityMineHarvestQuarry::CRegistrant CPlayerCapabilityMineHarvestQuarry::DRegistrant;

CPlayerCapabilityMineHarvestQuarry::CRegistrant::CRegistrant(){
    CPlayerCapability::Register(std::shared_ptr< CPlayerCapabilityMineHarvestQuarry >(new CPlayerCapabilityMineHarvestQuarry()));
}

CPlayerCapabilityMineHarvestQuarry::CPlayerCapabilityMineHarvestQuarry() : CPlayerCapability(std::string("Mine"), ETargetType::TerrainOrAsset){

}

/**
* It will check if the actor can gain/ initiate capability.
*
* @param[in] actor share pointer to the class of PlayerAsset.
* @param[in] playerdata  share pointer to the class of PlayerData
*
* @return boolean function if the actor can initiate capability or not
*
*/
bool CPlayerCapabilityMineHarvestQuarry::CanInitiate(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata){
    return actor->HasCapability(EAssetCapabilityType::Mine);
}

/**
* It will check if capability can be apply to the actor. condition in this case\n
*     <pre>
*     the actor must have a capability of mine.
*     the actor must not be a Lumber or Gold.
*     the targettype must not be GoldMine
*     </pre
*
* @param[in] actor share pointer to the class of PlayerAsset.
* @param[in] playerdata  share pointer to the class of PlayerData
* @param[in] playerdata  share pointer to the class of PlayerAsset
*
* @return boolean function if the actor can be can be applied for capability
*
*/
bool CPlayerCapabilityMineHarvestQuarry::CanApply(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata, std::shared_ptr< CPlayerAsset > target){

    if(!actor->HasCapability(EAssetCapabilityType::Mine)){
        return false;
    }
    if(actor->Lumber()||actor->Gold()||actor->Stone()){
        return false;
    }
    if(EAssetType::GoldMine == target->Type()){
        return true;
    }
    if(EAssetType::None != target->Type()){
        return false;
    }
    if(CTerrainMap::ETileType::Forest == playerdata->PlayerMap()->TileType(target->TilePosition())){
        return true;
    }
    else
        return (CTerrainMap::ETileType::Rock == playerdata->PlayerMap()->TileType(target->TilePosition()));
}

/**
* Set actor a new capability. using a struct and PushCommand function
*
* @param[in] actor share pointer to the class of PlayerAsset.
* @param[in] playerdata  share pointer to the class of PlayerData
* @param[in] playerdata  share pointer to the class of PlayerAsset
*
* @return boolean function if the actor got capability or not.
*
*/
bool CPlayerCapabilityMineHarvestQuarry::ApplyCapability(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata, std::shared_ptr< CPlayerAsset > target){
    SAssetCommand NewCommand;

    NewCommand.DAction = EAssetAction::Capability;
    NewCommand.DCapability = AssetCapabilityType();
    NewCommand.DAssetTarget = target;
    NewCommand.DActivatedCapability = std::make_shared< CActivatedCapability >(actor, playerdata, target);
    actor->ClearCommand();
    actor->PushCommand(NewCommand);

    return true;
}

CPlayerCapabilityMineHarvestQuarry::CActivatedCapability::CActivatedCapability(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata, std::shared_ptr< CPlayerAsset > target) :
CActivatedPlayerCapability(actor, playerdata, target){

}

/**
* Show the percentage completion of the capability.
*
* @param[in] max the number of max percent complete.
*
* @return number of percentage complete.
*
*/
int CPlayerCapabilityMineHarvestQuarry::CActivatedCapability::PercentComplete(int max){
    return 0;
}

/**
* Set/Initialize the next step.
*
* @brief it uses two struct to Acknowledge the actor and make the command to gain
*         capability.
*
* @param[in] it does not have a parameter.
*
* @return bool function if actor gain the capability or not.
*
*/
bool CPlayerCapabilityMineHarvestQuarry::CActivatedCapability::IncrementStep(){
    SAssetCommand AssetCommand;
    SGameEvent TempEvent;

    // Find target tile to know quarry or harvest
    int positx = DTarget->TilePositionX();
    int posity = DTarget->TilePositionY();

    TempEvent.DType = EEventType::Acknowledge;
    TempEvent.DAsset = DActor;
    DPlayerData->AddGameEvent(TempEvent);

    AssetCommand.DAssetTarget = DTarget;
    if(EAssetType::GoldMine == DTarget->Type()){
        AssetCommand.DAction = EAssetAction::MineGold;
    }
    else if(CTerrainMap::ETileType::Rock == DPlayerData->PlayerMap()->TileType(positx, posity)){
        AssetCommand.DAction = EAssetAction::QuarryStone;
    }
    else if(CTerrainMap::ETileType::Forest == DPlayerData->PlayerMap()->TileType(positx, posity)){
        AssetCommand.DAction = EAssetAction::HarvestLumber;
    } 
    else{
        //error
        PrintDebug(DEBUG_LOW, "Quarrying not working\n");  
    }

    DActor->ClearCommand();
    DActor->PushCommand(AssetCommand);
    AssetCommand.DAction = EAssetAction::Walk;
    if(!DActor->TileAligned()){
        DActor->Direction(DirectionOpposite(DActor->Position().TileOctant()));
    }
    DActor->PushCommand(AssetCommand);
    return true;
}

/**
* Cancel the capability of the actor.
*
* @param[in] it does not have a parameter.
*
* @return nothing.
*
*/
void CPlayerCapabilityMineHarvestQuarry::CActivatedCapability::Cancel(){

    DActor->PopCommand();
}

/**
 *
 * @class CPlayerCapabilityStandGround
 *
 * @ brief This class was written to give player capability of StandGround \n
 *     <pre>
 *    The class contains two other classes which are CRegistrant and CActivatedCapability.
 *    They are responsible for determining the player who should do StandGround and activation of
 *    capability. The class also keep tracks of percentage completion of the capability
 *    the steps and cancellation
             of capability.
 *     </pre>
 * @author Seza
 *
 * @version 1.0
 *
 * @date 10/13/2017 21:04:00
 *
 * Contact: shabibi@ucdavis.edu
 *
 */
class CPlayerCapabilityStandGround : public CPlayerCapability{
    protected:

      /**
       *
       *@class CRegistrant
       *
       * @brief the class was written to determine the player who should get the capability
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
         * @brief the class was written to activate the player capability
         *     <pre>
         *      The class is responsible for showing the percentage completion until
         *      the player get the capability,following steps to get capability, and
         *      cancellation
             of capability.
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

            public:
                CActivatedCapability(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata, std::shared_ptr< CPlayerAsset > target);
                virtual ~CActivatedCapability(){};

                int PercentComplete(int max);
                bool IncrementStep();
                void Cancel();
        };
        CPlayerCapabilityStandGround();

    public:
        virtual ~CPlayerCapabilityStandGround(){};

        bool CanInitiate(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata);
        bool CanApply(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata, std::shared_ptr< CPlayerAsset > target);
        bool ApplyCapability(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata, std::shared_ptr< CPlayerAsset > target);
};

CPlayerCapabilityStandGround::CRegistrant CPlayerCapabilityStandGround::DRegistrant;

CPlayerCapabilityStandGround::CRegistrant::CRegistrant(){
    CPlayerCapability::Register(std::shared_ptr< CPlayerCapabilityStandGround >(new CPlayerCapabilityStandGround()));
}

CPlayerCapabilityStandGround::CPlayerCapabilityStandGround() : CPlayerCapability(std::string("StandGround"), ETargetType::None){

}

/**
* It will check if the actor can gain/ initiate capability.
*
* @param[in] actor shar pointer to the class of PlayerAsset.
* @param[in] playerdata  share pointer to the class of PlayerData
*
* @return boolean function if the actor can initiate capability or not
*
*/
bool CPlayerCapabilityStandGround::CanInitiate(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata){
    return true;
}

/**
* It will check if capability can be apply to the actor. it does not have condition\n
*
* @param[in] actor share pointer to the class of PlayerAsset.
* @param[in] playerdata  share pointer to the class of PlayerData
* @param[in] playerdata  share pointer to the class of PlayerAsset
*
* @return always return true
*
*/
bool CPlayerCapabilityStandGround::CanApply(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata, std::shared_ptr< CPlayerAsset > target){

    return true;
}

/**
* set actor a new capability. using a struct and PushCommand function
*
* @param[in] actor share pointer to the class of PlayerAsset.
* @param[in] playerdata  share pointer to the class of PlayerData
* @param[in] playerdata  share pointer to the class of PlayerAsset
*
* @return boolean function if the actor got capability or not.
*
*/
bool CPlayerCapabilityStandGround::ApplyCapability(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata, std::shared_ptr< CPlayerAsset > target){
    SAssetCommand NewCommand;

    NewCommand.DAction = EAssetAction::Capability;
    NewCommand.DCapability = AssetCapabilityType();
    NewCommand.DAssetTarget = target;
    NewCommand.DActivatedCapability = std::make_shared< CActivatedCapability >(actor, playerdata, target);
    actor->ClearCommand();
    actor->PushCommand(NewCommand);

    return true;
}

CPlayerCapabilityStandGround::CActivatedCapability::CActivatedCapability(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata, std::shared_ptr< CPlayerAsset > target) :
CActivatedPlayerCapability(actor, playerdata, target){

}

/**
* Show the percentage completion of the capability.
*
* @param[in] max the number of max percent complete.
*
* @return number of percentage complete.
*
*/
int CPlayerCapabilityStandGround::CActivatedCapability::PercentComplete(int max){
    return 0;
}

/**
* Set/Initialize the next step.
*
* @brief it uses two struct to Acknowledge the actor and make the command to gain
*         capability.
*
* @param[in] it does not have a parameter.
*
* @return boolean function if actor gain the capability or not.
*
*/
bool CPlayerCapabilityStandGround::CActivatedCapability::IncrementStep(){
    SAssetCommand AssetCommand;
    SGameEvent TempEvent;

    TempEvent.DType = EEventType::Acknowledge;
    TempEvent.DAsset = DActor;
    DPlayerData->AddGameEvent(TempEvent);

    AssetCommand.DAssetTarget = DPlayerData->CreateMarker(DActor->Position(), false);
    AssetCommand.DAction = EAssetAction::StandGround;

    DActor->ClearCommand();
    DActor->PushCommand(AssetCommand);

    if(!DActor->TileAligned()){
        AssetCommand.DAction = EAssetAction::Walk;
        DActor->Direction(DirectionOpposite(DActor->Position().TileOctant()));
        DActor->PushCommand(AssetCommand);
    }

    return true;
}

/**
* Cancel the capability of the actor.
*
* @param[in] it does not have a parameter.
*
* @return nothing.
*
*/
void CPlayerCapabilityStandGround::CActivatedCapability::Cancel(){

    DActor->PopCommand();
}

/**
 *
 * @class CPlayerCapabilityCancel
 *
 * @ brief This class was written to give player capability of cancellation
             \n
 *     <pre>
 *    The class contains two other classes which are CRegistrant and CActivatedCapability.
 *    They are responsible for determining the player whose capability should be cancel and activation of
 *    capability. The class also keep tracks of percentage completion of the cancellation

 *    the steps and cancellation
             of capability.
 *     </pre>
 * @author Seza
 *
 * @version 4.0
 *
 * @date 10/13/2017 21:04:00
 *
 * Contact: shabibi@ucdavis.edu
 *
 */
class CPlayerCapabilityCancel : public CPlayerCapability{
    protected:

      /**
       *
       *@class CRegistrant
       *
       * @brief the class was written to determine the player who should get the capability
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
         * @brief the class was written to activate the player capability
         *     <pre>
         *      The class is responsible for showing the percentage completion until
         *      the player get the capability,following steps to get capability, and
         *      cancellation
             of capability.
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

            public:
                CActivatedCapability(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata, std::shared_ptr< CPlayerAsset > target);
                virtual ~CActivatedCapability(){};

                int PercentComplete(int max);
                bool IncrementStep();
                void Cancel();
        };
        CPlayerCapabilityCancel();

    public:
        virtual ~CPlayerCapabilityCancel(){};

        bool CanInitiate(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata);
        bool CanApply(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata, std::shared_ptr< CPlayerAsset > target);
        bool ApplyCapability(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata, std::shared_ptr< CPlayerAsset > target);
};

CPlayerCapabilityCancel::CRegistrant CPlayerCapabilityCancel::DRegistrant;

CPlayerCapabilityCancel::CRegistrant::CRegistrant(){
    CPlayerCapability::Register(std::shared_ptr< CPlayerCapabilityCancel >(new CPlayerCapabilityCancel()));
}

CPlayerCapabilityCancel::CPlayerCapabilityCancel() : CPlayerCapability(std::string("Cancel"), ETargetType::None){

}

/**
* It will check if the actor can gain/ initiate capability.
*
* @param[in] actor shar pointer to the class of PlayerAsset.
* @param[in] playerdata  share pointer to the class of PlayerData
*
* @return boolean function if the actor can initiate capability or not
*
*/
bool CPlayerCapabilityCancel::CanInitiate(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata){
    return true;
}

/**
* It will check if capability can be apply to the actor .
*
* @param[in] actor share pointer to the class of PlayerAsset.
* @param[in] playerdata  share pointer to the class of PlayerData
* @param[in] playerdata  share pointer to the class of PlayerAsset
*
* @return always return true
*
*/
bool CPlayerCapabilityCancel::CanApply(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata, std::shared_ptr< CPlayerAsset > target){
    return true;
}

/**
* Set actor a new capability. using a struct and PushCommand function
*
* @param[in] actor share pointer to the class of PlayerAsset.
* @param[in] playerdata  share pointer to the class of PlayerData
* @param[in] playerdata  share pointer to the class of PlayerAsset
*
* @return boolean function if the actor got capability or not.
*
*/
bool CPlayerCapabilityCancel::ApplyCapability(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata, std::shared_ptr< CPlayerAsset > target){
    SAssetCommand NewCommand;

    NewCommand.DAction = EAssetAction::Capability;
    NewCommand.DCapability = AssetCapabilityType();
    NewCommand.DAssetTarget = target;
    NewCommand.DActivatedCapability = std::make_shared< CActivatedCapability >(actor, playerdata, target);
    actor->PushCommand(NewCommand);

    return true;
}

CPlayerCapabilityCancel::CActivatedCapability::CActivatedCapability(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata, std::shared_ptr< CPlayerAsset > target) :
CActivatedPlayerCapability(actor, playerdata, target){

}

/**
* Show the percentage completion of the capability.
*
* @param[in] max the number of max percent complete.
*
* @return number of percentage complete.
*
*/
int CPlayerCapabilityCancel::CActivatedCapability::PercentComplete(int max){
    return 0;
}

/**
* Set/Initialize the next step.
*
* @brief it uses two struct to Acknowledge the actor and make the command to gain
*         capability.
*
* @param[in] it does not have a parameter.
*
* @return bool function if actor gain the capability or not.
*
*/
bool CPlayerCapabilityCancel::CActivatedCapability::IncrementStep(){
    DActor->PopCommand();

    if(EAssetAction::None != DActor->Action()){
        SAssetCommand AssetCommand;

        AssetCommand = DActor->CurrentCommand();
        if(EAssetAction::Construct == AssetCommand.DAction){
            if(AssetCommand.DAssetTarget){
                AssetCommand.DAssetTarget->CurrentCommand().DActivatedCapability->Cancel();
            }
            else if(AssetCommand.DActivatedCapability){
                AssetCommand.DActivatedCapability->Cancel();
            }
        }
        else if(AssetCommand.DActivatedCapability){
            AssetCommand.DActivatedCapability->Cancel();
        }
    }

    return true;
}

/**
* Cancel the capability of the actor.
*
* @param[in] it does not have a parameter.
*
* @return nothing.
*
*/
void CPlayerCapabilityCancel::CActivatedCapability::Cancel(){

    DActor->PopCommand();
}

/**
 *
 * @class CPlayerCapabilityConvey
 *
 * @ brief This class was written to give player capability to Convey \n
 *     <pre>
 *    The class contains two other classes which are CRegistrant and CActivatedCapability.
 *    They are responsible for determining the player who should get the capability and activation of
 *    capability. The class also keep tracks of percentage completion of the capability
 *    the steps and cancellation
             of capability.
 *     </pre>
 * @author Seza
 *
 * @version 1.0
 *
 * @date 10/13/2017 21:04:00
 *
 * Contact: shabibi@ucdavis.edu
 *
 */
class CPlayerCapabilityConvey : public CPlayerCapability{
    protected:

      /**
       *
       *@class CRegistrant
       *
       * @brief the class was written to determine the player who should get the capability
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
         * @brief the class was written to activate the player capability
         *     <pre>
         *      The class is responsible for showing the percentage completion until
         *      the player get the capability,following steps to get capability, and
         *      cancellation
             of capability.
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

            public:
                CActivatedCapability(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata, std::shared_ptr< CPlayerAsset > target);
                virtual ~CActivatedCapability(){};

                int PercentComplete(int max);
                bool IncrementStep();
                void Cancel();
        };
        CPlayerCapabilityConvey();

    public:
        virtual ~CPlayerCapabilityConvey(){};

        bool CanInitiate(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata);
        bool CanApply(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata, std::shared_ptr< CPlayerAsset > target);
        bool ApplyCapability(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata, std::shared_ptr< CPlayerAsset > target);
};

CPlayerCapabilityConvey::CRegistrant CPlayerCapabilityConvey::DRegistrant;

CPlayerCapabilityConvey::CRegistrant::CRegistrant(){
    CPlayerCapability::Register(std::shared_ptr< CPlayerCapabilityConvey >(new CPlayerCapabilityConvey()));
}

CPlayerCapabilityConvey::CPlayerCapabilityConvey() : CPlayerCapability(std::string("Convey"), ETargetType::Asset){

}

/**
* It will check if the actor can gain/ initiate capability.
*
* @param[in] actor shar pointer to the class of PlayerAsset.
* @param[in] playerdata  share pointer to the class of PlayerData
*
* @return boolean function if the actor can initiate capability or not
*
*/
bool CPlayerCapabilityConvey::CanInitiate(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata){
    return actor->Speed() > 0 && (actor->Lumber() || actor->Gold() || actor->Stone());
}

/**
* It will check if capability can be apply to the actor. condition in this case\n
*     <pre>
*     the actor has speed, and it is a lumber or gold it must not doing construction.
*     if target type be TownHall, Keep , and Castle it returns true .
*     if the actor is Lumber and targettypeis LumberMill it returns true.
*    </pre>
*
* @param[in] actor share pointer to the class of PlayerAsset.
* @param[in] playerdata  share pointer to the class of PlayerData
* @param[in] playerdata  share pointer to the class of PlayerAsset
*
* @return boolean function if the actor can be can be applied for capability
*
*/
bool CPlayerCapabilityConvey::CanApply(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata, std::shared_ptr< CPlayerAsset > target){

    if(actor->Speed() && (actor->Lumber() || actor->Gold() || actor->Stone())){
        if(EAssetAction::Construct == target->Action()){
            return false;
        }
        if((EAssetType::TownHall == target->Type())||(EAssetType::Keep == target->Type())||(EAssetType::Castle == target->Type())){
            return true;
        }
        if(actor->Lumber() && (EAssetType::LumberMill == target->Type())){
            return true;
        }
        if(actor->Stone() && (EAssetType::Blacksmith == target->Type())){
            return true;
        }
    }
    return false;
}

/**
* Set actor a new capability. using a struct and PushCommand function
*
* @param[in] actor share pointer to the class of PlayerAsset.
* @param[in] playerdata  share pointer to the class of PlayerData
* @param[in] playerdata  share pointer to the class of PlayerAsset
*
* @return boolean function if the actor got capability or not.
*
*/
bool CPlayerCapabilityConvey::ApplyCapability(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata, std::shared_ptr< CPlayerAsset > target){
    SAssetCommand NewCommand;

    NewCommand.DAction = EAssetAction::Capability;
    NewCommand.DCapability = AssetCapabilityType();
    NewCommand.DAssetTarget = target;
    NewCommand.DActivatedCapability = std::make_shared< CActivatedCapability >(actor, playerdata, target);
    actor->ClearCommand();
    actor->PushCommand(NewCommand);
    return true;
}

CPlayerCapabilityConvey::CActivatedCapability::CActivatedCapability(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata, std::shared_ptr< CPlayerAsset > target) :
CActivatedPlayerCapability(actor, playerdata, target){

}

/**
* Show the percentage completion of the capability.
*
* @param[in] max the number of max percent complete.
*
* @return number of percentage complete.
*
*/
int CPlayerCapabilityConvey::CActivatedCapability::PercentComplete(int max){
    return 0;
}

/**
* Set/Initialize the next step.
*
* @brief it uses two struct to Acknowledge the actor and make the command to gain
*         capability.
*
* @param[in] it does not have a parameter.
*
* @return boolean function if actor gain the capability or not.
*
*/
bool CPlayerCapabilityConvey::CActivatedCapability::IncrementStep(){
    std::weak_ptr< CPlayerAsset > NearestRepository;
    SAssetCommand AssetCommand;
    SGameEvent TempEvent;

    TempEvent.DType = EEventType::Acknowledge;
    TempEvent.DAsset = DActor;
    DPlayerData->AddGameEvent(TempEvent);

    DActor->PopCommand();
    if(DActor->Lumber()){
        AssetCommand.DAction = EAssetAction::ConveyLumber;
        AssetCommand.DAssetTarget = DTarget;
        DActor->PushCommand(AssetCommand);
        AssetCommand.DAction = EAssetAction::Walk;
        DActor->PushCommand(AssetCommand);
        DActor->ResetStep();
    }
    else if(DActor->Gold()){
        AssetCommand.DAction = EAssetAction::ConveyGold;
        AssetCommand.DAssetTarget = DTarget;
        DActor->PushCommand(AssetCommand);
        AssetCommand.DAction = EAssetAction::Walk;
        DActor->PushCommand(AssetCommand);
        DActor->ResetStep();
    }
    else if(DActor->Stone()){
        AssetCommand.DAction = EAssetAction::ConveyStone;
        AssetCommand.DAssetTarget = DTarget;
        DActor->PushCommand(AssetCommand);
        AssetCommand.DAction = EAssetAction::Walk;
        DActor->PushCommand(AssetCommand);
        DActor->ResetStep();
    }

    return true;
}

/**
* Cancel the capability of the actor.
*
* @param[in] it does not have a parameter.
*
* @return nothing.
*
*/
void CPlayerCapabilityConvey::CActivatedCapability::Cancel(){

    DActor->PopCommand();
}

/**
 *
 * @class CPlayerCapabilityPatrol
 *
 * @ brief This class was written to give player capability to Patrol \n
 *     <pre>
 *    The class contains two other classes which are CRegistrant and CActivatedCapability.
 *    They are responsible for determining the player who should get the capability and activation of
 *    capability. The class also keep tracks of percentage completion of the capability
 *    the steps and cancellation
             of capability.
 *     </pre>
 * @author Seza
 *
 * @version 1.0
 *
 * @date 10/13/2017 21:04:00
 *
 * Contact: shabibi@ucdavis.edu
 *
 */
class CPlayerCapabilityPatrol : public CPlayerCapability{
    protected:

      /**
       *
       *@class CRegistrant
       *
       * @brief the class was written to determine the player who should get the capability
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
         * @brief the class was written to activate the player capability
         *     <pre>
         *      The class is responsible for showing the percentage completion until
         *      the player get the capability,following steps to get capability, and
         *      cancellation
             of capability.
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

            public:
                CActivatedCapability(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata, std::shared_ptr< CPlayerAsset > target);
                virtual ~CActivatedCapability(){};

                int PercentComplete(int max);
                bool IncrementStep();
                void Cancel();
        };
        CPlayerCapabilityPatrol();

    public:
        virtual ~CPlayerCapabilityPatrol(){};

        bool CanInitiate(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata);
        bool CanApply(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata, std::shared_ptr< CPlayerAsset > target);
        bool ApplyCapability(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata, std::shared_ptr< CPlayerAsset > target);
};

CPlayerCapabilityPatrol::CRegistrant CPlayerCapabilityPatrol::DRegistrant;

CPlayerCapabilityPatrol::CRegistrant::CRegistrant(){
    CPlayerCapability::Register(std::shared_ptr< CPlayerCapabilityPatrol >(new CPlayerCapabilityPatrol()));
}

CPlayerCapabilityPatrol::CPlayerCapabilityPatrol() : CPlayerCapability(std::string("Patrol"), ETargetType::Terrain){

}

/**
* It will check if the actor can gain/ initiate capability.
*
* @param[in] actor shar pointer to the class of PlayerAsset.
* @param[in] playerdata  share pointer to the class of PlayerData
*
* @return boolean function if the actor can initiate capability or not
*
*/
bool CPlayerCapabilityPatrol::CanInitiate(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata){
    return actor->Speed() > 0;
}

/**
* It will check if capability can be apply to the actor. condition in this case actor's speed > 0.
*
* @param[in] actor share pointer to the class of PlayerAsset.
* @param[in] playerdata  share pointer to the class of PlayerData
* @param[in] playerdata  share pointer to the class of PlayerAsset
*
* @return boolean function if the actor can be applied for capability
*
*/
bool CPlayerCapabilityPatrol::CanApply(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata, std::shared_ptr< CPlayerAsset > target){
    return actor->Speed() > 0;
}

/**
* set actor a new capability. using a struct and PushCommand function
*
* @param[in] actor share pointer to the class of PlayerAsset.
* @param[in] playerdata  share pointer to the class of PlayerData
* @param[in] playerdata  share pointer to the class of PlayerAsset
*
* @return boolean function if the actor got capability or not.
*
*/
bool CPlayerCapabilityPatrol::ApplyCapability(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata, std::shared_ptr< CPlayerAsset > target){
    if(actor->TilePosition() != target->TilePosition()){
        SAssetCommand NewCommand;

        NewCommand.DAction = EAssetAction::Capability;
        NewCommand.DCapability = AssetCapabilityType();
        NewCommand.DAssetTarget = target;
        NewCommand.DActivatedCapability = std::make_shared< CActivatedCapability >(actor, playerdata, target);
        actor->ClearCommand();
        actor->PushCommand(NewCommand);
        return true;
    }

    return false;
}

CPlayerCapabilityPatrol::CActivatedCapability::CActivatedCapability(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata, std::shared_ptr< CPlayerAsset > target) :
CActivatedPlayerCapability(actor, playerdata, target){

}

/**
* show the percentage completion of the capability.
*
* @param[in] max the number of max percent complete.
*
* @return number of percentage complete.
*
*/
int CPlayerCapabilityPatrol::CActivatedCapability::PercentComplete(int max){
    return 0;
}

/**
* Set/Initialize the next step.
*
* @brief it uses two struct to Acknowledge the actor and make the command to gain
*         capability.
*
* @param[in] it does not have a parameter.
*
* @return boolean function if actor gain the capability or not.
*
*/
bool CPlayerCapabilityPatrol::CActivatedCapability::IncrementStep(){
    SAssetCommand PatrolCommand, WalkCommand;
    SGameEvent TempEvent;

    TempEvent.DType = EEventType::Acknowledge;
    TempEvent.DAsset = DActor;
    DPlayerData->AddGameEvent(TempEvent);

    PatrolCommand.DAction = EAssetAction::Capability;
    PatrolCommand.DCapability = EAssetCapabilityType::Patrol;
    PatrolCommand.DAssetTarget = DPlayerData->CreateMarker(DActor->Position(), false);
    PatrolCommand.DActivatedCapability = std::make_shared< CActivatedCapability >(DActor, DPlayerData, PatrolCommand.DAssetTarget);
    DActor->ClearCommand();
    DActor->PushCommand(PatrolCommand);

    WalkCommand.DAction = EAssetAction::Walk;
    WalkCommand.DAssetTarget = DTarget;
    if(!DActor->TileAligned()){
        DActor->Direction(DirectionOpposite(DActor->Position().TileOctant()));
    }
    DActor->PushCommand(WalkCommand);
    return true;
}

/**
* Cancel the capability of the actor.
*
* @param[in] it does not have a parameter.
*
* @return nothing.
*
*/
void CPlayerCapabilityPatrol::CActivatedCapability::Cancel(){

    DActor->PopCommand();
}

/**
 *
 * @class CPlayerCapabilityAttack
 *
 * @ brief This class was written to give player capability to Attack \n
 *     <pre>
 *    The class contains two other classes which are CRegistrant and CActivatedCapability.
 *    They are responsible for determining the player who should get the capability and activation of
 *    capability. The class also keep tracks of percentage completion of the capability
 *    the steps and cancellation
             of capability.
 *     </pre>
 * @author Seza
 *
 * @version 1.0
 *
 * @date 10/13/2017 21:04:00
 *
 * Contact: shabibi@ucdavis.edu
 *
 */
class CPlayerCapabilityAttack : public CPlayerCapability{
    protected:

      /**
       *
       *@class CRegistrant
       *
       * @brief the class was written to determine the player who should get the capability
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
         * @brief the class was written to activate the player capability
         *     <pre>
         *      The class is responsible for showing the percentage completion until
         *      the player get the capability,following steps to get capability, and
         *      cancellation
             of capability.
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

            public:
                CActivatedCapability(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata, std::shared_ptr< CPlayerAsset > target);
                virtual ~CActivatedCapability(){};

                int PercentComplete(int max);
                bool IncrementStep();
                void Cancel();
        };
        CPlayerCapabilityAttack();

    public:
        virtual ~CPlayerCapabilityAttack(){};

        bool CanInitiate(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata);
        bool CanApply(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata, std::shared_ptr< CPlayerAsset > target);
        bool ApplyCapability(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata, std::shared_ptr< CPlayerAsset > target);
};

CPlayerCapabilityAttack::CRegistrant CPlayerCapabilityAttack::DRegistrant;

CPlayerCapabilityAttack::CRegistrant::CRegistrant(){
    CPlayerCapability::Register(std::shared_ptr< CPlayerCapabilityAttack >(new CPlayerCapabilityAttack()));
}

CPlayerCapabilityAttack::CPlayerCapabilityAttack() : CPlayerCapability(std::string("Attack"), ETargetType::Asset){

}

/**
* It will check if the actor can gain/ initiate capability.
*
* @param[in] actor shar pointer to the class of PlayerAsset.
* @param[in] playerdata  share pointer to the class of PlayerData
*
* @return boolean function if the actor can initiate capability or not
*
*/
bool CPlayerCapabilityAttack::CanInitiate(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata){
    return actor->Speed() > 0;
}

/**
* It will check if capability can be apply to the actor. condition:
*     <pre>
*     actor's speed > 0.
*     The actor's color must not equal to target color and player color must not equal target color
*     </pre>
* @param[in] actor share pointer to the class of PlayerAsset.
* @param[in] playerdata  share pointer to the class of PlayerData
* @param[in] playerdata  share pointer to the class of PlayerAsset
*
* @return boolean function if the actor can be applied for capability
*
*/
bool CPlayerCapabilityAttack::CanApply(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata, std::shared_ptr< CPlayerAsset > target){
    if((actor->Color() == target->Color())||(EPlayerColor::None == target->Color())){
        return false;
    }
    return actor->Speed() > 0;
}

/**
* Set actor a new capability. using a struct and PushCommand function
*
* @param[in] actor share pointer to the class of PlayerAsset.
* @param[in] playerdata  share pointer to the class of PlayerData
* @param[in] playerdata  share pointer to the class of PlayerAsset
*
* @return boolean function if the actor got capability or not.
*
*/
bool CPlayerCapabilityAttack::ApplyCapability(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata, std::shared_ptr< CPlayerAsset > target){
    if(actor->TilePosition() != target->TilePosition()){
        SAssetCommand NewCommand;

        NewCommand.DAction = EAssetAction::Capability;
        NewCommand.DCapability = AssetCapabilityType();
        NewCommand.DAssetTarget = target;
        NewCommand.DActivatedCapability = std::make_shared< CActivatedCapability >(actor, playerdata, target);
        actor->ClearCommand();
        actor->PushCommand(NewCommand);
        return true;
    }

    return false;
}

CPlayerCapabilityAttack::CActivatedCapability::CActivatedCapability(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata, std::shared_ptr< CPlayerAsset > target) :
CActivatedPlayerCapability(actor, playerdata, target){

}

/**
* Show the percentage completion of the capability.
*
* @param[in] max the number of max percent complete.
*
* @return number of percentage complete.
*
*/
int CPlayerCapabilityAttack::CActivatedCapability::PercentComplete(int max){
    return 0;
}

/**
* Set/Initialize the next step.
*
* @brief it uses two struct to Acknowledge the actor and make the command to gain
*         capability.
*
* @param[in] it does not have a parameter.
*
* @return boolean function if actor gain the capability or not.
*
*/
bool CPlayerCapabilityAttack::CActivatedCapability::IncrementStep(){
    SAssetCommand AssetCommand;
    SGameEvent TempEvent;

    TempEvent.DType = EEventType::Acknowledge;
    TempEvent.DAsset = DActor;
    DPlayerData->AddGameEvent(TempEvent);

    AssetCommand.DAction = EAssetAction::Attack;
    AssetCommand.DAssetTarget = DTarget;
    DActor->ClearCommand();
    DActor->PushCommand(AssetCommand);

    AssetCommand.DAction = EAssetAction::Walk;
    if(!DActor->TileAligned()){
        DActor->Direction(DirectionOpposite(DActor->Position().TileOctant()));
    }
    DActor->PushCommand(AssetCommand);
    return true;
}

/**
* Cancel the capability of the actor.
*
* @param[in] it does not have a parameter.
*
* @return nothing.
*
*/
void CPlayerCapabilityAttack::CActivatedCapability::Cancel(){

    DActor->PopCommand();
}

/**
 *
 * @class CPlayerCapabilityRepair
 *
 * @ brief This class was written to give player capability to Repair \n
 *     <pre>
 *    The class contains two other classes which are CRegistrant and CActivatedCapability.
 *    They are responsible for determining the player who should get the capability and activation of
 *    capability. The class also keep tracks of percentage completion of the capability
 *    the steps and cancellation
             of capability.
 *     </pre>
 * @author Seza
 *
 * @version 1.0
 *
 * @date 10/13/2017 21:04:00
 *
 * Contact: shabibi@ucdavis.edu
 *
 */
class CPlayerCapabilityRepair : public CPlayerCapability{
    protected:

      /**
       *
       *@class CRegistrant
       *
       * @brief the class was written to determine the player who should get the capability
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
         * @brief the class was written to activate the player capability
         *     <pre>
         *      The class is responsible for showing the percentage completion until
         *      the player get the capability,following steps to get capability, and
         *      cancellation
             of capability.
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

            public:
                CActivatedCapability(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata, std::shared_ptr< CPlayerAsset > target);
                virtual ~CActivatedCapability(){};

                int PercentComplete(int max);
                bool IncrementStep();
                void Cancel();
        };
        CPlayerCapabilityRepair();

    public:
        virtual ~CPlayerCapabilityRepair(){};

        bool CanInitiate(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata);
        bool CanApply(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata, std::shared_ptr< CPlayerAsset > target);
        bool ApplyCapability(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata, std::shared_ptr< CPlayerAsset > target);
};

CPlayerCapabilityRepair::CRegistrant CPlayerCapabilityRepair::DRegistrant;

CPlayerCapabilityRepair::CRegistrant::CRegistrant(){
    CPlayerCapability::Register(std::shared_ptr< CPlayerCapabilityRepair >(new CPlayerCapabilityRepair()));
}

CPlayerCapabilityRepair::CPlayerCapabilityRepair() : CPlayerCapability(std::string("Repair"), ETargetType::Asset){

}

/**
* It will check if the actor can gain/ initiate capability.
*
* @param[in] actor shar pointer to the class of PlayerAsset.
* @param[in] playerdata  share pointer to the class of PlayerData
*
* @returnboolean function if the actor can initiate capability or not
*
*/
bool CPlayerCapabilityRepair::CanInitiate(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata){
    //figure out if need stone here
    return (actor->Speed() > 0) && playerdata->Gold() && playerdata->Lumber();
}

/**
* It will check if capability can be apply to the actor. condition:
*     <pre>
*     actor's color must equal target color and target must not has speed.
*     Target HitPoints  must be smaller than target MaxHitPoints
*     CanInitiate function should return true
*    </pre>
* @param[in] actor share pointer to the class of PlayerAsset.
* @param[in] playerdata  share pointer to the class of PlayerData
* @param[in] playerdata  share pointer to the class of PlayerAsset
*
* @return boolean function if the actor can be applied for capability
*
*/
bool CPlayerCapabilityRepair::CanApply(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata, std::shared_ptr< CPlayerAsset > target){
    if((actor->Color() != target->Color())||(target->Speed())){
        return false;
    }
    if(target->HitPoints() >= target->MaxHitPoints()){
        return false;
    }
    return CanInitiate(actor, playerdata);
}

/**
* Set actor a new capability. using a struct and PushCommand function
*
* @param[in] actor share pointer to the class of PlayerAsset.
* @param[in] playerdata  share pointer to the class of PlayerData
* @param[in] playerdata  share pointer to the class of PlayerAsset
*
* @return boolean function if the actor got apability or not.
*
*/
bool CPlayerCapabilityRepair::ApplyCapability(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata, std::shared_ptr< CPlayerAsset > target){
    if(actor->TilePosition() != target->TilePosition()){
        SAssetCommand NewCommand;

        NewCommand.DAction = EAssetAction::Capability;
        NewCommand.DCapability = AssetCapabilityType();
        NewCommand.DAssetTarget = target;
        NewCommand.DActivatedCapability = std::make_shared< CActivatedCapability >(actor, playerdata, target);
        actor->ClearCommand();
        actor->PushCommand(NewCommand);
        return true;
    }

    return false;
}

CPlayerCapabilityRepair::CActivatedCapability::CActivatedCapability(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata, std::shared_ptr< CPlayerAsset > target) :
CActivatedPlayerCapability(actor, playerdata, target){

}

/**
* Show the percentage completion of the capability.
*
* @param[in] max the number of max percent complete.
*
* @return number of percentage complete.
*
*/
int CPlayerCapabilityRepair::CActivatedCapability::PercentComplete(int max){
    return 0;
}

/**
* Set/Initialize the next step.
*
* @brief it uses two struct to Acknowledge the actor and make the command to gain
*         capability.
*
* @param[in] it does not have a parameter.
*
* @return boolean function if actor gain the capability or not.
*
*/
bool CPlayerCapabilityRepair::CActivatedCapability::IncrementStep(){
    SAssetCommand AssetCommand;
    SGameEvent TempEvent;

    TempEvent.DType = EEventType::Acknowledge;
    TempEvent.DAsset = DActor;
    DPlayerData->AddGameEvent(TempEvent);

    AssetCommand.DAction = EAssetAction::Repair;
    AssetCommand.DAssetTarget = DTarget;
    DActor->ClearCommand();
    DActor->PushCommand(AssetCommand);

    AssetCommand.DAction = EAssetAction::Walk;
    if(!DActor->TileAligned()){
        DActor->Direction(DirectionOpposite(DActor->Position().TileOctant()));
    }
    DActor->PushCommand(AssetCommand);
    return true;
}

/**
* Cancel the capability of the actor.
*
* @param[in] it does not have a parameter.
*
* @return nothing.
*
*/
void CPlayerCapabilityRepair::CActivatedCapability::Cancel(){

    DActor->PopCommand();
}
