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
#include "SoundClip.h"
#include <cstdint>
#include <vector>
#include <sndfile.h>
#include <mpg123.h>
#include <cstring>
                
class CSFVirtualIODataSource{
    protected:
      std::vector< uint8_t > DData;
      sf_count_t DOffset;
      
      static sf_count_t GetFileLength(CSFVirtualIODataSource *iosource);
      static sf_count_t Seek(sf_count_t offset, int whence, CSFVirtualIODataSource *iosource);
      static sf_count_t Read(void *ptr, sf_count_t count, CSFVirtualIODataSource *iosource);
      static sf_count_t Write(const void *ptr, sf_count_t count, CSFVirtualIODataSource *iosource);
      static sf_count_t Tell(CSFVirtualIODataSource *iosource);
      
    public:
        CSFVirtualIODataSource(std::shared_ptr< CDataSource > source);
        
        SF_VIRTUAL_IO SFVirtualIO() const;
};                                     

CSFVirtualIODataSource::CSFVirtualIODataSource(std::shared_ptr< CDataSource > source){
    uint8_t TempByte;
    
    while(0 < source->Read(&TempByte, 1)){
        DData.push_back(TempByte);   
    }
    DOffset = 0;
}

SF_VIRTUAL_IO CSFVirtualIODataSource::SFVirtualIO() const{
    SF_VIRTUAL_IO ReturnIO;
    
    ReturnIO.get_filelen = (sf_vio_get_filelen)GetFileLength;   
    ReturnIO.seek = (sf_vio_seek)Seek;
    ReturnIO.read = (sf_vio_read)Read;
    ReturnIO.write = (sf_vio_write)Write;   
    ReturnIO.tell = (sf_vio_tell)Tell;   
    return ReturnIO;
}

sf_count_t CSFVirtualIODataSource::GetFileLength(CSFVirtualIODataSource *iosource){
    return iosource->DData.size();
}

sf_count_t CSFVirtualIODataSource::Seek(sf_count_t offset, int whence, CSFVirtualIODataSource *iosource){
    if(SEEK_SET == whence){
        iosource->DOffset = offset;
    }
    else if(SEEK_CUR == whence){
        iosource->DOffset += offset;
    }
    else if(SEEK_END == whence){
        iosource->DOffset = iosource->DData.size() + offset;        
    }
    if(0 > iosource->DOffset){
        iosource->DOffset = 0;    
    }
    else if(iosource->DData.size() < iosource->DOffset){
        iosource->DOffset = iosource->DData.size();
    }
    return iosource->DOffset;
}

sf_count_t CSFVirtualIODataSource::Read(void *ptr, sf_count_t count, CSFVirtualIODataSource *iosource){
    if(iosource->DOffset + count > iosource->DData.size()){
        count = iosource->DData.size() - iosource->DOffset;
    }
    memcpy(ptr, iosource->DData.data() + iosource->DOffset, count);
    iosource->DOffset += count;
    return count;
}

sf_count_t CSFVirtualIODataSource::Write(const void *ptr, sf_count_t count, CSFVirtualIODataSource *iosource){
    return 0;
}

sf_count_t CSFVirtualIODataSource::Tell(CSFVirtualIODataSource *iosource){
    return iosource->DOffset;
}


CSoundClip::CLibraryInitializer::CLibraryInitializer(){
    mpg123_init();
}

CSoundClip::CLibraryInitializer::~CLibraryInitializer(){
    mpg123_exit();
}

CSoundClip::CLibraryInitializer &CSoundClip::LibraryReference(){
    static CLibraryInitializer MyLibraryInitializer;
    
    return MyLibraryInitializer;
}


CSoundClip::CSoundClip(){
    DChannels = 0;
    DTotalFrames = 0;
    DSampleRate = 0;
}

CSoundClip::CSoundClip(const CSoundClip &clip){
    DChannels = clip.DChannels;
    DTotalFrames = clip.DTotalFrames;
    DSampleRate = clip.DSampleRate;
    DData = clip.DData;
}

CSoundClip::~CSoundClip(){
    
}

CSoundClip &CSoundClip::operator=(const CSoundClip &clip){
    if(&clip != this){
        DChannels = clip.DChannels;
        DTotalFrames = clip.DTotalFrames;
        DSampleRate = clip.DSampleRate;
        DData = clip.DData;
    }
    return *this;
}

bool CSoundClip::Load(std::shared_ptr< CDataSource > source, bool ismp3){
    if(ismp3){
        std::vector< unsigned char > DataBuffer;
        uint8_t TempByte;
        mpg123_handle *MPG123Handle;
        int ReturnValue;
        bool ReturnStatus = false;
        
        LibraryReference();
        
        while(0 < source->Read(&TempByte, 1)){
            DataBuffer.push_back(TempByte);   
        }
        
        MPG123Handle = mpg123_new(NULL, &ReturnValue);
        if(NULL == MPG123Handle){
            return false;   
        }
        
        ReturnValue = mpg123_param(MPG123Handle, MPG123_REMOVE_FLAGS, MPG123_IGNORE_INFOFRAME, 0.0);
        if(MPG123_OK == ReturnValue){
            ReturnValue = mpg123_param(MPG123Handle, MPG123_ADD_FLAGS, MPG123_FORCE_FLOAT, 0.0);
            if(MPG123_OK == ReturnValue){
                ReturnValue = mpg123_open_feed(MPG123Handle);
                if(MPG123_OK == ReturnValue){
                    long Rate;
                    int Channels, Encoding;
                    size_t BytesRead;
                    
                    ReturnValue = mpg123_feed(MPG123Handle, DataBuffer.data(), DataBuffer.size());
                    if(MPG123_OK == ReturnValue){
                        ReturnValue = mpg123_getformat(MPG123Handle, &Rate, &Channels, &Encoding);
                        
                        if(MPG123_OK == ReturnValue){
                            DChannels = 2;
                            DSampleRate = Rate;
                            if(sizeof(float) == mpg123_encsize(Encoding)){
                                float Buffer[1024];
                                DData.clear();
                                while(((ReturnValue = mpg123_read(MPG123Handle, (unsigned char *)Buffer, sizeof(Buffer), &BytesRead)) == MPG123_OK) && (BytesRead > 0)){
                                    int Samples = BytesRead / sizeof(float);
                                    if(1 == Channels){
                                        for(int Index = 0; Index < Samples; Index++){
                                            DData.push_back(Buffer[Index]);
                                            DData.push_back(Buffer[Index]);
                                        }
                                    }
                                    else{
                                        DData.insert(DData.end(), Buffer, Buffer + Samples);
                                    }
                                } 
                                DTotalFrames = DData.size() / 2;
                                ReturnStatus = true;
                            }
                            else if(sizeof(double) == mpg123_encsize(Encoding)){
                                double Buffer[1024];
                                DData.clear();
                                while(((ReturnValue = mpg123_read(MPG123Handle, (unsigned char *)Buffer, sizeof(Buffer), &BytesRead)) == MPG123_OK) && (BytesRead > 0)){
                                    int Samples = BytesRead / sizeof(float);
                                    if(1 == Channels){
                                        for(int Index = 0; Index < Samples; Index++){
                                            DData.push_back(Buffer[Index]);
                                            DData.push_back(Buffer[Index]);
                                        }
                                    }
                                    else{
                                        for(int Index = 0; Index < Samples; Index++){
                                            DData.push_back(Buffer[Index]);
                                        }
                                    }
                                } 
                                DTotalFrames = DData.size() / 2;
                                ReturnStatus = true;
                            }
                        }
                    }
                    mpg123_close(MPG123Handle);
                }
            }
        }
        mpg123_delete(MPG123Handle);
        return ReturnStatus;
    }
    else{
        CSFVirtualIODataSource VIODataSource(source);
        SF_VIRTUAL_IO SFVirtualIO = VIODataSource.SFVirtualIO();
        SNDFILE *SoundFilePtr;
        SF_INFO SoundFileInfo;
        
        //SoundFilePtr = sf_open(filename.c_str(), SFM_READ, &SoundFileInfo);
        SoundFilePtr = sf_open_virtual(&SFVirtualIO, SFM_READ, &SoundFileInfo, (void *)&VIODataSource);
        if(NULL == SoundFilePtr){
            return false;
        }
        if(1 == SoundFileInfo.channels){
            DChannels = 2;
            DTotalFrames = SoundFileInfo.frames;
            DSampleRate = SoundFileInfo.samplerate;
            DData.resize(SoundFileInfo.frames * 2);
            for(int Frame = 0; Frame < DTotalFrames; Frame++){
                sf_readf_float(SoundFilePtr, DData.data() + Frame * 2, 1);   
                DData[Frame * 2 + 1] = DData[Frame * 2];
            }
        }
        else if(2 == SoundFileInfo.channels){
            DChannels = 2;
            DTotalFrames = SoundFileInfo.frames;
            DSampleRate = SoundFileInfo.samplerate;
            DData.resize(SoundFileInfo.frames * SoundFileInfo.channels);
            sf_readf_float(SoundFilePtr, DData.data(), DTotalFrames);
        }
        else{
            sf_close(SoundFilePtr);
            return false;    
        }
        
        sf_close(SoundFilePtr);
    }
    return true;
}
    
void CSoundClip::CopyStereoClip(float *data, int offset, int frames){
    if(offset + frames > DTotalFrames){
        int FramesToCopy = DTotalFrames - offset;
        
        if(0 > FramesToCopy){
            FramesToCopy = 0;
        }
        if(FramesToCopy){
            memcpy(data, DData.data() + (offset * 2), sizeof(float) * FramesToCopy * 2);
        }
        memset(data, 0, sizeof(float) * (frames - FramesToCopy) * 2);
    }
    else{
        memcpy(data, DData.data() + (offset * 2), sizeof(float) * frames * 2);
    }
}

void CSoundClip::MixStereoClip(float *data, int offset, int frames, float volume, float rightbias, bool loop){
    const float *DataPointer = DData.data() + (offset * 2);
    int FramesToMix = frames;
    
    if(!DTotalFrames){
        return;   
    }
    
    if(loop){
        offset = offset % DTotalFrames;
        DataPointer = DData.data() + (offset * 2);
         for(int Frame = 0; Frame < FramesToMix; Frame++){
            *data++ += volume * (1.0 - rightbias) * *DataPointer++;
            *data++ += volume * (1.0 + rightbias) * *DataPointer++;
            offset = (offset + 1) % DTotalFrames;
            if(!offset){
               DataPointer = DData.data();      
            }
        }  
    }
    else{
        DataPointer = DData.data() + (offset * 2);
        if(offset + frames > DTotalFrames){
            FramesToMix = DTotalFrames - offset;
            if(0 > FramesToMix){
                FramesToMix = 0;
            }
        }
        for(int Frame = 0; Frame < FramesToMix; Frame++){
            *data++ += volume * (1.0 - rightbias) * *DataPointer++;
            *data++ += volume * (1.0 + rightbias) * *DataPointer++;
        }
    }
}

