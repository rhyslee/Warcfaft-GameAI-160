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
 * @class EditRenderer
 *
 * @brief EditRenderer creates an instance of CEditRenderer which controls the rendering of text and color in bevels.
 *   
 * @author Christopher Ta
 *
 * @version 1.0
 *
 * @date 10/16/2017
 *
 * Contact: cta@ucdavis.edu
 *
*/

#include "EditRenderer.h"

/**
 * Constructor initializes DDarkIndices and DLightIndices integer vectors with colors from DColorMap
 *
 * @param[in] colors Shared pointer of CGraphicRecolorMap 
 * @param[in] innerbevel Shared pointer of CBevel
 * @param[in] font Shared pointer of CFontTileset
 *
 * @return None
*/
CEditRenderer::CEditRenderer(std::shared_ptr< CGraphicRecolorMap > colors, std::shared_ptr< CBevel > innerbevel, std::shared_ptr< CFontTileset > font){
    // Initializes data members with the parameters provided
    DColorMap = colors;
    DInnerBevel = innerbevel;
    DFont = font;
    DBackgroundColor = EPlayerColor::None;

    // Initializes DLightIndices and DDarkIndicies integer vectors with colors parameter
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
    
    // Initializes data members with the parameters provided
    DWhiteIndex = DFont->FindColor("white");
    DGoldIndex = DFont->FindColor("gold");
    DRedIndex = DFont->FindColor("red");
    DBlackIndex = DFont->FindColor("black");
    
    // Sets DWidth and DHeight data members to 0 and calls MinimumCharacters function
    DWidth = 0;
    DHeight = 0;
    MinimumCharacters(16);
}

/**
 * Setter function to change protected data members DText and DTextValid.
 *
 * @param[in] text Constant reference to standard string containing text
 * @param[in] valid Boolean to determine if the string is valid
 *
 * @return DText String protected data member
*/
std::string CEditRenderer::Text(const std::string &text, bool valid){
    DText = text;
    DTextValid = valid;
    return DText;
}

/**
 * Setter function to calculate and retrieve DMinimumCharacters data member. Changes DMinimumWidth and DMinimumHeight to fit text in DInnerBevel.
 *
 * @param[in] minchar Integer containing the minimum amount of characters
 *
 * @return DMinimumCharacters Integer protected data member
*/
int CEditRenderer::MinimumCharacters(int minchar){
    if(0 < minchar){
        int TextWidth, TextHeight;
        std::string TempText;
        
        for(int Index = 0; Index < minchar; Index++){
            TempText += "X";
        }
        TempText += "|";
        DFont->MeasureText(TempText, TextWidth, TextHeight);    
        DMinimumCharacters = minchar;
    
        // Calculates a new DMinimumWidth and DMinimumHeight to fit in DInnerBevel if necessary
        DMinimumWidth = TextWidth + DInnerBevel->Width() * 2;
        DMinimumHeight = TextHeight + DInnerBevel->Width() * 2;
        if(DWidth < DMinimumWidth){
            DWidth = DMinimumWidth;   
        }
        if(DHeight < DMinimumHeight){
            DHeight = DMinimumHeight;   
        }
    }
    return DMinimumCharacters;
}

/**
 * Setter function to set and retrive DWidth data member
 *
 * @param[in] width Integer containing new width
 *
 * @return DWidth Integer protected data member
*/
int CEditRenderer::Width(int width){
    if(DMinimumWidth <= width){
        DWidth = width;   
    }
    return DWidth;
}

/**
 * Setter function to set and retrive DHeight data member
 *
 * @param[in] height Integer containing new height
 *
 * @return DHeight Integer protected data member
*/
int CEditRenderer::Height(int height){
    if(DMinimumHeight <= height){
        DHeight = height;   
    }
    return DHeight;
}

/**
 * Changes the DFont and DInnerBevel based on the coordinates passed to this function
 *
 * @param[in] surface Shared pointer of CGraphicSurface
 * @param[in] x Integer of x coordinate
 * @param[in] y Integer of y coordinate
 * @param[in] cursorpos Integer of cursor position
 *
 * @return None
*/
void CEditRenderer::DrawEdit(std::shared_ptr< CGraphicSurface > surface, int x, int y, int cursorpos){
    // Initialize local resources to calculate new DFont and DInnerBevel
    auto ResourceContext = surface->CreateResourceContext();
    int BevelWidth = DInnerBevel->Width();
    int TextColorIndex;
    int TextWidth, TextHeight;
    std::string RenderText;
    
    // Calculate new rectangle dimensions based on parameter coordinates
    ResourceContext->SetSourceRGBA( DColorMap->ColorValue(0 <= cursorpos ? DLightIndices[to_underlying(DBackgroundColor)] : DDarkIndices[to_underlying(DBackgroundColor)], 0) );
    ResourceContext->Rectangle(x, y, DWidth, DHeight);
    ResourceContext->Fill();
    
    //DColorMap->DrawTileRectangle(drawable, x, y, DWidth, DHeight, 0 <= cursorpos ? DLightIndices[DBackgroundColor] : DDarkIndices[DBackgroundColor]);
    if(0 <= cursorpos){
        if(DText.length()){
            bool RemoveCharacters = false;
            RenderText = DText.substr(0,cursorpos);
            RenderText += "|";
            do{
                DFont->MeasureText(RenderText, TextWidth, TextHeight); 
                TextWidth += BevelWidth * 2;
                if(TextWidth > DWidth){
                    RenderText = RenderText.substr(1, RenderText.length()-1);
                    RemoveCharacters = true;
                }
            }while(DWidth < TextWidth);
            if(!RemoveCharacters){
                if(cursorpos < DText.length()){
                    RenderText += DText.substr(cursorpos);    
                }
            }
        }
        else{
            RenderText = "|";   
        }
    }
    else{
        RenderText = DText;
    }
    do{
        DFont->MeasureText(RenderText, TextWidth, TextHeight); 
        TextWidth += BevelWidth * 2;
        if(TextWidth > DWidth){
            RenderText = RenderText.substr(0, RenderText.length()-1);
        }
    }while(DWidth < TextWidth);
    if(DTextValid){
        TextColorIndex = 0 <= cursorpos ? DWhiteIndex : DGoldIndex;
    }
    else{
        TextColorIndex = DRedIndex;   
    }
    // Set new DFont and DInnerBevel
    DFont->DrawTextWithShadow(surface, x + BevelWidth, y + BevelWidth, TextColorIndex, DBlackIndex, 1, RenderText);
    DInnerBevel->DrawBevel(surface, x + BevelWidth, y + BevelWidth, DWidth - BevelWidth * 2, DHeight - BevelWidth * 2);
}

