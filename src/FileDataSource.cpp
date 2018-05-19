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
*
* @class CFileDataSource
*
* @brief This class provides an encapsulation for reading to files. 
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

#include "FileDataSource.h"
#include "FileDataContainer.h"
#include "Path.h"
#include <unistd.h>
#include <fcntl.h>
#include <cstdio>

/**
 * Creates a data source for reading from files. If handed a valid file
 * descriptor (meaning a valid open file), then it won't try to reopen
 * the file.
 * 
 * @param[in] filename The name of file to open for reading.
 * @param[in] fd       The file descriptor for an open file or -1 by default
 * 
 * @return Nothing.
 * 
 */

CFileDataSource::CFileDataSource(const std::string &filename, int fd) : CDataSource(){
    DFullPath = CPath::CurrentPath().Simplify(filename).ToString();

    DFileHandle = fd;
    if(DFullPath.length() && (0 > fd)){
        DFileHandle = open(DFullPath.c_str(), O_RDONLY);
        DCloseFile = true;
    }
}

/** 
 * Closes the current file if open.
 * 
 * @return Nothing.
 * 
 */

CFileDataSource::~CFileDataSource(){
    if(DCloseFile && (0 <= DFileHandle)){
        close(DFileHandle);
    }
}

/**
 * Reads from the current open file.
 * 
 * @param[in] data   The buffer to store data read from the file.
 * @param[in] length The number of bytes to read.
 * 
 * @return Number of bytes read.
 * 
 */ 

int CFileDataSource::Read(void *data, int length){
    if(0 <= DFileHandle){
        int BytesRead = read(DFileHandle, data, length);

        if(0 < BytesRead){
            return BytesRead;
        }
    }
    return -1;
}

/**
 * Creates a directory data container for I/O for the current path.
 * 
 * @return A new directory data container.
 * 
 */

std::shared_ptr< CDataContainer > CFileDataSource::Container(){
    std::string ContainerName = CPath(DFullPath).Containing().ToString();

    if(ContainerName.length()){
        return std::make_shared< CDirectoryDataContainer >(ContainerName);

    }
    return nullptr;
}
