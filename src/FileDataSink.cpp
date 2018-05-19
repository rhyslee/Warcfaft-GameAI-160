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
* @class CFileDataSink
*
* @brief This class provides an encapsulation for writing to files. 
* 
* @author Felix
* 
* @version 4.06
* 
* @date 10/2/2017
* 
* Contact: fple@ucdavis.edu
* 
*/

#include "FileDataSink.h"
#include "FileDataContainer.h"
#include "Path.h"
#include <unistd.h>
#include <fcntl.h>
#include <cstdio>

/**
 * Creates a data sink for writing to files. If handed a valid file
 * descriptor (meaning a valid open file), then it won't try to reopen
 * the file.
 * 
 * @param[in] filename The name of file to open for writing.
 * @param[in] fd       The file descriptor for an open file or -1 by default
 * 
 * @return Nothing.
 * 
 */

CFileDataSink::CFileDataSink(const std::string &filename, int fd) : CDataSink(){
    DFullPath = CPath::CurrentPath().Simplify(filename).ToString();

    DFileHandle = fd;
    if(DFullPath.length() && (0 > fd)){
        DFileHandle = open(DFullPath.c_str(), O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
        
        DCloseFile = true;
    }
}

/** 
 * Closes the current file if open.
 * 
 * @return Nothing.
 * 
 */

CFileDataSink::~CFileDataSink(){
    if(DCloseFile && (0 <= DFileHandle)){
        close(DFileHandle);   
    }
}

/**
 * Writes to the current open file.
 * 
 * @param[in] data   The data to write to the file (can be of any type).
 * @param[in] length The number of bytes to write.
 * 
 * @return Number of bytes written.
 * 
 */ 

int CFileDataSink::Write(const void *data, int length){
    if(0 <= DFileHandle){
        int BytesWritten = write(DFileHandle, data, length);
        
        if(0 < BytesWritten){
            return BytesWritten;   
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

std::shared_ptr< CDataContainer > CFileDataSink::Container(){
    std::string ContainerName = CPath(DFullPath).Containing().ToString();
    
    if(ContainerName.length()){
        return std::make_shared< CDirectoryDataContainer >(ContainerName);
        
    }
    return nullptr;
}



