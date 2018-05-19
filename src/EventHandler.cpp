#include "EventHandler.h"
#include "Debug.h"
#include "BattleMode.h"
#include "GameModel.h"

extern "C" {
    #include "lua.h"
    #include "lauxlib.h"
    #include "lualib.h"
}

std::shared_ptr< CGameModel > CEventHandler::DGameModel;
std::string CEventHandler::DEventScript;

void CEventHandler::SetGameModelReference (std::shared_ptr< CGameModel > ptr){
    DGameModel = ptr;
}

void CEventHandler::RegisterAction (){
    CTriggerHandler::DEventCall = &DoEvent;
}

void CEventHandler::SetEventScript (std::string scriptName){
    DEventScript = scriptName;
}

void CEventHandler::DoEvent (int offenderID, std::string event, std::vector< std::string > params, EPlayerColor color){
	printf("DoEvent\n");
	
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    RegisterFunctions(L);
    luaL_dofile(L, DEventScript.c_str());
    
    lua_pushnumber(L, (int)color);
    lua_setglobal(L, "PlayerColor");
    lua_pushnumber(L, offenderID);
    lua_setglobal(L, "OffenderID");

    lua_getglobal(L, event.c_str());
    int argCount = params.size();
    for (int j = 0; j < argCount; j++)
        lua_pushstring(L, params[j].c_str());

	printf("Offender \"%d\" Calling Function : \"%s\" with %d args\n", offenderID, event.c_str(), argCount);
    int response = 0;
    if (response = lua_pcall(L, argCount, 0, 0))
        printf("Could not execute function \"%s\" in \"%s\": error code %d\n", event.c_str(), DEventScript.c_str(), response);
    
    lua_close(L);
}

/**
 * Registers all C++ side Lua Getter and Setter functions
 *
 * @param[in] L The lua_State the register the functions into
 */
 void CEventHandler::RegisterFunctions (lua_State *L){
    lua_register(L, "EndGame_CPP", EndGame);
    lua_register(L, "ChangeResources_CPP", ChangeResources);
    lua_register(L, "ModifyAssetHealth_CPP", ModifyAssetHealth);
    lua_register(L, "RemoveAsset_CPP", RemoveAsset);
    lua_register(L, "AddAsset_CPP", AddAsset);
    lua_register(L, "AddUpgrade_CPP", AddUpgrade);
    lua_register(L, "RemoveUpgrade_CPP", RemoveUpgrade);
}



/**
 * Ends battle mode
 * 
 * ---Parameters and returns are documented as Lua Side
 *
 * @param[in] won Whether it is a player win or not
 */
int CEventHandler::EndGame (lua_State *L){
    bool won = lua_toboolean(L, -1);
    CBattleMode::TriggeredEnd(won);
    return 0;
}

int CEventHandler::ChangeResources (lua_State *L){
	printf("BLUE");
	EPlayerColor color = static_cast<EPlayerColor>(lua_tointeger(L,-3));
    int amount = lua_tointeger(L,-2);
    std::string type = std::string(lua_tostring(L, -1));
    if (type == "Gold"){
        DGameModel->Player(color)->IncrementGold(amount);
    }
    else if(type == "Lumber"){
        DGameModel->Player(color)->IncrementLumber(amount);
    }
    return 0;
}

int CEventHandler::ModifyAssetHealth (lua_State *L){
    int offenderID = lua_tointeger(L, -2);
    int delta = lua_tointeger(L, -1);
    std::shared_ptr< CPlayerAsset > asset = FindAssetObj(offenderID);
    if (delta > 0)
        asset->IncrementHitPoints(delta);
    else if (delta < 0)
        asset->DecrementHitPoints(-delta);
    return 0;
}

int CEventHandler::RemoveAsset(lua_State *L)
{
    int offenderID = lua_tointeger(L,-1);
    std::shared_ptr< CPlayerAsset > asset = FindAssetObj(offenderID);
    if(!asset) // if nullptr is returned
        return 0;
    SAssetCommand cmd;
    cmd.DAction = EAssetAction::Death;
    asset->ClearCommand();
    asset->PushCommand(cmd);
    return 0;
}

int CEventHandler::AddAsset(lua_State *L)
{
    char* assetName = (char*)lua_tostring(L,-5);
    EPlayerColor color = static_cast<EPlayerColor>(lua_tointeger(L,-4));
    int positionX = lua_tointeger(L,-3);
    int positionY = lua_tointeger(L,-2);
    int amount = lua_tointeger(L, -1);
    EAssetType AssetType = CPlayerAssetType::NameToType(assetName);

    auto asset = DGameModel->Player(color)->CreateAsset(assetName);
    CTilePosition position = CTilePosition(positionX,positionY);

    asset->TilePosition(position);
    if(EAssetType::GoldMine == asset->Type()){
        asset->Gold(amount);
    }
        
    // else if(EAssetType::StoneQuarry == asset->Type()) asset->Stone = amount //not yet implemented


}

/**
 *  Add upgrade by name
 */
int CEventHandler::AddUpgrade(lua_State *L)
{
    printf("add\n");
    EPlayerColor color = static_cast<EPlayerColor>(lua_tointeger(L,-2));
    DGameModel->Player(color)->AddUpgrade(lua_tostring(L,-1));
}

/**
 *  Removes upgrade by name
 */
int CEventHandler::RemoveUpgrade(lua_State *L)
{    

    printf("rmv\n");
    EPlayerColor color = static_cast<EPlayerColor>(lua_tointeger(L,-2));
    DGameModel->Player(color)->RemoveUpgrade(lua_tostring(L,-1));
}