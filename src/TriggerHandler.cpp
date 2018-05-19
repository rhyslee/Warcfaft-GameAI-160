#include "TriggerHandler.h"
#include "CommentSkipLineDataSource.h"
#include "Debug.h"
#include "Tokenizer.h"
#include "GameDataTypes.h"

extern "C" {
    #include "lua.h"
    #include "lauxlib.h"
    #include "lualib.h"
}

std::vector< std::shared_ptr< CTriggerHandler > > CTriggerHandler::DAllTriggerHandlers;
std::function< void(int, std::string, std::vector< std::string >, EPlayerColor color) > CTriggerHandler::DEventCall;

#pragma region CTrigger Construct

/**
 * In place of a constructor
 *
 * @param[in] triggerType The triggers type
 * @param[in] triggerablePlayers Whether or not the event can be triggered by human players, ai or both
 * @param[in] repeatable Whether the event can be repeated (be wary of loops with this capability)
 * @param[in] params The remaining tokens containing more trigger conditions and the event triggered
 */
void CTrigger::Construct (ETriggerType triggerType, int triggerablePlayers, bool repeatable, std::vector< std::string >params){
    DType = triggerType;
    DPlayerActivated = (triggerablePlayers == 0 || triggerablePlayers == 1);
    DAIActivated = (triggerablePlayers == 0 || triggerablePlayers == 2);
    DRepeatable = repeatable;
}

#pragma endregion

#pragma region CTriggerResource Construct and Check

/**
 * Resource Triggers constructor
 *
 * @param[in] triggerType The triggers type
 * @param[in] triggerablePlayers Whether or not the event can be triggered by human players, ai or both
 * @param[in] repeatable Whether the event can be repeated (be wary of loops with this capability)
 * @param[in] params The remaining tokens containing more trigger conditions and the event triggered
 */
void CTriggerResource::Construct (ETriggerType triggerType, int triggerablePlayers, bool repeatable, std::vector< std::string > params){
    CTrigger::Construct(triggerType, triggerablePlayers, repeatable, params);

    if (params.at(0) == "Gold"){
        DResourceType = EResourceType::Gold;
    }
    else if (params.at(0) == "Lumber"){
        DResourceType = EResourceType::Lumber;
    }
    else if (params.at(0) == "Stone"){
        DResourceType = EResourceType::Stone;
    }
    else {
        PrintDebug(DEBUG_HIGH, "Invalid TriggerResource resourceType: %s\n", params.at(0));
    }

    DGreaterThan = (params.at(1) == ">");
    DAmount = stoi(params.at(2));
    DEvent = params.at(3);

    params.erase(params.begin(), params.begin() + 4);
    DEventParameters = params;
}

/**
 * Checks a resource trigger to see if it has been tripped
 *
 * @param[in] size Number of arguments to check
 * @param[in] args Args passed. Should be {resourceType, amount}
 *
 * @return True if trigger tripped, false elsewise
 */
bool CTriggerResource::Check (int size, int* args){
    //printf("Resource Check\n");
    if (size != 2){
        printf("CTriggerResource::Check - %d is not the correct number of arguments. Should be 2.\n", size);
        return false;
    }
    int resourceTypeInt = args[0];
    int amount = args[1];

    EResourceType resourceType = static_cast<EResourceType>(resourceTypeInt);
    if (resourceType == DResourceType)
        if ((DGreaterThan && amount > DAmount) || (!DGreaterThan && amount < DAmount))
            return true;
    return false;
}

#pragma endregion

#pragma region CTriggerAssetCount Construct and Check

/**
 * AssetCount Triggers constructor
 *
 * @param[in] triggerType The triggers type
 * @param[in] triggerablePlayers Whether or not the event can be triggered by human players, ai or both
 * @param[in] repeatable Whether the event can be repeated (be wary of loops with this capability)
 * @param[in] params The remaining tokens containing more trigger conditions and the event triggered
 */
 void CTriggerAssetCount::Construct (ETriggerType triggerType, int triggerablePlayers, bool repeatable, std::vector< std::string > params){
    CTrigger::Construct(triggerType, triggerablePlayers, repeatable, params);

    DAssetTypeName = params.at(0);
    DComparison = params.at(1);
    DAmount = stoi(params.at(2));
    DEvent = params.at(3);

    params.erase(params.begin(), params.begin() + 4);
    DEventParameters = params;
}

/**
 * Checks an asset count trigger to see if it has been tripped
 *
 * @param[in] size Number of arguments to check
 * @param[in] args Args passed. Should be {assetType, amount}
 *
 * @return True if trigger tripped, false elsewise
 */
bool CTriggerAssetCount::Check (int size, int* args){
    //printf("AssetCount Check\n");

    int count = 0;
    for (int i = 0; i < size; i++)
        if (CTriggerHandler::AssetTypeMatchesName(DAssetTypeName, static_cast<EAssetType>(i)))
            count += args[i];
    //printf("    count = %d\n", count);
    if ((count > DAmount && DComparison == ">") || (count < DAmount && DComparison == "<") || (count == DAmount && DComparison == "="))
        return true;
    return false;
}

#pragma endregion

#pragma region CTriggerAssetLocation Construct and Check

/**
 * AssetLocation Triggers constructor
 *
 * @param[in] triggerType The triggers type
 * @param[in] triggerablePlayers Whether or not the event can be triggered by human players, ai or both
 * @param[in] repeatable Whether the event can be repeated (be wary of loops with this capability)
 * @param[in] params The remaining tokens containing more trigger conditions and the event triggered
 */
 void CTriggerAssetLocation::Construct (ETriggerType triggerType, int triggerablePlayers, bool repeatable, std::vector< std::string > params){
    CTrigger::Construct(triggerType, triggerablePlayers, repeatable, params);

    DAssetTypeName = params.at(0);
    DXMin = stoi(params.at(1));
    DXMax = stoi(params.at(2));
    DYMin = stoi(params.at(3));
    DYMax = stoi(params.at(4));
    DEvent = params.at(5);

    params.erase(params.begin(), params.begin() + 6);
    DEventParameters = params;
}

/**
 * Checks an asset location trigger to see if it has been tripped
 *
 * @param[in] size Number of arguments to check
 * @param[in] args Args passed. Should be {assetType, tileX, tileY}
 *
 * @return True if trigger tripped, false elsewise
 */
bool CTriggerAssetLocation::Check (int size, int* args){
    //printf("AssetCount Check\n");
    if (size != 3)
        return false;
    if (!CTriggerHandler::AssetTypeMatchesName(DAssetTypeName, static_cast<EAssetType>(args[0])))
        return false;
    if (args[1] < DXMin || args[1] > DXMax || args[2] < DYMin || args[2] > DYMax)
        return false;
    return true;
}

#pragma endregion

#pragma region CTriggerTime Construct and Check

/**
 * Time Triggers constructor
 *
 * @param[in] triggerType The triggers type
 * @param[in] triggerablePlayers Whether or not the event can be triggered by human players, ai or both
 * @param[in] repeatable Whether the event can be repeated (be wary of loops with this capability)
 * @param[in] params The remaining tokens containing more trigger conditions and the event triggered
 */
 void CTriggerTime::Construct (ETriggerType triggerType, int triggerablePlayers, bool repeatable, std::vector< std::string > params){
    CTrigger::Construct(triggerType, triggerablePlayers, repeatable, params);

    DGreaterThan = (params.at(0) == ">");
    DInitialTime = stof(params.at(1));
    DTime = DInitialTime;
    DInterval = stof(params.at(2));
    DEvent = params.at(3);

    params.erase(params.begin(), params.begin() + 4);
    DEventParameters = params;
}

/**
 * Checks a time trigger to see if it has been tripped
 *
 * @param[in] size Number of arguments to check
 * @param[in] args Args passed. Should be {time}
 *
 * @return True if trigger tripped, false elsewise
 */
bool CTriggerTime::Check (int size, int* args){
    //printf("AssetCount Check\n");

    if (size != 1)
        return false;
    float time = (float)args[0] / 1000.0;
    if ((DGreaterThan && time > DTime) || (!DGreaterThan && time < DTime))
        return true;
    return false;
}

void CTriggerTime::IncrementTime (){
    DTime += DInterval;
}

#pragma endregion

#pragma region CTriggerAssetsCreated Construct and Check

/**
 * AssetCreated Triggers constructor
 *
 * @param[in] triggerType The triggers type
 * @param[in] triggerablePlayers Whether or not the event can be triggered by human players, ai or both
 * @param[in] repeatable Whether the event can be repeated (be wary of loops with this capability)
 * @param[in] params The remaining tokens containing more trigger conditions and the event triggered
 */
 void CTriggerAssetsCreated::Construct (ETriggerType triggerType, int triggerablePlayers, bool repeatable, std::vector< std::string > params){
    CTrigger::Construct(triggerType, triggerablePlayers, repeatable, params);

    DAssetTypeName = params.at(0);
    DComparison = params.at(1);
    DAmount = stoi(params.at(2));
    DEvent = params.at(3);

    params.erase(params.begin(), params.begin() + 4);
    DEventParameters = params;
}

/**
 * Checks an assets created trigger to see if it has been tripped
 *
 * @param[in] size Number of arguments to check
 * @param[in] args Args passed. Should be {assetType, amount}
 *
 * @return True if trigger tripped, false elsewise
 */
bool CTriggerAssetsCreated::Check (int size, int* args){
    //printf("AssetCount Check\n");

    int count = 0;
    for (int i = 0; i < size; i++)
        if (CTriggerHandler::AssetTypeMatchesName(DAssetTypeName, static_cast<EAssetType>(i)))
            count += args[i];
    //printf("    count = %d\n", count);
    if ((count > DAmount && DComparison == ">") || (count < DAmount && DComparison == "<") || (count == DAmount && DComparison == "="))
        return true;
    return false;
}

#pragma endregion

#pragma region CTriggerAssetsLost Construct and Check

/**
 * AssetsLost Triggers constructor
 *
 * @param[in] triggerType The triggers type
 * @param[in] triggerablePlayers Whether or not the event can be triggered by human players, ai or both
 * @param[in] repeatable Whether the event can be repeated (be wary of loops with this capability)
 * @param[in] params The remaining tokens containing more trigger conditions and the event triggered
 */
 void CTriggerAssetsLost::Construct (ETriggerType triggerType, int triggerablePlayers, bool repeatable, std::vector< std::string > params){
    CTrigger::Construct(triggerType, triggerablePlayers, repeatable, params);

    DAssetTypeName = params.at(0);
    DComparison = params.at(1);
    DAmount = stoi(params.at(2));
    DEvent = params.at(3);

    params.erase(params.begin(), params.begin() + 4);
    DEventParameters = params;
}

/**
 * Checks an assets lost trigger to see if it has been tripped
 *
 * @param[in] size Number of arguments to check
 * @param[in] args Args passed. Should be {assetType, amount}
 *
 * @return True if trigger tripped, false elsewise
 */
bool CTriggerAssetsLost::Check (int size, int* args){
    //printf("AssetCount Check\n");

    int count = 0;
    for (int i = 0; i < size; i++)
        if (CTriggerHandler::AssetTypeMatchesName(DAssetTypeName, static_cast<EAssetType>(i)))
            count += args[i];
    //printf("    count = %d\n", count);
    if ((count > DAmount && DComparison == ">") || (count < DAmount && DComparison == "<") || (count == DAmount && DComparison == "="))
        return true;
    return false;
}

#pragma endregion

#pragma region CTriggerAssetsDestroyed Construct and Check

/**
 * AssetDestroyed Triggers constructor
 *
 * @param[in] triggerType The triggers type
 * @param[in] triggerablePlayers Whether or not the event can be triggered by human players, ai or both
 * @param[in] repeatable Whether the event can be repeated (be wary of loops with this capability)
 * @param[in] params The remaining tokens containing more trigger conditions and the event triggered
 */
 void CTriggerAssetsDestroyed::Construct (ETriggerType triggerType, int triggerablePlayers, bool repeatable, std::vector< std::string > params){
    CTrigger::Construct(triggerType, triggerablePlayers, repeatable, params);

    DAssetTypeName = params.at(0);
    DComparison = params.at(1);
    DAmount = stoi(params.at(2));
    DEvent = params.at(3);

    params.erase(params.begin(), params.begin() + 4);
    DEventParameters = params;
}

/**
 * Checks an assets destroyed trigger to see if it has been tripped
 *
 * @param[in] size Number of arguments to check
 * @param[in] args Args passed. Should be {assetType, amount}
 *
 * @return True if trigger tripped, false elsewise
 */
bool CTriggerAssetsDestroyed::Check (int size, int* args){
    //printf("AssetCount Check\n");

    int count = 0;
    for (int i = 0; i < size; i++)
        if (CTriggerHandler::AssetTypeMatchesName(DAssetTypeName, static_cast<EAssetType>(i)))
            count += args[i];
    //printf("    count = %d\n", count);
    if ((count > DAmount && DComparison == ">") || (count < DAmount && DComparison == "<") || (count == DAmount && DComparison == "=")){
        return true;
    }
    return false;
}

#pragma endregion

#pragma region CTriggerHandler Constructors/Deconstructor

CTriggerHandler::CTriggerHandler () {}

CTriggerHandler::CTriggerHandler(const CTriggerHandler &handler){
    DAIDifficultyScripts = handler.DAIDifficultyScripts;
    DEventScript = handler.DEventScript;
    DTriggers = handler.DTriggers;
    DTimeTriggers = handler.DTimeTriggers;
    DHandlerIndex = handler.DHandlerIndex;
}

CTriggerHandler::~CTriggerHandler () {}

#pragma endregion

#pragma region CTriggerHandler Resolve

/**
 * Iterates over trigger vectors and resolves any events of tripped triggers
 *
 * @param[in] triggerType The type of trigger to check
 * @param[in] isAI Whether the caller is an AI or not
 * @param[in] size Number of arguments given
 * @param[in] args Arguments passed
 */
void CTriggerHandler::Resolve (ETriggerType triggerType, bool isAI, EPlayerColor color, int offenderID, int size, int* args){
    if (triggerType == ETriggerType::Time){
        for (int i = 0; i < DTimeTriggers.size(); i++){
            if (DTimeTriggers[i]->Check(size, args) && DTimeTriggers[i]->DActive){
                DEventCall(offenderID, DTimeTriggers[i]->DEvent, DTimeTriggers[i]->DEventParameters, color);

                if (!DTimeTriggers[i]->DRepeatable){
                    DTimeTriggers[i]->DActive = false;
                }
                else{
                    DTimeTriggers[i]->IncrementTime();
                }
            }
        }
        return;
    }

    for (int i = 0; i < DTriggers.size(); i++){
        //printf("Triggers[%d] type: %d\n", i, (int)DTriggers[i]->DType);
        if (DTriggers[i]->DType == triggerType && DTriggers[i]->DActive){
            //printf("At AI Check %d\n", isAI);
            if ((isAI && DTriggers[i]->DAIActivated) || (!isAI && DTriggers[i]->DPlayerActivated)){
                //printf("Triggers[%d] Type = %d, compared to %d\n", i, (int)DTriggers[i]->DType, (int)triggerType);
                if (DTriggers[i]->Check(size, args)){
                    
                    /*
                    printf("Trigger[%d] matched - type = %d - isAI = %d - size = %d\n", i, (int)triggerType, (int)isAI, size);
                    for (int j = 0; j < size; j++)
                        printf("%d ", args[j]);
                    printf("\n");
                    printf("    %s\n    ", DEventScript.c_str());
                    for (int j = 0; j < DTriggers[i]->DEventParameters.size(); j++)
                        printf("(%s)", DTriggers[i]->DEventParameters[j].c_str());
                    printf("\n");
                    */

                    DEventCall(offenderID, DTriggers[i]->DEvent, DTriggers[i]->DEventParameters, color);
                    if (!DTriggers[i]->DRepeatable){
                        DTriggers[i]->DActive = false;
                    }
                }
            }
        }
    }
}

#pragma endregion

#pragma region CTriggerHandler Matching String with AssetTypeMatchesName

bool CTriggerHandler::AssetTypeMatchesName(std::string assetName, EAssetType assetType){
    switch (assetType){
        case EAssetType::Peasant:
            return assetName == "All" || assetName == "Peasant" || assetName == "Unit";
        case EAssetType::Footman:
            return assetName == "All" || assetName == "Footman" || assetName == "Unit" || assetName == "Fighter";
        case EAssetType::Archer:
            return assetName == "All" || assetName == "Archer" || assetName == "Unit" || assetName == "Fighter";
        case EAssetType::Ranger:
            return assetName == "All" || assetName == "Ranger" || assetName == "Unit" || assetName == "Fighter";
        case EAssetType::GoldMine:
            return assetName == "All" || assetName == "GoldMine" || assetName == "Building";
        case EAssetType::TownHall:
            return assetName == "All" || assetName == "TownHall" || assetName == "Building" || assetName == "TownCenter";
        case EAssetType::Keep:
            return assetName == "All" || assetName == "Keep" || assetName == "Building" || assetName == "TownCenter";
        case EAssetType::Castle:
            return assetName == "All" || assetName == "Castle" || assetName == "Building" || assetName == "TownCenter";
        case EAssetType::Farm:
            return assetName == "All" || assetName == "Farm" || assetName == "Building";
        case EAssetType::Barracks:
            return assetName == "All" || assetName == "Barracks" || assetName == "Building";
        case EAssetType::LumberMill:
            return assetName == "All" || assetName == "LumberMill" || assetName == "Building";
        case EAssetType::Blacksmith:
            return assetName == "All" || assetName == "Blacksmith" || assetName == "Building";
        case EAssetType::ScoutTower:
            return assetName == "All" || assetName == "ScoutTower" || assetName == "Building" || assetName == "Tower";
        case EAssetType::GuardTower:
            return assetName == "All" || assetName == "GuardTower" || assetName == "Building" || assetName == "Tower";
        case EAssetType::CannonTower:
            return assetName == "All" || assetName == "CannonTower" || assetName == "Building" || assetName == "Tower";
        default:
            PrintDebug(DEBUG_HIGH, "Invalid asset type for matching\n");
            return false;
    }
}

#pragma endregion

#pragma region CTriggerHandler Load and Duplicate

/**
 * Loads in all the triggers for a .map file
 *
 * @param[in] source Map info is loaded from .map file and stored in source
 *
 * @return true or false if map successfully loaded or not
 */
bool CTriggerHandler::LoadTriggers (std::shared_ptr< CDataSource > source){
    CCommentSkipLineDataSource LineSource(source, '#');
    std::string tempString;
    std::vector< std::string > tokens;
    int triggerCount;
    std::shared_ptr< CTriggerHandler > tempHandler = std::make_shared< CTriggerHandler >();
    std::shared_ptr< CTrigger > tempTrigger;

    // read in AI difficulties
    for(int i = 0; i < 3; i++) {
        if(!LineSource.Read(tempString)){
            return false;
        } 
        tempHandler->DAIDifficultyScripts.push_back(tempString);
    }

    // read in Trigger script
    if(!LineSource.Read(tempString)){
        return false;
    } 
    tempHandler->DEventScript = tempString;

    // read in number of triggers    
    if(!LineSource.Read(tempString)) {
        return false;
    }
    triggerCount = stoi(tempString);

    // read in triggers
    for(int i = 0; i < triggerCount; i++){
        if(!LineSource.Read(tempString)){
            return false;
        }
        CTokenizer::Tokenize(tokens, tempString);
        ETriggerType triggerType = static_cast< ETriggerType > (stoi(tokens.at(0)));
        int triggerablePlayers = stoi(tokens.at(1));
        int repeatable = stoi(tokens.at(2));
        tokens.erase(tokens.begin(), tokens.begin() + 3);

        //printf("Loading trigger type %d\n", (int)triggerType);

        switch (triggerType){
            case ETriggerType::Resource:
                {
                    tempTrigger = std::make_shared< CTriggerResource >();
                    tempTrigger->Construct(triggerType, triggerablePlayers, repeatable, tokens);
                    tempHandler->DTriggers.push_back(tempTrigger);
                }
                break;
            case ETriggerType::AssetCount:
                {
                    tempTrigger = std::make_shared< CTriggerAssetCount >();
                    tempTrigger->Construct(triggerType, triggerablePlayers, repeatable, tokens);
                    tempHandler->DTriggers.push_back(tempTrigger);
                }
                break;
            case ETriggerType::AssetLocation:
                {
                    tempTrigger = std::make_shared< CTriggerAssetLocation >();
                    tempTrigger->Construct(triggerType, triggerablePlayers, repeatable, tokens);
                    tempHandler->DTriggers.push_back(tempTrigger);
                }
                break;
            case ETriggerType::Time:
                {
                    std::shared_ptr< CTriggerTime > tempTimeTrigger = std::make_shared< CTriggerTime >();
                    tempTimeTrigger->Construct(triggerType, triggerablePlayers, repeatable, tokens);
                    tempHandler->DTimeTriggers.push_back(tempTimeTrigger);
                }
                break;
            case ETriggerType::AssetsCreated:
                {
                    tempTrigger = std::make_shared< CTriggerAssetsCreated >();
                    tempTrigger->Construct(triggerType, triggerablePlayers, repeatable, tokens);
                    tempHandler->DTriggers.push_back(tempTrigger);
                }
                break;
            case ETriggerType::AssetsLost:
                {
                    tempTrigger = std::make_shared< CTriggerAssetsLost >();
                    tempTrigger->Construct(triggerType, triggerablePlayers, repeatable, tokens);
                    tempHandler->DTriggers.push_back(tempTrigger);
                }
                break;
            case ETriggerType::AssetsDestroyed:
                {
                    tempTrigger = std::make_shared< CTriggerAssetsDestroyed >();
                    tempTrigger->Construct(triggerType, triggerablePlayers, repeatable, tokens);
                    tempHandler->DTriggers.push_back(tempTrigger);
                }
                break;
            default:
                PrintDebug(DEBUG_HIGH, "Trigger type invalid\n");
                break;
        }
    }
    //printf("\n");

    tempHandler->DHandlerIndex = DAllTriggerHandlers.size();
    DAllTriggerHandlers.push_back(tempHandler);
    return true;
}

/**
 * Duplicates a handler for a given map index
 *
 * @param[in] index The index of the map
 *
 * @return A shared pointer to a copy of the maps trigger handler
 */
 std::shared_ptr< CTriggerHandler > CTriggerHandler::DuplicateHandler (int index){
    if (index < 0 || index >= DAllTriggerHandlers.size())
        return std::shared_ptr< CTriggerHandler >();
    return std::make_shared< CTriggerHandler >(*DAllTriggerHandlers[index]);
 }

 #pragma endregion

#pragma region CTriggerHandler Activation

void CTriggerHandler::ActivateTriggers (){
    for (int i = 0; i < DTriggers.size(); i++)
        DTriggers[i]->DActive = true;
    for (int i = 0; i < DTimeTriggers.size(); i++){
        DTimeTriggers[i]->DActive = true;
        DTimeTriggers[i]->SetTime(DTimeTriggers[i]->GetInitialTime());
    }
}

#pragma endregion