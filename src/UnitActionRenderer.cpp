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
#include "UnitActionRenderer.h"

/**
 * @brief Constructor Renders action tiles  
 * Imports tiles from bin/img to use as assets in the game
 * 
 * @param bevel pointer slope of shading to create 3D effect
 * @param icons graphics pointer
 * @param color player's color 
 * @param player game data specific to player 
 */
CUnitActionRenderer::CUnitActionRenderer(std::shared_ptr< CBevel > bevel, std::shared_ptr< CGraphicTileset > icons, EPlayerColor color, std::shared_ptr< CPlayerData > player){
    DIconTileset = icons;
    DBevel = bevel;
    DPlayerData = player;
    DPlayerColor = color;
    
    DCommandIndices.resize(to_underlying(EAssetCapabilityType::Max));
    DFullIconWidth = DIconTileset->TileWidth() + DBevel->Width() * 2;
    DFullIconHeight = DIconTileset->TileHeight() + DBevel->Width() * 2;
    DDisplayedCommands.resize(9);
    for(auto &Commands : DDisplayedCommands){
        Commands = EAssetCapabilityType::None;
    }
    DCommandIndices.resize(to_underlying(EAssetCapabilityType::Max));
    DCommandIndices[to_underlying(EAssetCapabilityType::None)] = -1;
    DCommandIndices[to_underlying(EAssetCapabilityType::BuildPeasant)] = DIconTileset->FindTile("peasant");
    DCommandIndices[to_underlying(EAssetCapabilityType::BuildFootman)] = DIconTileset->FindTile("footman");
    DCommandIndices[to_underlying(EAssetCapabilityType::BuildArcher)] = DIconTileset->FindTile("archer");
    DCommandIndices[to_underlying(EAssetCapabilityType::BuildRanger)] = DIconTileset->FindTile("ranger");
    DCommandIndices[to_underlying(EAssetCapabilityType::BuildFarm)] = DIconTileset->FindTile("chicken-farm");
    DCommandIndices[to_underlying(EAssetCapabilityType::BuildTownHall)] = DIconTileset->FindTile("town-hall");
    DCommandIndices[to_underlying(EAssetCapabilityType::BuildBarracks)] = DIconTileset->FindTile("human-barracks");
    DCommandIndices[to_underlying(EAssetCapabilityType::BuildLumberMill)] = DIconTileset->FindTile("human-lumber-mill");
    DCommandIndices[to_underlying(EAssetCapabilityType::BuildBlacksmith)] = DIconTileset->FindTile("human-blacksmith");
    DCommandIndices[to_underlying(EAssetCapabilityType::BuildKeep)] = DIconTileset->FindTile("keep");
    DCommandIndices[to_underlying(EAssetCapabilityType::BuildCastle)] = DIconTileset->FindTile("castle");
    DCommandIndices[to_underlying(EAssetCapabilityType::BuildScoutTower)] = DIconTileset->FindTile("scout-tower");
    DCommandIndices[to_underlying(EAssetCapabilityType::BuildGuardTower)] = DIconTileset->FindTile("human-guard-tower");
    DCommandIndices[to_underlying(EAssetCapabilityType::BuildCannonTower)] = DIconTileset->FindTile("human-cannon-tower");
    DCommandIndices[to_underlying(EAssetCapabilityType::Move)] = DIconTileset->FindTile("human-move");
    DCommandIndices[to_underlying(EAssetCapabilityType::Repair)] = DIconTileset->FindTile("repair");
    DCommandIndices[to_underlying(EAssetCapabilityType::Mine)] = DIconTileset->FindTile("mine");
    DCommandIndices[to_underlying(EAssetCapabilityType::BuildSimple)] = DIconTileset->FindTile("build-simple");
    DCommandIndices[to_underlying(EAssetCapabilityType::BuildAdvanced)] = DIconTileset->FindTile("build-advanced");
    DCommandIndices[to_underlying(EAssetCapabilityType::Convey)] = DIconTileset->FindTile("human-convey");
    DCommandIndices[to_underlying(EAssetCapabilityType::Cancel)] = DIconTileset->FindTile("cancel");
    DCommandIndices[to_underlying(EAssetCapabilityType::BuildWall)] = DIconTileset->FindTile("human-wall");
    DCommandIndices[to_underlying(EAssetCapabilityType::Attack)] = DIconTileset->FindTile("human-weapon-1");
    DCommandIndices[to_underlying(EAssetCapabilityType::StandGround)] = DIconTileset->FindTile("human-armor-1");
    DCommandIndices[to_underlying(EAssetCapabilityType::Patrol)] = DIconTileset->FindTile("human-patrol");
    DCommandIndices[to_underlying(EAssetCapabilityType::WeaponUpgrade1)] = DIconTileset->FindTile("human-weapon-1");
    DCommandIndices[to_underlying(EAssetCapabilityType::WeaponUpgrade2)] = DIconTileset->FindTile("human-weapon-2");
    DCommandIndices[to_underlying(EAssetCapabilityType::WeaponUpgrade3)] = DIconTileset->FindTile("human-weapon-3");
    DCommandIndices[to_underlying(EAssetCapabilityType::ArrowUpgrade1)] = DIconTileset->FindTile("human-arrow-1");
    DCommandIndices[to_underlying(EAssetCapabilityType::ArrowUpgrade2)] = DIconTileset->FindTile("human-arrow-2");
    DCommandIndices[to_underlying(EAssetCapabilityType::ArrowUpgrade3)] = DIconTileset->FindTile("human-arrow-3");
    DCommandIndices[to_underlying(EAssetCapabilityType::ArmorUpgrade1)] = DIconTileset->FindTile("human-armor-1");
    DCommandIndices[to_underlying(EAssetCapabilityType::ArmorUpgrade2)] = DIconTileset->FindTile("human-armor-2");
    DCommandIndices[to_underlying(EAssetCapabilityType::ArmorUpgrade3)] = DIconTileset->FindTile("human-armor-3");
    DCommandIndices[to_underlying(EAssetCapabilityType::Longbow)] = DIconTileset->FindTile("longbow");
    DCommandIndices[to_underlying(EAssetCapabilityType::RangerScouting)] = DIconTileset->FindTile("ranger-scouting");
    DCommandIndices[to_underlying(EAssetCapabilityType::Marksmanship)] = DIconTileset->FindTile("marksmanship");
    
    DDisabledIndex = DIconTileset->FindTile("disabled");
}

/**
 * @brief Default empty destructor
 */
CUnitActionRenderer::~CUnitActionRenderer(){
    
}

int CUnitActionRenderer::MinimumWidth() const{
    return DFullIconWidth * 3 + DBevel->Width() * 2;
}

int CUnitActionRenderer::MinimumHeight() const{
    return DFullIconHeight * 3 + DBevel->Width() * 2;
}

/**
 * @brief Selection of action/asset corresponding to the tile clicked
 * @param pos the pixel position
 * @return The selected asset (i.e. building, upgrade, unit)
 */
EAssetCapabilityType CUnitActionRenderer::Selection(const CPosition &pos) const{
    if(((pos.X() % (DFullIconWidth + DBevel->Width())) < DFullIconWidth)&&((pos.Y() % (DFullIconHeight + DBevel->Width())) < DFullIconHeight)){
        int Index = (pos.X() / (DFullIconWidth + DBevel->Width())) + (pos.Y() / (DFullIconHeight + DBevel->Width())) * 3;
        return DDisplayedCommands[Index];
    }
    return EAssetCapabilityType::None;
}

/**
 * @brief Used in BattleMode's Render() function
 * Renders the icons in the bottom left corner of the screen 
 * Only icons that are capable by the unit are rendered
 * 
 * @param surface pointer to render image
 * @param selectionlist list of all capabilities
 * @param currentaction chosen action (i.e. move, build, attack, etc)
 */
void CUnitActionRenderer::DrawUnitAction(std::shared_ptr< CGraphicSurface > surface, const std::list< std::weak_ptr< CPlayerAsset > > &selectionlist, EAssetCapabilityType currentaction){
    bool AllSame = true;
    bool IsFirst = true;
    bool Moveable = true;
    bool HasCargo = false;
    EAssetType UnitType;
    
    for(auto &Command : DDisplayedCommands){
        Command = EAssetCapabilityType::None;    
    }
    if(!selectionlist.size()){
        return;    
    }
    for(auto &Iterator : selectionlist){
        if(auto Asset = Iterator.lock()){
            if(DPlayerColor != Asset->Color()){
                return;   
            }
            if(IsFirst){
                UnitType = Asset->Type();     
                IsFirst = false;
                Moveable = 0 < Asset->Speed();
            }
            else if(UnitType != Asset->Type()){
                AllSame = false;   
            }
            if(Asset->Lumber() || Asset->Gold()){
                HasCargo = true;      
            }
        }
    }
    if(EAssetCapabilityType::None == currentaction){
        if(Moveable){

            DDisplayedCommands[0] = HasCargo ? EAssetCapabilityType::Convey : EAssetCapabilityType::Move;
            DDisplayedCommands[1] = EAssetCapabilityType::StandGround;
            DDisplayedCommands[2] = EAssetCapabilityType::Attack;
            if(auto Asset = selectionlist.front().lock()){
                if(Asset->HasCapability(EAssetCapabilityType::Repair)){
                    DDisplayedCommands[3] = EAssetCapabilityType::Repair;
                }
                if(Asset->HasCapability(EAssetCapabilityType::Patrol)){
                    DDisplayedCommands[3] = EAssetCapabilityType::Patrol;
                }
                if(Asset->HasCapability(EAssetCapabilityType::Mine)){
                    DDisplayedCommands[4] = EAssetCapabilityType::Mine;
                }
                if(Asset->HasCapability(EAssetCapabilityType::BuildSimple) && (1 == selectionlist.size())){
                    DDisplayedCommands[6] = EAssetCapabilityType::BuildSimple;
                }
            }
        }
        else{
            if(auto Asset = selectionlist.front().lock()){
                if((EAssetAction::Construct == Asset->Action())||(EAssetAction::Capability == Asset->Action())){
                    DDisplayedCommands.back() = EAssetCapabilityType::Cancel;    
                }
                else{
                    int Index = 0;
                    for(auto &Capability : Asset->Capabilities()){
                        DDisplayedCommands[Index] = Capability;
                        Index++;
                        if(DDisplayedCommands.size() <= Index){
                            break;   
                        }
                    }
                }
            }
        }
    }
    else if(EAssetCapabilityType::BuildSimple == currentaction){
        if(auto Asset = selectionlist.front().lock()){
            int Index = 0;
            for(auto Capability : {EAssetCapabilityType::BuildFarm,
                                    EAssetCapabilityType::BuildTownHall,
                                    EAssetCapabilityType::BuildBarracks,
                                    EAssetCapabilityType::BuildLumberMill,
                                    EAssetCapabilityType::BuildBlacksmith,
                                    EAssetCapabilityType::BuildKeep,
                                    EAssetCapabilityType::BuildCastle,
                                    EAssetCapabilityType::BuildScoutTower,
                                    EAssetCapabilityType::BuildGuardTower,
                                    EAssetCapabilityType::BuildCannonTower,
                                    EAssetCapabilityType::BuildWall}){
                if(Asset->HasCapability(Capability)){
                    DDisplayedCommands[Index] = Capability;
                    Index++;
                    if(DDisplayedCommands.size() <= Index){
                        break;   
                    }
                }
            }
            DDisplayedCommands.back() = EAssetCapabilityType::Cancel;
        }
    }
    else{
        DDisplayedCommands.back() = EAssetCapabilityType::Cancel;
    }
    
    int XOffset = DBevel->Width();
    int YOffset = DBevel->Width();
    int Index = 0;

    /**
     * @brief Draws Possible Icons with Bevel Shading 
     */
    for(auto IconType : DDisplayedCommands){
        if(EAssetCapabilityType::None != IconType){
            auto PlayerCapability = CPlayerCapability::FindCapability(IconType);
            DBevel->DrawBevel(surface, XOffset, YOffset, DIconTileset->TileWidth(), DIconTileset->TileHeight());   
            DIconTileset->DrawTile(surface, XOffset, YOffset, DCommandIndices[to_underlying(IconType)]);
            if(PlayerCapability){
                if(!PlayerCapability->CanInitiate(selectionlist.front().lock(), DPlayerData)){
                    DIconTileset->DrawTile(surface, XOffset, YOffset, DDisabledIndex);
                }
            }
        }
        
        XOffset += DFullIconWidth + DBevel->Width();
        Index++;
        if(0 == (Index % 3)){
            XOffset = DBevel->Width();
            YOffset += DFullIconHeight + DBevel->Width();  
        }
    }
}

