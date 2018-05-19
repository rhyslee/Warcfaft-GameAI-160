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
 * @class CButtonRenderer
 *
 * @brief This class renders individual buttons on the screen during each mode
 *
 *   CButtonRenderer renders buttons on the screen, and re-renders them based on
 *   the player's interaction with each button. For example, if the player hovers
 *   the cursor over a button, the button is re-rendered with the hover parameter.
 *
 * @author Spencer Van Dyke
 *
 * @version 1.0
 *
 * @date 10/8/2017
 *
 * Contact: savandyke@ucdavis.edu
 *
 */

#include "ButtonRenderer.h"
#include "Debug.h"


/**
 * CButtonRenderer constructor is passed the parameters for a new button object
 *
 * @param[in] colors The set of color values associated with the new button object
 * @param[in] innerbevel The location and size of the inside bevel of the button object
 * @param[in] outerbevel The location and size of the outside bevel of the button object
 * @param[in] font The colors, size, and location asso
 *
 * @return None
 */
CButtonRenderer::CButtonRenderer(std::shared_ptr< CGraphicRecolorMap > colors, std::shared_ptr< CBevel > innerbevel, std::shared_ptr< CBevel > outerbevel, std::shared_ptr< CFontTileset > font){
    DColorMap = colors;
    DOuterBevel = outerbevel;
    DInnerBevel = innerbevel;
    DFont = font;
    DButtonColor = EPlayerColor::None;
    DTextOffsetX = 0;
    DTextOffsetY = 0;
    DWidth = DOuterBevel->Width() * 2;
    DHeight = DWidth;
    DLightIndices.resize(to_underlying(EPlayerColor::Max));
    DDarkIndices.resize(to_underlying(EPlayerColor::Max));             
    DDarkIndices[to_underlying(EPlayerColor::None)] = DDarkIndices[to_underlying(EPlayerColor::Blue)] = DColorMap->FindColor("blue-dark");
    DDarkIndices[to_underlying(EPlayerColor::Red)] = DColorMap->FindColor("red-dark");
    DDarkIndices[to_underlying(EPlayerColor::Green)] = DColorMap->FindColor("green-dark");
    DDarkIndices[to_underlying(EPlayerColor::Purple)] = DColorMap->FindColor("purple-dark");
    DDarkIndices[to_underlying(EPlayerColor::Orange)] = DColorMap->FindColor("orange-dark");
    DDarkIndices[to_underlying(EPlayerColor::Yellow)] = DColorMap->FindColor("yellow-dark");
    DDarkIndices[to_underlying(EPlayerColor::Black)] = DColorMap->FindColor("black-dark");
    DDarkIndices[to_underlying(EPlayerColor::White)] = DColorMap->FindColor("white-dark");

    DLightIndices[to_underlying(EPlayerColor::None)] = DLightIndices[to_underlying(EPlayerColor::Blue)] = DColorMap->FindColor("blue-light");
    DLightIndices[to_underlying(EPlayerColor::Red)] = DColorMap->FindColor("red-light");
    DLightIndices[to_underlying(EPlayerColor::Green)] = DColorMap->FindColor("green-light");
    DLightIndices[to_underlying(EPlayerColor::Purple)] = DColorMap->FindColor("purple-light");
    DLightIndices[to_underlying(EPlayerColor::Orange)] = DColorMap->FindColor("orange-light");
    DLightIndices[to_underlying(EPlayerColor::Yellow)] = DColorMap->FindColor("yellow-light");
    DLightIndices[to_underlying(EPlayerColor::Black)] = DColorMap->FindColor("black-light");
    DLightIndices[to_underlying(EPlayerColor::White)] = DColorMap->FindColor("white-light");
    
    DWhiteIndex = DFont->FindColor("white");
    DGoldIndex = DFont->FindColor("gold");
    DBlackIndex = DFont->FindColor("black");
    PrintDebug(DEBUG_HIGH,"CButtonRenderer w = %d, g = %d, b = %d\n", DWhiteIndex, DGoldIndex, DBlackIndex);
}

/**
 * Calculates the size needed for the entire button so that the text can be rendered in the button's font
 *
 * @param[in] text Used to get the total screen space required to render the button text in the button's font
 * @param[in] minimize Bool to set if the size of the button should be just enough to fit the text
 *
 * @return The text originally passed in as the first parameter
 */
std::string CButtonRenderer::Text(const std::string &text, bool minimize){
    int TotalWidth, TotalHeight, Top, Bottom;
    DText = text;
    DFont->MeasureTextDetailed(DText, TotalWidth, TotalHeight, Top, Bottom);
    
    TotalHeight = Bottom - Top + 1;
    if(TotalHeight + DOuterBevel->Width() * 2 > DHeight){
        DHeight = TotalHeight + DOuterBevel->Width() * 2;  
    }
    else if(minimize){
        DHeight = TotalHeight + DOuterBevel->Width() * 2;  
    }
    if(TotalWidth + DOuterBevel->Width() * 2 > DWidth){
        DWidth = TotalWidth + DOuterBevel->Width() * 2;
    }
    else if(minimize){
        DWidth = TotalWidth + DOuterBevel->Width() * 2;
    }
    DTextOffsetX = DWidth/2 - TotalWidth/2;
    DTextOffsetY = DHeight/2 - TotalHeight/2 - Top;
    return DText;
}

/**
 * Setter function to change the overall width of the button object
 *
 * @param[in] width If the width parameter is larger than the current width, then resize the button
 *
 * @return The current value of the width of the button
 */
int CButtonRenderer::Width(int width){
    if(width > DWidth){
        int TotalWidth, TotalHeight, Top, Bottom;
        
        DFont->MeasureTextDetailed(DText, TotalWidth, TotalHeight, Top, Bottom);
        DWidth = width;  
        DTextOffsetX = DWidth/2 - TotalWidth/2;
    }
    return DWidth;
}

/**
 * Setter function to change the overall width of the button object
 *
 * @param[in] height If the height parameter is larger than the current height, then resize the button
 *
 * @return The current value of the width of the button
 */
int CButtonRenderer::Height(int height){
    if(height > DHeight){
        int TotalWidth, TotalHeight, Top, Bottom;
        
        DFont->MeasureTextDetailed(DText, TotalWidth, TotalHeight, Top, Bottom);
        TotalHeight = Bottom - Top + 1;
        DHeight = height;
        DTextOffsetY = DHeight/2 - TotalHeight/2 - Top;
        
    }
    return DHeight;    
}

/**
 * Uses the parameters of the button object to render the button on the screen. This
 * also re-renders a button depending on player input, rendering the button in a
 * depressed state when the player clicks it, for example
 *
 * @param[in] surface The surface over which the button object is rendered
 * @param[in] x The X coordinate for where to begin rendering the button on the screen
 * @param[in] y The Y coordinate for where to begin rendering the button on the screen
 * @param[in] state The current state of the button, whose enum values can be found in ButtonRenderer.h
 *
 * @return None
 */
void CButtonRenderer::DrawButton(std::shared_ptr<CGraphicSurface> surface, int x, int y, EButtonState state){
    auto ResourceContext = surface->CreateResourceContext();
    if(EButtonState::Pressed == state){
        int BevelWidth = DInnerBevel->Width();
        
        ResourceContext->SetSourceRGBA( DColorMap->ColorValue(DDarkIndices[to_underlying(DButtonColor)], 0) );
        ResourceContext->Rectangle(x, y, DWidth, DHeight);
        ResourceContext->Fill();
        //DColorMap->DrawTileRectangle(drawable, x, y, DWidth, DHeight, DDarkIndices[DButtonColor]);
        DFont->DrawTextWithShadow(surface, x + DTextOffsetX, y + DTextOffsetY, DWhiteIndex, DBlackIndex, 1, DText);
        DInnerBevel->DrawBevel(surface, x + BevelWidth, y + BevelWidth, DWidth - BevelWidth * 2, DHeight - BevelWidth * 2);
    }
    else if(EButtonState::Inactive == state){
        int BevelWidth = DOuterBevel->Width();
        
        ResourceContext->SetSourceRGBA( DColorMap->ColorValue(DDarkIndices[to_underlying(DButtonColor)], 0) );
        ResourceContext->Rectangle(x, y, DWidth, DHeight);
        ResourceContext->Fill();
        //DColorMap->DrawTileRectangle(drawable, x, y, DWidth, DHeight, DDarkIndices[pcBlack]);
        DFont->DrawTextWithShadow(surface, x + DTextOffsetX, y + DTextOffsetY, DBlackIndex, DWhiteIndex, 1, DText);
        DOuterBevel->DrawBevel(surface, x + BevelWidth, y + BevelWidth, DWidth - BevelWidth * 2, DHeight - BevelWidth * 2);
    }
    else{
        int BevelWidth = DOuterBevel->Width();
        
        ResourceContext->SetSourceRGBA( DColorMap->ColorValue(DLightIndices[to_underlying(DButtonColor)], 0) );
        ResourceContext->Rectangle(x, y, DWidth, DHeight);
        ResourceContext->Fill();
        //DColorMap->DrawTileRectangle(drawable, x, y, DWidth, DHeight, DLightIndices[DButtonColor]);
        DFont->DrawTextWithShadow(surface, x + DTextOffsetX, y + DTextOffsetY, EButtonState::Hover == state ? DWhiteIndex : DGoldIndex, DBlackIndex, 1, DText);
        DOuterBevel->DrawBevel(surface, x + BevelWidth, y + BevelWidth, DWidth - BevelWidth * 2, DHeight - BevelWidth * 2);
    }
}


