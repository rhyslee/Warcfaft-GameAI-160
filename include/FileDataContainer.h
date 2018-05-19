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
#ifndef FILEDATACONTAINER_H
#define FILEDATACONTAINER_H

#include "DataContainer.h"
#include <dirent.h>

class CDirectoryDataContainerIterator : public CDataContainerIterator{
    friend class CDirectoryDataContainer;
    
    protected:
        struct SPrivateConstructorKey{};
        DIR *DDirectory;
        struct dirent DEntry;
        struct dirent *DEntryResult;
        
    public:  
        explicit CDirectoryDataContainerIterator(const std::string &path, const SPrivateConstructorKey &key);
        ~CDirectoryDataContainerIterator();
        std::string Name();
        bool IsContainer();
        bool IsValid();
        void Next();
};

class CDirectoryDataContainer : public CDataContainer{
    protected:
        std::string DFullPath;
        
    public:
        CDirectoryDataContainer(const std::string &path);
        ~CDirectoryDataContainer();
        
        std::shared_ptr< CDataContainerIterator > First() override;
        std::shared_ptr< CDataSource > DataSource(const std::string &name) override;
        std::shared_ptr< CDataSink > DataSink(const std::string &name)  override;
        std::shared_ptr< CDataContainer > Container()  override;
        std::shared_ptr< CDataContainer > DataContainer(const std::string &name)  override;
};

#endif
