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
#ifndef TOKENIZER_H
#define TOKENIZER_H

#include "DataSource.h"
#include <string>
#include <vector>

class CTokenizer{
    protected:
        std::shared_ptr< CDataSource > DDataSource;
        std::string DDelimiters;
        
    public:
        CTokenizer(std::shared_ptr< CDataSource > source, const std::string delimiters = "");
        
        bool Read(std::string &token);
        static void Tokenize(std::vector< std::string > &tokens, const std::string &data, const std::string delimiters = "");
};

#endif
