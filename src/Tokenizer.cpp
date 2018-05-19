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
#include "Tokenizer.h"

CTokenizer::CTokenizer(std::shared_ptr< CDataSource > source, const std::string delimiters){
    DDataSource = source;
    if(delimiters.length()){
        DDelimiters = delimiters;
    }
    else{
        DDelimiters = " \t\r\n";
    }
}

bool CTokenizer::Read(std::string &token){
    char TempChar;
    
    token.clear();
    while(true){
        if(0 < DDataSource->Read(&TempChar, 1)){
            if(std::string::npos != DDelimiters.find(TempChar)){
                token += TempChar;
            }
            else if(token.length()){
                return true;
            }
        }
        else{
            return 0 < token.length();
        }
    }
}

void CTokenizer::Tokenize(std::vector< std::string > &tokens, const std::string &data, const std::string delimiters){
    std::string Delimiters, TempString;
    
    if(delimiters.length()){
        Delimiters = delimiters;
    }
    else{
        Delimiters = " \t\r\n";
    }
    tokens.clear();
    for(size_t Index = 0; Index < data.length(); Index++){
        if(std::string::npos == Delimiters.find(data[Index])){
            TempString += data[Index];
        }
        else if(TempString.length()){
            tokens.push_back(TempString);
            TempString.clear();
        }
    }
    if(TempString.length()){
        tokens.push_back(TempString);
    }
}

