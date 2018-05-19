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


#include "PlayerAsset.h"
#include "ApplicationData.h"
#include "CommentSkipLineDataSource.h"
#include "Debug.h"
#include "GameModel.h"
#include <algorithm>


std::unordered_map< std::string, std::shared_ptr< CPlayerAssetType > > CPlayerAssetType::DRegistry;
std::vector< std::string > CPlayerAssetType::DTypeStrings({
"None",
"Peasant",
"Footman",
"Archer",
"Ranger",
"GoldMine",
"TownHall",
"Keep",
"Castle",
"Farm",
"Barracks",
"LumberMill",
"Blacksmith",
"ScoutTower",
"GuardTower",
"CannonTower",
"Wall"});
std::unordered_map< std::string, EAssetType > CPlayerAssetType::DNameTypeTranslation({
{"None", EAssetType::None},
{"Peasant", EAssetType::Peasant},
{"Footman", EAssetType::Footman},
{"Archer", EAssetType::Archer},
{"Ranger", EAssetType::Ranger},
{"GoldMine", EAssetType::GoldMine},
{"TownHall", EAssetType::TownHall},
{"Keep", EAssetType::Keep},
{"Castle", EAssetType::Castle},
{"Farm", EAssetType::Farm},
{"Barracks", EAssetType::Barracks},
{"LumberMill", EAssetType::LumberMill},
{"Blacksmith", EAssetType::Blacksmith},
{"ScoutTower", EAssetType::ScoutTower},
{"GuardTower", EAssetType::GuardTower},
{"CannonTower", EAssetType::CannonTower},
{"Wall", EAssetType::Wall}});

/**
 *
 * @class CPlayerAsset.cpp
 *
 * @brief This class was written to activate player capability. It choose capability
 *        data and Asset target and shows steps of percentage complete.
 *
 * @author Seza
 *
 * @version 4.0
 *
 * @date 10/23/2017 21:04:00
 *
 * Contact: shabibi@ucdavis.edu
 *
 */


/**
* Constructor of the class CActivatedPlayerCapability.
*
* @param[in] actor share pointer to the class of PlayerAsset.
* @param[in] playerdata  share pointer to the class of PlayerData
* @param[in] target  share pointer to the class of PlayerAsset
*
* @return nothing
*
*/
CActivatedPlayerCapability::CActivatedPlayerCapability(std::shared_ptr< CPlayerAsset > actor, std::shared_ptr< CPlayerData > playerdata, std::shared_ptr< CPlayerAsset > target){
    DActor = actor;
    DPlayerData = playerdata;
    DTarget = target;
}

/**
 *
 * @class CPlayerCapability.cpp
 *
 * @brief This class was written to determine a capability by type or name or NameRegistry
 *
 * @author Seza
 *
 * @version 4.0
 *
 * @date 10/23/2017 21:04:00
 *
 * Contact: shabibi@ucdavis.edu
 *
 */

/**
* Constructor of the class CPlayerCapability.
*
* @param[in] name reference to string of type name.
* @param[in] targettype  Enum to ETargetType
*
* @return nothing
*
*/
CPlayerCapability::CPlayerCapability(const std::string &name, ETargetType targettype){
    DName = name;
    DAssetCapabilityType = NameToType(name);
    DTargetType = targettype;
}

/**
* Assigns/returns  unordered_map , name registery of player capability.
*
* @return unordered_map
*
*/
std::unordered_map< std::string, std::shared_ptr< CPlayerCapability > > &CPlayerCapability::NameRegistry(){
    static std::unordered_map< std::string, std::shared_ptr< CPlayerCapability > > TheRegistry;

    return TheRegistry;
}

/**
* Assigns/returns  unordered_map , type registry of player capabilities
*
* @return unordered_map
*
*/
std::unordered_map< int, std::shared_ptr< CPlayerCapability > > &CPlayerCapability::TypeRegistry(){
    static std::unordered_map< int, std::shared_ptr< CPlayerCapability > > TheRegistry;

    return TheRegistry;
}

/**
* Set/Register a capability to a player.
*
* @brief it checks if player already has the capability, if not calls the NameRegistry()
*        and TypeRegistry()function to register name and type of capability
*
* @param[in] capability shared_ptr CPlayerCapability
*
* @return boolean
*
*/
bool CPlayerCapability::Register(std::shared_ptr< CPlayerCapability > capability){
    if(nullptr != FindCapability(capability->Name())){
        return false;
    }
    NameRegistry()[capability->Name()] = capability;
    TypeRegistry()[to_underlying(NameToType(capability->Name()))] = capability;
    return true;
}

/**
* Find capability based on its existence in TypeRegistry
*
* @param[in] type Enum to EAssetCapabilityType
*
* @return shared_ptr to CPlayerCapability
*
*/
std::shared_ptr< CPlayerCapability > CPlayerCapability::FindCapability(EAssetCapabilityType type){
    auto Iterator = TypeRegistry().find(to_underlying(type));

    if(Iterator != TypeRegistry().end()){
        return Iterator->second;
    }
    return std::shared_ptr< CPlayerCapability >();
}

/**
* Find capability based on its existence in NameRegistry
*
* @param[in] name reference to string name of the capability
*
* @return shared_ptr to CPlayerCapability
*
*/
std::shared_ptr< CPlayerCapability > CPlayerCapability::FindCapability(const std::string &name){
    auto Iterator = NameRegistry().find(name);

    if(Iterator != NameRegistry().end()){
        return Iterator->second;
    }
    return std::shared_ptr< CPlayerCapability >();
}

/**
* Get the name of capability and return its corresponding type
*
* @brief
*     <pre>
*     It contains a static unordered_map to name and type. By giving the
*     name of capability it search through the map and if find it return the type
*     if not it print an Error and return asset type None
*     </pre>
*
* @param[in] name reference to string
*
* @return a type in EAssetCapabilityType
*
*/
EAssetCapabilityType CPlayerCapability::NameToType(const std::string &name){
    static std::unordered_map< std::string, EAssetCapabilityType > NameTypeTranslation({
        {"None",EAssetCapabilityType::None},
        {"BuildPeasant",EAssetCapabilityType::BuildPeasant},
        {"BuildFootman",EAssetCapabilityType::BuildFootman},
        {"BuildArcher",EAssetCapabilityType::BuildArcher},
        {"BuildRanger",EAssetCapabilityType::BuildRanger},
        {"BuildKnight",EAssetCapabilityType::BuildKnight},
        {"BuildFarm",EAssetCapabilityType::BuildFarm},
        {"BuildTownHall",EAssetCapabilityType::BuildTownHall},
        {"BuildBarracks",EAssetCapabilityType::BuildBarracks},
        {"BuildLumberMill",EAssetCapabilityType::BuildLumberMill},
        {"BuildBlacksmith",EAssetCapabilityType::BuildBlacksmith},
        {"BuildKeep",EAssetCapabilityType::BuildKeep},
        {"BuildCastle",EAssetCapabilityType::BuildCastle},
        {"BuildScoutTower",EAssetCapabilityType::BuildScoutTower},
        {"BuildGuardTower",EAssetCapabilityType::BuildGuardTower},
        {"BuildCannonTower",EAssetCapabilityType::BuildCannonTower},
        {"Move",EAssetCapabilityType::Move},
        {"Repair",EAssetCapabilityType::Repair},
        {"Mine",EAssetCapabilityType::Mine},
        {"BuildSimple",EAssetCapabilityType::BuildSimple},
        {"BuildAdvanced",EAssetCapabilityType::BuildAdvanced},
        {"Convey",EAssetCapabilityType::Convey},
        {"Cancel",EAssetCapabilityType::Cancel},
        {"BuildWall",EAssetCapabilityType::BuildWall},
        {"Attack",EAssetCapabilityType::Attack},
        {"StandGround",EAssetCapabilityType::StandGround},
        {"Patrol",EAssetCapabilityType::Patrol},
        {"WeaponUpgrade1",EAssetCapabilityType::WeaponUpgrade1},
        {"WeaponUpgrade2",EAssetCapabilityType::WeaponUpgrade2},
        {"WeaponUpgrade3",EAssetCapabilityType::WeaponUpgrade3},
        {"ArrowUpgrade1",EAssetCapabilityType::ArrowUpgrade1},
        {"ArrowUpgrade2",EAssetCapabilityType::ArrowUpgrade2},
        {"ArrowUpgrade3",EAssetCapabilityType::ArrowUpgrade3},
        {"ArmorUpgrade1",EAssetCapabilityType::ArmorUpgrade1},
        {"ArmorUpgrade2",EAssetCapabilityType::ArmorUpgrade2},
        {"ArmorUpgrade3",EAssetCapabilityType::ArmorUpgrade3},
        {"Longbow",EAssetCapabilityType::Longbow},
        {"RangerScouting",EAssetCapabilityType::RangerScouting},
        {"Marksmanship",EAssetCapabilityType::Marksmanship}
    });
    auto Iterator = NameTypeTranslation.find(name);

    if(Iterator != NameTypeTranslation.end()){
        return Iterator->second;
    }
    PrintError("Unknown capability name \"%s\"\n",name.c_str());
    return EAssetCapabilityType::None;
}




/**
* Get the type of capability and return its corresponding name.
*     It contains a static vector to contain types. By giving the
*     type it checks its validation and return the name of the type
*
* @param[in] type enum to EAssetCapabilityType
*
* @return a string name
*
*/
std::string CPlayerCapability::TypeToName(EAssetCapabilityType type){
    static std::vector< std::string > TypeStrings({
        "None",
        "BuildPeasant",
        "BuildFootman",
        "BuildArcher",
        "BuildRanger",
        "BuildFarm",
        "BuildTownHall",
        "BuildBarracks",
        "BuildLumberMill",
        "BuildBlacksmith",
        "BuildKeep",
        "BuildCastle",
        "BuildScoutTower",
        "BuildGuardTower",
        "BuildCannonTower",
        "Move",
        "Repair",
        "Mine",
        "BuildSimple",
        "BuildAdvanced",
        "Convey",
        "Cancel",
        "BuildWall",
        "Attack",
        "StandGround",
        "Patrol",
        "WeaponUpgrade1",
        "WeaponUpgrade2",
        "WeaponUpgrade3",
        "ArrowUpgrade1",
        "ArrowUpgrade2",
        "ArrowUpgrade3",
        "ArmorUpgrade1",
        "ArmorUpgrade2",
        "ArmorUpgrade3",
        "Longbow",
        "RangerScouting",
        "Marksmanship",});
    if((0 > to_underlying(type))||(to_underlying(type) >= TypeStrings.size())){
        return "";
    }
    return TypeStrings[to_underlying(type)];
}


std::unordered_map< std::string, std::shared_ptr< CPlayerUpgrade > > CPlayerUpgrade::DRegistryByName;
std::unordered_map< int, std::shared_ptr< CPlayerUpgrade > > CPlayerUpgrade::DRegistryByType;

/**
 *
 * @class CPlayerUpgrade.cpp
 *
 * @brief This class was written to upgrade player capabilities. it has list of all
 *        capabilities and load upgrades finding them from type or name
 *
 * @author Seza
 *
 * @version 4.0
 *
 * @date 10/23/2017 21:04:00
 *
 * Contact: shabibi@ucdavis.edu
 *
 */

CPlayerUpgrade::CPlayerUpgrade(){

}

/**
* Upgrade/Load the player capability
*    <pre>
*     It assign container pointer to the CDataContainer FileIterator. if the container
*     is empty it return false otherwise if  container is valid it reads the Data
*     from the next file. if .dat data filename be equal container name and if it
*     can load container's DataSource it upgrades player capability
*     </pre>
*
* @param[in] container a shared_ptr to CDataContainer
*
* @return boolean if player be upgraded or not
*/
bool CPlayerUpgrade::LoadUpgrades(std::shared_ptr< CDataContainer > container){
    auto FileIterator = container->First();
    if(FileIterator == nullptr){
        PrintError("FileIterator == nullptr\n");
        return false;
    }

    while((FileIterator != nullptr)&&(FileIterator->IsValid())){
        std::string Filename = FileIterator->Name();
        FileIterator->Next();
        if(Filename.rfind(".dat") == (Filename.length() - 4)){
            if(!CPlayerUpgrade::Load(container->DataSource(Filename))){
                PrintError("Failed to load upgrade \"%s\".\n",Filename.c_str());
                continue;
            }
            else{
                PrintDebug(DEBUG_LOW,"Loaded upgrade \"%s\".\n",Filename.c_str());
            }
        }
    }

    return true;
}

/**
* Determine the condition for load upgrate
*     <pre>
*     - If the source file is empty or if the LineSource could not read the name
*       of the upgrade it return false
*     - If upgrade type and player capability name is equal "None" it returns false.
*       else it assigns the upgrade name and type to DRegistryByName and DRegistryByType
*     - try to upgrade all the capabilities in order of DArmor, DSight, DSpeed, DBasicDamage,
*       DPiercingDamage, DRange, DGoldCost, DLumberCost, DResearchTime, DAffectedAssets. anytime
*       there is no more line to read it prints an Error and it goes to the LoadExit lable
*     - if it could not read the file it catch an Exception and print an Error
*     </pre>
*
* @param[in] source a shared_ptr to CDataSource
*
* @return boolean if load is possible or not
*/
bool CPlayerUpgrade::Load(std::shared_ptr< CDataSource > source){
    CCommentSkipLineDataSource LineSource(source, '#');
    std::string Name, TempString;
    std::shared_ptr< CPlayerUpgrade > PlayerUpgrade;
    EAssetCapabilityType UpgradeType;
    int AffectedAssetCount;
    bool ReturnStatus = false;

    if(nullptr == source){
        return false;
    }
    if(!LineSource.Read(Name)){
        PrintError("Failed to get player upgrade name.\n");
        return false;
    }
    UpgradeType = CPlayerCapability::NameToType(Name);
    if((EAssetCapabilityType::None == UpgradeType) && (Name != CPlayerCapability::TypeToName(EAssetCapabilityType::None))){
        PrintError("Unknown upgrade type %s.\n", Name.c_str());
        return false;
    }
    auto Iterator = DRegistryByName.find(Name);
    if(DRegistryByName.end() != Iterator){
        PlayerUpgrade = Iterator->second;
    }
    else{
        PlayerUpgrade = std::make_shared< CPlayerUpgrade >();
        PlayerUpgrade->DName = Name;
        DRegistryByName[Name] = PlayerUpgrade;
        DRegistryByType[to_underlying(UpgradeType)] = PlayerUpgrade;
    }
    try{
        if(!LineSource.Read(TempString)){
            PrintError("Failed to get upgrade armor.\n");
            goto LoadExit;
        }
        PlayerUpgrade->DArmor = std::stoi(TempString);
        if(!LineSource.Read(TempString)){
            PrintError("Failed to get upgrade sight.\n");
            goto LoadExit;
        }
        PlayerUpgrade->DSight = std::stoi(TempString);
        if(!LineSource.Read(TempString)){
            PrintError("Failed to get upgrade speed.\n");
            goto LoadExit;
        }
        PlayerUpgrade->DSpeed = std::stoi(TempString);
        if(!LineSource.Read(TempString)){
            PrintError("Failed to get upgrade basic damage.\n");
            goto LoadExit;
        }
        PlayerUpgrade->DBasicDamage = std::stoi(TempString);
        if(!LineSource.Read(TempString)){
            PrintError("Failed to get upgrade piercing damage.\n");
            goto LoadExit;
        }
        PlayerUpgrade->DPiercingDamage = std::stoi(TempString);
        if(!LineSource.Read(TempString)){
            PrintError("Failed to get upgrade range.\n");
            goto LoadExit;
        }
        PlayerUpgrade->DRange = std::stoi(TempString);
        if(!LineSource.Read(TempString)){
            PrintError("Failed to get upgrade gold cost.\n");
            goto LoadExit;
        }
        PlayerUpgrade->DGoldCost = std::stoi(TempString);
        if(!LineSource.Read(TempString)){
            PrintError("Failed to get upgrade lumber cost.\n");
            goto LoadExit;
        }
        PlayerUpgrade->DLumberCost = std::stoi(TempString);
        if(!LineSource.Read(TempString)){
            PrintError("Failed to get upgrade stone cost.\n");
            goto LoadExit;
        }
        PlayerUpgrade->DStoneCost = std::stoi(TempString);
        if(!LineSource.Read(TempString)){
            PrintError("Failed to get upgrade research time.\n");
            goto LoadExit;
        }
        PlayerUpgrade->DResearchTime = std::stoi(TempString);
        if(!LineSource.Read(TempString)){
            PrintError("Failed to get upgrade affected asset count.\n");
            goto LoadExit;
        }
        AffectedAssetCount = std::stoi(TempString);
        for(int Index = 0; Index < AffectedAssetCount; Index++){
            if(!LineSource.Read(TempString)){
                PrintError("Failed to read upgrade affected asset %d.\n", Index);
                goto LoadExit;
            }
            PlayerUpgrade->DAffectedAssets.push_back(CPlayerAssetType::NameToType(TempString));
        }
        ReturnStatus = true;
    }
    catch(std::exception &E){
        PrintError("%s\n",E.what());
    }
LoadExit:
    return ReturnStatus;
}

/**
* Find upgrade by looking at its type
*     <pre>
*     if it finds the upgrade by its type, it returns the upgrade.
*     </pre>
*
* @param[in] type enum to EAssetCapabilityType
*
* @return a shared_ptr to CPlayerUpgrade
*
*/
std::shared_ptr< CPlayerUpgrade > CPlayerUpgrade::FindUpgradeFromType(EAssetCapabilityType type){
    auto Iterator = DRegistryByType.find(to_underlying(type));

    if(Iterator != DRegistryByType.end()){
        return Iterator->second;
    }
    return std::shared_ptr< CPlayerUpgrade >();
}

/**
* Find upgrade by looking at its name
*     <pre>
*     if it finds the upgrade by its name, it returns the upgrade.
*     </pre>
*
* @param[in] type enum to EAssetCapabilityType
*
* @return a shared_ptr to CPlayerUpgrade
*
*/
std::shared_ptr< CPlayerUpgrade > CPlayerUpgrade::FindUpgradeFromName(const std::string &name){
    auto Iterator = DRegistryByName.find( name );

    if(Iterator != DRegistryByName.end()){
        return Iterator->second;
    }
    return std::shared_ptr< CPlayerUpgrade >();
}

/**
 *
 * @class CPlayerAssetType.cpp
 *
 * @brief This class was written to upgrade asset type capabilities. it has list of all
 *        Asset types and load and upgrades them from the new asset
 *
 * @author Seza
 *
 * @version 4.0
 *
 * @date 10/23/2017 21:04:00
 *
 * Contact: shabibi@ucdavis.edu
 *
 */

/**
* Default constructor CPlayerAssetType
*     <pre>
*     it resizes DCapabilities and assigns a false to all of them. also initilizes
*     other capabilities.
*     </pre>
*
* @param[in] None
*
* @return None
*
*/
CPlayerAssetType::CPlayerAssetType(){
    DCapabilities.resize(to_underlying(EAssetCapabilityType::Max));
    for(int Index = 0; Index < DCapabilities.size(); Index++){
        DCapabilities[Index] = false;
    }
    DHitPoints = 1;
    DArmor = 0;
    DSight = 0;
    DConstructionSight = 0;
    DSize = 1;
    DSpeed = 0;
    DGoldCost = 0;
    DLumberCost = 0;
    DStoneCost = 0;
    DFoodConsumption = 0;
    DBuildTime = 0;
    DAttackSteps = 0;
    DReloadSteps = 0;
    DBasicDamage = 0;
    DPiercingDamage = 0;
    DRange = 0;
}

/**
* Copy Constructor CPlayerAssetType
*     <pre>
*     it gets an asset and initiate the player with its capabilities
*     </pre>
*
* @param[in] asset shared_ptr to CPlayerAssetType
*
* @return None
*
*/
CPlayerAssetType::CPlayerAssetType(std::shared_ptr< CPlayerAssetType > asset){
    if(nullptr != asset){
        DName = asset->DName;
        DType = asset->DType;
        DColor = asset->DColor;
        DCapabilities = asset->DCapabilities;
        DAssetRequirements = asset->DAssetRequirements;
        DHitPoints = asset->DHitPoints;
        DArmor = asset->DArmor;
        DSight = asset->DSight;
        DConstructionSight = asset->DConstructionSight;
        DSize = asset->DSize;
        DSpeed = asset->DSpeed;
        DGoldCost = asset->DGoldCost;
        DLumberCost = asset->DLumberCost;
        DStoneCost = asset->DStoneCost;
        DFoodConsumption = asset->DFoodConsumption;
        DBuildTime = asset->DBuildTime;
        DAttackSteps = asset->DAttackSteps;
        DReloadSteps = asset->DReloadSteps;
        DBasicDamage = asset->DBasicDamage;
        DPiercingDamage = asset->DPiercingDamage;
        DRange = asset->DRange;
    }
}

/**
* Destructor CPlayerAssetType
*
* @param[in] None
*
* @return None
*
*/

CPlayerAssetType::~CPlayerAssetType(){

}

/**
* Do upgrade on Armor over range current capability and new DAssetUpgrades
*
* @param[in] None
*
* @return current upgrade value
*
*/
int CPlayerAssetType::ArmorUpgrade() const{
    int ReturnValue = 0;
    for(auto Upgrade : DAssetUpgrades){
        ReturnValue += Upgrade->Armor();
    }
    return ReturnValue;
}

/**
* Do upgrade on Sight over range current capability and new DAssetUpgrades
*
* @param[in] None
*
* @return current upgrade value
*
*/
int CPlayerAssetType::SightUpgrade() const{
    int ReturnValue = 0;
    for(auto Upgrade : DAssetUpgrades){
        ReturnValue += Upgrade->Sight();
    }
    return ReturnValue;
}

/**
* Do upgrade on Speed over range current capability and new DAssetUpgrades
*
* @param[in] None
*
* @return current upgrade value
*
*/
int CPlayerAssetType::SpeedUpgrade() const{
    int ReturnValue = 0;
    for(auto Upgrade : DAssetUpgrades){
        ReturnValue += Upgrade->Speed();
    }
    return ReturnValue;
}

/**
* Do upgrade on BasicDamage over range current capability and new DAssetUpgrades
*
* @param[in] None
*
* @return current upgrade value
*
*/
int CPlayerAssetType::BasicDamageUpgrade() const{
    int ReturnValue = 0;
    for(auto Upgrade : DAssetUpgrades){
        ReturnValue += Upgrade->BasicDamage();
    }
    return ReturnValue;
}

/**
* Do upgrade on PiercingDamage over range current capability and new DAssetUpgrades
*
* @param[in] None
*
* @return current upgrade value
*
*/
int CPlayerAssetType::PiercingDamageUpgrade() const{
    int ReturnValue = 0;
    for(auto Upgrade : DAssetUpgrades){
        ReturnValue += Upgrade->PiercingDamage();
    }
    return ReturnValue;
}

/**
* Do upgrade on Range over range current capability and new DAssetUpgrades
*
* @param[in] None
*
* @return current upgrade value
*
*/
int CPlayerAssetType::RangeUpgrade() const{
    int ReturnValue = 0;
    for(auto Upgrade : DAssetUpgrades){
        ReturnValue += Upgrade->Range();
    }
    return ReturnValue;
}

/**
* Go through all the capabilities and if a capability is true, push_back it in the vector
*
* @param[in] None
*
* @return vectore of capabilities
*
*/
std::vector< EAssetCapabilityType > CPlayerAssetType::Capabilities() const{
    std::vector< EAssetCapabilityType > ReturnVector;

    for(int Index = to_underlying(EAssetCapabilityType::None); Index < to_underlying(EAssetCapabilityType::Max); Index++){
        if(DCapabilities[Index]){
            ReturnVector.push_back((EAssetCapabilityType)Index);
        }
    }
    return ReturnVector;
}

/**
* Gets the name of a capability and return its type
* if it finds the name it returns its type or None if no success
*
* @param[in] name reference to string
*
* @return EAssetType
*
*/
EAssetType CPlayerAssetType::NameToType(const std::string &name){
    auto Iterator = DNameTypeTranslation.find(name);
    if(Iterator != DNameTypeTranslation.end()){
        return Iterator->second;
    }
    return EAssetType::None;
}

/**
* Gets the type of a capability and return its name
* if it finds the type it returns its name or empty string if no success
*
* @param[in] name reference to string
*
* @return EAssetType
*
*/
std::string CPlayerAssetType::TypeToName(EAssetType type){
    if((0 > to_underlying(type))||(to_underlying(type) >= DTypeStrings.size())){
        return "";
    }
    return DTypeStrings[to_underlying(type)];
}

/**
* Upgrade the max sight can be seen by player
* if the maxsight is samller it keeps the maxsight otherwise it create the maxsight
*
* @param[in] None
*
* @return Integer of maxsight
*
*/
int CPlayerAssetType::MaxSight(){
    int MaxSightFound = 0;

    for(auto &ResType : DRegistry){
        MaxSightFound = MaxSightFound > ResType.second->DSight + ResType.second->DSize ? MaxSightFound : ResType.second->DSight + ResType.second->DSize;
    }
    return MaxSightFound;
}

/**
* Upgrade/Load the PalyerAssettype capability
*     <pre>
*     It assigns container pointer to the CDataContainer FileIterator. If the container
*     is empty it return false otherwise if  container is valid it reads the Data
*     from the next file. if .dat data filename be equal container name and if it
*     can load container's DataSource it upgrades player capability type
*     </pre>
*
* @param[in] container a shared_ptr to CDataContainer
*
* @return boolean if PlayerAssetType be upgraded or not
*/

bool CPlayerAssetType::LoadTypes(std::shared_ptr< CDataContainer > container){
    auto FileIterator = container->First();
    if(FileIterator == nullptr){
        PrintError("FileIterator == nullptr\n");
        return false;
    }

    while((FileIterator != nullptr)&&(FileIterator->IsValid())){
        std::string Filename = FileIterator->Name();
        FileIterator->Next();
        if(Filename.rfind(".dat") == (Filename.length() - 4)){
            if(!CPlayerAssetType::Load(container->DataSource(Filename))){
                PrintError("Failed to load resource \"%s\".\n",Filename.c_str());
                continue;
            }
            else{
                PrintDebug(DEBUG_LOW,"Loaded resource \"%s\".\n",Filename.c_str());
            }
        }
    }
    std::shared_ptr< CPlayerAssetType >  PlayerAssetType = std::make_shared< CPlayerAssetType >();
    PlayerAssetType->DThis = PlayerAssetType;
    PlayerAssetType->DName = "None";
    PlayerAssetType->DType = EAssetType::None;
    PlayerAssetType->DColor = EPlayerColor::None;
    PlayerAssetType->DHitPoints = 256;
    DRegistry["None"] = PlayerAssetType;

    return true;
}

/**
* Determine the condition for type load upgrate
*     <pre>
*     - If the source file is empty or if the LineSource could not read the name
*       of the upgrade it return false
*     - If upgrade type and palyer capabilty type is equal "None" it returns false.
*       else it assigns name and type and color of the type to player
*     - try to upgrade all the capabilities in order of DHitPoints, DArmor, DSight,DConstructionSight, Dsize, DSpeed,
*       DGoldCost, DLumberCost, DFoodConsumption,DBuildTime, DAttackSteps,DReloadSteps,
*       DBasicDamage, DPiercingDamage, DRange, DCapabilities, DAssetRequirements. anytime
*       there is no more line to read it prints an Error and it goes to the LoadExit label
*     - if it could not read the file it catch an Exception and print an Error
*     </pre>
*
* @param[in] source a shared_ptr to CDataSource
*
* @return boolean if load is possible or not
*/
bool CPlayerAssetType::Load(std::shared_ptr< CDataSource > source){
    CCommentSkipLineDataSource LineSource(source, '#');
    std::string Name, TempString;
    std::shared_ptr< CPlayerAssetType > PlayerAssetType;
    EAssetType AssetType;
    int CapabilityCount, AssetRequirementCount;
    bool ReturnStatus = false;

    if(nullptr == source){
        return false;
    }
    if(!LineSource.Read(Name)){
        PrintError("Failed to get resource type name.\n");
        return false;
    }
    AssetType = NameToType(Name);
    if((EAssetType::None == AssetType) && (Name != DTypeStrings[to_underlying(EAssetType::None)])){
        PrintError("Unknown resource type %s.\n", Name.c_str());
        return false;
    }
    auto Iterator = DRegistry.find(Name);
    if(DRegistry.end() != Iterator){
        PlayerAssetType = Iterator->second;
    }
    else{
        PlayerAssetType = std::make_shared< CPlayerAssetType >();
        PlayerAssetType->DThis = PlayerAssetType;
        PlayerAssetType->DName = Name;
        DRegistry[Name] = PlayerAssetType;
    }
    PlayerAssetType->DType = AssetType;
    PlayerAssetType->DColor = EPlayerColor::None;
    try{
        if(!LineSource.Read(TempString)){
            PrintError("Failed to get resource type hit points.\n");
            goto LoadExit;
        }
        PlayerAssetType->DHitPoints = std::stoi(TempString);
        if(!LineSource.Read(TempString)){
            PrintError("Failed to get resource type armor.\n");
            goto LoadExit;
        }
        PlayerAssetType->DArmor = std::stoi(TempString);
        if(!LineSource.Read(TempString)){
            PrintError("Failed to get resource type sight.\n");
            goto LoadExit;
        }
        PlayerAssetType->DSight = std::stoi(TempString);
        if(!LineSource.Read(TempString)){
            PrintError("Failed to get resource type construction sight.\n");
            goto LoadExit;
        }
        PlayerAssetType->DConstructionSight = std::stoi(TempString);
        if(!LineSource.Read(TempString)){
            PrintError("Failed to get resource type size.\n");
            goto LoadExit;
        }
        PlayerAssetType->DSize = std::stoi(TempString);
        if(!LineSource.Read(TempString)){
            PrintError("Failed to get resource type speed.\n");
            goto LoadExit;
        }
        PlayerAssetType->DSpeed = std::stoi(TempString);
        if(!LineSource.Read(TempString)){
            PrintError("Failed to get resource type gold cost.\n");
            goto LoadExit;
        }
        PlayerAssetType->DGoldCost = std::stoi(TempString);
        if(!LineSource.Read(TempString)){
            PrintError("Failed to get resource type lumber cost.\n");
            goto LoadExit;
        }
        PlayerAssetType->DLumberCost = std::stoi(TempString);
        if(!LineSource.Read(TempString)){
            PrintError("Failed to get resource type stone cost.\n");
            goto LoadExit;
        }
        PlayerAssetType->DStoneCost = std::stoi(TempString);
        if(!LineSource.Read(TempString)){
            PrintError("Failed to get resource type food consumption.\n");
            goto LoadExit;
        }
        PlayerAssetType->DFoodConsumption = std::stoi(TempString);
        if(!LineSource.Read(TempString)){
            PrintError("Failed to get resource type build time.\n");
            goto LoadExit;
        }
        PlayerAssetType->DBuildTime = std::stoi(TempString);
        if(!LineSource.Read(TempString)){
            PrintError("Failed to get resource type attack steps.\n");
            goto LoadExit;
        }
        PlayerAssetType->DAttackSteps = std::stoi(TempString);
        if(!LineSource.Read(TempString)){
            PrintError("Failed to get resource type reload steps.\n");
            goto LoadExit;
        }
        PlayerAssetType->DReloadSteps = std::stoi(TempString);
        if(!LineSource.Read(TempString)){
            PrintError("Failed to get resource type basic damage.\n");
            goto LoadExit;
        }
        PlayerAssetType->DBasicDamage = std::stoi(TempString);
        if(!LineSource.Read(TempString)){
            PrintError("Failed to get resource type piercing damage.\n");
            goto LoadExit;
        }
        PlayerAssetType->DPiercingDamage = std::stoi(TempString);
        if(!LineSource.Read(TempString)){
            PrintError("Failed to get resource type range.\n");
            goto LoadExit;
        }
        PlayerAssetType->DRange = std::stoi(TempString);
        if(!LineSource.Read(TempString)){
            PrintError("Failed to get capability count.\n");
            goto LoadExit;
        }
        CapabilityCount = std::stoi(TempString);
        PlayerAssetType->DCapabilities.resize(to_underlying(EAssetCapabilityType::Max));
        for(int Index = 0; Index < PlayerAssetType->DCapabilities.size(); Index++){
            PlayerAssetType->DCapabilities[Index] = false;
        }
        for(int Index = 0; Index < CapabilityCount; Index++){
            if(!LineSource.Read(TempString)){
                PrintError("Failed to read capability %d.\n", Index);
                goto LoadExit;
            }
            PlayerAssetType->AddCapability(CPlayerCapability::NameToType(TempString));
        }

        if(!LineSource.Read(TempString)){
            PrintError("Failed to get asset requirement count.\n");
            goto LoadExit;
        }
        AssetRequirementCount = std::stoi(TempString);
        for(int Index = 0; Index < AssetRequirementCount; Index++){
            if(!LineSource.Read(TempString)){
                PrintError("Failed to read asset requirement %d.\n", Index);
                goto LoadExit;
            }
            PlayerAssetType->DAssetRequirements.push_back(NameToType(TempString));
        }


        ReturnStatus = true;
    }
    catch(std::exception &E){
        PrintError("%s\n",E.what());
    }
LoadExit:
    return ReturnStatus;
}

/**
* Find type of upgrade by looking at its name
*     <pre>
*     if it finds the upgrade by its name, it returns the type of upgrade.
*     </pre>
*
* @param[in] name a reference to string
8
* @return a shared_ptr to CPlayerAssetType
*
*/
std::shared_ptr< CPlayerAssetType > CPlayerAssetType::FindDefaultFromName(const std::string &name){
    auto Iterator = DRegistry.find( name );

    if(Iterator != DRegistry.end()){
        return Iterator->second;
    }
    return std::shared_ptr< CPlayerAssetType >();
}

/**
* Find type of upgrade by looking at its TYPE
*     <pre>
*     if it finds the upgrade by its type, it returns the type of upgrade.
*     </pre>
*
* @param[in] type Enum to EAssetType
8
* @return a shared_ptr to CPlayerAssetType
*
*/
std::shared_ptr< CPlayerAssetType > CPlayerAssetType::FindDefaultFromType(EAssetType type){
    return FindDefaultFromName( TypeToName(type) );
}

/**
* Duplice a player asset
*
* @param[in] color Enum to EPlayerColor
*
* @return a shared_ptr to an unordered_map
*
*/
std::shared_ptr< std::unordered_map< std::string, std::shared_ptr< CPlayerAssetType > > > CPlayerAssetType::DuplicateRegistry(EPlayerColor color){
    std::shared_ptr< std::unordered_map< std::string, std::shared_ptr< CPlayerAssetType > > > ReturnRegistry( new std::unordered_map< std::string, std::shared_ptr< CPlayerAssetType > > );

    for(auto &It : DRegistry){
        std::shared_ptr< CPlayerAssetType > NewAssetType(new CPlayerAssetType(It.second));
        NewAssetType->DThis = NewAssetType;
        NewAssetType->DColor = color;
        (*ReturnRegistry)[It.first] = NewAssetType;
    }

    return ReturnRegistry;
}

/**
* Build/ construct new asset type for the player
*
* @param[in] None
*
* @return shared_ptr to CPlayerAsset
*/
std::shared_ptr< CPlayerAsset > CPlayerAssetType::Construct(){
    if(auto ThisShared = DThis.lock()){
        return std::shared_ptr< CPlayerAsset >(new CPlayerAsset(ThisShared));
    }
    return nullptr;
}

int CPlayerAsset::DUpdateFrequency = 1;
int CPlayerAsset::DUpdateDivisor = 32;
CRandomNumberGenerator CPlayerAsset::DGenerateRandomNum;

/**
* Determine the frequency that player asset should be updated
*
* @brief it update DUpdateDivisor as well as DUpdateFrequency
*
* @param[in] freq int
*
* @return shared_ptr to CPlayerAsset
*/
int CPlayerAsset::UpdateFrequency(int freq){
    if(0 < freq){
       DUpdateFrequency = freq;
       DUpdateDivisor = 32 * DUpdateFrequency;
    }
    return DUpdateFrequency;
}

/**
 *
 * @class CPlayerAsset.cpp
 *
 * @brief This class was written to keep tracks of player asset such as gold, lumber, stone.
 * also it turns assets inorder, keep tracks of their position, their steps toward destention,thier
 *  type, their update frequency and divisor, and their command that can take.
 *
 * @author Seza
 *
 * @version 4.0
 *
 * @date 10/30/2017 21:04:00
 *
 * Contact: shabibi@ucdavis.edu
 *
 */

/**
* Defult constrctor for CPlayerAsset and CPixelPosition
*
* @param[in] type shared_ptr to CPlayerAssetType
*
* @return None
*/
CPlayerAsset::CPlayerAsset(std::shared_ptr< CPlayerAssetType > type) : DPosition(0,0){
    DAssetID = GetAssetIDCount();
    DCreationCycle = 0;
    DType = type;
    DHitPoints = type->HitPoints();
    DGold = 0;
    DLumber = 0;
    DStone = 0; //initilize Dstone
    DStep = 0;
    DMoveRemainderX = 0;
    DMoveRemainderY = 0;
    DDirection = EDirection::South;
    TilePosition(CTilePosition());
}

CPlayerAsset::~CPlayerAsset(){

}

/**
* Get current position of Tile
*
* @param[in] None
*
* @return CTilePosition
*/
CTilePosition CPlayerAsset::TilePosition() const{
    CTilePosition ReturnPosition;

    ReturnPosition.SetFromPixel(DPosition);
    return ReturnPosition;
}

/**
* Set object position which is pass to function
*
* @param[in] pos reference to CTilePosition
*
* @return CTilePosition
*/
CTilePosition CPlayerAsset::TilePosition(const CTilePosition &pos){
    DPosition.SetFromTile(pos);
    return pos;
}

/**
* Get object x axis
*
* @param[in] None
*
* @return int to X axis
*/
int CPlayerAsset::TilePositionX() const{
    CTilePosition ReturnPosition;

    ReturnPosition.SetFromPixel(DPosition);
    return ReturnPosition.X();
}

/**
* Set object X axis
*
* @param[in] x int
*
* @return int to x axis
*/
int CPlayerAsset::TilePositionX(int x){
    DPosition.SetXFromTile(x);
    return x;
}

/**
* Get object Y axis
*
* @param[in] None
*
* @return int to Y axis
*/
int CPlayerAsset::TilePositionY() const{
    CTilePosition ReturnPosition;

    ReturnPosition.SetFromPixel(DPosition);
    return ReturnPosition.Y();
}

/**
* Set object Y axis
*
* @param[in] y int
*
* @return int to y axis
*/
int CPlayerAsset::TilePositionY(int y){
    DPosition.SetYFromTile(y);
    return y;
}

/**
* Set position of CPlayerAsset
*
* @param[in] pos reference to CPixelPosition
*
* @return CPixelPosition to pos
*/
CPixelPosition CPlayerAsset::Position(const CPixelPosition &pos){
    return DPosition = pos;
}

/**
* Set X axis of the CPlayerAsset's position
*
* @param[in] x int
*
* @return int to DPosition.X
*/
int CPlayerAsset::PositionX(int x){
    return DPosition.X(x);
}

/**
* Set Y axis of the CPlayerAsset's position
*
* @param[in] y int
*
* @return int to DPosition.Y
*/
int CPlayerAsset::PositionY(int y){
    return DPosition.Y(y);
};

/**
* Get closest position that player asset can go
* it calls ClosestPosition function from CPixelPosition which has two params
*
* @param[in] pos a reference to CPixelPosition
*
* @return CPixelPosition ClosestPosition
*/
CPixelPosition CPlayerAsset::ClosestPosition(const CPixelPosition &pos) const{
    return pos.ClosestPosition(DPosition, Size());
}

/**
* Get the current command/interrupt handeler
* based on current command action assign an action
*
* @param[in] None
*
* @return boolean based on the Command.Daction case
*/
bool CPlayerAsset::Interruptible() const{
    SAssetCommand Command = CurrentCommand();

    switch(Command.DAction){
        case EAssetAction::Construct:
        case EAssetAction::Build:
        case EAssetAction::MineGold:
        case EAssetAction::ConveyLumber:
        case EAssetAction::ConveyStone:
        case EAssetAction::ConveyGold:
        case EAssetAction::Death:
        case EAssetAction::Decay:           return false;
        case EAssetAction::Capability:      if(Command.DAssetTarget){
                                                return EAssetAction::Construct != Command.DAssetTarget->Action();
                                            }
        default:                            return true;
    }
}

/**
* Move player asset across diagonals
*     <pre>
*     the function move player assets from the current tile to new tile position.
*     for doing that it organize its step octant by Octant if the destantion is
*     in current octant it moves to destention and return true. Else, it save move
*     reminder and start from that point in the next octant using IncrementX and
*     IncrementY functions. if occupancymap shows that the current position is equal
*     new position or diagonals dimentions are equal current dimentions it return
*     false.
*     </pre>
*
* @param[in] occupancymap a reference to a vector of shared_ptrS to PlayerAsset
* @param[in] diagonals a reference to a vector of boolean vectore
*
* @return boolean function if Player assets move or not
*/
bool CPlayerAsset::MoveStep(std::vector< std::vector< std::shared_ptr< CPlayerAsset > > > &occupancymap, std::vector< std::vector< bool > > &diagonals){
    EDirection CurrentOctant = DPosition.TileOctant();
    const int DeltaX[] = {0, 5, 7, 5, 0, -5, -7, -5};
    const int DeltaY[] = {-7, -5, 0, 5, 7, 5, 0, -5};
    CTilePosition CurrentTile, NewTilePosition;
    CPixelPosition CurrentPosition(DPosition);

    CurrentTile.SetFromPixel(DPosition);
    if((EDirection::Max == CurrentOctant)||(CurrentOctant == DDirection)){// Aligned just move
        int NewX = Speed() * DeltaX[to_underlying(DDirection)] * CPosition::TileWidth() + DMoveRemainderX;
        int NewY = Speed() * DeltaY[to_underlying(DDirection)] * CPosition::TileHeight() + DMoveRemainderY;
        DMoveRemainderX = NewX % DUpdateDivisor;
        DMoveRemainderY = NewY % DUpdateDivisor;
        DPosition.IncrementX(NewX / DUpdateDivisor);
        DPosition.IncrementY(NewY / DUpdateDivisor);
    }
    else{ // Entering
        int NewX = Speed() * DeltaX[to_underlying(DDirection)] * CPosition::TileWidth() + DMoveRemainderX;
        int NewY = Speed() * DeltaY[to_underlying(DDirection)] * CPosition::TileHeight() + DMoveRemainderY;
        int TempMoveRemainderX = NewX % DUpdateDivisor;
        int TempMoveRemainderY = NewY % DUpdateDivisor;
        CPixelPosition NewPosition(DPosition.X() + NewX / DUpdateDivisor, DPosition.Y() + NewY / DUpdateDivisor);

        if(NewPosition.TileOctant() == DDirection){
            // Center in tile
            NewTilePosition.SetFromPixel(NewPosition);
            NewPosition.SetFromTile(NewTilePosition);
            TempMoveRemainderX = TempMoveRemainderY = 0;
        }
        DPosition = NewPosition;
        DMoveRemainderX = TempMoveRemainderX;
        DMoveRemainderY = TempMoveRemainderY;
    }
    NewTilePosition.SetFromPixel(DPosition);

    if(CurrentTile != NewTilePosition){
        bool Diagonal = (CurrentTile.X() != NewTilePosition.X()) && (CurrentTile.Y() != NewTilePosition.Y());
        int DiagonalX = std::min(CurrentTile.X(), NewTilePosition.X());
        int DiagonalY = std::min(CurrentTile.Y(), NewTilePosition.Y());

        if(occupancymap[NewTilePosition.Y()][NewTilePosition.X()] || (Diagonal && diagonals[DiagonalY][DiagonalX])){
            bool ReturnValue = false;
            // if(EAssetAction::Walk == occupancymap[NewTilePosition.Y()][NewTilePosition.X()]->Action()){
            //     ReturnValue = occupancymap[NewTilePosition.Y()][NewTilePosition.X()]->Direction() == CurrentPosition.TileOctant();
            // }
            NewTilePosition = CurrentTile;
            DPosition = CurrentPosition;
            return ReturnValue;
        }
        if(Diagonal){
            diagonals[DiagonalY][DiagonalX] = true;
        }
        occupancymap[NewTilePosition.Y()][NewTilePosition.X()] = occupancymap[CurrentTile.Y()][CurrentTile.X()];
        occupancymap[CurrentTile.Y()][CurrentTile.X()] = nullptr;
    }

    IncrementStep();
    return true;
}
