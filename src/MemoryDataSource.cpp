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
 * @class MemoryDataSource
 *
 * @brief 
 *      MemoryDataSource creates an instance of CMemoryDataSource which reads data and determines the offset
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
#include "MemoryDataSource.h"
#include <cstring>

/**
 * Constructor to instantiate CLineDataSource object
 *
 * @param[in] src constant reference to character vector containing the source
 *
 * @return None
*/
CMemoryDataSource::CMemoryDataSource(const std::vector< char > &src){
    // Instantiate data with the passed vector
    DData = src;

    // Set the default offset to 0
    DOffset = 0;
}

/**
 * Reads the data that is passed into this function and returns the calculated length with offset
 *
 * @param[in] data void pointer containing the data to read
 * @param[in] length integer of length of data 
 *
 * @return length integer containing the length of the data with offset
*/
int CMemoryDataSource::Read(void *data, int length){
    if(length + DOffset > DData.size()){
        length = DData.size() - DOffset;   
    }
    if(length){
        memcpy(data, DData.data() + DOffset, length);
        DOffset += length;
    }
    
    return length;
}
