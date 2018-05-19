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
* @class CPlayerAIColorSelectMode
*
* @brief
*   This class handles the mode and screen for customizing players/opponents (the difficulties of the AIs and the color
*   they appear as in battlemode). If "Play Game" is selected, the mode is changed to Battle Mode.
*   The remainder of the class takes care of conditions for Multiplayer (users cannot change the difficulty or 
*   color of other human players) and the rendering of the screen including buttons, titles, and the minimap.
*
* @author $Author: Spencer $
*
* @version $Revision: Revision 5.0 $
*
* @date $Date: 11/6/2017 $
*
* Contact: savandyke@ucdavis.edu
*
*/

#include "PlayerAIColorSelectMode.h"
#include "ApplicationData.h"
#include "MainMenuMode.h"
#include "MultiPlayerOptionsMenuMode.h"
#include "MapSelectionMode.h"
#include "MemoryDataSource.h"
#include "BattleMode.h"

std::shared_ptr< CPlayerAIColorSelectMode > CPlayerAIColorSelectMode::DPlayerAIColorSelectModePointer;

/**
*  Constructor. Options for selecting AI (colors and difficulty) and playing or cancelling the game
*
*  @param[in] key Constructor
*
*/
CPlayerAIColorSelectMode::CPlayerAIColorSelectMode(const SPrivateConstructorType & key){   
    DTitle = "Select Colors/Difficulty";     
    
    DButtonTexts.push_back("Play Game");
    DButtonFunctions.push_back(PlayGameButtonCallback);
    DButtonTexts.push_back("Cancel");
    DButtonFunctions.push_back(BackButtonCallback);
}

/**
* When the "Play Game" button is pressed, the game mode switches to Battle Mode
*
* @param[in] context Shared pointer to CApplicationData
*
*/
void CPlayerAIColorSelectMode::PlayGameButtonCallback(std::shared_ptr< CApplicationData > context){
	/* Set player color here so that the entries in the color selection window don't become jumbled
	 * Note: Always associates player with Player 1 starting location, leave as is or change this?
	 */
	CPlayerAIColorSelectMode::SetPlayerColor(context, context->DLoadingPlayerColors[1]);
    context->ChangeApplicationMode(CBattleMode::Instance());
}

/**
 * Sets the player color value to the color specified by the player
 *
 * @param[in] context Shared pointer to CApplicationData instance
 * @param[in] color New color value for the player
 *
 */
void CPlayerAIColorSelectMode::SetPlayerColor(std::shared_ptr < CApplicationData > context, EPlayerColor color){
	context->DPlayerColor = color;
}

/**
* If the user is in Multiplayer, the game mode switches to Multiplayer Options Menu Mode. Otherwise, it switches to Map Selection Mode
*
* @param[in] context Shared pointer to CApplicationData
*
*/
void CPlayerAIColorSelectMode::BackButtonCallback(std::shared_ptr< CApplicationData > context){
    if(CApplicationData::gstMultiPlayerClient == context->DGameSessionType){
        context->ChangeApplicationMode(CMultiPlayerOptionsMenuMode::Instance());
    }
    else{
        context->ChangeApplicationMode(CMapSelectionMode::Instance());
    }
}

/**
* Sets or resets settings on the Player AI Color Select Mode (where colors and difficulties of AI are chosen) to their defaults
*
* @param[in] context Shared pointer to CApplicationData
*
*/
void CPlayerAIColorSelectMode::InitializeChange(std::shared_ptr< CApplicationData > context){   
    DButtonHovered = false;
    DButtonLocations.clear();
    DColorButtonLocations.clear();
    DPlayerTypeButtonLocations.clear();
}

/**
* Handles input for selecting the colors of players.
*
*
* @param[in] context Shared pointer to CApplicationData
*
*/
void CPlayerAIColorSelectMode::Input(std::shared_ptr< CApplicationData > context){
    int CurrentX, CurrentY;
    CurrentX = context->DCurrentX;
    CurrentY = context->DCurrentY;
    DPlayerColorRequestingChange = EPlayerColor::None;
    DPlayerColorChangeRequest = EPlayerColor::None;
    DPlayerColorRequesTypeChange = EPlayerColor::None;
    if(context->DLeftClick && !context->DLeftDown){
        for(int Index = 0; Index < DColorButtonLocations.size(); Index++){
            if(DColorButtonLocations[Index].PointInside(CurrentX, CurrentY)){
                int PlayerSelecting = 1 + (Index / (to_underlying(EPlayerColor::Max) - 1));
                int ColorSelecting = 1 + (Index % (to_underlying(EPlayerColor::Max) - 1));
                if((PlayerSelecting == to_underlying(context->DPlayerColor))||(CApplicationData::gstMultiPlayerClient != context->DGameSessionType)){
                    if((PlayerSelecting == to_underlying(context->DPlayerColor))||(CApplicationData::ptHuman != context->DLoadingPlayerTypes[PlayerSelecting])){
                        DPlayerColorRequestingChange = static_cast<EPlayerColor>(PlayerSelecting);
                        DPlayerColorChangeRequest = static_cast<EPlayerColor>(ColorSelecting);
                    }
                }
            } 
        }
        for(int Index = 0; Index < DButtonLocations.size(); Index++){
            if(DButtonLocations[Index].PointInside(CurrentX, CurrentY)){
                DButtonFunctions[Index](context);
            }
        }
        for(int Index = 0; Index < DPlayerTypeButtonLocations.size(); Index++){
            if(DPlayerTypeButtonLocations[Index].PointInside(CurrentX, CurrentY)){
                DPlayerColorRequesTypeChange = static_cast<EPlayerColor>(Index+2);
                break;
            }
            
        }
    }
}

/**
* Cycles through difficulties (or "Closed" or "Human" when in Multiplayer) when the appropriate button is pressed
*
* @param[in] context Shared pointer to CApplicationData
*
*/
void CPlayerAIColorSelectMode::Calculate(std::shared_ptr< CApplicationData > context){   
    if(EPlayerColor::None != DPlayerColorRequestingChange){
        EPlayerColor NewColorInUse = EPlayerColor::None;
        for(int Index = 1; Index < to_underlying(EPlayerColor::Max); Index++){
            if(Index != to_underlying(DPlayerColorRequestingChange)){
                if(CApplicationData::ptNone != context->DLoadingPlayerTypes[Index]){
                    if(context->DLoadingPlayerColors[Index] == DPlayerColorChangeRequest){
                        NewColorInUse = static_cast<EPlayerColor>(Index);
                        break;
                    }
                }
            }
        }
        if(EPlayerColor::None != NewColorInUse){
            context->DLoadingPlayerColors[to_underlying(NewColorInUse)] = context->DLoadingPlayerColors[to_underlying(DPlayerColorRequestingChange)];
        }
        context->DLoadingPlayerColors[to_underlying(DPlayerColorRequestingChange)] = DPlayerColorChangeRequest;
                                    
        *context->DSelectedMap = *CAssetDecoratedMap::DuplicateMap(context->DSelectedMapIndex,context->DLoadingPlayerColors);
    }
    if(EPlayerColor::None != DPlayerColorRequesTypeChange){
        if(CApplicationData::gstSinglePlayer == context->DGameSessionType){
            switch(context->DLoadingPlayerTypes[to_underlying(DPlayerColorRequesTypeChange)]){
                case CApplicationData::ptAIEasy:    context->DLoadingPlayerTypes[to_underlying(DPlayerColorRequesTypeChange)] = CApplicationData::ptAIMedium;
                                                    break;
                case CApplicationData::ptAIMedium:  context->DLoadingPlayerTypes[to_underlying(DPlayerColorRequesTypeChange)] = CApplicationData::ptAIHard;
                                                    break;
                default:                            context->DLoadingPlayerTypes[to_underlying(DPlayerColorRequesTypeChange)] = CApplicationData::ptAIEasy;
                                                    break;
            }
        }
        else if(CApplicationData::gstMultiPlayerHost == context->DGameSessionType){
            switch(context->DLoadingPlayerTypes[to_underlying(DPlayerColorRequesTypeChange)]){
                case CApplicationData::ptHuman:     context->DLoadingPlayerTypes[to_underlying(DPlayerColorRequesTypeChange)] = CApplicationData::ptAIEasy;
                                                    break;
                case CApplicationData::ptAIEasy:    context->DLoadingPlayerTypes[to_underlying(DPlayerColorRequesTypeChange)] = CApplicationData::ptAIMedium;
                                                    break;
                case CApplicationData::ptAIMedium:  context->DLoadingPlayerTypes[to_underlying(DPlayerColorRequesTypeChange)] = CApplicationData::ptAIHard;
                                                    break;
                case CApplicationData::ptAIHard:    context->DLoadingPlayerTypes[to_underlying(DPlayerColorRequesTypeChange)] = CApplicationData::ptNone;
                                                    break;
                default:                            context->DLoadingPlayerTypes[to_underlying(DPlayerColorRequesTypeChange)] = CApplicationData::ptHuman;
                                                    break;
            }
        }
    }
}

/**
* Renders minimap, buttons, colors, titles, etc for the mode. Also plays a sound when the mouse hovers over a button and when the mode is
* changed (ie. "Play Game" or "Cancel" is selected.
*
* @param[in] context Shared pointer to CApplicationData
*
*/
void CPlayerAIColorSelectMode::Render(std::shared_ptr< CApplicationData > context){   
    int CurrentX, CurrentY;
    int BufferWidth, BufferHeight;
    int TitleHeight;
    int TextWidth, TextHeight, MaxTextWidth;
    int ColumnWidth, RowHeight;
    int MiniMapWidth, MiniMapHeight, MiniMapCenter, MiniMapLeft;
    int TextTop, ButtonLeft, ButtonTop, AIButtonLeft, ColorButtonHeight;
    int GoldColor, WhiteColor, ShadowColor;
    std::string TempString;
    std::array< std::string, to_underlying(EPlayerColor::Max) > PlayerNames;
    CButtonRenderer::EButtonState ButtonState = CButtonRenderer::EButtonState::None;
    bool ButtonXAlign = false, ButtonHovered = false;
    
    CurrentX = context->DCurrentX;
    CurrentY = context->DCurrentY;
    
    context->RenderMenuTitle(DTitle, TitleHeight, BufferWidth, BufferHeight); 
    
    GoldColor = context->DFonts[to_underlying(CUnitDescriptionRenderer::EFontSize::Large)]->FindColor("gold");
    WhiteColor = context->DFonts[to_underlying(CUnitDescriptionRenderer::EFontSize::Large)]->FindColor("white");
    ShadowColor = context->DFonts[to_underlying(CUnitDescriptionRenderer::EFontSize::Large)]->FindColor("black");
    
    MiniMapWidth = context->DMiniMapSurface->Width();
    MiniMapHeight = context->DMiniMapSurface->Height();
    
    context->DMiniMapRenderer->DrawMiniMap(context->DMiniMapSurface);
    MiniMapLeft = BufferWidth - MiniMapWidth - context->DBorderWidth;
    context->DWorkingBufferSurface->Draw(context->DMiniMapSurface, MiniMapLeft, TitleHeight + context->DInnerBevel->Width(), -1, -1, 0, 0);
    context->DInnerBevel->DrawBevel(context->DWorkingBufferSurface, MiniMapLeft, TitleHeight + context->DInnerBevel->Width(), MiniMapWidth, MiniMapHeight);
    
    TextTop = TitleHeight + MiniMapHeight + context->DInnerBevel->Width() * 3;
    MiniMapCenter = MiniMapLeft + MiniMapWidth / 2;
    
    TempString = std::to_string(context->DSelectedMap->PlayerCount()) + " Players";
    context->DFonts[to_underlying(CUnitDescriptionRenderer::EFontSize::Large)]->MeasureText(TempString, TextWidth, TextHeight);
    context->DFonts[to_underlying(CUnitDescriptionRenderer::EFontSize::Large)]->DrawTextWithShadow(context->DWorkingBufferSurface, MiniMapCenter - TextWidth/2, TextTop, WhiteColor, ShadowColor, 1, TempString);
    TextTop += TextHeight;
    TempString = std::to_string(context->DSelectedMap->Width()) + " x " + std::to_string(context->DSelectedMap->Height());
    context->DFonts[to_underlying(CUnitDescriptionRenderer::EFontSize::Large)]->MeasureText(TempString, TextWidth, TextHeight);
    context->DFonts[to_underlying(CUnitDescriptionRenderer::EFontSize::Large)]->DrawTextWithShadow(context->DWorkingBufferSurface, MiniMapCenter - TextWidth/2, TextTop, WhiteColor, ShadowColor, 1, TempString);

    
    TextTop = TitleHeight;
    TempString = "Player";
    context->DFonts[to_underlying(CUnitDescriptionRenderer::EFontSize::Large)]->MeasureText(TempString, TextWidth, TextHeight);
    context->DFonts[to_underlying(CUnitDescriptionRenderer::EFontSize::Large)]->DrawTextWithShadow(context->DWorkingBufferSurface, context->DBorderWidth, TextTop, WhiteColor, ShadowColor, 1, TempString);
    TextTop += TextHeight;
    
    context->DButtonRenderer->Text("AI Easy", true);
    ColorButtonHeight = context->DButtonRenderer->Height();
    RowHeight = context->DButtonRenderer->Height() + context->DInnerBevel->Width() * 2;
    if(RowHeight < TextHeight){
        RowHeight = TextHeight;
    }
    context->DButtonRenderer->Text("X", true);
    context->DButtonRenderer->Height(ColorButtonHeight);
    ColumnWidth = context->DButtonRenderer->Width() + context->DInnerBevel->Width() * 2;
    MaxTextWidth = 0;
    for(int Index = 1; Index <= context->DSelectedMap->PlayerCount(); Index++){
        if(Index == to_underlying(context->DPlayerColor)){
            PlayerNames[Index] = TempString = std::to_string(Index) + ". You";   
        }
        else if(CApplicationData::ptHuman != context->DLoadingPlayerTypes[Index]){
            PlayerNames[Index] = TempString = std::to_string(Index) + ". Player " + std::to_string(Index);   
        }
        context->DFonts[to_underlying(CUnitDescriptionRenderer::EFontSize::Large)]->MeasureText(TempString, TextWidth, TextHeight);
        if(MaxTextWidth < TextWidth){
            MaxTextWidth = TextWidth;   
        }
    }
    TempString = "Color";
    context->DFonts[to_underlying(CUnitDescriptionRenderer::EFontSize::Large)]->MeasureText(TempString, TextWidth, TextHeight);
    context->DFonts[to_underlying(CUnitDescriptionRenderer::EFontSize::Large)]->DrawTextWithShadow(context->DWorkingBufferSurface, context->DBorderWidth + MaxTextWidth + (ColumnWidth * (to_underlying(EPlayerColor::Max) + 1))/2 - TextWidth/2, TitleHeight, WhiteColor, ShadowColor, 1, TempString);
    DColorButtonLocations.clear();
    for(int Index = 1; Index <= context->DSelectedMap->PlayerCount(); Index++){
        TempString = PlayerNames[Index];
        context->DFonts[to_underlying(CUnitDescriptionRenderer::EFontSize::Large)]->MeasureText(TempString, TextWidth, TextHeight);
        context->DFonts[to_underlying(CUnitDescriptionRenderer::EFontSize::Large)]->DrawTextWithShadow(context->DWorkingBufferSurface, context->DBorderWidth, TextTop, Index == to_underlying(context->DPlayerColor) ? GoldColor : WhiteColor, ShadowColor, 1, TempString);
        for(int ColorIndex = 1; ColorIndex < to_underlying(EPlayerColor::Max); ColorIndex++){
            int ButtonLeft = context->DBorderWidth + MaxTextWidth + ColorIndex * ColumnWidth;
            context->DButtonRenderer->Text(to_underlying(context->DLoadingPlayerColors[Index]) == ColorIndex ? "X" : "");
            context->DButtonRenderer->ButtonColor(static_cast<EPlayerColor>(ColorIndex));
            context->DButtonRenderer->DrawButton(context->DWorkingBufferSurface, ButtonLeft, TextTop, CButtonRenderer::EButtonState::None);
            DColorButtonLocations.push_back(SRectangle({ButtonLeft, TextTop, context->DButtonRenderer->Width(), context->DButtonRenderer->Height()}));
            AIButtonLeft = ButtonLeft + ColumnWidth;
        }
        TextTop += RowHeight;
    }
    
    
    context->DButtonRenderer->ButtonColor(EPlayerColor::None);
    TempString = "AI Easy";
    context->DButtonRenderer->Text(TempString);   
    context->DButtonRenderer->Width(context->DButtonRenderer->Width() * 3 / 2);
    
    TextTop = TitleHeight;
    TempString = "Difficulty";
    context->DFonts[to_underlying(CUnitDescriptionRenderer::EFontSize::Large)]->MeasureText(TempString, TextWidth, TextHeight);
    context->DFonts[to_underlying(CUnitDescriptionRenderer::EFontSize::Large)]->DrawTextWithShadow(context->DWorkingBufferSurface, AIButtonLeft + (context->DButtonRenderer->Width() - TextWidth)/2, TextTop, WhiteColor, ShadowColor, 1, TempString);
    
    ButtonXAlign = false;
    if((AIButtonLeft <= CurrentX)&&(AIButtonLeft + context->DButtonRenderer->Width() > CurrentX)){
        ButtonXAlign = true;
    }
    TextTop += RowHeight + TextHeight;
    DPlayerTypeButtonLocations.clear();
    for(int Index = 2; Index <= context->DSelectedMap->PlayerCount(); Index++){
        switch(context->DLoadingPlayerTypes[Index]){
            case CApplicationData::ptHuman:     context->DButtonRenderer->Text("Human");
                                                break;
            case CApplicationData::ptAIEasy:    context->DButtonRenderer->Text("AI Easy");
                                                break;
            case CApplicationData::ptAIMedium:  context->DButtonRenderer->Text("AI Medium");
                                                break;
            case CApplicationData::ptAIHard:    context->DButtonRenderer->Text("AI Hard");
                                                break;
            default:                            context->DButtonRenderer->Text("Closed");
                                                break;
        }
        ButtonState = CButtonRenderer::EButtonState::None;
        if(ButtonXAlign){
            if((TextTop <= CurrentY)&&((TextTop + context->DButtonRenderer->Height() > CurrentY))){
                ButtonState = context->DLeftDown ? CButtonRenderer::EButtonState::Pressed : CButtonRenderer::EButtonState::Hover;
                ButtonHovered = true;
            }
        }
        context->DButtonRenderer->DrawButton(context->DWorkingBufferSurface, AIButtonLeft, TextTop, ButtonState);
        DPlayerTypeButtonLocations.push_back(SRectangle({AIButtonLeft, TextTop, context->DButtonRenderer->Width(), context->DButtonRenderer->Height()}));
                
        TextTop += RowHeight;
    }
    
    DButtonLocations.clear();
    
    context->DButtonRenderer->ButtonColor(EPlayerColor::None);

    context->DButtonRenderer->Text(DButtonTexts[0], true);   
    context->DButtonRenderer->Height(context->DButtonRenderer->Height() * 3 / 2);
    context->DButtonRenderer->Width(MiniMapWidth);
    ButtonLeft = BufferWidth - context->DButtonRenderer->Width() - context->DBorderWidth;
    ButtonTop = BufferHeight - (context->DButtonRenderer->Height() * 9 / 4) - context->DBorderWidth;
    ButtonState = CButtonRenderer::EButtonState::None;
    if((ButtonLeft <= CurrentX)&&(ButtonLeft + context->DButtonRenderer->Width() > CurrentX)){
        ButtonXAlign = true;
    }
    if(ButtonXAlign){
        if((ButtonTop <= CurrentY)&&((ButtonTop + context->DButtonRenderer->Height() > CurrentY))){
            ButtonState = context->DLeftDown ? CButtonRenderer::EButtonState::Pressed : CButtonRenderer::EButtonState::Hover;
            ButtonHovered = true;
        }
    }
    context->DButtonRenderer->DrawButton(context->DWorkingBufferSurface, ButtonLeft, ButtonTop, ButtonState);
    DButtonLocations.push_back(SRectangle({ButtonLeft, ButtonTop, context->DButtonRenderer->Width(), context->DButtonRenderer->Height()}));
    
    ButtonTop = BufferHeight - context->DButtonRenderer->Height() - context->DBorderWidth;
    ButtonState = CButtonRenderer::EButtonState::None;
    if(ButtonXAlign){
        if((ButtonTop <= CurrentY)&&((ButtonTop + context->DButtonRenderer->Height() > CurrentY))){
            ButtonState = context->DLeftDown ? CButtonRenderer::EButtonState::Pressed : CButtonRenderer::EButtonState::Hover;
            ButtonHovered = true;
        }
    }

    context->DButtonRenderer->Text(DButtonTexts[1], false);   
    context->DButtonRenderer->DrawButton(context->DWorkingBufferSurface, ButtonLeft, ButtonTop, ButtonState);
    DButtonLocations.push_back(SRectangle({ButtonLeft, ButtonTop, context->DButtonRenderer->Width(), context->DButtonRenderer->Height()}));
    
    if(!DButtonHovered && ButtonHovered){
        context->DSoundLibraryMixer->PlayClip(context->DSoundLibraryMixer->FindClip("tick"), context->DSoundVolume, 0.0);
    }
    if(context->ModeIsChanging()){
        context->DSoundLibraryMixer->PlayClip(context->DSoundLibraryMixer->FindClip("place"), context->DSoundVolume, 0.0);
    }
    DButtonHovered = ButtonHovered;
}

/**
* Creates an instance of CPlayerAIColorSelectMode and returns a shared pointer to it or just returns a shared pointer to the current
* instance of the game.
*
* @return shared pointer to DPlayerAIColorSelectModePointer
*
*/
std::shared_ptr< CApplicationMode > CPlayerAIColorSelectMode::Instance(){   
    if(DPlayerAIColorSelectModePointer == nullptr){
        DPlayerAIColorSelectModePointer = std::make_shared< CPlayerAIColorSelectMode >(SPrivateConstructorType());
    }
    return DPlayerAIColorSelectModePointer;   
}
