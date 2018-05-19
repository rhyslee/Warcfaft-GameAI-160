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
#ifndef GUIFACTORY_H
#define GUIFACTORY_H
#include <memory>
#include <string>
#include "GUIApplication.h"
#include "GUIDisplay.h"
#include "GUIMenu.h"
#include "GUIBox.h"
#include "GUIMenuItem.h"
#include "GUIMenuBar.h"
#include "GUIWindow.h"
#include "GUIFileChooserDialog.h"
#include "GUIDrawingArea.h"

class CGUIFactory{     
    protected:
        static std::shared_ptr<CGUIApplication> DApplicationPointer;
    
    public:
        static std::shared_ptr<CGUIApplication> ApplicationInstance(const std::string &appname = "");
        static std::shared_ptr<CGUIDisplay> DefaultDisplay();
        static std::shared_ptr<CGUIDrawingArea> NewDrawingArea();
        static std::shared_ptr<CGUIBox> NewBox(CGUIBox::EOrientation orientation, int spacing);
        static std::shared_ptr<CGUIMenu> NewMenu();
        static std::shared_ptr<CGUIMenuBar> NewMenuBar();
        static std::shared_ptr<CGUIMenuItem> NewMenuItem(const std::string &label);
        static std::shared_ptr<CGUIFileFilter> NewFileFilter();
        static std::shared_ptr<CGUIFileChooserDialog> NewFileChooserDialog(const std::string &title, bool openfile, std::shared_ptr<CGUIWindow> parent);
};

#endif

