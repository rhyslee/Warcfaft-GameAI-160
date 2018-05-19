function debugprint(s)
    if true then --set false to disable text
        print(s)
    end
end

--- Determines the behaviours that the AI will perform
-- ANY function that should modify command must return true or false upon completion
-- And under NO circumstances should multiple function calls to setCommand should occur in one instance of the Lua State
-- Otherwise, bugs will occur. This is because commands are interpreted by the C side once.
-- So if you call SearchMap and ActivateFighters consecutively, only one of the two functions will occur based on order.
function CalculateCommand()
    ACTIONS_ASSIGNED = 0;
    if GetFoundAssetCount(AIPointer, "GoldMine") == 0 then
        commandGot = SearchMap()
        if commandGot then PushBackCommand(AIPointer, CmdPointer) end
        return
    elseif GetPlayerAssetCount(AIPointer, "TownHall") == 0 and GetPlayerAssetCount(AIPointer, "Keep") == 0 and GetPlayerAssetCount(AIPointer, "Castle") == 0 then
        commandGot = BuildTownHall()
        if commandGot then PushBackCommand(AIPointer, CmdPointer) end
        return
    else
        if GetPlayerAssetCount(AIPointer, "Peasant") < 4 then
            commandGot = ActivatePeasants(true)
            if commandGot then PushBackCommand(AIPointer, CmdPointer) end
            return
        end
        if GetPlayerAssetCount(AIPointer, "Barracks") < 3 and ACTIONS_ASSIGNED < 2 then
            commandGot = BuildBuildings("Barracks")
            if commandGot then
                PushBackCommand(AIPointer, CmdPointer)
                ACTIONS_ASSIGNED = ACTIONS_ASSIGNED + 1
            end
        end
        if GetPlayerAssetCount(AIPointer, "Blacksmith") == 0 and ACTIONS_ASSIGNED < 2 then
            commandGot = BuildBuildings("Blacksmith")
            if commandGot then
                PushBackCommand(AIPointer, CmdPointer)
                ACTIONS_ASSIGNED = ACTIONS_ASSIGNED + 1
            end
        end
        if GetPlayerAssetCount(AIPointer, "LumberMill") == 0 and ACTIONS_ASSIGNED < 2 then
            commandGot = BuildBuildings("LumberMill")
            if commandGot then
                PushBackCommand(AIPointer, CmdPointer)
                ACTIONS_ASSIGNED = ACTIONS_ASSIGNED + 1
            end
        end

        if GetPlayerAssetCount(AIPointer, "Farm") < 20 and ACTIONS_ASSIGNED < 2 then
            commandGot = BuildBuildings("Farm")
            if commandGot then
                PushBackCommand(AIPointer, CmdPointer)
                ACTIONS_ASSIGNED = ACTIONS_ASSIGNED + 1
            end
        end

        if GetPlayerAssetCount(AIPointer, "ScoutTower") < 4 and ACTIONS_ASSIGNED < 2 then
            commandGot = BuildBuildings("ScoutTower")
            if commandGot then
                PushBackCommand(AIPointer, CmdPointer)
                ACTIONS_ASSIGNED = ACTIONS_ASSIGNED + 1
            end
        end
        if GetPlayerAssetCount(AIPointer, "ScoutTower") == 4 and ACTIONS_ASSIGNED < 2 then
            commandGot = BuildBuildings("OffensiveTower")
            if commandGot then
                PushBackCommand(AIPointer, CmdPointer)
                ACTIONS_ASSIGNED = ACTIONS_ASSIGNED + 1
            end
        end
        if GetPlayerAssetCount(AIPointer, "Barracks") ==3 and ACTIONS_ASSIGNED < 2 then
            commandGot = BuildBuildings("OffensiveBarrack")
            if commandGot then
                PushBackCommand(AIPointer, CmdPointer)
                ACTIONS_ASSIGNED = ACTIONS_ASSIGNED + 1
            end
        end

    end
        --[[
    else
        debugprint("midway")
        completedAction = false; 
        --completedAction = BuildUpgrade("ArmorUpgrade2");
        peasantCount = GetPlayerAssetCount(AIPointer, "Peasant")
        BarracksCount = GetPlayerAssetCount(AIPointer, "Barracks")
        FootmanCount = GetPlayerAssetCount(AIPointer, "Footman")
        ArcherCount = GetPlayerAssetCount(AIPointer, "Archer") + GetPlayerAssetCount(AIPointer,"Ranger")
        consume = GetPlayerFoodConsumption(AIPointer)
        produce = GetPlayerFoodProduction(AIPointer)
        debugprint("getters")
        if not completedAction and consume >= produce then
            debugprint("bbing")
            completedAction = BuildBuildings("Farm", "TownHall")            
        end
        if not completedAction then
            debugprint("ap2")
            if peasantCount > FootmanCount + ArcherCount then
                completedAction = ActivatePeasants(false)
            else if peasantCount < 5 then
                completedAction = ActivatePeasants(true)
            end
        end
        
        if not completedAction and 0 == BarracksCount then
            debugprint("bbarracks")
            completedAction = BuildBuildings("Barracks", "Farm")
        end
        if not completedAction and peasantCount * .83 > FootmanCount then
            debugprint("tfm")
            completedAction = TrainFootmen()          
        end
        if not completedAction and 0 == GetPlayerAssetCount(AIPointer, "LumberMill") then
            debugprint("blumberm")
            completedAction = BuildBuildings("LumberMill", "Barracks")  
        end
        
        if not completedAction and FootmanCount/5 > ArcherCount then
            debugprint("tarcher")
            completedAction = TrainArcher()            
        end

        if not completedAction and 2 > BarracksCount then
            debugprint("bbarrack2")
            completedAction = BuildBuildings("Barracks", "Farm")
        end

        if not completedAction and GetPlayerAssetCount(AIPointer, "Footman") then
            debugprint("searchforenemy")
            townHallAsset = GetSingleAssetWithCapability(AIPointer, "BuildPeasant")
            if(townHallAsset ~= -1) then
                completedAction = SearchMapForEnemies(AIPointer, townHallAsset)
            end
            if(completedAction) then
                completedAction = SearchMap()
            end
        end
        if( not completedAction) then
            debugprint("af")
            completedAction = ActivateFighters()
        end
        if not completedAction and (FootmanCount > 4) and (0 < ArcherCount) then
            debugprint("AE")
            completedAction = AttackEnemies()
        end
    end--]]
end


---------------------------- Behaviours --------------------------------------------

--- Selects first idle asset with movement capabilities and gives it a command
-- to move towards the nearest unseen tile
-- @return true if any unit is able to successfully complete this command
function SearchMap ()
    --print("SearchMap")
    debugprint("SearchMap")

    movableAsset = GetSingleAssetWithCapability(AIPointer, "Move")
    if movableAsset ~= -1 then
        x, y = GetNearestTileOfType(AIPointer, movableAsset, "None")
        --print("  Moving to " .. x .. ", " .. y)
        if(0 <= x) then
            GiveActorAction(movableAsset, "Move", x, y)
            return true
        end
    end
    return false
end

--- Selects first idle asset with movement capabilities and gives it a command
-- to move towards the nearest unseen tile
-- @return true if any unit is able to successfully complete this command
function SearchForest ()
    --print("SearchMap")
    debugprint("SearchForest")
    TownHallNumber = 1 --  search around the ith Townhall
    THID, THX,THY = getTownhallPosition(TownHallNumber);
    movableAsset = GetSingleAssetWithCapability(AIPointer, "Move")
    if movableAsset ~= -1 then
        x, y = GetNearestTileOfType(AIPointer, THID,  "None")
        --print("  Moving to " .. x .. ", " .. y)
        if(0 <= x) then
            GiveActorAction(movableAsset, "Move", x, y)
            return true
        end
    end
    return false
end


--- Selects single idle asset with movement capabilities and gives it a command
-- to move towards the nearest unseen tile, commands all fighters to follow
-- @return true if any unit is able to successfully complete this command
function SiegeMap ()
    --print("SearchMap")
    debugprint("SiegeMap")
    movableAsset = GetSingleAssetWithCapability(AIPointer, "Move")
    if movableAsset ~= -1 then
        x, y = GetNearestTileOfType(AIPointer, movableAsset, "None")
        if(0 > x) then
            return false
        end
        --print("  Moving to " .. x .. ", " .. y)
        size = GetAllAssetsWithCapability(AIPointer, CmdPointer, "StandGround", "Patrol")   
        if(size > 1) then
            GiveActorsAction("Move", x, y)
        end
        return true
    end
    return false
end


function encodeConstraints(tuple)
    result = 0;
    if(tuple[1]==1) then
        result=result+1
    end
    if(tuple[2]==1) then
        result=result+2
    end
    if(tuple[3]==1) then
        result=result+4
    end
    return result
end
--- Find the best placement of a given building with Constraint Parameters
-- to search in a specific area whehter there is proper position for placement
-- @param builderAsset Id of the builder to build the building
-- @param HConstrants Horizontal search Constrants, {target'sleft,target'smiddle,target's right} eg. {0,1,0}
-- @param VConstrants vertical search Constrants, {target's above,target'smiddle,target's below}
-- @param targetX target's x
-- @param targetY target's y
-- @param offset Minimal offset between this building and the target
-- @param padding extra padding added to the size of the building to be built
-- @param assetType Type of the asset to be build
-- @param farthest the fartherst distance to search within
-- @return buildable,x,y bool is true if found pos x y; otherwise return false, and invalid x, y

function findBestPlacementWithConstraints(AIPointer,builderAsset,HConstrants,VConstrants,targetX,targetY,offset,padding,assetType,farthest)
    posx=-1
    posy=-1
    H = encodeConstraints(HConstrants)
    V = encodeConstraints(VConstrants)

    posx,posy = FindAssetPlacementWithConstraints(AIPointer, builderAsset,H,V,targetX,targetY,offset,padding,assetType,farthest)

    return posx,posy
end


--- Helper function for findBestPlacementWithConstraints for townhall
-- @return buildable,x,y bool is true if found pos x y; otherwise return false, and invalid x, y

function findBestPlacementForTownHall(AIPointer, goldMineX, goldMineY,builder)
    --as close to gold mine as possible, no farther than 10 away, buildingtype townhall
    posx,posy = findBestPlacementWithConstraints(AIPointer,builder,{1,1,0},{1,1,1},goldMineX,goldMineY,0,0,7,10)

    return posx,posy
end

--- Helper function for findBestPlacementWithConstraints for Lumbermill
-- @return buildable,x,y bool is true if found pos x y; otherwise return false, and invalid x, y

function findBestPlacementForLumberMill(AIPointer, woodX, woodY,builder)
    --as close to forest as possible, no farther than 10 away, buildingtype lumbermill
    posx,posy = findBestPlacementWithConstraints(AIPointer,builder,{1,1,1},{1,1,1},woodX,woodY,0,0,12,10)

    return posx,posy
end



--- Helper function to handle building of offensive barrack
-- search for enemy, and if spoted build a barrack safe distance away from enermy
-- @return true if any unit is able to scucessfully complete this command, or if searchmap returns true
function OffensiveBarrack()
    debugprint("Offensive Barrack")
    SafeDistanceFromEnermy = 15;
    --change this setting to make the this offensive building closer or further from enermy

    BuilderAsset = GetSingleAssetWithCapability(AIPointer, "BuildBarracks")
    if BuilderAsset ~= -1 then
        buildAction = "BuildBarracks"

        TownhallNumber =1
        THID, TH_x,TH_y = getTownhallPosition(TownhallNumber)

        targetAsset = GetNearestEnemy(AIPointer, TH_x,TH_y,-1);
        if targetAsset == -1 then
            return SearchMap()
        else

            E_X, E_Y = GetNearestEnemyLocation(AIPointer,TH_x,TH_y,-1);

            PlacementX,PlacementY = findBestPlacementWithConstraints(AIPointer,BuilderAsset,{1,1,1},{1,1,1},E_X,E_Y,SafeDistanceFromEnermy,0,10,18)

            CanBuild = CanInitiate(AIPointer, BuilderAsset, buildAction)

            if(0 > PlacementX) then

                return SearchMap()
            end
            if(CanBuild) then
--                print("builfding bsrrscks")
                --debugprint("Giving Action")
                GiveActorAction(BuilderAsset, "BuildBarracks", PlacementX, PlacementY)
                return true
            end
--            print("cant build")
            return false
        end
    end
end

--- Helper function to handle building of offensive Tower
-- search for enemy, and if spoted build a Tower safe distance away from enermy
-- @return true if any unit is able to scucessfully complete this command, or if searchmap returns true
function OffensiveTower()
    debugprint("Offensive Tower ")

    SafeDistanceFromEnermy = 15;
    --change this setting to make the this offensive building closer or further from enermy

    BuilderAsset = GetSingleAssetWithCapability(AIPointer, "BuildScoutTower")
    if BuilderAsset ~= -1 then
        buildAction = "BuildScoutTower"
        TownhallNumber =1
        THID, TH_x,TH_y = getTownhallPosition(TownhallNumber)
        targetAsset = GetNearestEnemy(AIPointer, TH_x,TH_y,-1);

        if targetAsset == -1 then
            return SearchMap()
        else

            E_X, E_Y = GetNearestEnemyLocation(AIPointer,TH_x,TH_y,-1);
            PlacementX,PlacementY = findBestPlacementWithConstraints(AIPointer,BuilderAsset,{1,1,1},{1,1,1},E_X,E_Y,SafeDistanceFromEnermy,0,13,20)
            CanBuild = CanInitiate(AIPointer, BuilderAsset, buildAction)

            if(0 > PlacementX) then
                return SearchMap()
            end
            if(CanBuild) then
                GiveActorAction(BuilderAsset, "BuildScoutTower", PlacementX, PlacementY)
                return true
            end
            return false
        end
    end
end



--- Selects first idle asset with build Lumber Mill capabilities and gives it a command
-- to build a town hall close to the nearest wood
-- @return true if any unit is able to scucessfully complete this command, or if searchmap returns true
function BuildLumberMill ()
    debugprint("BuildLumberMill")
    --print("BuildTownHall")
    builderAsset = GetSingleAssetWithCapability(AIPointer, "BuildLumberMill")
    print(" Builder Asset ID: " .. builderAsset)
    debugprint("gotbuilder")
    if builderAsset ~= -1 then
        woodX, woodY = GetNearestTileOfType(AIPointer, builderAsset, "Forest")
        debugprint("got forest pos")

        if (woodX ~= -1)and(woodY~=-1) then
            buildX, buildY = findBestPlacementForLumberMill(AIPointer,woodX, woodY, builderAsset)
            if (buildX ~= -1)and(buildY~=-1) then
                assetX, assetY = GetAssetTilePosition(builderAsset)
                debugprint("got thing")


                print("ForestPos:",woodX,woodY,"lumberMill pos",buildX,buildY)
                debugprint("gotbuildpos")

                GiveActorAction(builderAsset, "BuildLumberMill", buildX, buildY)
                debugprint("hereyougo")
                return true
            end
        end
        print("Searchfor wood")
        return SearchForest()
    end
    return false
end




--- Selects first idle asset with build town hall capabilities and gives it a command
-- to build a town hall close to the nearest gold mine
-- @return true if any unit is able to scucessfully complete this command, or if searchmap returns true
function BuildTownHall ()
    debugprint("BuildTownHall")
    --print("BuildTownHall")
    builderAsset = GetSingleAssetWithCapability(AIPointer, "BuildTownHall")
    --print(" Builder Asset ID: " .. builderAsset)
	--debugprint("gotbuilder")
    if builderAsset ~= -1 then
        goldMine = GetNearestAssetToOfType(AIPointer, builderAsset, "GoldMine")
	    --debugprint("got goldmine")
        goldMineX, goldMineY = GetAssetTilePosition(goldMine)
	    --debugprint("got gm pos")        
	    --buildX, buildY = GetBestAssetPlacement(AIPointer, goldMineX, goldMineY, builderAsset, "TownHall", 1)

        buildX, buildY = findBestPlacementForTownHall(AIPointer,goldMineX, goldMineY, builderAsset)

        print("GoldMinePos:",goldMineX,goldMineY,"townHallpos",buildX,buildY)
        --debugprint("gotbuildpos")
        if (buildX ~= -1)and(buildY~=-1) then
            assetX, assetY = GetAssetTilePosition(builderAsset)
            --debugprint("got thing")         
            GiveActorAction(builderAsset, "BuildTownHall", buildX, buildY)
            --debugprint("hereyougo")
            return true
        else
            return SearchMap()
        end
    end
    return false
end

--- Basic peasant functionality. Will tell peasants to gather mine by default and the harvest lumber
-- if there is enough gold, or there are more the enough miners
-- @param trainMore whether or not the town hall should train more peasants
-- @return true if any unit is able to complete this command or if a townhall trains a peasants.
function ActivatePeasants (trainMore)
    debugprint("ActivatePeasants")
    goldMiners = CountAssetsWithAction(AIPointer, "MineGold")
    lumberHarvesters = CountAssetsWithAction(AIPointer, "HarvestLumber")
    miningAsset = GetSingleAssetWithCapability(AIPointer, "Mine")
    townHallAsset = GetSingleAssetWithCapability(AIPointer, "BuildPeasant")
    interruptableAsset = GetInterruptableAssetWithCapability(AIPointer, "MineGold", "Mine")
    if interruptableAsset == -1 then
        interruptableAsset = GetInterruptableAssetWithCapability(AIPointer, "HarvestLumber", "Mine")
    end

    switchToGold = false
    switchToLumber = false
    if goldMiners >= 2 and lumberHarvesters == 0 then
        switchToLumber = true
    elseif lumberHarvesters >= 2 and goldMiners == 0 then
        switchToGold = true
    end
    if miningAsset ~= -1 or (interruptableAsset ~= -1 and (switchToGold or switchToLumber)) then
        if miningAsset~= -1 and (GetAssetLumber(miningAsset) ~= 0 or GetAssetGold(miningAsset) ~= 0) then
            townHallX, townHallY = GetAssetTilePosition(townHallAsset)
            ConveyResources(miningAsset, townHallAsset, townHallX, townHallY)
        else
            if miningAsset == -1 then
                miningAsset = interruptableAsset
            end
            --debugprint(goldMiners .. " " ..  GetPlayerGold(AIPointer) .. " " .. GetPlayerLumber(AIPointer) .. " ")
            if goldMiners ~= 0 and (GetPlayerGold(AIPointer) > GetPlayerLumber(AIPointer) * 3 or switchToLumber) then
                --debugprint("is true")
                forestX, forestY = GetNearestTileOfType(AIPointer, miningAsset, "Forest")
                if forestX >= 0 then
                    --print("harvesting " .. miningAsset)
                    GiveActorAction(miningAsset, "Mine", forestX, forestY)
                else
                    return SearchMap()
                end
            else
                goldMine = GetNearestAssetToOfType(AIPointer, miningAsset, "GoldMine")
                goldMineX, goldMineY = GetAssetTilePosition(goldMine)
                --debugprint("mining " .. miningAsset)
                MineGold(miningAsset, goldMineX, goldMineY)
            end
            return true
        end
    elseif townHallAsset ~= -1 and trainMore then
	    --debugprint("buildingp")
        canApply = GetCanApplyCapability(AIPointer, townHallAsset, townHallAsset, "BuildPeasant")
        if canApply then
            --print("training")
            townHallX, townHallY = GetAssetTilePosition(townHallAsset)
            GiveActorAction(townHallAsset, "BuildPeasant", townHallX, townHallY)
            return true
        end
    end
    return false
end

--- Command unused asset which can build the desired upgrade to do so. If the upgrade is already being researched, returns false.
--  If the asset does not exit, returns false.
--  If the command can be successfully set, returns true.
--  
--  @param[in] upgrade - type string; name of the upgrade i.e. WeaponUpgrade2
--  @return true if command is successfully set, else false
function BuildUpgrade(upgrade)
    debugprint("BuildUpgrade")
    asset = GetInterruptableAssetWithCapability(AIPointer, "None", upgrade)
    if asset~=-1 and GetCanApplyCapability(AIPointer, asset, asset, upgrade) then
        X,Y = GetAssetTilePosition(asset)
        GiveActorAction(asset, upgrade, X,Y)
        return true
    end
    debugprint("cannot research")
    return false

end

function getTownhallPosition(TownhallNumber)
    --to be implemented, need to keep a list of multiple townhalls
    TH = TownhallNumber
    TownhallX=5
    TownhallY=49
    TH_table = GetLocationsOfAllAssetsOfType(AIPointer,"TownHall")
    num_item = (#TH_table)/3

    if(num_item>0)then
--        print("\n\nfound TH ",num_item)
--        for i = 1,num_item do print(i,TH_table[i],TH_table[i+1],TH_table[i+2]) end
    end
    TownhallID= TH_table[(TownhallNumber-1)*3+1+0]
    TownhallX = TH_table[(TownhallNumber-1)*3+1+1]
    TownhallY = TH_table[(TownhallNumber-1)*3+1+2]
--    print("\n\nfound _X ",TownhallX)
--    print("\n\nfound _Y ",TownhallY)
    return TownhallID,TownhallX,TownhallY
end


function getFarmPosition()
    --return the nearest wood tile position

    FarmX=10
    FarmY=50
    return FarmX,FarmY
end

--- Commands an unused builder unit to construct a building of type other than town hall. The
-- default structure is a farmhouse, though the type of structure passed in can be changed
--
-- @param[in] buildingType Enum value that dictates what kind of building is constructed
-- @param[in] nearType Enum value that dictates what type of asset this building must be constructed next to
-- @return true if any unit successfully complete this command, or if search map returns true
-- (calls search map returning true)
function BuildBuildings(buildingType)
    debugprint("BuildBuilding: " .. buildingType)
    buildingTypeToActionType = {
        ["TownHall"]="TownHall",
        ["Keep"]="BuildKeep",
        ["Castle"]="BuildCastle",  
        ["Farm"]="BuildFarm",
        ["Barracks"]="BuildBarracks",
        ["LumberMill"]="BuildLumberMill",   
        ["Blacksmith"]="BuildBlacksmith",   
        ["ScoutTower"]="BuildScoutTower",
        ["OffensiveTower"]="BuildScoutTower",
        ["OffensiveBarrack"]="BuildBarracks"
    }

--  Special Building Handlers
    if(buildingType=="TownHall") then
        return BuildTownHall()
    end
    if(buildingType=="LumberMill") then
        return BuildLumberMill();
    end
    if(buildingType=="OffensiveTower") then
        return OffensiveTower()
    end
    if(buildingType=="OffensiveBarrack") then
        return OffensiveBarrack();
    end


-- Gneral Building Handlers

    buildAction = buildingTypeToActionType[buildingType]
    BuilderAsset = GetSingleAssetWithCapability(AIPointer, buildAction )
    --debugprint("BuildBuilding " .. buildingType .. nearType .. buildAction)
    --BuilderAsset, PlacementX, PlacementY = BuildBuilding(AIPointer,buildAction, buildingType, nearType)
    --debugprint("Build Asset Set: " .. BuilderAsset)

    TH_ID, TH_X,TH_Y = getTownhallPosition(1)

    if(buildingType=="Farm") then
        PlacementX,PlacementY = findBestPlacementWithConstraints(AIPointer,builder,{1,0,1},{1,0,1},TH_X,TH_Y,7,0,9,8)
        if(PlacementX==-1) then
            PlacementX,PlacementY = findBestPlacementWithConstraints(AIPointer,builder,{1,0,1},{1,0,1},TH_X,TH_Y,4,0,9,8)
        end
    end

    if(buildingType=="Barracks") then
        num = GetPlayerAssetCount(AIPointer,"Barracks")

        num_of_Defensive_Barracks = 2
        --change this setting to allow more or less barracks be built next to townhall
        if num<num_of_Defensive_Barracks then
            PlacementX,PlacementY = findBestPlacementWithConstraints(AIPointer,builder,{1,1,1},{1,1,1},TH_X,TH_Y,5,0,10,20)
        else
            return OffensiveBarrack();
        end
    end
    if(buildingType=="Blacksmith") then
        PlacementX,PlacementY = findBestPlacementWithConstraints(AIPointer,builder,{1,1,1},{1,1,1},TH_X,TH_Y,0,0,12,20)
    end

    if(buildingType=="ScoutTower") then
        PlacementX,PlacementY = findBestPlacementWithConstraints(AIPointer,builder,{1,0,1},{1,0,1},TH_X,TH_Y,7,0,13,20)
    end



    CanBuild = CanInitiate(AIPointer, BuilderAsset, buildAction)
    --debugprint("Build Action can be applied = ")
    --debugprint(CanBuild)
    
    if(0 > PlacementX) then
        return SearchMap()
    end
    if(CanBuild) then
        --debugprint("Giving Action")
        GiveActorAction(BuilderAsset, buildAction, PlacementX, PlacementY)
        return true
    end
    return false
end
--- Commands assets with the ability to build footmen to do so, i.e. commands the barracks to
--  begin constructing footmen
--  @return True if any of the AI's units can successfully complete this command
function TrainFootmen()
    debugprint("TrainFootmen")
    trainerAsset = GetSingleAssetWithCapability(AIPointer, "BuildFootman")
    
    if( trainerAsset ~= -1) then
        canApply = GetCanApplyCapability(AIPointer, trainerAsset, trainerAsset, "BuildFootman")
        
        if canApply then
            trainerX, trainerY = GetAssetTilePosition(trainerAsset)
            GiveActorAction(trainerAsset, "BuildFootman", trainerX, trainerY)
            return true
        end
    end
    return false
end
    
--- Commands assets with the ability to build archers to do so, i.e. commands the barracks to
--  begin constructing archers. If the AI player has decided to upgrade archers into rangers,
--  then the barracks will begin constructing rangers instead
--  @return True if any of the AI's units can successfully complete this command

function TrainArcher()
    debugprint("TrainArcher")
    trainerAsset, which = GetSingleAssetWithOneOfTwoCapability(AIPointer, "BuildArcher", "BuildRanger")
    if( trainerAsset ~= -1) then
        
        canApply = GetCanApplyCapability(AIPointer, trainerAsset, trainerAsset, which)
        
        if canApply then
            trainerX, trainerY = GetAssetTilePosition(trainerAsset)
            GiveActorAction(trainerAsset, which, trainerX, trainerY)
            return true
        end
    end
    return false
end
--- Commands assets with the ability to standground to do so, if they are not already standing ground.
-- @return true if any AI units can successfully standground
function ActivateFighters()
    debugprint("ActivateFighters")
   size = GetAllIdleAssetsWithCapability(AIPointer, CmdPointer, "StandGround", "StandGround")   
   if(size > 0) then
        SetCommandAction(AIPointer,CmdPointer, "StandGround")
        return true
   end 
   return false
end
--- Commands assets that are fighters to attack the nearest enemy.
--  The function calculates the average position of all units, and uses that point to determine the nearest enemy.
--  @return true if a command is set, otherwise false.
function AttackEnemies()
    debugprint("AttackEnemies")
    averageX = 0
    averageY = 0

    locationTable = {}
    locationTable = ConcatTables(locationTable, GetLocationsOfAllAssetsOfType(AIPointer, 2))
    locationTable = ConcatTables(locationTable, GetLocationsOfAllAssetsOfType(AIPointer, 3))
    locationTable = ConcatTables(locationTable, GetLocationsOfAllAssetsOfType(AIPointer, 4))

    if #locationTable > 1 then
        for i = 1, #locationTable, 3 do
            averageX = averageX + locationTable[i + 1]
            averageY = averageY + locationTable[i + 2]
        end
        averageX = averageX / (#locationTable / 3)
        averageY = averageY / (#locationTable / 3)

        targetAsset = GetNearestEnemy(AIPointer, averageX, averageY, -1)
        if targetAsset == -1 then
            return SearchMap()
        else
            targetX, targetY = GetAssetTilePosition(targetAsset)
            targetColor = GetAssetColor(targetAsset)

            for i = 1, #locationTable, 3 do
                AddCommandActor(AIPointer, CmdPointer, locationTable[i])
            end
            targetType = GetAssetType(AIPointer, targetAsset)
            SetCommandAction(AIPointer, CmdPointer, "Attack")   
            SetCommandTargetPos(AIPointer, CmdPointer, targetX, targetY)
            SetCommandTargetColor(AIPointer, CmdPointer, targetColor)
            SetCommandTargetType(AIPointer, CmdPointer, targetType)
            return true
        end
    end
    return false
end

---------------------------- Set Command Macros ------------------------------------


--- Sets command to an action on a single actor on neutral target
-- @param y y coordinate
-- @param x x coordinate
-- @param action capability to act on
-- @param actorID ID of asset given build action
function GiveActorAction (actorID, action, x, y)
    debugprint("Giving Actor Action")
    SetCommandAction(AIPointer, CmdPointer, action)
    debugprint("    SetAction ".. action)
    debugprint("    AddedActor " .. actorID)


    AddCommandActor(AIPointer, CmdPointer, actorID)



    debugprint("    AddedActor " .. actorID)
    SetCommandTargetPos(AIPointer, CmdPointer, x, y)
    debugprint("    SetTargetPos " .. x .. ", " .. y)
end

--- Sets command to an action on all pre-selected actor on neutral target
-- @param y y coordinate
-- @param x x coordinate
-- @param action capability to act on
function GiveActorsAction (action, x, y)
    SetCommandAction(AIPointer, CmdPointer, action)
    SetCommandTargetPos(AIPointer, CmdPointer, x, y)
end

--- Sets command to tell a given peasant to mine
-- @param mineY the y coordinate of the mine
-- @param mineX the x coordinate of the mine
-- @param actorID the AssetID of the peasant to mine
function MineGold (actorID, mineX, mineY)
    SetCommandAction(AIPointer, CmdPointer, "Mine")
    AddCommandActor(AIPointer, CmdPointer, actorID)
    SetCommandTargetType(AIPointer, CmdPointer, "GoldMine")
    SetCommandTargetPos(AIPointer, CmdPointer, mineX, mineY)
    
end

--- Sets command to tell a given peasant to convey resources to a town hall
-- @param townHallY the y coordinate of the town hall
-- @param townHallY the x coordinate of the town hall
-- @param townHallAssetID the AssetID of the town hall to return to
-- @param actorID the AssetID of the peasant to convey
function ConveyResources (actorID, townHallAssetID, townHallX, townHallY)
    SetCommandAction(AIPointer, CmdPointer, "Convey")
    AddCommandActor(AIPointer, CmdPointer, actorID)
    SetCommandTargetColor(AIPointer, CmdPointer, AIColor)
    SetCommandTargetType(AIPointer, CmdPointer, GetAssetType(townHallAssetID))
    SetCommandTargetPos(AIPointer, CmdPointer, townHallX, townHallY)
end

---------------------------- Helper Functions ------------------------------------

function ConcatTables (t1, t2)
    for i = 1, #t2 do
        t1[#t1 + 1] = t2[i]
    end
    return t1
end
