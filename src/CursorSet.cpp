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
#include "CursorSet.h"
#include "Tokenizer.h"
#include "CommentSkipLineDataSource.h"
#include "Debug.h"

CCursorSet::CCursorSet() : CGraphicTileset(){
    
}

CCursorSet::~CCursorSet(){
    
}

        
int CCursorSet::FindCursor(const std::string &cursorname) const{
    return FindTile(cursorname);
}

bool CCursorSet::LoadCursors(std::shared_ptr< CDataSource > source){
    CCommentSkipLineDataSource LineSource(source, '#');
    std::string TempString;
    std::vector< std::string > Tokens;
    bool ReturnStatus = false;

    if(nullptr == source){
        return false;   
    }    
    if(!CGraphicTileset::LoadTileset(source)){
        return false;    
    }
    
    try{
        for(int Index = 0; Index < DTileCount; Index++){
            if(!LineSource.Read(TempString)){
                PrintError("Failed to get %d cursor point.\n", Index);
                goto LoadCursorsExit;
            }
            CTokenizer::Tokenize(Tokens, TempString);
            if(2 != Tokens.size()){
                PrintError("Invalid cursor points %s (%zd).\n",TempString.c_str(), Tokens.size());
                for(int I = 0; I < Tokens.size(); I++){
                    PrintError("Tokens[%d] = %s\n",I,Tokens[I].c_str());
                }
                goto LoadCursorsExit;
            }
            DCursorXPoint.push_back(std::stoi(Tokens[0]));
            DCursorYPoint.push_back(std::stoi(Tokens[1]));
        }
    }
    catch(std::exception &E){
        PrintError("%s\n",E.what());
    }
    ReturnStatus = true;
LoadCursorsExit:
    return ReturnStatus;
}

void CCursorSet::DrawCursor(std::shared_ptr<CGraphicSurface> surface, int xpos, int ypos, int cursorindex){
    if((0 > cursorindex)||(cursorindex >= DTileCount)){
        return;
    }
    
    DrawTile(surface, xpos - DCursorXPoint[cursorindex], ypos - DCursorYPoint[cursorindex], cursorindex);
}

