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
#ifndef GUIWINDOW_H
#define GUIWINDOW_H
#include "GUIContainer.h"

class CGUIWindow : public virtual CGUIContainer{
    public:
        virtual ~CGUIWindow(){};
        
        virtual void SetMinSize(int width, int height) = 0;
        virtual void SetMaxSize(int width, int height) = 0;
        virtual void SetMinSize(std::shared_ptr<CGUIWidget> widget, int width, int height) = 0;
        virtual void SetMaxSize(std::shared_ptr<CGUIWidget> widget, int width, int height) = 0;
        virtual void SetTitle(const std::string &title) = 0;
        virtual void Close() = 0;
};

#endif

