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
#ifndef GUIMENUITEM_H
#define GUIMENUITEM_H
#include "GUIContainer.h"
#include "GUILabel.h"

class CGUIMenuItem : public virtual CGUIContainer{
    public:
        virtual ~CGUIMenuItem(){};
        
        virtual std::shared_ptr<CGUILabel> GetLabel() = 0;
        virtual void SetSubMenu(std::shared_ptr<CGUIWidget> widget) = 0;
};

#endif

