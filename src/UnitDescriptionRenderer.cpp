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
#include "UnitDescriptionRenderer.h"
#include "TextFormatter.h"

#define FG_COLOR        0
#define BG_COLOR        1
#define MAX_HP_COLOR    3
#define HEALTH_HEIGHT   5


/**
 * @brief Unit Description Constructor
 * Loads TileSets for Units, Buildings, Upgrades
 * 
 * @param bevel pointer to shading technique
 * @param icons vector of tiles pictures
 * @param fonts array of various sized fonts (i.e. small, medium, large, etc)
 * @param color enumeration integer representing player color
 */
CUnitDescriptionRenderer::CUnitDescriptionRenderer(std::shared_ptr< CBevel > bevel, std::shared_ptr< CGraphicMulticolorTileset > icons, std::array< std::shared_ptr< CFontTileset >, to_underlying(EFontSize::Max) > fonts, EPlayerColor color){
    int TextWidth, TextHeight;
    
    DPlayerColor = color;
    DIconTileset = icons;
    DBevel = bevel;
    for(int Index = 0; Index < to_underlying(EFontSize::Max); Index++){
        DFonts[Index] = fonts[Index];
        
        DFontColorIndices[Index].resize(2);
        DFontColorIndices[Index][FG_COLOR] = DFonts[Index]->FindColor("white");
        DFontColorIndices[Index][BG_COLOR] = DFonts[Index]->FindColor("black");
    }
    DFonts[to_underlying(EFontSize::Small)]->MeasureText("0123456789", TextWidth, TextHeight);
    DFullIconWidth = DIconTileset->TileWidth() + DBevel->Width() * 2;
    DFullIconHeight = DIconTileset->TileHeight() + DBevel->Width() * 3 + HEALTH_HEIGHT + 2 + TextHeight;
    
    /**
     * @brief Health Bar Colors 
     */
    DHealthColors.resize(MAX_HP_COLOR);
    DHealthColors[0] = 0xFC0000;/**red*/
    DHealthColors[1] = 0xFCFC00;/**yellow*/
    DHealthColors[2] = 0x307000;/**green*/
    
    DHealthRectangleFG = 0x000000;
    DHealthRectangleBG = 0x303030;
    
    DConstructionRectangleFG = 0xA0A060;
    DConstructionRectangleBG = 0x505050;
    DConstructionRectangleCompletion = 0x307000;
    DConstructionRectangleShadow = 0x000000;
    
    DAssetIndices.resize(to_underlying(EAssetType::Max));

    /**
     * @brief Load Unit and Buildings TileSets
     */
    DAssetIndices[to_underlying(EAssetType::Peasant)] = DIconTileset->FindTile("peasant");
    DAssetIndices[to_underlying(EAssetType::Footman)] = DIconTileset->FindTile("footman");
    DAssetIndices[to_underlying(EAssetType::Archer)] = DIconTileset->FindTile("archer");
    DAssetIndices[to_underlying(EAssetType::Ranger)] = DIconTileset->FindTile("ranger");
    DAssetIndices[to_underlying(EAssetType::GoldMine)] = DIconTileset->FindTile("gold-mine");
    DAssetIndices[to_underlying(EAssetType::TownHall)] = DIconTileset->FindTile("town-hall");
    DAssetIndices[to_underlying(EAssetType::Keep)] = DIconTileset->FindTile("keep");
    DAssetIndices[to_underlying(EAssetType::Castle)] = DIconTileset->FindTile("castle");
    DAssetIndices[to_underlying(EAssetType::Farm)] = DIconTileset->FindTile("chicken-farm");
    DAssetIndices[to_underlying(EAssetType::Barracks)] = DIconTileset->FindTile("human-barracks");
    DAssetIndices[to_underlying(EAssetType::LumberMill)] = DIconTileset->FindTile("human-lumber-mill");
    DAssetIndices[to_underlying(EAssetType::Blacksmith)] = DIconTileset->FindTile("human-blacksmith");
    DAssetIndices[to_underlying(EAssetType::ScoutTower)] = DIconTileset->FindTile("scout-tower");
    DAssetIndices[to_underlying(EAssetType::GuardTower)] = DIconTileset->FindTile("human-guard-tower");
    DAssetIndices[to_underlying(EAssetType::CannonTower)] = DIconTileset->FindTile("human-cannon-tower");
    DAssetIndices[to_underlying(EAssetType::Wall)] = DIconTileset->FindTile("human-wall");
    
    DResearchIndices.resize(to_underlying(EAssetCapabilityType::Max));

     /**
     * @brief Load Upgrade TileSets
     */
    DResearchIndices[to_underlying(EAssetCapabilityType::WeaponUpgrade1)] = DIconTileset->FindTile("human-weapon-1");
    DResearchIndices[to_underlying(EAssetCapabilityType::WeaponUpgrade2)] = DIconTileset->FindTile("human-weapon-2");
    DResearchIndices[to_underlying(EAssetCapabilityType::WeaponUpgrade3)] = DIconTileset->FindTile("human-weapon-3");
    DResearchIndices[to_underlying(EAssetCapabilityType::ArrowUpgrade1)] = DIconTileset->FindTile("human-arrow-1");
    DResearchIndices[to_underlying(EAssetCapabilityType::ArrowUpgrade2)] = DIconTileset->FindTile("human-arrow-2");
    DResearchIndices[to_underlying(EAssetCapabilityType::ArrowUpgrade3)] = DIconTileset->FindTile("human-arrow-3");
    DResearchIndices[to_underlying(EAssetCapabilityType::ArmorUpgrade1)] = DIconTileset->FindTile("human-armor-1");
    DResearchIndices[to_underlying(EAssetCapabilityType::ArmorUpgrade2)] = DIconTileset->FindTile("human-armor-2");
    DResearchIndices[to_underlying(EAssetCapabilityType::ArmorUpgrade3)] = DIconTileset->FindTile("human-armor-3");
    DResearchIndices[to_underlying(EAssetCapabilityType::Longbow)] = DIconTileset->FindTile("longbow");
    DResearchIndices[to_underlying(EAssetCapabilityType::RangerScouting)] = DIconTileset->FindTile("ranger-scouting");
    DResearchIndices[to_underlying(EAssetCapabilityType::Marksmanship)] = DIconTileset->FindTile("marksmanship");
    DResearchIndices[to_underlying(EAssetCapabilityType::BuildRanger)] = DIconTileset->FindTile("ranger");
}

/**
 * @brief Default empty destructor
 */
CUnitDescriptionRenderer::~CUnitDescriptionRenderer(){
                                                                           
}

/**
 * @brief Creates NameSpace for Asset
 * Adds separation between letters and formats the text properly
 * @param name input string
 * @return string for AssetName 
 */
std::string CUnitDescriptionRenderer::AddAssetNameSpaces(const std::string &name){
    std::string ReturnString;
    
    for(auto CurChar : name){
        if(ReturnString.size() && (('A' <= CurChar)&&('Z' >= CurChar))){
            ReturnString += ' ';   
        }
        ReturnString += CurChar;
    }
    
    return ReturnString;
}

/**
 * @brief Sets Minimum Width
 * Used in Application Data to Set Left Panel Dimensions
 * @return integer x-coordinate dimension 
 */
int CUnitDescriptionRenderer::MinimumWidth() const{
    return DFullIconWidth * 3 + DBevel->Width() * 2;
}

/**
 * @brief Sets Minimum Height
 * Used in Application Data to Set Left Panel Dimensions
 * @return integer y-coordinate dimension 
 */
int CUnitDescriptionRenderer::MinimumHeight(int width, int count) const{
    int Columns = (width / DFullIconWidth);
    int Rows = (count + Columns-1) / Columns;
    return Rows * DFullIconHeight + (Rows - 1) * DBevel->Width();
}

int CUnitDescriptionRenderer::MaxSelection(int width, int height) const{
    return (width / DFullIconWidth) * (height / DFullIconHeight);
}

int CUnitDescriptionRenderer::Selection(const CPosition &pos) const{
    int HorizontalIcons, VerticalIcons;
    int HorizontalGap, VerticalGap;
    int SelectedIcon = -1;
    
    HorizontalIcons = DDisplayedWidth / DFullIconWidth;
    VerticalIcons = DDisplayedHeight / DFullIconHeight;
    HorizontalGap = (DDisplayedWidth - (HorizontalIcons * DFullIconWidth)) / (HorizontalIcons - 1);
    VerticalGap = (DDisplayedHeight - (VerticalIcons * DFullIconHeight)) / (VerticalIcons - 1);
    if(((pos.X() % (DFullIconWidth + HorizontalGap)) < DFullIconWidth)&&((pos.Y() % (DFullIconHeight + VerticalGap)) < DFullIconHeight)){
        SelectedIcon = (pos.X() / (DFullIconWidth + HorizontalGap)) + HorizontalIcons * (pos.Y() / (DFullIconHeight + VerticalGap));
        if(DDisplayedIcons <= SelectedIcon){
            SelectedIcon = -1;    
        }
    }
    
    return SelectedIcon;
}

void CUnitDescriptionRenderer::DrawCompletionBar(std::shared_ptr< CGraphicSurface > surface, int percent){
    auto ResourceContext = surface->CreateResourceContext();
    int TextWidth, TextHeight, DisplayWidth, TextTop, TextBottom;
    uint32_t BlackColor = 0x000000;
    DFonts[to_underlying(EFontSize::Large)]->MeasureTextDetailed("% Complete", TextWidth, TextHeight, TextTop, TextBottom);
    
    TextHeight = TextBottom - TextTop + 1;
    /*
    gdk_gc_set_rgb_fg_color(gc, &BlackColor);
    gdk_gc_set_rgb_bg_color(gc, &BlackColor);
    gdk_draw_rectangle(drawable, gc, TRUE, 0, DDisplayedHeight - (TextHeight + 12),  DDisplayedWidth, TextHeight + 12);
    */
    ResourceContext->SetSourceRGB(BlackColor);
    ResourceContext->Rectangle(0, DDisplayedHeight - (TextHeight + 12),  DDisplayedWidth, TextHeight + 12);
    ResourceContext->Fill();
    
    /*
    gdk_gc_set_rgb_fg_color(gc, &DConstructionRectangleFG);
    gdk_draw_rectangle(drawable, gc, FALSE, 1, DDisplayedHeight - (TextHeight + 11),  DDisplayedWidth-2, TextHeight + 10);
    gdk_draw_rectangle(drawable, gc, TRUE, 2, DDisplayedHeight - (TextHeight + 10),  DDisplayedWidth-4, TextHeight + 8);
    */
    
    ResourceContext->SetSourceRGB(DConstructionRectangleFG);
    ResourceContext->Rectangle(1, DDisplayedHeight - (TextHeight + 11),  DDisplayedWidth-2, TextHeight + 10);
    ResourceContext->Stroke();
    
    ResourceContext->SetSourceRGB(BlackColor);
    ResourceContext->Rectangle(3, DDisplayedHeight - (TextHeight + 9),  DDisplayedWidth-6, TextHeight + 6);
    ResourceContext->Fill();

    /*
    gdk_gc_set_rgb_fg_color(gc, &DConstructionRectangleBG);
    gdk_gc_set_rgb_bg_color(gc, &DConstructionRectangleBG);
    gdk_draw_rectangle(drawable, gc, TRUE, 4, DDisplayedHeight - (TextHeight + 8),  DDisplayedWidth-8, TextHeight + 4);
    */
    
    ResourceContext->SetSourceRGB(DConstructionRectangleBG);
    ResourceContext->Rectangle(4, DDisplayedHeight - (TextHeight + 8),  DDisplayedWidth-8, TextHeight + 4);
    ResourceContext->Fill();

    /*
    gdk_gc_set_rgb_fg_color(gc, &DConstructionRectangleShadow);
    gdk_gc_set_rgb_bg_color(gc, &DConstructionRectangleShadow);
    gdk_draw_rectangle(drawable, gc, TRUE, 4, DDisplayedHeight - (TextHeight + 8),  DDisplayedWidth-8, TextHeight/2 + 2);
    */
    
    ResourceContext->SetSourceRGB(DConstructionRectangleShadow);
    ResourceContext->Rectangle(4, DDisplayedHeight - (TextHeight + 8),  DDisplayedWidth-8, TextHeight/2 + 2);
    ResourceContext->Fill();

    
    DisplayWidth = percent * (DDisplayedWidth-8) / 100;
    
    /*
    gdk_gc_set_rgb_fg_color(gc, &DConstructionRectangleCompletion);
    gdk_gc_set_rgb_bg_color(gc, &DConstructionRectangleCompletion);
    gdk_draw_rectangle(drawable, gc, TRUE, 4, DDisplayedHeight - (TextHeight + 8), DisplayWidth, TextHeight + 4);
    */
    
    ResourceContext->SetSourceRGB(DConstructionRectangleCompletion);
    ResourceContext->Rectangle(4, DDisplayedHeight - (TextHeight + 8), DisplayWidth, TextHeight + 4);
    ResourceContext->Fill();

    std::string percentage = std::to_string(percent) + "% Complete";
    DFonts[to_underlying(EFontSize::Large)]->DrawTextWithShadow(surface, 
                        DDisplayedWidth/2 - TextWidth/2, 
                        DDisplayedHeight  - (TextHeight + TextTop + 6), 
                        DFontColorIndices[to_underlying(EFontSize::Large)][FG_COLOR], DFontColorIndices[to_underlying(EFontSize::Large)][BG_COLOR], 1, percentage);
    
}

void CUnitDescriptionRenderer::DrawUnitDescription(std::shared_ptr< CGraphicSurface > surface, const std::list< std::weak_ptr< CPlayerAsset > > &selectionlist){
    DDisplayedIcons = 0;
    if(selectionlist.size()){
        auto ResoureContext = surface->CreateResourceContext();
        int HorizontalIcons, VerticalIcons;
        int HorizontalGap, VerticalGap;
        
        DDisplayedWidth = surface->Width();
        DDisplayedHeight = surface->Height();
        HorizontalIcons = DDisplayedWidth / DFullIconWidth;
        VerticalIcons = DDisplayedHeight / DFullIconHeight;
        HorizontalGap = (DDisplayedWidth - (HorizontalIcons * DFullIconWidth)) / (HorizontalIcons - 1);
        VerticalGap = (DDisplayedHeight - (VerticalIcons * DFullIconHeight)) / (VerticalIcons - 1);

        if(1 == selectionlist.size()){
            DDisplayedIcons = 1;
            if(auto Asset = selectionlist.front().lock()){
                int HPColor = (Asset->HitPoints() - 1) * MAX_HP_COLOR / Asset->MaxHitPoints();
                int TextWidth, TextHeight;
                int TextCenter, TextTop;
                std::string AssetName = AddAssetNameSpaces(CPlayerAssetType::TypeToName(Asset->Type()));
                std::string TempString;
                
                DBevel->DrawBevel(surface, DBevel->Width(), DBevel->Width(), DIconTileset->TileWidth(), DIconTileset->TileHeight());
                DIconTileset->DrawTile(surface, DBevel->Width(), DBevel->Width(), DAssetIndices[to_underlying(Asset->Type())], to_underlying(Asset->Color()) ? to_underlying(Asset->Color()) - 1 : 0);
                
                DFonts[to_underlying(EFontSize::Medium)]->MeasureText(AssetName, TextWidth, TextHeight);
                
                TextCenter = (DDisplayedWidth + DIconTileset->TileWidth() + DBevel->Width() * 2)/ 2;
                DFonts[to_underlying(EFontSize::Medium)]->DrawTextWithShadow(surface,
                    TextCenter - TextWidth/2, 
                    (DIconTileset->TileHeight()/2 + DBevel->Width()) - TextHeight/2, 
                    DFontColorIndices[to_underlying(EFontSize::Medium)][FG_COLOR], DFontColorIndices[to_underlying(EFontSize::Medium)][BG_COLOR], 1, AssetName);

                if(EPlayerColor::None != Asset->Color()){
                    ResoureContext->SetSourceRGB(DHealthRectangleFG);
                    ResoureContext->Rectangle(0, DIconTileset->TileHeight() + DBevel->Width() * 3,  DIconTileset->TileWidth() + DBevel->Width() * 2, HEALTH_HEIGHT + 2);
                    ResoureContext->Fill();
                    /*
                    gdk_gc_set_rgb_fg_color(TempGC, &DHealthRectangleFG);
                    gdk_gc_set_rgb_bg_color(TempGC, &DHealthRectangleBG);
                    gdk_draw_rectangle(surface, TRUE, 0, DIconTileset->TileHeight() + DBevel->Width() * 3,  DIconTileset->TileWidth() + DBevel->Width() * 2, HEALTH_HEIGHT + 2);
                    */
                    
                    ResoureContext->SetSourceRGB(DHealthColors[HPColor]);
                    ResoureContext->Rectangle(1, DIconTileset->TileHeight() + DBevel->Width() * 3 + 1,  (DIconTileset->TileWidth() + DBevel->Width() * 2 - 2) * Asset->HitPoints()/ Asset->MaxHitPoints() , HEALTH_HEIGHT);
                    ResoureContext->Fill();
                    /*
                    gdk_gc_set_rgb_fg_color(TempGC, &DHealthColors[HPColor]);
                    gdk_gc_set_rgb_bg_color(TempGC, &DHealthColors[HPColor]);
                    gdk_draw_rectangle(surface, TRUE, 1, DIconTileset->TileHeight() + DBevel->Width() * 3 + 1,  (DIconTileset->TileWidth() + DBevel->Width() * 2 - 2) * Asset->HitPoints()/ Asset->MaxHitPoints() , HEALTH_HEIGHT);
                    */
                    
                    TempString = std::to_string(Asset->HitPoints()) + std::string(" / ") + std::to_string(Asset->MaxHitPoints());
                    DFonts[to_underlying(EFontSize::Small)]->MeasureText(TempString, TextWidth, TextHeight);
                
                    TextTop = DIconTileset->TileHeight() + DBevel->Width() * 4 + HEALTH_HEIGHT + 2;
                    DFonts[to_underlying(EFontSize::Small)]->DrawTextWithShadow(surface, 
                        (DIconTileset->TileWidth()/2 + DBevel->Width()) - TextWidth/2, 
                        TextTop, 
                        DFontColorIndices[to_underlying(EFontSize::Small)][FG_COLOR], DFontColorIndices[to_underlying(EFontSize::Small)][BG_COLOR], 1, TempString);
                
                    TextTop += TextHeight;
                }
                if(DPlayerColor == Asset->Color()){
                    if(Asset->Speed()){
                        int TextLineHeight;
                        int UpgradeValue;
                        
                        TempString = "Armor: ";
                        DFonts[to_underlying(EFontSize::Medium)]->MeasureText(TempString, TextWidth, TextHeight);
                        TextLineHeight = TextHeight;
                        DFonts[to_underlying(EFontSize::Medium)]->DrawTextWithShadow(surface, 
                            TextCenter - TextWidth, 
                            TextTop, 
                            DFontColorIndices[to_underlying(EFontSize::Medium)][FG_COLOR], DFontColorIndices[to_underlying(EFontSize::Medium)][BG_COLOR], 1, TempString);
                        
                        UpgradeValue = Asset->ArmorUpgrade();
                        TempString = std::to_string(Asset->Armor());
                        if(UpgradeValue){
                            TempString += " + ";
                            TempString += std::to_string(UpgradeValue);
                        }
                        DFonts[to_underlying(EFontSize::Medium)]->DrawTextWithShadow(surface, 
                            TextCenter, 
                            TextTop, 
                            DFontColorIndices[to_underlying(EFontSize::Medium)][FG_COLOR], DFontColorIndices[to_underlying(EFontSize::Medium)][BG_COLOR], 1, TempString);
                        
                        
                        TextTop += TextLineHeight;
                        TempString = "Damage: ";
                        DFonts[to_underlying(EFontSize::Medium)]->MeasureText(TempString, TextWidth, TextHeight);
                        DFonts[to_underlying(EFontSize::Medium)]->DrawTextWithShadow(surface, 
                            TextCenter - TextWidth, 
                            TextTop, 
                            DFontColorIndices[to_underlying(EFontSize::Medium)][FG_COLOR], DFontColorIndices[to_underlying(EFontSize::Medium)][BG_COLOR], 1, TempString);
                        
                        UpgradeValue = Asset->BasicDamageUpgrade() + Asset->PiercingDamageUpgrade();
                        TempString.clear();
                        if(UpgradeValue){
                            TempString += " + ";
                            TempString += std::to_string(UpgradeValue);
                        }
                        DFonts[to_underlying(EFontSize::Medium)]->DrawTextWithShadow(surface, 
                            TextCenter, 
                            TextTop, 
                            DFontColorIndices[to_underlying(EFontSize::Medium)][FG_COLOR], DFontColorIndices[to_underlying(EFontSize::Medium)][BG_COLOR], 1, std::to_string(Asset->PiercingDamage()/2) + "-" + std::to_string(Asset->PiercingDamage() + Asset->BasicDamage()) + TempString);
                        
                        TextTop += TextLineHeight;
                        TempString = "Range: ";
                        DFonts[to_underlying(EFontSize::Medium)]->MeasureText(TempString, TextWidth, TextHeight);
                        DFonts[to_underlying(EFontSize::Medium)]->DrawTextWithShadow(surface, 
                            TextCenter - TextWidth, 
                            TextTop, 
                            DFontColorIndices[to_underlying(EFontSize::Medium)][FG_COLOR], DFontColorIndices[to_underlying(EFontSize::Medium)][BG_COLOR], 1, TempString);
                        
                        UpgradeValue = Asset->RangeUpgrade();
                        TempString = std::to_string(Asset->Range());
                        if(UpgradeValue){
                            TempString += " + ";
                            TempString += std::to_string(UpgradeValue);
                        }
                        DFonts[to_underlying(EFontSize::Medium)]->DrawTextWithShadow(surface, 
                            TextCenter, 
                            TextTop, 
                            DFontColorIndices[to_underlying(EFontSize::Medium)][FG_COLOR], DFontColorIndices[to_underlying(EFontSize::Medium)][BG_COLOR], 1, TempString);
                        
                        TextTop += TextLineHeight;
                        TempString = "Sight: ";
                        DFonts[to_underlying(EFontSize::Medium)]->MeasureText(TempString, TextWidth, TextHeight);
                        DFonts[to_underlying(EFontSize::Medium)]->DrawTextWithShadow(surface, 
                            TextCenter - TextWidth, 
                            TextTop, 
                            DFontColorIndices[to_underlying(EFontSize::Medium)][FG_COLOR], DFontColorIndices[to_underlying(EFontSize::Medium)][BG_COLOR], 1, TempString);
                        
                        UpgradeValue = Asset->SightUpgrade();
                        TempString = std::to_string(Asset->Sight());
                        if(UpgradeValue){
                            TempString += " + ";
                            TempString += std::to_string(UpgradeValue);
                        }
                        DFonts[to_underlying(EFontSize::Medium)]->DrawTextWithShadow(surface, 
                            TextCenter, 
                            TextTop, 
                            DFontColorIndices[to_underlying(EFontSize::Medium)][FG_COLOR], DFontColorIndices[to_underlying(EFontSize::Medium)][BG_COLOR], 1, TempString);
                        
                        TextTop += TextLineHeight;
                        TempString = "Speed: ";
                        DFonts[to_underlying(EFontSize::Medium)]->MeasureText(TempString, TextWidth, TextHeight);
                        DFonts[to_underlying(EFontSize::Medium)]->DrawTextWithShadow(surface, 
                            TextCenter - TextWidth, 
                            TextTop, 
                            DFontColorIndices[to_underlying(EFontSize::Medium)][FG_COLOR], DFontColorIndices[to_underlying(EFontSize::Medium)][BG_COLOR], 1, TempString);
                        UpgradeValue = Asset->SpeedUpgrade();
                        TempString = std::to_string(Asset->Speed());
                        if(UpgradeValue){
                            TempString += " + ";
                            TempString += std::to_string(UpgradeValue);
                        }
                        DFonts[to_underlying(EFontSize::Medium)]->DrawTextWithShadow(surface, 
                            TextCenter, 
                            TextTop, 
                            DFontColorIndices[to_underlying(EFontSize::Medium)][FG_COLOR], DFontColorIndices[to_underlying(EFontSize::Medium)][BG_COLOR], 1, TempString);
                    }
                    else{
                        if(EAssetAction::Construct == Asset->Action()){
                            auto Command = Asset->CurrentCommand();
                            int PercentComplete = 0;
                            if(Command.DAssetTarget){
                                Command = Command.DAssetTarget->CurrentCommand();
                                if(Command.DActivatedCapability){
                                    PercentComplete = Command.DActivatedCapability->PercentComplete(100);
                                }
                            }
                            else if(Command.DActivatedCapability){
                                PercentComplete = Command.DActivatedCapability->PercentComplete(100);
                            }
                            DrawCompletionBar(surface, PercentComplete);
                        }
                        else if(EAssetAction::Capability == Asset->Action()){
                            auto Command = Asset->CurrentCommand();
                            int PercentComplete = 0;
                            if(Command.DActivatedCapability){
                                PercentComplete = Command.DActivatedCapability->PercentComplete(100);
                            }
                            if(Command.DAssetTarget){
                                //int HPColor = (Command.DAssetTarget->HitPoints() - 1) * MAX_HP_COLOR / Command.DAssetTarget->MaxHitPoints();
                                int HorizontalOffset = DBevel->Width(), VerticalOffset = DBevel->Width();
                                
                                HorizontalOffset += 2 * (DFullIconWidth + HorizontalGap);
                                VerticalOffset += DFullIconHeight + VerticalGap;
                                
                                DBevel->DrawBevel(surface, HorizontalOffset, VerticalOffset, DIconTileset->TileWidth(), DIconTileset->TileHeight());
                                DIconTileset->DrawTile(surface, HorizontalOffset, VerticalOffset, DAssetIndices[to_underlying(Command.DAssetTarget->Type())], to_underlying(Command.DAssetTarget->Color()) ? to_underlying(Command.DAssetTarget->Color()) - 1 : 0);
                                /*
                                gdk_gc_set_rgb_fg_color(TempGC, &DHealthRectangleFG);
                                gdk_gc_set_rgb_bg_color(TempGC, &DHealthRectangleBG);
                                gdk_draw_rectangle(surface, TRUE, HorizontalOffset - DBevel->Width(), VerticalOffset + DIconTileset->TileHeight() + DBevel->Width() * 3,  DIconTileset->TileWidth() + DBevel->Width() * 2, HEALTH_HEIGHT + 2);
                                
                                gdk_gc_set_rgb_fg_color(TempGC, &DHealthColors[HPColor]);
                                gdk_gc_set_rgb_bg_color(TempGC, &DHealthColors[HPColor]);
                                gdk_draw_rectangle(surface, TRUE, HorizontalOffset - DBevel->Width() + 1, VerticalOffset + DIconTileset->TileHeight() + DBevel->Width() * 3 + 1,  (DIconTileset->TileWidth() + DBevel->Width() * 2 - 2) * Command.DAssetTarget->HitPoints()/ Command.DAssetTarget->MaxHitPoints() , HEALTH_HEIGHT);
                                
                                TempString = std::to_string(Command.DAssetTarget->HitPoints()) + std::string(" / ") + std::to_string(Command.DAssetTarget->MaxHitPoints());
                                DFonts[to_underlying(EFontSize::Small)]->MeasureText(TempString, TextWidth, TextHeight);
                                
                                TextTop = VerticalOffset + DIconTileset->TileHeight() + DBevel->Width() * 4 + HEALTH_HEIGHT + 2;
                                DFonts[to_underlying(EFontSize::Small)]->DrawTextWithShadow(surface, 
                                    HorizontalOffset + (DIconTileset->TileWidth()/2 + DBevel->Width()) - TextWidth/2, 
                                    TextTop, 
                                    DFontColorIndices[to_underlying(EFontSize::Small)][FG_COLOR], DFontColorIndices[to_underlying(EFontSize::Small)][BG_COLOR], 1, TempString);
                                
                                */
                                TempString = "Training: ";
                                DFonts[to_underlying(EFontSize::Medium)]->MeasureText(TempString, TextWidth, TextHeight);
                                DFonts[to_underlying(EFontSize::Medium)]->DrawTextWithShadow(surface, 
                                    HorizontalOffset - TextWidth - DBevel->Width(), 
                                    (VerticalOffset + DIconTileset->TileHeight()/2) - TextHeight/2, 
                                    DFontColorIndices[to_underlying(EFontSize::Medium)][FG_COLOR], DFontColorIndices[to_underlying(EFontSize::Medium)][BG_COLOR], 1, TempString);
                            }
                            else{
                                int HorizontalOffset = DBevel->Width(), VerticalOffset = DBevel->Width();
                                
                                HorizontalOffset += 2 * (DFullIconWidth + HorizontalGap);
                                VerticalOffset += DFullIconHeight + VerticalGap;
                                
                                DBevel->DrawBevel(surface, HorizontalOffset, VerticalOffset, DIconTileset->TileWidth(), DIconTileset->TileHeight());
                                DIconTileset->DrawTile(surface, HorizontalOffset, VerticalOffset, DResearchIndices[to_underlying(Command.DCapability)], to_underlying(Asset->Color()) ? to_underlying(Asset->Color()) - 1 : 0);

                                TempString = "Researching: ";
                                DFonts[to_underlying(EFontSize::Medium)]->MeasureText(TempString, TextWidth, TextHeight);
                                DFonts[to_underlying(EFontSize::Medium)]->DrawTextWithShadow(surface, 
                                    HorizontalOffset - TextWidth - DBevel->Width(), 
                                    (VerticalOffset + DIconTileset->TileHeight()/2) - TextHeight/2, 
                                    DFontColorIndices[to_underlying(EFontSize::Medium)][FG_COLOR], DFontColorIndices[to_underlying(EFontSize::Medium)][BG_COLOR], 1, TempString);
                            }
                            DrawCompletionBar(surface, PercentComplete);
                        }
                    }
                }
                else{
                    if(EAssetType::GoldMine == Asset->Type()){
                        int TextLineHeight;
                        
                        TextTop = DIconTileset->TileHeight() + DBevel->Width() * 4 + HEALTH_HEIGHT + 2;
                        
                        TempString = "Gold: ";
                        DFonts[to_underlying(EFontSize::Medium)]->MeasureText(TempString, TextWidth, TextHeight);
                        TextLineHeight = TextHeight;
                        DFonts[to_underlying(EFontSize::Medium)]->DrawTextWithShadow(surface, 
                            TextCenter - TextWidth, 
                            TextTop, 
                            DFontColorIndices[to_underlying(EFontSize::Medium)][FG_COLOR], DFontColorIndices[to_underlying(EFontSize::Medium)][BG_COLOR], 1, TempString);
                        
                        TempString = CTextFormatter::IntegerToPrettyString(Asset->Gold());
                        DFonts[to_underlying(EFontSize::Medium)]->DrawTextWithShadow(surface, 
                            TextCenter, 
                            TextTop, 
                            DFontColorIndices[to_underlying(EFontSize::Medium)][FG_COLOR], DFontColorIndices[to_underlying(EFontSize::Medium)][BG_COLOR], 1, TempString);
                    }
                }
            }
        }
        else{
            DDisplayedIcons = 0;
            int HorizontalOffset = DBevel->Width(), VerticalOffset = DBevel->Width();
            for(auto &Item : selectionlist){
                if(auto Asset = Item.lock()){
                    int HPColor = (Asset->HitPoints() - 1) * MAX_HP_COLOR / Asset->MaxHitPoints();
                    int TextWidth, TextHeight;
                    int TextTop;
                    std::string TempString;
                    
                    DBevel->DrawBevel(surface, HorizontalOffset, VerticalOffset, DIconTileset->TileWidth(), DIconTileset->TileHeight());
                    DIconTileset->DrawTile(surface, HorizontalOffset, VerticalOffset, DAssetIndices[to_underlying(Asset->Type())], to_underlying(Asset->Color()) ? to_underlying(Asset->Color()) - 1 : 0);
                    
                    ResoureContext->SetSourceRGB(DHealthRectangleFG);
                    ResoureContext->Rectangle(HorizontalOffset - DBevel->Width(), VerticalOffset + DIconTileset->TileHeight() + DBevel->Width() * 3,  DIconTileset->TileWidth() + DBevel->Width() * 2, HEALTH_HEIGHT + 2);
                    ResoureContext->Fill();
                    /*
                    gdk_gc_set_rgb_fg_color(TempGC, &DHealthRectangleFG);
                    gdk_gc_set_rgb_bg_color(TempGC, &DHealthRectangleBG);
                    gdk_draw_rectangle(surface, TRUE, HorizontalOffset - DBevel->Width(), VerticalOffset + DIconTileset->TileHeight() + DBevel->Width() * 3,  DIconTileset->TileWidth() + DBevel->Width() * 2, HEALTH_HEIGHT + 2);
                    */
                    
                    ResoureContext->SetSourceRGB(DHealthColors[HPColor]);
                    ResoureContext->Rectangle(HorizontalOffset - DBevel->Width() + 1, VerticalOffset + DIconTileset->TileHeight() + DBevel->Width() * 3 + 1,  (DIconTileset->TileWidth() + DBevel->Width() * 2 - 2) * Asset->HitPoints()/ Asset->MaxHitPoints() , HEALTH_HEIGHT);
                    ResoureContext->Fill();
                    /*
                    gdk_gc_set_rgb_fg_color(TempGC, &DHealthColors[HPColor]);
                    gdk_gc_set_rgb_bg_color(TempGC, &DHealthColors[HPColor]);
                    gdk_draw_rectangle(surface, TRUE, HorizontalOffset - DBevel->Width() + 1, VerticalOffset + DIconTileset->TileHeight() + DBevel->Width() * 3 + 1,  (DIconTileset->TileWidth() + DBevel->Width() * 2 - 2) * Asset->HitPoints()/ Asset->MaxHitPoints() , HEALTH_HEIGHT);
                    */
                    TempString = std::to_string(Asset->HitPoints()) + std::string(" / ") + std::to_string(Asset->MaxHitPoints());
                    DFonts[to_underlying(EFontSize::Small)]->MeasureText(TempString, TextWidth, TextHeight);
                    
                    TextTop = VerticalOffset + DIconTileset->TileHeight() + DBevel->Width() * 4 + HEALTH_HEIGHT + 2;
                    DFonts[to_underlying(EFontSize::Small)]->DrawTextWithShadow(surface, 
                        HorizontalOffset + (DIconTileset->TileWidth()/2 + DBevel->Width()) - TextWidth/2, 
                        TextTop, 
                        DFontColorIndices[to_underlying(EFontSize::Small)][FG_COLOR], DFontColorIndices[to_underlying(EFontSize::Small)][BG_COLOR], 1, TempString);
                    
                    
                    HorizontalOffset += DFullIconWidth + HorizontalGap;
                    DDisplayedIcons++;
                    if(0 == (DDisplayedIcons % HorizontalIcons)){
                        HorizontalOffset = DBevel->Width();
                        VerticalOffset +=  DFullIconHeight + VerticalGap;
                    }
                }
            }
        }
    }
}

