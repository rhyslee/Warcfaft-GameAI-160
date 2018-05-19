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

#include "FileDataContainer.h"
#include "FileDataSource.h"
#include "FileDataSink.h"
#include "Path.h"
#include <unistd.h>
#include <fcntl.h>
#include <cstdio>

/**
*
* @class CDirectoryDataContainerIterator
*
* @brief This class iterates through directories.
* 
* @author Felix
* 
* @version 4.0
* 
* @date 10/26/2017
* 
* Contact: fple@ucdavis.edu
* 
*/

/**
 * Opens a directory to iterate through.
 * 
 * @param[in] path The path of the directory to open for iteration.
 * @param[in] key Used to distinguish this constructor from the base constructor.
 * 
 * @return Nothing.
 * 
 */

CDirectoryDataContainerIterator::CDirectoryDataContainerIterator(const std::string &path, const SPrivateConstructorKey &key) : CDataContainerIterator(){
    DDirectory = opendir(path.c_str());
    if(nullptr != DDirectory){
        if(0 != readdir_r(DDirectory, &DEntry, &DEntryResult)){
            DEntry.d_name[0] = '\0';
            DEntryResult = nullptr;
        }
    }
}

/** 
 * Closes the directory currently iterating through.
 * 
 * @return Nothing.
 *
 */

CDirectoryDataContainerIterator::~CDirectoryDataContainerIterator(){
    if(nullptr != DDirectory){
        closedir(DDirectory);
    }
}

/**
 * Gets the name of the directory.
 *
 * @return The name of the directory.
 * 
 */

std::string CDirectoryDataContainerIterator::Name(){
    return DEntry.d_name;
}

/**
 * Determines if the current entry is a directory.
 * 
 * @return True if a directory, false otherwise.
 * 
 */

bool CDirectoryDataContainerIterator::IsContainer(){
    return DT_DIR == DEntry.d_type;
}

/** 
 * Determines if directory entry is valid.
 * 
 * @return True if entry is valid, false otherwise.
 * 
 */

bool CDirectoryDataContainerIterator::IsValid(){
    return (nullptr != DEntryResult) || ('\0' != DEntry.d_name[0]);
}

/** 
 * Iterates to the next entry in the directory.
 * 
 * @return Nothing.
 * 
 */

void CDirectoryDataContainerIterator::Next(){
    if(nullptr != DDirectory){
        if(nullptr == DEntryResult){
            DEntry.d_name[0] = '\0';
        }
        else{
            if(0 != readdir_r(DDirectory, &DEntry, &DEntryResult)){
                DEntry.d_name[0] = '\0';
                DEntryResult = nullptr;
            }
            if(nullptr == DEntryResult){
                DEntry.d_name[0] = '\0';
            }

        }
    }
}

/**
*
* @class CDirectoryDataContainer
*
* @brief This class is a container for directories and returns an interface
* for reading and writing files.
* 
* @author Felix
* 
* @version 4.0
* 
* @date 10/26/2017
* 
* Contact: fple@ucdavis.edu
* 
*/

/** 
 * Initializes the path to the directory for the directory container.
 * 
 * @param[in] path The path to a directory.
 * 
 * @return Nothing.
 * 
 */

CDirectoryDataContainer::CDirectoryDataContainer(const std::string &path) : CDataContainer(){
    DFullPath = CPath::CurrentPath().Simplify(path).ToString();
}

CDirectoryDataContainer::~CDirectoryDataContainer(){

}

/**
 * Creates an object to iterate through the directory.
 * 
 * @return A directory iterator.
 * 
 */

std::shared_ptr< CDataContainerIterator > CDirectoryDataContainer::First(){
    if(DFullPath.length()){
        return std::make_shared< CDirectoryDataContainerIterator >(DFullPath, CDirectoryDataContainerIterator::SPrivateConstructorKey());
    }
    return nullptr;
}

/**
 * Creates an interface for reading from a file.
 * 
 * @param[in] The file name in the current directory to open for reading.
 * 
 * @return A data source for file reading.
 * 
 */

std::shared_ptr< CDataSource > CDirectoryDataContainer::DataSource(const std::string &name){
    std::string FileName = CPath(DFullPath).Simplify(CPath(name)).ToString();

    // checks if file exists
    if(FileName.length() && (access(FileName.c_str(), F_OK) != -1)){
        return std::make_shared< CFileDataSource >(FileName);
    }
    return nullptr;
}

/**
 * Creates an interface for writing from a file.
 * 
 * @param[in] The file name in the current directory to open for writing.
 * 
 * @return A data sink for file writing.
 * 
 */

std::shared_ptr< CDataSink > CDirectoryDataContainer::DataSink(const std::string &name){
    std::string FileName = CPath(DFullPath).Simplify(CPath(name)).ToString();

    if(FileName.length()){
        return std::make_shared< CFileDataSink >(FileName);
    }
    return nullptr;
}

/**
 * Creates a directory container for the current path saved.
 * 
 * @return Directory data container.
 *
 */

std::shared_ptr< CDataContainer > CDirectoryDataContainer::Container(){
    std::string ContainerName = CPath(DFullPath).Containing().ToString();

    if(ContainerName.length()){
        return std::make_shared< CDirectoryDataContainer >(ContainerName);
    }
    return nullptr;
}

/**
 * Creates a directory container for the current path (simplified) saved.
 * 
 * @return Directory data container
 * 
 */

std::shared_ptr< CDataContainer > CDirectoryDataContainer::DataContainer(const std::string &name){
    std::string ContainerName = CPath(DFullPath).Simplify(CPath(name)).ToString();

    if(ContainerName.length()){
        return std::make_shared< CDirectoryDataContainer >(ContainerName);
    }
    return nullptr;
}
