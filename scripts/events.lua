--- Ends the game with a win or lose state
function EndGame (won)
    EndGame_CPP(won == "0") --FIXME: NICK WHY DO YOU HAVE SO MANY DAMN END GAME TRIGGERS.
end

function ChangeResources (amount, typeR)
    a = tonumber(amount)
    ChangeResources_CPP(PlayerColor, a, typeR)
end

function ModifyAssetHealth (delta)
    if OffenderID == -1 then
        return
    end
    d = tonumber (delta)
    ModifyAssetHealth_CPP(OffenderID, d)
end

--- Removes asset if valid asset ID
-- @param - No parameters
function RemoveAsset()
    if OffenderID == -1 then 
        return
    end
    RemoveAsset_CPP(OffenderID)
end

--- Adds an asset of assetType onto the location (PositionX, PositionY)
--  if an asset already exists at this location, it will spawn at the closest avalible spot
-- @param AssetName - name of asset to spawn
-- @param PositionX - Location to spawn X-coord
-- @param PositionY - Location to spawn Y-coord 
-- @param amount - Amount of resources in the resource asset (ignored if not resource )
function AddAsset(AssetName, Targetcolor, PositionX, PositionY, amount)
    AddAsset_CPP(AssetName, Targetcolor, PositionX, PositionY, amount)
end

--- For creating resource assets: GoldMine, Stone Quarry
--  Adds an asset of assetType onto the location (PositionX, PositionY)
--  if an asset already exists at this location, it will spawn at the closest avalible spot
-- @param AssetName - name of asset to spawn
-- @param PositionX - Location to spawn X-coord
-- @param PositionY - Location to spawn Y-coord 
function AddResourceAsset(AssetName, Targetcolor, PositionX, PositionY, amount)
    AddResourceAsset_CPP(AssetName, Targetcolor, PositionX, PositionY)
end

--- Adds an upgrade to designated player color's assets
function AddUpgrade(Targetcolor, upgrade)
    AddUpgrade_CPP(Targetcolor, upgrade)
end

--- Removes an upgrade to designated player color's assets
function RemoveUpgrade(Targetcolor, upgrade)
    RemoveUpgrade_CPP(Targetcolor, upgrade)
end
    