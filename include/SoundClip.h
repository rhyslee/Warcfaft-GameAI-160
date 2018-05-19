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
#ifndef SOUNDCLIP_H
#define SOUNDCLIP_H

#include <string>
#include <vector>
#include "DataSource.h"

class CSoundClip{
    protected:
        int DChannels;
        int DTotalFrames;
        int DSampleRate;
        std::vector< float > DData;
        
        class CLibraryInitializer{
            public:  
                CLibraryInitializer();
                ~CLibraryInitializer();
        };
        
        static CLibraryInitializer &LibraryReference();
        
    public:
        CSoundClip();
        CSoundClip(const CSoundClip &clip);
        ~CSoundClip();
        
        CSoundClip &operator=(const CSoundClip &clip);
        
        int Channels() const{
            return DChannels;  
        };
        
        int TotalFrames() const{
            return DTotalFrames;  
        };
        
        int SampleRate() const{
            return DSampleRate;  
        };
        
        bool Load(std::shared_ptr< CDataSource > source, bool ismp3 = false);
            
        void CopyStereoClip(float *data, int offset, int frames);
        void MixStereoClip(float *data, int offset, int frames, float volume = 1.0, float rightbias = 0.0, bool loop = false);
        
            
};

#endif
