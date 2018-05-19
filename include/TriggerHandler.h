#ifndef TRIGGER_HANDLER_H
#define TRIGGER_HANDLER_H

#include <vector>
#include "DataSource.h"
#include "CommentSkipLineDataSource.h"
#include "GameDataTypes.h"

extern "C" {
    #include "lua.h"
    #include "lauxlib.h"
    #include "lualib.h"
}

enum class ETriggerType{
    Resource = 0,
    AssetCount,
    AssetLocation,
    Time,
    AssetsCreated,
    AssetsLost,
    AssetsDestroyed
};

enum class EResourceType{
    None = 0,
    Gold,
    Lumber,
    Stone,
    Max
};

class CTrigger : public std::enable_shared_from_this< CTrigger >{
    public:
        ETriggerType DType;
        bool DPlayerActivated;
        bool DAIActivated;
        bool DRepeatable;
        bool DActive;
        std::string DEvent;
        std::vector< std::string > DEventParameters;
        int offenderID;

        explicit CTrigger() : DType(ETriggerType::Resource), DPlayerActivated(false), DAIActivated(false), DRepeatable(false), DActive(true) {};
        virtual void Construct(ETriggerType triggerType, int triggerablePlayers, bool repeatable, std::vector< std::string > params);
        virtual bool Check(int size, int* args) = 0;
};

class CTriggerResource : public CTrigger {
    protected:
        EResourceType DResourceType;
        bool DGreaterThan;
        int DAmount;
    
    public:
        CTriggerResource() : CTrigger() {};
        void Construct(ETriggerType triggerType, int triggerablePlayers, bool repeatable, std::vector< std::string > params);
        bool Check(int size, int* args) override;
};

class CTriggerAssetCount : public CTrigger {
    protected:
        std::string DAssetTypeName;
        std::string DComparison;
        int DAmount;

    public:
        CTriggerAssetCount() : CTrigger() {};
        void Construct(ETriggerType triggerType, int triggerablePlayers, bool repeatable, std::vector< std::string > params);
        bool Check(int size, int* args) override;
};

class CTriggerAssetLocation : public CTrigger {
    protected:
        std::string DAssetTypeName;
        int DXMin;
        int DXMax;
        int DYMin;
        int DYMax;
    
    public:
        CTriggerAssetLocation() : CTrigger() {};
        void Construct(ETriggerType triggerType, int triggerablePlayers, bool repeatable, std::vector< std::string > params);
        bool Check(int size, int* args) override;
};

class CTriggerTime : public CTrigger {
    protected:
        bool DGreaterThan;
        float DInitialTime;
        float DTime;
        float DInterval;

    public:
        CTriggerTime() : CTrigger() {};
        void Construct(ETriggerType triggerType, int triggerablePlayers, bool repeatable, std::vector< std::string > params);
        bool Check(int size, int* args) override;
        void IncrementTime();
        void SetTime(float newTime) { DTime = newTime; }
        float GetInitialTime() { return DInitialTime; }
};

class CTriggerAssetsCreated : public CTrigger {
    protected:
        std::string DAssetTypeName;
        std::string DComparison;
        int DAmount;

    public:
        CTriggerAssetsCreated() : CTrigger() {};
        void Construct(ETriggerType triggerType, int triggerablePlayers, bool repeatable, std::vector< std::string > params);
        bool Check(int size, int* args) override;
};

class CTriggerAssetsLost : public CTrigger {
    protected:
        std::string DAssetTypeName;
        std::string DComparison;
        int DAmount;

    public:
        CTriggerAssetsLost() : CTrigger() {};
        void Construct(ETriggerType triggerType, int triggerablePlayers, bool repeatable, std::vector< std::string > params);
        bool Check(int size, int* args) override;
};

class CTriggerAssetsDestroyed : public CTrigger {
    protected:
        std::string DAssetTypeName;
        std::string DComparison;
        int DAmount;

    public:
        CTriggerAssetsDestroyed() : CTrigger() {};
        void Construct(ETriggerType triggerType, int triggerablePlayers, bool repeatable, std::vector< std::string > params);
        bool Check(int size, int* args) override;
};

class CTriggerHandler : public std::enable_shared_from_this< CTriggerHandler >{
    protected:
        static std::vector< std::shared_ptr< CTriggerHandler > > DAllTriggerHandlers;
        std::vector< std::string > DAIDifficultyScripts;
        std::string DEventScript;
        std::vector< std::shared_ptr< CTrigger > > DTriggers;
        std::vector< std::shared_ptr< CTriggerTime > > DTimeTriggers;
        int DHandlerIndex;

    public:
        static std::function< void(int, std::string, std::vector< std::string >, EPlayerColor color) > DEventCall;

        CTriggerHandler();
        CTriggerHandler(const CTriggerHandler &handler);
        ~CTriggerHandler();

        void Resolve(ETriggerType triggerType, bool isAI, EPlayerColor color, int offenderID, int size, int* args);
        static bool LoadTriggers(std::shared_ptr< CDataSource > source);
        static std::shared_ptr< CTriggerHandler > DuplicateHandler(int index);
        static bool AssetTypeMatchesName(std::string assetName, EAssetType assetType);
        std::string GetEventScript () { return DEventScript; }
        void ActivateTriggers ();
};

#endif
