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
#ifndef SOUNDEVENTRENDERER_H
#define SOUNDEVENTRENDERER_H
#include "SoundLibraryMixer.h"
#include "GameModel.h"
#include <vector>
#include <unordered_map>

class CSoundEventRenderer{
    protected:
        CRandomNumberGenerator DRandomNumberGenerator;
        float DVolume;
        std::shared_ptr< CSoundLibraryMixer > DSoundMixer;
        std::shared_ptr< CPlayerData > DPlayer;
        std::vector< SGameEvent > DDelayedEvents;
        int DPlaceIndex;
        int DTickIndex;
        int DTockIndex;
        std::vector< int > DDelayedSelectionIndices;
        std::vector< int > DDelayedAcknowledgeIndices;
        std::vector< int > DConstructIndices;
        std::vector< std::vector< int > > DWorkCompleteIndices;
        std::vector< std::vector< int > > DSelectionIndices;
        std::vector< std::vector< int > > DAcknowledgeIndices;
        std::vector< std::vector< int > > DReadyIndices;
        std::vector< std::vector< int > > DDeathIndices;
        std::vector< std::vector< int > > DAttackedIndices;
        std::vector< std::vector< int > > DMissleFireIndices;
        std::vector< std::vector< int > > DMissleHitIndices;
        std::vector< std::vector< int > > DHarvestIndices;
        std::vector< std::vector< int > > DQuarryIndices;
        std::vector< std::vector< int > > DMeleeHitIndices;
        
        static float RightBias(const SRectangle &viewportrect, const CPosition &position);
        static bool OnScreen(const SRectangle &viewportrect, const CPosition &position);
        
    public:
        CSoundEventRenderer(std::shared_ptr< CSoundLibraryMixer > mixer, std::shared_ptr< CPlayerData > player);
        
        float Volume() const{
            return DVolume;
        };
        
        float Volume(int vol){
            if((0.0 <= vol)&&(1.0 >= vol)){
                DVolume = vol;   
            }
            return DVolume;
        };
        
        void RenderEvents(const SRectangle &viewportrect);
};

#endif

