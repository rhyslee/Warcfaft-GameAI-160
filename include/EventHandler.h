#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include "GameModel.h"
#include "TriggerHandler.h"

extern "C" {
    #include "lua.h"
    #include "lauxlib.h"
    #include "lualib.h"
}

class CEventHandler : public std::enable_shared_from_this< CEventHandler >{
    protected:
        static std::shared_ptr< CGameModel > DGameModel;
        static std::string DEventScript;

    public:
        static void SetGameModelReference (std::shared_ptr< CGameModel > ptr);
        static void RegisterAction ();
        static void SetEventScript (std::string scriptName);
        static void DoEvent (int offenderID, std::string event, std::vector< std::string > params, EPlayerColor color);

        static void RegisterFunctions(lua_State *L);
        static int EndGame(lua_State *L);
        static int ChangeResources(lua_State *L);
        static int ModifyAssetHealth(lua_State *L);
        static int RemoveAsset(lua_State *L);
        static int AddAsset(lua_State *L);
        static int AddUpgrade(lua_State *L);
        static int RemoveUpgrade(lua_State *L);
};

#endif
