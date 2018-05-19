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
#ifndef GUIWIDGET_H
#define GUIWIDGET_H
#include "GUIEvent.h"
#include "GUICursor.h"
#include "GraphicSurface.h"

class CGUIWidget{
    public:
        virtual void Show() = 0;
        virtual void ShowAll() = 0;
        
        virtual void Hide() = 0;
        
        virtual int AllocatedWidth() = 0;
        virtual int AllocatedHeight() = 0;
        
        virtual void Invalidate() = 0;
        
        virtual void SetCursor(std::shared_ptr< CGUICursor > cursor) = 0;
        
        virtual std::shared_ptr<CGraphicSurface> CreateSimilarSurface(int width, int height) = 0;
        virtual std::shared_ptr<CGraphicResourceContext> CreateResourceContext() = 0;
        
        virtual void EnableEvent(EGUIEvent event) = 0;
        virtual void DisableEvent(EGUIEvent event) = 0;
        
        virtual void SetActivateEventCallback(TGUICalldata calldata, TGUIActivateEventCallback callback) = 0;
        virtual void SetButtonPressEventCallback(TGUICalldata calldata, TGUIButtonEventCallback callback) = 0;
        virtual void SetButtonReleaseEventCallback(TGUICalldata calldata, TGUIButtonEventCallback callback) = 0;
        virtual void SetDeleteEventCallback(TGUICalldata calldata, TGUIDeleteEventCallback callback) = 0;
        virtual void SetDestroyEventCallback(TGUICalldata calldata, TGUIDestroyEventCallback callback) = 0;
        virtual void SetMotionEventCallback(TGUICalldata calldata, TGUIMotionEventCallback callback) = 0;
        virtual void SetKeyPressEventCallback(TGUICalldata calldata, TGUIKeyEventCallback callback) = 0;
        virtual void SetKeyReleaseEventCallback(TGUICalldata calldata, TGUIKeyEventCallback callback) = 0;
        virtual void SetExposeEventCallback(TGUICalldata calldata, TGUIConfigureEventCallback callback) = 0;
        virtual void SetConfigureEventCallback(TGUICalldata calldata, TGUIConfigureEventCallback callback) = 0;
        virtual void SetDrawEventCallback(TGUICalldata calldata, TGUIDrawEventCallback callback) = 0;
};

#endif

