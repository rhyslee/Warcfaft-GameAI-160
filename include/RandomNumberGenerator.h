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
#ifndef RANDOMNUMBERGENERATOR_H
#define RANDOMNUMBERGENERATOR_H

#include <cstdint>

class CRandomNumberGenerator{
    protected:
        uint32_t DRandomSeedHigh;
        uint32_t DRandomSeedLow;
    public:
        CRandomNumberGenerator(){
            DRandomSeedHigh = 0x01234567;
            DRandomSeedLow = 0x89ABCDEF;
        };
        
        void Seed(uint64_t seed){
            Seed(seed>32, seed);
        };
        
        void Seed(uint32_t high, uint32_t low){
            if((high != low) && low && high){
                DRandomSeedHigh = high;
                DRandomSeedLow = low;   
            }
        };
        
        uint32_t Random(){
            DRandomSeedHigh = 36969 * (DRandomSeedHigh & 65535) + (DRandomSeedHigh >> 16);
            DRandomSeedLow = 18000 * (DRandomSeedLow & 65535) + (DRandomSeedLow >> 16);
            return (DRandomSeedHigh << 16) + DRandomSeedLow;
        };
};

#endif
