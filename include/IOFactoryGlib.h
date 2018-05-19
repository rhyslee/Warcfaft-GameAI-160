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
#ifndef IOFACTORYGLIB_H
#define IOFACTORYGLIB_H
#include "IOFactory.h"
#include <glib.h>

class CIOChannelFileGlib : public CIOChannel, public std::enable_shared_from_this<CIOChannel>{
    protected:
        GIOChannel *DChannel;
        int DFileHandle;
        std::shared_ptr<CDataSource> DDataSource;
        std::shared_ptr<CDataSink> DDataSink;

        TIOCalldata DInCalldata;
        TIOCallback DInCallback;
        guint DInTag = 0;

        TIOCalldata DOutCalldata;
        TIOCallback DOutCallback;
        guint DOutTag = 0;

        TIOCalldata DPriorityCalldata;
        TIOCallback DPriorityCallback;
        guint DPriorityTag = 0;

        TIOCalldata DErrorCalldata;
        TIOCallback DErrorCallback;
        guint DErrorTag = 0;

        TIOCalldata DHangUpCalldata;
        TIOCallback DHangUpCallback;
        guint DHangUpTag = 0;

        TIOCalldata DInvalidCalldata;
        TIOCallback DInvalidCallback;
        guint DInvalidTag = 0;

        static gboolean InEventCallback(GIOChannel *source, GIOCondition condition, gpointer data);
        static gboolean OutEventCallback(GIOChannel *source, GIOCondition condition, gpointer data);
        static gboolean PriorityEventCallback(GIOChannel *source, GIOCondition condition, gpointer data);
        static gboolean ErrorEventCallback(GIOChannel *source, GIOCondition condition, gpointer data);
        static gboolean HangUpEventCallback(GIOChannel *source, GIOCondition condition, gpointer data);
        static gboolean InvalidEventCallback(GIOChannel *source, GIOCondition condition, gpointer data);


    public:
        CIOChannelFileGlib(int fd, uint32_t mode);
        virtual ~CIOChannelFileGlib();

        std::shared_ptr<CDataSource> CreateDataSouce() override;
        std::shared_ptr<CDataSink> CreateDataSink() override;

        void SetInEventCallback(TIOCalldata calldata, TIOCallback callback) override;
        void SetOutEventCallback(TIOCalldata calldata, TIOCallback callback) override;
        void SetPriorityEventCallback(TIOCalldata calldata, TIOCallback callback) override;
        void SetErrorEventCallback(TIOCalldata calldata, TIOCallback callback) override;
        void SetHangUpEventCallback(TIOCalldata calldata, TIOCallback callback) override;
        void SetInvalidEventCallback(TIOCalldata calldata, TIOCallback callback) override;

};

#endif
