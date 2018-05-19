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
/**
*
*@class CGameSelectionMode
*
*@brief This class provides text and functionality for the buttons available in the Map Selection Mode.
*       The buttons include:
*
*                            Select:- When a map is selected, the player types (human or AI) are loaded into the game.
*                                     As player 1 is always human, the first player is set to human. After this,
*                                     The game session type (multiplayer or singleplayer) is checked, and depending on this,
*                                     The other players are assigned as either also humans (if multiplayer), or AI (if singleplayer).
*
*
*                            Cancel:- When the cancel button is pressed, if the game would have been multiplayer, the application mode is changed back to
*                                     the multiplayer options menu. If the game would have been singleplayer, the application mode is changed back to the
*                                     main menu mode.
*
* @note Other member functions include InitializeChange, Input, Calculate, and Render.
*
* @author Greg
*
* @version 1.1
*
* @date 10/11/2017 00:15:12
*
* Contact: gjdost@ucdavis.edu
*
*/

#include "GameSelectionMode.h"
#include "ApplicationData.h"
#include "MainMenuMode.h"
#include "MultiPlayerOptionsMenuMode.h"
#include "MemoryDataSource.h"
#include "PlayerAIColorSelectMode.h"
#include "HostGameOptionsMode.h"

std::shared_ptr< CGameSelectionMode > CGameSelectionMode::DGameSelectionModePointer;

CGameSelectionMode::CGameSelectionMode(const SPrivateConstructorType & key){
    DTitle = "Select Game";

    DButtonTexts.push_back("Select");
    DButtonFunctions.push_back(SelectMapButtonCallback);
    DButtonTexts.push_back("Join Private");
    DButtonFunctions.push_back(JoinPrivateButtonCallback);
    DButtonTexts.push_back("Host Game");
    DButtonFunctions.push_back(HostGameButtonCallback);
    DButtonTexts.push_back("Cancel");
    DButtonFunctions.push_back(BackButtonCallback);
}


void CGameSelectionMode::JoinPrivateButtonCallback(std::shared_ptr< CApplicationData > context){
}


void CGameSelectionMode::HostGameButtonCallback(std::shared_ptr< CApplicationData > context){
    context->ChangeApplicationMode(CHostGameOptionsMode::Instance());
}


/**
*
*Provides functionality for the "Select" button. It uses information on whether or not this is a multiplayer game to load each selected player slot as either a human or AI.
*After doing so, it changes the application mode to Color Select Mode. (See CPlayerAIColorSelectMode)
*
*@param[in] context The current game state.
*
*@return Nothing.
*/
void CGameSelectionMode::SelectMapButtonCallback(std::shared_ptr< CApplicationData > context){
    for(int Index = 0; Index < to_underlying(EPlayerColor::Max); Index++){
        context->DLoadingPlayerTypes[Index] = CApplicationData::ptNone;
        if(Index){
            if(1 == Index){
                context->DLoadingPlayerTypes[Index] = CApplicationData::ptHuman;
            }
            else if(Index <= context->DSelectedMap->PlayerCount()){
                context->DLoadingPlayerTypes[Index] = CApplicationData::gstMultiPlayerHost == context->DGameSessionType ?  CApplicationData::ptHuman : CApplicationData::ptAIEasy;
            }
        }
    }
    context->DPlayerColor = static_cast<EPlayerColor>(1);

    // context->ChangeApplicationMode(CPlayerAIColorSelectMode::Instance());
    // context->ChangeApplicationMode(CHostGameOptionsMode::Instance());
    if(CApplicationData::gstMultiPlayerHost == context->DGameSessionType){
        context->ChangeApplicationMode(CHostGameOptionsMode::Instance());
    }
    else{
        context->ChangeApplicationMode(CPlayerAIColorSelectMode::Instance());
    }
}
/**
*
*Provides functionality for the "Back" button. If the game lobby was a multiplayer, the application mode is changed to the multiplayer options menu (See CMultiPlayerOptionsMenuMode).
*If the game was singleplayer, the application mode is changed to the main menu (See CMainMenuMode).
*
*@param[in] context The current game state.
*
*@return Nothing.
*
*/
void CGameSelectionMode::BackButtonCallback(std::shared_ptr< CApplicationData > context){
    if(CApplicationData::gstMultiPlayerHost == context->DGameSessionType){
        context->ChangeApplicationMode(CMultiPlayerOptionsMenuMode::Instance());
    }
    else{
        context->ChangeApplicationMode(CMainMenuMode::Instance());
    }
}
/**
*
*Gives initial values for the settings in Map Selection.
*
*@param[in] context The current game state.
*
*@return Nothing.
*
*/
void CGameSelectionMode::InitializeChange(std::shared_ptr< CApplicationData > context){
    DButtonHovered = false;
    DMapOffset = 0;

    context->ResetPlayerColors();
    context->DSelectedMapIndex = 0;
    context->DSelectedMap = CAssetDecoratedMap::DuplicateMap(0,context->DLoadingPlayerColors);
    context->DMapRenderer = std::make_shared< CMapRenderer >(std::make_shared< CMemoryDataSource >(context->DMapRendererConfigurationData), context->DTerrainTileset, context->DSelectedMap);
    context->DAssetRenderer = std::make_shared< CAssetRenderer >(context->DAssetRecolorMap, context->DAssetTilesets, context->DMarkerTileset, context->DCorpseTileset, context->DFireTilesets, context->DBuildingDeathTileset, context->DArrowTileset, nullptr, context->DSelectedMap);
    context->DMiniMapRenderer = std::make_shared< CMiniMapRenderer >(context->DMapRenderer, context->DAssetRenderer, nullptr, nullptr, context->DDoubleBufferSurface->Format() );
}

/**
*
*This function enacts any settings changes that the player makes by taking user input and performing different actions depending on the setting they're changing.
*
*@param[in] context The current state of the game.
*
*@return Nothing.
*
*/
void CGameSelectionMode::Input(std::shared_ptr< CApplicationData > context){
    int CurrentX, CurrentY;
    CurrentX = context->DCurrentX;
    CurrentY = context->DCurrentY;
    if(context->DLeftClick && !context->DLeftDown){
        int ItemSelected = context->DMapSelectListViewRenderer->ItemAt(CurrentX - context->DMapSelectListViewXOffset, CurrentY - context->DMapSelectListViewYOffset);

        if(to_underlying(CListViewRenderer::EListViewObject::UpArrow) == ItemSelected){
            if(DMapOffset){
                DMapOffset--;
            }
        }
        else if(to_underlying(CListViewRenderer::EListViewObject::DownArrow) == ItemSelected){
            DMapOffset++;
        }
        else if(to_underlying(CListViewRenderer::EListViewObject::None) != ItemSelected){
            if(context->DSelectedMapIndex != ItemSelected){
                context->DSelectedMapIndex = ItemSelected;

                *context->DSelectedMap = *CAssetDecoratedMap::GetMap(context->DSelectedMapIndex);
            }
        }
        else{
            for(int Index = 0; Index < DButtonLocations.size(); Index++){
                if((DButtonLocations[Index].DXPosition <= CurrentX)&&((DButtonLocations[Index].DXPosition + DButtonLocations[Index].DWidth > CurrentX))){
                    if((DButtonLocations[Index].DYPosition <= CurrentY)&&((DButtonLocations[Index].DYPosition + DButtonLocations[Index].DHeight > CurrentY))){
                        DButtonFunctions[Index](context);
                    }
                }
            }
        }
    }
}

/**
*
*This function is not yet implemented.
*
*@param[in] context The current game state.
*
*@return Nothing.
*
*/
void CGameSelectionMode::Calculate(std::shared_ptr< CApplicationData > context){

}
/**
*
*This function takes the map settings and renders a game map depending on them.
*
*@param[in] context The current game state.
*
*@return Nothing.
*
*/
void CGameSelectionMode::Render(std::shared_ptr< CApplicationData > context){
    int CurrentX, CurrentY;
    int BufferWidth, BufferHeight;
    int MiniMapWidth, MiniMapHeight, MiniMapCenter, MiniMapLeft;
    int ListViewWidth = 0, ListViewHeight = 0;
    int TitleHeight;
    int TextWidth, TextHeight, TextTop;
    int TextColor, ShadowColor;
    std::string TempString;
    CButtonRenderer::EButtonState ButtonState = CButtonRenderer::EButtonState::None;
    bool ButtonXAlign = false, ButtonHovered = false;

    CurrentX = context->DCurrentX;
    CurrentY = context->DCurrentY;
    context->RenderMenuTitle(DTitle, TitleHeight, BufferWidth, BufferHeight);

    MiniMapWidth = context->DMiniMapSurface->Width();
    MiniMapHeight = context->DMiniMapSurface->Height();
    if(nullptr != context->DMapSelectListViewSurface){
        ListViewWidth = context->DMapSelectListViewSurface->Width();
        ListViewHeight = context->DMapSelectListViewSurface->Height();
    }

    if((ListViewHeight != (BufferHeight - TitleHeight - context->DInnerBevel->Width() - context->DBorderWidth))||(ListViewWidth != (BufferWidth - context->DViewportXOffset - context->DBorderWidth - context->DInnerBevel->Width() * 2))){
        if(nullptr != context->DMapSelectListViewSurface){
            context->DMapSelectListViewSurface = nullptr;
        }
        ListViewHeight = BufferHeight - TitleHeight - context->DInnerBevel->Width() - context->DBorderWidth;
        ListViewWidth = BufferWidth - context->DViewportXOffset - context->DBorderWidth - context->DInnerBevel->Width() * 2;
        context->DMapSelectListViewSurface = CGraphicFactory::CreateSurface(ListViewWidth, ListViewHeight, context->DDoubleBufferSurface->Format());
    }

    context->DMapSelectListViewXOffset = context->DBorderWidth;
    context->DMapSelectListViewYOffset = TitleHeight + context->DInnerBevel->Width();

    context->DMapSelectListViewSurface->Draw(context->DWorkingBufferSurface, 0, 0, ListViewWidth, ListViewHeight, context->DMapSelectListViewXOffset, context->DMapSelectListViewYOffset);

    std::vector< std::string > MapNames;
    while(CAssetDecoratedMap::GetMap(MapNames.size())){
        MapNames.push_back(CAssetDecoratedMap::GetMap(MapNames.size())->MapName());
    }


    context->DMapSelectListViewRenderer->DrawListView(context->DMapSelectListViewSurface, context->DSelectedMapIndex, DMapOffset, MapNames);
    context->DWorkingBufferSurface->Draw(context->DMapSelectListViewSurface, context->DMapSelectListViewXOffset, context->DMapSelectListViewYOffset, ListViewWidth, ListViewHeight, 0, 0);
    context->DInnerBevel->DrawBevel(context->DWorkingBufferSurface, context->DMapSelectListViewXOffset, context->DMapSelectListViewYOffset, ListViewWidth, ListViewHeight);

    context->DMiniMapRenderer->DrawMiniMap(context->DMiniMapSurface);
    MiniMapLeft = context->DMapSelectListViewXOffset + ListViewWidth + context->DInnerBevel->Width() * 4;
    context->DWorkingBufferSurface->Draw(context->DMiniMapSurface, MiniMapLeft, context->DMapSelectListViewYOffset, -1, -1, 0, 0);
    context->DInnerBevel->DrawBevel(context->DWorkingBufferSurface, MiniMapLeft, context->DMapSelectListViewYOffset, MiniMapWidth, MiniMapHeight);

    TextTop = context->DMapSelectListViewYOffset + MiniMapHeight + context->DInnerBevel->Width() * 2;
    MiniMapCenter = MiniMapLeft + MiniMapWidth / 2;
    TextColor = context->DFonts[to_underlying(CUnitDescriptionRenderer::EFontSize::Large)]->FindColor("white");
    ShadowColor = context->DFonts[to_underlying(CUnitDescriptionRenderer::EFontSize::Large)]->FindColor("black");

    TempString = std::to_string(context->DSelectedMap->PlayerCount()) + " Players";
    context->DFonts[to_underlying(CUnitDescriptionRenderer::EFontSize::Large)]->MeasureText(TempString, TextWidth, TextHeight);
    context->DFonts[to_underlying(CUnitDescriptionRenderer::EFontSize::Large)]->DrawTextWithShadow(context->DWorkingBufferSurface, MiniMapCenter - TextWidth/2, TextTop, TextColor, ShadowColor, 1, TempString);
    TextTop += TextHeight;
    TempString = std::to_string(context->DSelectedMap->Width()) + " x " + std::to_string(context->DSelectedMap->Height());
    context->DFonts[to_underlying(CUnitDescriptionRenderer::EFontSize::Large)]->MeasureText(TempString, TextWidth, TextHeight);
    context->DFonts[to_underlying(CUnitDescriptionRenderer::EFontSize::Large)]->DrawTextWithShadow(context->DWorkingBufferSurface, MiniMapCenter - TextWidth/2, TextTop, TextColor, ShadowColor, 1, TempString);
    TextTop += TextHeight * 3 / 2;

    DButtonLocations.resize(4);
    if((MiniMapLeft <= CurrentX)&&(MiniMapLeft + MiniMapWidth > CurrentX)){
        ButtonXAlign = true;
    }

    context->DButtonRenderer->Text(DButtonTexts[0], true);
    context->DButtonRenderer->Height(context->DButtonRenderer->Height() * 3 / 2);
    context->DButtonRenderer->Width(MiniMapWidth);

    TextTop = BufferHeight - context->DBorderWidth - (context->DButtonRenderer->Height() * 6);
    if(ButtonXAlign){
        if((TextTop <= CurrentY)&&((TextTop + context->DButtonRenderer->Height() > CurrentY))){
            ButtonState = context->DLeftDown ? CButtonRenderer::EButtonState::Pressed : CButtonRenderer::EButtonState::Hover;
            ButtonHovered = true;
        }
    }
    context->DButtonRenderer->DrawButton(context->DWorkingBufferSurface, MiniMapLeft, TextTop, ButtonState);
    DButtonLocations[0] = SRectangle({MiniMapLeft, TextTop, context->DButtonRenderer->Width(), context->DButtonRenderer->Height()});

    TextTop = TextTop + context->DButtonRenderer->Height() * 1.5;
    ButtonState = CButtonRenderer::EButtonState::None;
    if(ButtonXAlign){
        if((TextTop <= CurrentY)&&((TextTop + context->DButtonRenderer->Height() > CurrentY))){
            ButtonState = context->DLeftDown ? CButtonRenderer::EButtonState::Pressed : CButtonRenderer::EButtonState::Hover;
            ButtonHovered = true;
        }
    }
    context->DButtonRenderer->Text(DButtonTexts[1]);
    context->DButtonRenderer->DrawButton(context->DWorkingBufferSurface, MiniMapLeft, TextTop, ButtonState);
    DButtonLocations[1] = SRectangle({MiniMapLeft, TextTop, context->DButtonRenderer->Width(), context->DButtonRenderer->Height()});

    TextTop = TextTop + context->DButtonRenderer->Height() * 1.5;
    context->DButtonRenderer->Text(DButtonTexts[2]);
    context->DButtonRenderer->DrawButton(context->DWorkingBufferSurface, MiniMapLeft, TextTop, ButtonState);
    DButtonLocations[2] = SRectangle({MiniMapLeft, TextTop, context->DButtonRenderer->Width(), context->DButtonRenderer->Height()});

    TextTop = TextTop + context->DButtonRenderer->Height() * 1.5;
    context->DButtonRenderer->Text(DButtonTexts[3]);
    context->DButtonRenderer->DrawButton(context->DWorkingBufferSurface, MiniMapLeft, TextTop, ButtonState);
    DButtonLocations[3] = SRectangle({MiniMapLeft, TextTop, context->DButtonRenderer->Width(), context->DButtonRenderer->Height()});

    if(!DButtonHovered && ButtonHovered){
        context->DSoundLibraryMixer->PlayClip(context->DSoundLibraryMixer->FindClip("tick"), context->DSoundVolume, 0.0);
    }
    if(context->ModeIsChanging()){
        context->DSoundLibraryMixer->PlayClip(context->DSoundLibraryMixer->FindClip("place"), context->DSoundVolume, 0.0);
    }
    DButtonHovered = ButtonHovered;
}

std::shared_ptr< CApplicationMode > CGameSelectionMode::Instance(){
    if(DGameSelectionModePointer == nullptr){
        DGameSelectionModePointer = std::make_shared< CGameSelectionMode >(SPrivateConstructorType());
    }
    return DGameSelectionModePointer;
}
