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
* @class ButtonMenuMode
*
* @brief This class is for the MenuMode Buttons
*  
*   This class shows checks if a button has been clicked
*   as well as renders each individual Button in MenuMode
*
* @author Nishant
*
* @version 1.0
*
* @date 10/10/2017 11:14:55
*
* Contact: nchandrashekar@ucdavis.edu
*
*/

#include "ButtonMenuMode.h"
#include "ApplicationData.h"
#include "BattleMode.h"   
#include "Debug.h"

CButtonMenuMode::CButtonMenuMode(){
    DButtonHovered = false;
}
        
void CButtonMenuMode::Input(std::shared_ptr< CApplicationData > context){
    int CurrentX, CurrentY;
    CurrentX = context->DCurrentX;
    CurrentY = context->DCurrentY;
    if(context->DLeftClick && !context->DLeftDown){
        for(int Index = 0; Index < DButtonLocations.size(); Index++){
            if(DButtonLocations[Index].PointInside(CurrentX, CurrentY)){
                DButtonFunctions[Index](context);
            }
        }
    }
}

void CButtonMenuMode::Calculate(std::shared_ptr< CApplicationData > context){
    
}

void CButtonMenuMode::Render(std::shared_ptr< CApplicationData > context){
    int CurrentX, CurrentY;
    int BufferWidth, BufferHeight;
    int TitleHeight;
    int ButtonLeft, ButtonSkip, ButtonTop;
    int ButtonIndex;
    bool ButtonXAlign = false, ButtonHovered = false;

    CurrentX = context->DCurrentX;
    CurrentY = context->DCurrentY;
    context->RenderMenuTitle(DTitle, TitleHeight, BufferWidth, BufferHeight); 
    context->DButtonRenderer->Text(DButtonTexts.front(), true);   
    for(auto Text : DButtonTexts){
        context->DButtonRenderer->Text(Text);
    }
    context->DButtonRenderer->Height(context->DButtonRenderer->Height() * 3 / 2);
    context->DButtonRenderer->Width(context->DButtonRenderer->Width() * 5 / 4);
    ButtonSkip = context->DButtonRenderer->Height() * 3 / 2;
    ButtonLeft = BufferWidth/2 - context->DButtonRenderer->Width()/2;
    ButtonTop = (BufferHeight - TitleHeight)/2 - (ButtonSkip * (DButtonTexts.size() - 1) + context->DButtonRenderer->Height())/2;
    ButtonTop += context->DOuterBevel->Width();
    if((ButtonLeft <= CurrentX)&&((ButtonLeft + context->DButtonRenderer->Width() > CurrentX))){
        ButtonXAlign = true;
    }
    ButtonIndex = 0;
    DButtonLocations.resize(DButtonTexts.size());
    for(auto Text : DButtonTexts){
        if(Text.length()){
            CButtonRenderer::EButtonState ButtonState = CButtonRenderer::EButtonState::None;
            if(ButtonXAlign){
                if((ButtonTop <= CurrentY)&&((ButtonTop + context->DButtonRenderer->Height() > CurrentY))){
                    ButtonState = context->DLeftDown ? CButtonRenderer::EButtonState::Pressed : CButtonRenderer::EButtonState::Hover;
                    ButtonHovered = true;
                }
            }
            context->DButtonRenderer->Text(Text);
            context->DButtonRenderer->DrawButton(context->DWorkingBufferSurface, ButtonLeft, ButtonTop, ButtonState);
            DButtonLocations[ButtonIndex] = SRectangle({ButtonLeft, ButtonTop, context->DButtonRenderer->Width(), context->DButtonRenderer->Height()});
        }
        else{
            DButtonLocations[ButtonIndex] = SRectangle({0, 0, 0, 0});
        }
        ButtonIndex++;
        ButtonTop += ButtonSkip;
    }
    if(!DButtonHovered && ButtonHovered){
        context->DSoundLibraryMixer->PlayClip(context->DSoundLibraryMixer->FindClip("tick"), context->DSoundVolume, 0.0);
    }
    
    if(context->ModeIsChanging()){
        context->DSoundLibraryMixer->PlayClip(context->DSoundLibraryMixer->FindClip("place"), context->DSoundVolume, 0.0);
    }
    DButtonHovered = ButtonHovered;
}

