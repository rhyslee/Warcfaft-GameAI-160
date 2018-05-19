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
 * @class IOFactoryGlib
 *
 * @brief
 *     IOFactoryGlib involves four header files: IOChannel.h, IOEvent.h, IOFactory.h, and IOFactoryGlib.h
 *
 *     IOChannel.h defines the CIOChannel pure abstract class containing functions for calldata and callback
 *     IOEvent.h defines the SIOEventType object (note that it is a C++ struct) which can set, clear, or get status of events
 *     IOFactory.h defines the CIOFactory object which nests a SFileOptions object.
 *     IOFactoryGlib.h defines the CIOChannelFileGlib object which inherits from CIOChannel. 
 *
 * @author Christopher Ta
 *
 * @version 4.0
 *
 * @date 10/26/2017
 *
 * Contact: cta@ucdavis.edu
 *
*/
#include "IOFactoryGlib.h"
#include <fcntl.h>
#include "FileDataSource.h"
#include "FileDataSink.h"

// Local constants for SIOEventType object
const uint32_t SIOEventType::EventIn = G_IO_IN;
const uint32_t SIOEventType::EventOut = G_IO_OUT;
const uint32_t SIOEventType::EventPriority = G_IO_PRI;
const uint32_t SIOEventType::EventError = G_IO_ERR;
const uint32_t SIOEventType::EventHangUp = G_IO_HUP;
const uint32_t SIOEventType::EventInvalid = G_IO_NVAL;

/**
 * Sets the unsigned integer DValue to "bitwise inclusive or" with EventIn
 *
 * @param[in] None
 *
 * @return None*/
void SIOEventType::SetIn(){
    DValue |= EventIn;
}

/**
 * Sets the unsigned integer DValue to "bitwise inclusive or" with EventOut
 *
 * @param[in] None
 *
 * @return None*/
void SIOEventType::SetOut(){
    DValue |= EventOut;
}

/**
 * Sets the unsigned integer DValue to "bitwise inclusive or" with EventPriority
 *
 * @param[in] None
 *
 * @return None*/
void SIOEventType::SetPriority(){
    DValue |= EventPriority;
}

/**
 * Sets the unsigned integer DValue to "bitwise inclusive or" with EventError
 *
 * @param[in] None
 *
 * @return None*/
void SIOEventType::SetError(){
    DValue |= EventError;
}

/**
 * Sets the unsigned integer DValue to "bitwise inclusive or" with EventHangUp
 *
 * @param[in] None
 *
 * @return None*/
void SIOEventType::SetHangUp(){
    DValue |= EventHangUp;
}

/**
 * Sets the unsigned integer DValue to "bitwise inclusive or" with EventInvalid
 *
 * @param[in] None
 *
 * @return None*/
void SIOEventType::SetInvalid(){
    DValue |= EventInvalid;
}

/**
 * Sets the unsigned integer DValue to "bitwise and" with ~EventIn
 *
 * @param[in] None
 *
 * @return None*/
void SIOEventType::ClearIn(){
    DValue &= ~EventIn;
}

/**
 * Sets the unsigned integer DValue to "bitwise and" with ~EventOut
 *
 * @param[in] None
 *
 * @return None*/
void SIOEventType::ClearOut(){
    DValue &= ~EventOut;
}

/**
 * Sets the unsigned integer DValue to "bitwise and" with ~EventPriority
 *
 * @param[in] None
 *
 * @return None*/
void SIOEventType::ClearPriority(){
    DValue &= ~EventPriority;
}

/**
 * Sets the unsigned integer DValue to "bitwise and" with ~EventError
 *
 * @param[in] None
 *
 * @return None*/
void SIOEventType::ClearError(){
    DValue &= ~EventError;
}

/**
 * Sets the unsigned integer DValue to "bitwise and" with ~EventHangUp
 *
 * @param[in] None
 *
 * @return None*/
void SIOEventType::ClearHangUp(){
    DValue &= ~EventHangUp;
}

/**
 * Sets the unsigned integer DValue to "bitwise and" with ~EventInvalid
 *
 * @param[in] None
 *
 * @return None*/
void SIOEventType::ClearInvalid(){
    DValue &= ~EventInvalid;
}

/**
 * Calculates the unsigned integer DValue to "bitwise and" with EventIn
 *
 * @param[in] None
 *
 * @return DValue & EventIn boolean
*/
bool SIOEventType::IsIn() const{
    return DValue & EventIn;
}

/**
 * Calculates the unsigned integer DValue to "bitwise and" with EventOut
 *
 * @param[in] None
 *
 * @return DValue & EventOut boolean
*/
bool SIOEventType::IsOut() const{
    return DValue & EventOut;
}

/**
 * Calculates the unsigned integer DValue to "bitwise and" with EventPriority
 *
 * @param[in] None
 *
 * @return DValue & EventPriority boolean
*/
bool SIOEventType::IsPriority() const{
    return DValue & EventPriority;
}

/**
 * Calculates the unsigned integer DValue to "bitwise and" with EventError
 *
 * @param[in] None
 *
 * @return DValue & EventError boolean
*/
bool SIOEventType::IsError() const{
    return DValue & EventError;
}

/**
 * Calculates the unsigned integer DValue to "bitwise and" with EventHangUp
 *
 * @param[in] None
 *
 * @return DValue & EventHangup boolean
*/
bool SIOEventType::IsHangUp() const{
    return DValue & EventHangUp;
}

/**
 * Calculates the unsigned integer DValue to "bitwise and" with EventInvalid
 *
 * @param[in] None
 *
 * @return DValue & EventInvalid boolean
*/
bool SIOEventType::IsInvalid() const{
    return DValue & EventInvalid;
}

// Local constants for CIOFactory object
const uint32_t CIOFactory::SFileOptions::ReadOnly = O_RDONLY;
const uint32_t CIOFactory::SFileOptions::WriteOnly = O_WRONLY;
const uint32_t CIOFactory::SFileOptions::ReadWrite = O_RDWR;
const uint32_t CIOFactory::SFileOptions::Append = O_APPEND;
const uint32_t CIOFactory::SFileOptions::Truncate = O_TRUNC;
const uint32_t CIOFactory::SFileOptions::Create = O_CREAT;
const uint32_t CIOFactory::SFileOptions::Exclusive = O_EXCL;

/**
 * Sets the unsigned integer DValue permission to read only
 *
 * @param[in] None
 *
 * @return None
*/
void CIOFactory::SFileOptions::SetReadOnly(){
    DValue &= ~WriteOnly;
    DValue &= ~ReadWrite;
    DValue |= ReadOnly;
}

/**
 * Sets the unsigned integer DValue permission to write only
 *
 * @param[in] None
 *
 * @return None
*/
void CIOFactory::SFileOptions::SetWriteOnly(){
    DValue &= ~ReadOnly;
    DValue &= ~ReadWrite;
    DValue |= WriteOnly;
}

/**
 * Sets the unsigned integer DValue permissions to read and write
 *
 * @param[in] None
 *
 * @return None
*/
void CIOFactory::SFileOptions::SetReadWrite(){
    DValue &= ~ReadOnly;
    DValue &= ~WriteOnly;
    DValue |= ReadWrite;
}

/**
 * Sets the unsigned integer DValue permission to append
 *
 * @param[in] None
 *
 * @return None
*/
void CIOFactory::SFileOptions::SetAppend(){
    DValue |= Append;
}

/**
 * Sets the unsigned integer DValue permission to truncate only
 *
 * @param[in] None
 *
 * @return None
*/
void CIOFactory::SFileOptions::SetTruncate(){
    DValue |= Truncate;
}

/**
 * Sets the unsigned integer DValue permission to create only
 *
 * @param[in] None
 *
 * @return None
*/
void CIOFactory::SFileOptions::SetCreate(){
    DValue |= Create;
}

/**
 * Sets the unsigned integer DValue permission to exclusive only
 *
 * @param[in] None
 *
 * @return None
*/
void CIOFactory::SFileOptions::SetExclusive(){
    DValue |= Exclusive;
}

/**
 * EMPTY FUNCTION: Should set the unsigned integer DValue permission to clear and read only
 *
 * @param[in] None
 *
 * @return None
*/
void CIOFactory::SFileOptions::ClearReadOnly(){

}

/**
 * EMPTY FUNCTION: Should set the unsigned integer DValue permission to clear and write only
 *
 * @param[in] None
 *
 * @return None
*/
void CIOFactory::SFileOptions::ClearWriteOnly(){

}

/**
 * EMPTY FUNCTION: Should set the unsigned integer DValue permission to clear, read, and write only
 *
 * @param[in] None
 *
 * @return None
*/
void CIOFactory::SFileOptions::ClearReadWrite(){

}

/**
 * Sets the unsigned integer DValue permission to clear and append only
 *
 * @param[in] None
 *
 * @return None
*/
void CIOFactory::SFileOptions::ClearAppend(){
    DValue &= ~Append;
}

/**
 * Sets the unsigned integer DValue permission to clear and truncate only
 *
 * @param[in] None
 *
 * @return None
*/
void CIOFactory::SFileOptions::ClearTruncate(){
    DValue &= ~Truncate;
}

/**
 * Sets the unsigned integer DValue permission to clear and create only
 *
 * @param[in] None
 *
 * @return None
*/
void CIOFactory::SFileOptions::ClearCreate(){
    DValue &= ~Create;
}

/**
 * Sets the unsigned integer DValue permission to clear and exclusive only
 *
 * @param[in] None
 *
 * @return None
*/
void CIOFactory::SFileOptions::ClearExclusive(){
    DValue &= ~Exclusive;
}

/**
 * Creates a new file input/output channel and returns a shared pointer with the specified filename and option settings
 *
 * @param[in] filename Constant reference to a std::string
 * @param[in] options Constant reference to a SFileOptions object
 *
 * @return std::shared_ptr<CIOChannel>
*/
std::shared_ptr<CIOChannel> CIOFactory::NewFileIOChannel(const std::string &filename, const struct SFileOptions &options){
    return std::make_shared<CIOChannelFileGlib>(open(filename.c_str(), options.DValue, 0600), options.DValue);
}

/**
 * Constructor to initialize a CIOChannelFileGlib object
 *
 * @param[in] fd integer
 * @param[in] mode unsigned integer
 *
 * @return None
*/
CIOChannelFileGlib::CIOChannelFileGlib(int fd, uint32_t mode){
    // Instantiate protected variables with fd parameter
    DChannel = g_io_channel_unix_new(fd);
    DFileHandle = fd;

    // Sets permissions with "bitwise and" from the SFileOptions
    mode &= CIOFactory::SFileOptions::ReadOnly | CIOFactory::SFileOptions::WriteOnly | CIOFactory::SFileOptions::ReadWrite;
    if((CIOFactory::SFileOptions::ReadOnly == mode)||(CIOFactory::SFileOptions::ReadWrite == mode)){
        DDataSource = std::make_shared<CFileDataSource>("", DFileHandle);
    }
    if((CIOFactory::SFileOptions::WriteOnly == mode)||(CIOFactory::SFileOptions::ReadWrite == mode)){
        DDataSink = std::make_shared<CFileDataSink>("", DFileHandle);
    }
}

/**
 * Destructor to unallocate a CIOChannelFileGlib object
 *
 * @param[in] None
 *
 * @return None
*/
CIOChannelFileGlib::~CIOChannelFileGlib(){
    GError *Error = NULL;
    DDataSource = nullptr;
    DDataSink = nullptr;

    // Terminate the IOChannel that was created for this object
    g_io_channel_shutdown(DChannel, TRUE, &Error);
    if(DInTag){
        g_source_remove(DInTag);
    }
    if(DOutTag){
        g_source_remove(DOutTag);
    }
    if(DPriorityTag){
        g_source_remove(DPriorityTag);
    }
    if(DErrorTag){
        g_source_remove(DErrorTag);
    }
    if(DHangUpTag){
        g_source_remove(DHangUpTag);
    }
    if(DInvalidTag){
        g_source_remove(DInvalidTag);
    }
    g_io_channel_unref(DChannel);
}

/**
 * Determines if using an InEventCallback callback
 *
 * @param[in] source Pointer to GIOChannel object
 * @param[in] condition GIOCondition object
 * @param[in] data gpointer object
 *
 * @return gboolean True if the callback is InEventCallback
*/
gboolean CIOChannelFileGlib::InEventCallback(GIOChannel *source, GIOCondition condition, gpointer data){
    // Cast the passed data as a channel
    CIOChannelFileGlib *Channel = static_cast<CIOChannelFileGlib *>(data);

    if(Channel->DInCallback){
        SIOEventType EventType;

        EventType.SetIn();
        if(Channel->DInCallback(Channel->shared_from_this(), EventType, Channel->DInCalldata)){
            return TRUE;
        }
    }
    Channel->DInTag = 0;
    return FALSE;
}

/**
 * Determines if using an OutEvent callback
 *
 * @param[in] source Pointer to GIOChannel object
 * @param[in] condition GIOCondition object
 * @param[in] data gpointer object
 *
 * @return gboolean True if the callback is OutEventCallback
*/
gboolean CIOChannelFileGlib::OutEventCallback(GIOChannel *source, GIOCondition condition, gpointer data){
    CIOChannelFileGlib *Channel = static_cast<CIOChannelFileGlib *>(data);

    if(Channel->DOutCallback){
        SIOEventType EventType;

        EventType.SetOut();
        if(Channel->DOutCallback(Channel->shared_from_this(), EventType, Channel->DOutCalldata)){
            return TRUE;
        }
    }
    Channel->DOutTag = 0;
    return FALSE;
}

/**
 * Determines if using a PriorityEvent callback
 *
 * @param[in] source Pointer to GIOChannel object
 * @param[in] condition GIOCondition object
 * @param[in] data gpointer object
 *
 * @return gboolean True if the callback is PriorityEventCallback
*/
gboolean CIOChannelFileGlib::PriorityEventCallback(GIOChannel *source, GIOCondition condition, gpointer data){
    CIOChannelFileGlib *Channel = static_cast<CIOChannelFileGlib *>(data);

    if(Channel->DPriorityCallback){
        SIOEventType EventType;

        EventType.SetPriority();
        if(Channel->DPriorityCallback(Channel->shared_from_this(), EventType, Channel->DPriorityCalldata)){
            return TRUE;
        }
    }
    Channel->DPriorityTag = 0;
    return FALSE;
}

/**
 * Determines if using an ErrorEvent callback
 *
 * @param[in] source Pointer to GIOChannel object
 * @param[in] condition GIOCondition object
 * @param[in] data gpointer object
 *
 * @return gboolean True if the callback is ErrorEventCallback
*/
gboolean CIOChannelFileGlib::ErrorEventCallback(GIOChannel *source, GIOCondition condition, gpointer data){
    CIOChannelFileGlib *Channel = static_cast<CIOChannelFileGlib *>(data);

    if(Channel->DErrorCallback){
        SIOEventType EventType;

        EventType.SetError();
        if(Channel->DErrorCallback(Channel->shared_from_this(), EventType, Channel->DErrorCalldata)){
            return TRUE;
        }
    }
    Channel->DErrorTag = 0;
    return FALSE;
}

/**
 * Determines if using a HangUpEvent callback
 *
 * @param[in] source Pointer to GIOChannel object
 * @param[in] condition GIOCondition object
 * @param[in] data gpointer object
 *
 * @return gboolean True if the callback is HangUpEventCallback
*/
gboolean CIOChannelFileGlib::HangUpEventCallback(GIOChannel *source, GIOCondition condition, gpointer data){
    CIOChannelFileGlib *Channel = static_cast<CIOChannelFileGlib *>(data);

    if(Channel->DHangUpCallback){
        SIOEventType EventType;

        EventType.SetHangUp();
        if(Channel->DHangUpCallback(Channel->shared_from_this(), EventType, Channel->DHangUpCalldata)){
            return TRUE;
        }
    }
    Channel->DHangUpTag = 0;
    return FALSE;
}

/**
 * Determines if using an InvalidEvent callback
 *
 * @param[in] source Pointer to GIOChannel object
 * @param[in] condition GIOCondition object
 * @param[in] data gpointer object
 *
 * @return gboolean True if the callback is InvalidEventCallback
*/
gboolean CIOChannelFileGlib::InvalidEventCallback(GIOChannel *source, GIOCondition condition, gpointer data){
    CIOChannelFileGlib *Channel = static_cast<CIOChannelFileGlib *>(data);

    if(Channel->DInvalidCallback){
        SIOEventType EventType;

        EventType.SetInvalid();
        if(Channel->DInvalidCallback(Channel->shared_from_this(), EventType, Channel->DInvalidCalldata)){
            return TRUE;
        }
    }
    Channel->DInvalidTag = 0;
    return FALSE;
}

/**
 * Creates a new data source and returns the shared pointer containing the CDataSource object
 *
 * @param[in] None
 *
 * @return DDataSource shared pointer of DDataSource
*/
std::shared_ptr<CDataSource> CIOChannelFileGlib::CreateDataSouce(){
    return DDataSource;
}

/**
 * Creates a new data sink and returns the shared pointer containing the CDataSink object
 *
 * @param[in] None
 *
 * @return DDataSink shared pointer of DDataSink
*/
std::shared_ptr<CDataSink> CIOChannelFileGlib::CreateDataSink(){
    return DDataSink;
}

/**
 * Sets the callback to SetInEventCallback
 *
 * @param[in] calldata TIOCalldata object containing the data for this event
 * @param[in] callback TIOCallback object containing the callback for this event
 *
 * @return None
*/
void CIOChannelFileGlib::SetInEventCallback(TIOCalldata calldata, TIOCallback callback){
    DInCalldata = calldata;
    DInCallback = callback;
    if(callback){
        if(0 == DInTag){
            DInTag = g_io_add_watch(DChannel, G_IO_IN, InEventCallback, this);
        }
    }
    else if(DInTag){
        g_source_remove(DInTag);
        DInTag = 0;
    }
}

/**
 * Sets the callback to SetOutEventCallback
 *
 * @param[in] calldata TIOCalldata object containing the data for this event
 * @param[in] callback TIOCallback object containing the callback for this event
 *
 * @return None
*/
void CIOChannelFileGlib::SetOutEventCallback(TIOCalldata calldata, TIOCallback callback){
    DOutCalldata = calldata;
    DOutCallback = callback;
    if(callback){
        if(0 == DOutTag){
            DOutTag = g_io_add_watch(DChannel, G_IO_OUT, OutEventCallback, this);
        }
    }
    else if(DOutTag){
        g_source_remove(DOutTag);
        DOutTag = 0;
    }
}

/**
 * Sets the callback to SetPriorityEventCallback
 *
 * @param[in] calldata TIOCalldata object containing the data for this event
 * @param[in] callback TIOCallback object containing the callback for this event
 *
 * @return None
*/
void CIOChannelFileGlib::SetPriorityEventCallback(TIOCalldata calldata, TIOCallback callback){
    DPriorityCalldata = calldata;
    DPriorityCallback = callback;
    if(callback){
        if(0 == DPriorityTag){
            DPriorityTag = g_io_add_watch(DChannel, G_IO_PRI, PriorityEventCallback, this);
        }
    }
    else if(DPriorityTag){
        g_source_remove(DPriorityTag);
        DPriorityTag = 0;
    }
}

/**
 * Sets the callback to SetErrorEventCallback
 *
 * @param[in] calldata TIOCalldata object containing the data for this event
 * @param[in] callback TIOCallback object containing the callback for this event
 *
 * @return None
*/
void CIOChannelFileGlib::SetErrorEventCallback(TIOCalldata calldata, TIOCallback callback){
    DErrorCalldata = calldata;
    DErrorCallback = callback;
    if(callback){
        if(0 == DErrorTag){
            DErrorTag = g_io_add_watch(DChannel, G_IO_ERR, ErrorEventCallback, this);
        }
    }
    else if(DErrorTag){
        g_source_remove(DErrorTag);
        DErrorTag = 0;
    }
}

/**
 * Sets the callback to SetHangUpEventCallback
 *
 * @param[in] calldata TIOCalldata object containing the data for this event
 * @param[in] callback TIOCallback object containing the callback for this event
 *
 * @return None
*/
void CIOChannelFileGlib::SetHangUpEventCallback(TIOCalldata calldata, TIOCallback callback){
    DHangUpCalldata = calldata;
    DHangUpCallback = callback;
    if(callback){
        if(0 == DHangUpTag){
            DHangUpTag = g_io_add_watch(DChannel, G_IO_HUP, HangUpEventCallback, this);
        }
    }
    else if(DHangUpTag){
        g_source_remove(DHangUpTag);
        DHangUpTag = 0;
    }
}

/**
 * Sets the callback to SetInvalidEventCallback
 *
 * @param[in] calldata TIOCalldata object containing the data for this event
 * @param[in] callback TIOCallback object containing the callback for this event
 *
 * @return None
*/
void CIOChannelFileGlib::SetInvalidEventCallback(TIOCalldata calldata, TIOCallback callback){
    DInvalidCalldata = calldata;
    DInvalidCallback = callback;
    if(callback){
        if(0 == DInvalidTag){
            DInvalidTag = g_io_add_watch(DChannel, G_IO_NVAL, InvalidEventCallback, this);
        }
    }
    else if(DInvalidTag){
        g_source_remove(DInvalidTag);
        DInvalidTag = 0;
    }
}
