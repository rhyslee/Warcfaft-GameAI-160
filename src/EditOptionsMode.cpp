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
* @class EditOptionsMode
*
* @brief This class renders the page to allow the user to edit options
*   When you select Sound Options in the Options Menu
*   you can modify the FX Volume or the Music Volume
*
*   When you select the Network Options you can modify
*   the User Name and Remote Hostname.
*
*   This class renders the edit pages and determines the page logic
*
* @author Nishant
*
* @version 1.0
*
* @date 10/11/2017 5:09:11 AM
*
* Contact: nchandrashekar@ucdavis.edu
*
*/

#include "EditOptionsMode.h"
#include "ApplicationData.h"
/*
        std::string DTitle;
        std::vector< std::string > DButtonTexts;
        std::vector< TButtonMenuCallbackFunction > DButtonFunctions;
        std::vector< SRectangle > DButtonLocations;
        bool DButtonHovered;
        
        int DEditSelected;
        int DEditSelectedCharacter;
        std::vector< SRectangle > DEditLocations;
        std::vector< std::string > DEditTitles;
        std::vector< std::string > DEditText;
        std::vector< TEditOptionsTextValidationCallbackFunction > DEditValidationFunctions;  
        
*/

CEditOptionsMode::CEditOptionsMode(){
    DButtonHovered = false;
}
        
void CEditOptionsMode::Input(std::shared_ptr< CApplicationData > context){
    int CurrentX, CurrentY;
    CurrentX = context->DCurrentX;
    CurrentY = context->DCurrentY;
    if(context->DLeftClick && !context->DLeftDown){
        bool ClickedEdit = false;
        for(int Index = 0; Index < DButtonLocations.size(); Index++){
            if(DButtonLocations[Index].PointInside(CurrentX, CurrentY)){
                DButtonFunctions[Index](context);
            }
        }
        for(int Index = 0; Index < DEditLocations.size(); Index++){
            if(DEditLocations[Index].PointInside(CurrentX, CurrentY)){
                if(Index != DEditSelected){
                    DEditSelected = Index;
                    DEditSelectedCharacter = DEditText[Index].size();
                    ClickedEdit = true;
                }
            } 
        }
        if(!ClickedEdit){
            DEditSelected = -1;     
        }
    }
    for(auto Key : context->DReleasedKeys){
        if(SGUIKeyType::Escape == Key){
            DEditSelected = -1;
        }
        else if(0 <= DEditSelected){
            SGUIKeyType TempKey;
            TempKey.DValue = Key;
            if((SGUIKeyType::Delete == Key)||(SGUIKeyType::BackSpace == Key)){
                if(DEditSelectedCharacter){             
                    DEditText[DEditSelected] = DEditText[DEditSelected].substr(0,DEditSelectedCharacter-1) + DEditText[DEditSelected].substr(DEditSelectedCharacter, DEditText[DEditSelected].length()-DEditSelectedCharacter);
                    DEditSelectedCharacter--;
                }
                else if(DEditText[DEditSelected].length()){
                    DEditText[DEditSelected] = DEditText[DEditSelected].substr(1);
                }
            }
            else if(SGUIKeyType::LeftArrow == Key){
                if(DEditSelectedCharacter){
                   DEditSelectedCharacter--; 
                }
            }
            else if(SGUIKeyType::RightArrow == Key){
                if(DEditSelectedCharacter < DEditText[DEditSelected].length()){
                   DEditSelectedCharacter++; 
                }
            }
            else if(TempKey.IsAlphaNumeric()||(SGUIKeyType::Period == Key)){
                DEditText[DEditSelected] = DEditText[DEditSelected].substr(0,DEditSelectedCharacter) + std::string(1, (char)Key) + DEditText[DEditSelected].substr(DEditSelectedCharacter,DEditText[DEditSelected].length()-DEditSelectedCharacter);
                DEditSelectedCharacter++;
            }
        }
    }
    context->DReleasedKeys.clear();
}

void CEditOptionsMode::Calculate(std::shared_ptr< CApplicationData > context){
    
}

void CEditOptionsMode::Render(std::shared_ptr< CApplicationData > context){
    int CurrentX, CurrentY;
    int BufferWidth, BufferHeight, BufferCenter;
    int TitleHeight, OptionSkip, OptionTop, TextOffsetY;
    int ButtonLeft, ButtonSkip, ButtonTop;
    bool ButtonXAlign = false, ButtonHovered = false;
    bool FirstButton = true, AllInputsValid = true;
    int GoldColor, WhiteColor, ShadowColor;
    
    CurrentX = context->DCurrentX;
    CurrentY = context->DCurrentY;
    context->RenderMenuTitle(DTitle, TitleHeight, BufferWidth, BufferHeight); 
    
    GoldColor = context->DFonts[to_underlying(CUnitDescriptionRenderer::EFontSize::Large)]->FindColor("gold");
    WhiteColor = context->DFonts[to_underlying(CUnitDescriptionRenderer::EFontSize::Large)]->FindColor("white");
    ShadowColor = context->DFonts[to_underlying(CUnitDescriptionRenderer::EFontSize::Large)]->FindColor("black");
    
    DButtonLocations.clear();
    for(auto Text : DButtonTexts){
        context->DButtonRenderer->Text(Text, FirstButton);
        FirstButton = false;
    }
    for(int Index = 0; Index < DEditText.size(); Index++){
        if(!DEditValidationFunctions[Index](DEditText[Index])){
            AllInputsValid = false;
            break;
        }
    }
    
    context->DButtonRenderer->Width(context->DButtonRenderer->Width() * 3 / 2);
    context->DButtonRenderer->Height(context->DButtonRenderer->Height() * 3 / 2);                            
    ButtonLeft = BufferWidth - context->DBorderWidth - context->DButtonRenderer->Width();
    ButtonSkip = context->DButtonRenderer->Height() * 3 / 2;
    ButtonTop = BufferHeight - context->DBorderWidth - (DButtonTexts.size() * ButtonSkip - context->DButtonRenderer->Height()/2);
    
    if((ButtonLeft <= CurrentX)&&((ButtonLeft + context->DButtonRenderer->Width() > CurrentX))){
        ButtonXAlign = true;
    }
    FirstButton = true;
    for(auto Text : DButtonTexts){
        CButtonRenderer::EButtonState ButtonState = CButtonRenderer::EButtonState::None;
        
        context->DButtonRenderer->Text(Text);
        if(ButtonXAlign){
            if((ButtonTop <= CurrentY)&&((ButtonTop + context->DButtonRenderer->Height() > CurrentY))){
                ButtonState = context->DLeftDown ? CButtonRenderer::EButtonState::Pressed : CButtonRenderer::EButtonState::Hover;
                ButtonHovered = true;
            }
        }
        if(FirstButton && !AllInputsValid){
            if(CButtonRenderer::EButtonState::None != ButtonState){
                ButtonHovered = false;   
            }
            ButtonState = CButtonRenderer::EButtonState::Inactive;
        }    
        FirstButton = false;
        context->DButtonRenderer->DrawButton(context->DWorkingBufferSurface, ButtonLeft, ButtonTop, ButtonState);
        if(CButtonRenderer::EButtonState::Inactive != ButtonState){
            DButtonLocations.push_back( SRectangle({ButtonLeft, ButtonTop, context->DButtonRenderer->Width(), context->DButtonRenderer->Height()}));
        }
        else{
            DButtonLocations.push_back( SRectangle({0, 0, 0, 0}));
        }
        ButtonTop += ButtonSkip;
    }
    
    DEditLocations.clear();
    BufferCenter = BufferWidth/2;
    OptionSkip = context->DOptionsEditRenderer->Height() * 3 / 2;
    OptionTop = (BufferHeight + TitleHeight) / 2 - (OptionSkip * DEditTitles.size()) / 2;
    for(int Index = 0; Index < DEditTitles.size(); Index++){
        std::string TempString;
        int TextWidth, TextHeight;
        TempString = DEditTitles[Index];
        
        context->DFonts[to_underlying(CUnitDescriptionRenderer::EFontSize::Large)]->MeasureText(TempString, TextWidth, TextHeight);
        TextOffsetY = context->DOptionsEditRenderer->Height()/2 - TextHeight/2;
        context->DFonts[to_underlying(CUnitDescriptionRenderer::EFontSize::Large)]->DrawTextWithShadow(context->DWorkingBufferSurface, BufferCenter - TextWidth, OptionTop + TextOffsetY, WhiteColor, ShadowColor, 1, TempString);
        
        context->DOptionsEditRenderer->Text(DEditText[Index], DEditValidationFunctions[Index](DEditText[Index]));
        context->DOptionsEditRenderer->DrawEdit(context->DWorkingBufferSurface, BufferCenter, OptionTop, Index == DEditSelected ? DEditSelectedCharacter : -1);
        DEditLocations.push_back(SRectangle({BufferCenter, OptionTop, context->DOptionsEditRenderer->Width(), context->DOptionsEditRenderer->Height()}));
        OptionTop += OptionSkip;
    }
    
    if(!DButtonHovered && ButtonHovered){
        context->DSoundLibraryMixer->PlayClip(context->DSoundLibraryMixer->FindClip("tick"), context->DSoundVolume, 0.0);
    }
    if(context->ModeIsChanging()){
        context->DSoundLibraryMixer->PlayClip(context->DSoundLibraryMixer->FindClip("place"), context->DSoundVolume, 0.0);
    }
    DButtonHovered = ButtonHovered;
}

