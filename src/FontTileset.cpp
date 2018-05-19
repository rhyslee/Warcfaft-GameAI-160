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
 * @class FontTileset
 *
 * @brief 
 *      FontTileset creates an instance of CFontTileset which draws the text in the game.
 *   
 * @author Christopher Ta
 *
 * @version 4.0
 *
 * @date 10/29/2017
 *
 * Contact: cta@ucdavis.edu
 *
*/
#include "FontTileset.h"
#include "LineDataSource.h"
#include "Tokenizer.h"
#include "Debug.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * Empty Constructor to instantiate CFontTileset object
 *
 * @param[in] None 
 *
 * @return None
*/
CFontTileset::CFontTileset(){
    
}

/**
 * Empty Destructor to deallocate CFontTileset object
 *
 * @param[in] None 
 *
 * @return None
*/
CFontTileset::~CFontTileset(){

}

/**
 * Searches for the pixel containing the data provided
 *
 * @param[in] data void* pointer containing font data
 * @param[in] pixel uint32_t containing the pixel to search for 
 *
 * @return pixel uint32_t Resulting pixel from the search
*/
uint32_t CFontTileset::TopBottomSearch(void *data, uint32_t pixel){
    CFontTileset *Font = static_cast<CFontTileset *>(data);
    int Row = Font->DSearchCall / Font->DTileWidth;
    
    Font->DSearchCall++;
    if(pixel & 0xFF000000){
        if(Row < Font->DTopOpaque){
            Font->DTopOpaque = Row;
        }
        Font->DBottomOpaque = Row;
    }
    
    return pixel;
}

/**
 * Loads the font into the application instance
 *
 * @param[in] colormap shared pointer of CGraphicRecolorMap
 * @param[in] source shared pointer of CDataSource
 *
 * @return ReturnStatus bool indicating if this function was able to load the font
*/
bool CFontTileset::LoadFont(std::shared_ptr< CGraphicRecolorMap > colormap, std::shared_ptr< CDataSource > source){
    // Instantiate local variables to load font
    CLineDataSource LineSource(source);
    std::string TempString;
    bool ReturnStatus = false;
    std::vector< int > BottomOccurence;
    int BestLine = 0;
    
    if(!CGraphicMulticolorTileset::LoadTileset(colormap, source)){
        return false;    
    }    
    
    // Resize based on the FontTileset's DTileCount
    DCharacterWidths.resize(DTileCount);
    DDeltaWidths.resize(DTileCount);
    DCharacterTops.resize(DTileCount);
    DCharacterBottoms.resize(DTileCount);
    DCharacterBaseline = DTileHeight;
    try{
        for(int Index = 0; Index < DTileCount; Index++){
            if(!LineSource.Read(TempString)){
                goto LoadFontExit;
            }
            DCharacterWidths[Index] = std::stoi(TempString);
        }
        for(int FromIndex = 0; FromIndex < DTileCount; FromIndex++){
            std::vector< std::string > Values;
            DDeltaWidths[FromIndex].resize(DTileCount);
            if(!LineSource.Read(TempString)){
                goto LoadFontExit;
            }
            CTokenizer::Tokenize(Values, TempString);
            if(Values.size() != DTileCount){
                goto LoadFontExit;
            }
            for(int ToIndex = 0; ToIndex < DTileCount; ToIndex++){
                DDeltaWidths[FromIndex][ToIndex] = std::stoi(Values[ToIndex]);
            }
        }
        ReturnStatus = true;
    }
    catch(std::exception &E){
        PrintError("%s\n",E.what());
    }
    
    BottomOccurence.resize(DTileHeight + 1);
    for(int Index = 0; Index < BottomOccurence.size(); Index++){
        BottomOccurence[Index] = 0;
    }
    for(int Index = 0; Index < DTileCount; Index++){
        DTopOpaque = DTileHeight;
        DBottomOpaque = 0;
        DSearchCall = 0;
        DSurfaceTileset->Transform(DSurfaceTileset, 0, Index * DTileHeight, DTileWidth, DTileHeight, 0, Index * DTileHeight, this, TopBottomSearch); 
        DCharacterTops[Index] = DTopOpaque;
        DCharacterBottoms[Index] = DBottomOpaque;
        BottomOccurence[DBottomOpaque]++;
    }
    for(int Index = 1; Index < BottomOccurence.size(); Index++){
        if(BottomOccurence[BestLine] < BottomOccurence[Index]){
            BestLine = Index;
        }
    }
    DCharacterBaseline = BestLine;

    
LoadFontExit:
    return ReturnStatus;
}

/**
 * Draws the font into the application instance
 *
 * @param[in] surface shared pointer of CGraphicSurface
 * @param[in] xpos integer of x coordinate
 * @param[in] ypos integer of y coordinate
 * @param[in] str constant reference to the text that this function will draw
 *
 * @return None
*/
void CFontTileset::DrawText(std::shared_ptr<CGraphicSurface> surface, int xpos, int ypos, const std::string &str){
    int LastChar, NextChar;
    for(int Index = 0; Index < str.length(); Index++){
        NextChar = str[Index] - ' ';
        
        if(Index){
            xpos += DCharacterWidths[LastChar] + DDeltaWidths[LastChar][NextChar]; 
        }
        CGraphicTileset::DrawTile(surface, xpos, ypos, NextChar);
        LastChar = NextChar;
    }
}

/**
 * Draws the text font color into the application instance
 *
 * @param[in] surface shared pointer of CGraphicSurface
 * @param[in] xpos integer of x coordinate
 * @param[in] ypos integer of y coordinate
 * @param[in] colorindex integer of the color index to use
 * @param[in] str constant reference to the text that this function will draw
 *
 * @return None
*/
void CFontTileset::DrawTextColor(std::shared_ptr<CGraphicSurface> surface, int xpos, int ypos, int colorindex, const std::string &str){
    int LastChar, NextChar;
    
    if((0 > colorindex)||(colorindex >= DColoredTilesets.size())){
        return;    
    }
    for(int Index = 0; Index < str.length(); Index++){
        NextChar = str[Index] - ' ';
        
        if(Index){
            xpos += DCharacterWidths[LastChar] + DDeltaWidths[LastChar][NextChar]; 
        }
        
        CGraphicMulticolorTileset::DrawTile(surface, xpos, ypos, NextChar, colorindex);
        
        LastChar = NextChar;
    }
}

/**
 * Draws the text with a shadow into the application instance
 *
 * @param[in] surface shared pointer of CGraphicSurface
 * @param[in] xpos integer of x coordinate
 * @param[in] ypos integer of y coordinate
 * @param[in] color integer of the color index to use
 * @param[in] shadowcol integer of the shadow color index
 * @param[in] shadowwidth integer of the width of shadow
 * @param[in] str constant reference to the text that this function will draw
 *
 * @return None
*/
void CFontTileset::DrawTextWithShadow(std::shared_ptr<CGraphicSurface> surface, int xpos, int ypos, int color, int shadowcol, int shadowwidth, const std::string &str){
    if((0 > color)||(color >= DColoredTilesets.size())){
        PrintDebug(DEBUG_HIGH,"  Invalid color %d of %zd\n",color, DColoredTilesets.size());
        return;    
    }
    if((0 > shadowcol)||(shadowcol >= DColoredTilesets.size())){
        PrintDebug(DEBUG_HIGH,"  Invalid shadcolor %d of %zd\n",shadowcol, DColoredTilesets.size());
        return;    
    }
    DrawTextColor(surface, xpos + shadowwidth, ypos + shadowwidth, shadowcol, str);
    DrawTextColor(surface, xpos, ypos, color, str);
}

/**
 * Measures the text based on the indices and calls MeasureTextDetailed
 *
 * @param[in] str constant reference to the text that this function will measure
 * @param[in] width integer reference to width of text
 * @param[in] height integer reference to height of text
 *
 * @return None
*/
void CFontTileset::MeasureText(const std::string &str, int &width, int &height){
    int TempTop, TempBottom;
    MeasureTextDetailed(str, width, height, TempTop, TempBottom);
}

/**
 * Measures the text based on the details provided in MesaureText
 *
 * @param[in] str constant reference to the text that this function will measure
 * @param[in] width integer reference to width of text
 * @param[in] height integer reference to height of text
 * @param[in] top integer reference to top length of text
 * @param[in] bottom integer reference to bottom length of text
 *
 * @return None
*/
void CFontTileset::MeasureTextDetailed(const std::string &str, int &width, int &height, int &top, int &bottom){
    int LastChar, NextChar;
    width = 0;
    top = DTileHeight;
    bottom = 0;
    for(int Index = 0; Index < str.length(); Index++){
        NextChar = str[Index] - ' ';
        
        if(Index){
            width += DDeltaWidths[LastChar][NextChar]; 
        }
        width += DCharacterWidths[NextChar]; 
        if(DCharacterTops[NextChar] < top){
            top = DCharacterTops[NextChar];   
        }
        if(DCharacterBottoms[NextChar] > bottom){
            bottom = DCharacterBottoms[NextChar];   
        }
        LastChar = NextChar;
    }
    height = DTileHeight;
}

