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
#include "ResourceRenderer.h"
#include "TextFormatter.h"
#include "Debug.h"

enum class EMiniIconTypes{
    Gold = 0,
    Lumber,
    Food,
    Oil,
    Stone,
    Max
};

        
CResourceRenderer::CResourceRenderer(std::shared_ptr< CGraphicTileset > icons, std::shared_ptr< CFontTileset > font, std::shared_ptr< CPlayerData > player){
    int Width;
    
    DIconTileset = icons;
    DFont = font;
    DPlayer = player;
    DForegroundColor = DFont->FindColor("white");
    DBackgroundColor = DFont->FindColor("black");
    DInsufficientColor = DFont->FindColor("red");
    DLastGoldDisplay = 0;
    DLastLumberDisplay = 0;
    DLastStoneDisplay = 0;
    
    DIconIndices.resize(to_underlying(EMiniIconTypes::Max));
    DIconIndices[to_underlying(EMiniIconTypes::Gold)] = DIconTileset->FindTile("gold");
    DIconIndices[to_underlying(EMiniIconTypes::Lumber)] = DIconTileset->FindTile("lumber");
    DIconIndices[to_underlying(EMiniIconTypes::Food)] = DIconTileset->FindTile("food");
    DIconIndices[to_underlying(EMiniIconTypes::Stone)] = DIconTileset->FindTile("stone");
    DFont->MeasureText("0123456789", Width, DTextHeight);
}

CResourceRenderer::~CResourceRenderer(){

}

void CResourceRenderer::DrawResources(std::shared_ptr< CGraphicSurface > surface){
    int Width, Height;
    int TextYOffset, ImageYOffset;
    int WidthSeparation, XOffset;
    int DeltaGold = DPlayer->Gold() - DLastGoldDisplay;
    int DeltaLumber = DPlayer->Lumber() - DLastLumberDisplay;
    int DeltaStone = DPlayer->Stone() - DLastStoneDisplay;
    
    DeltaGold /= 5;
    if((-3 < DeltaGold) && (3 > DeltaGold)){
        DLastGoldDisplay = DPlayer->Gold();
    }
    else{
        DLastGoldDisplay += DeltaGold; 
    }
    DeltaLumber /= 5;
    if((-3 < DeltaLumber) && (3 > DeltaLumber)){
        DLastLumberDisplay = DPlayer->Lumber();
    }
    else{
        DLastLumberDisplay += DeltaLumber; 
    }
    DeltaStone /= 5;
    if((-3 < DeltaStone) && (3 > DeltaStone)){
        DLastStoneDisplay = DPlayer->Stone();
    }
    else{
        DLastStoneDisplay += DeltaStone;
    }
    Width = surface->Width();
    Height = surface->Height();
    TextYOffset = Height/2 - DTextHeight/2;
    ImageYOffset = Height/2 - DIconTileset->TileHeight()/2;
    WidthSeparation = Width/4;
    XOffset = Width / 8;
    
    DIconTileset->DrawTile(surface, XOffset, ImageYOffset, DIconIndices[to_underlying(EMiniIconTypes::Gold)]);
    DFont->DrawTextWithShadow(surface, XOffset +  DIconTileset->TileWidth(), TextYOffset, DForegroundColor, DBackgroundColor, 1, std::string(" ") + CTextFormatter::IntegerToPrettyString(DLastGoldDisplay));
    XOffset += WidthSeparation;
    
    DIconTileset->DrawTile(surface, XOffset, ImageYOffset, DIconIndices[to_underlying(EMiniIconTypes::Lumber)]);
    DFont->DrawTextWithShadow(surface, XOffset +  DIconTileset->TileWidth(), TextYOffset, DForegroundColor, DBackgroundColor, 1, std::string(" ") + CTextFormatter::IntegerToPrettyString(DLastLumberDisplay));
    XOffset += WidthSeparation;
    
    DIconTileset->DrawTile(surface, XOffset, ImageYOffset, DIconIndices[to_underlying(EMiniIconTypes::Food)]);
    
    if(DPlayer->FoodConsumption() > DPlayer->FoodProduction()){
        int SecondTextWidth, TotalTextWidth, TextHeight;
        //DFont->MeasureText( std::string(" ") + std::to_string(DPlayer->FoodConsumption()), FirstTextWidth, TextHeight);
        DFont->MeasureText( std::string(" / ") + std::to_string(DPlayer->FoodProduction()), SecondTextWidth, TextHeight);
        DFont->MeasureText( std::string(" ") + std::to_string(DPlayer->FoodConsumption()) + std::string(" / ") + std::to_string(DPlayer->FoodProduction()), TotalTextWidth, TextHeight);
        DFont->DrawTextWithShadow(surface, XOffset +  DIconTileset->TileWidth(), TextYOffset, DInsufficientColor, DBackgroundColor, 1, std::string(" ") + std::to_string(DPlayer->FoodConsumption()));
        DFont->DrawTextWithShadow(surface, XOffset +  DIconTileset->TileWidth() + TotalTextWidth - SecondTextWidth, TextYOffset, DForegroundColor, DBackgroundColor, 1, std::string(" / ") + std::to_string(DPlayer->FoodProduction()));
    }
    else{
        DFont->DrawTextWithShadow(surface, XOffset +  DIconTileset->TileWidth(), TextYOffset, DForegroundColor, DBackgroundColor, 1, std::string(" ") + std::to_string(DPlayer->FoodConsumption()) + std::string(" / ") + std::to_string(DPlayer->FoodProduction()));   
    }
    XOffset += WidthSeparation;    

    DIconTileset->DrawTile(surface, XOffset, ImageYOffset, DIconIndices[to_underlying(EMiniIconTypes::Stone)]);
    DFont->DrawTextWithShadow(surface, XOffset +  DIconTileset->TileWidth(), TextYOffset, DForegroundColor, DBackgroundColor, 1, std::string(" ") + CTextFormatter::IntegerToPrettyString(DLastStoneDisplay));
}

