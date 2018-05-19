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
#ifndef GUIFACTORYGTK3_H
#define GUIFACTORYGTK3_H
#include "GUIFactory.h"
#include "PeriodicTimeout.h"
#include <gtk/gtk.h>

class CGUIApplicationGTK3 : public CGUIApplication{
    protected:
        GtkApplication *DApplication;
        TGUICalldata DActivateCalldata;
        TGUIApplicationCallback DActivateCallback;
        
        std::shared_ptr< CPeriodicTimeout > DPeriodicTimeout;
        int DTimerHandle;
        TGUICalldata DTimerCalldata;
        TGUITimeoutCallback DTimerCallback;
        
        static void ActivateCallback(GtkApplication* app, gpointer data);
        static gboolean TimeoutCallback(gpointer data);
        
    public:
        CGUIApplicationGTK3(const std::string &appname);
        virtual ~CGUIApplicationGTK3();
        
        virtual void SetActivateCallback(TGUICalldata calldata, TGUIApplicationCallback callback) override;
        virtual void SetTimer(int timems, TGUICalldata calldata, TGUITimeoutCallback callback) override;
        
        virtual void ProcessEvents(bool block) override;
        
        virtual int Run(int argc, char *argv[]) override;
        virtual void Quit() override;
        
        virtual std::shared_ptr<CGUIWindow> NewWindow() override;
};

class CGUIDisplayGTK3 : public CGUIDisplay{
    protected:
        GdkDisplay *DDisplay;
    public:
        CGUIDisplayGTK3(GdkDisplay *display);
        virtual ~CGUIDisplayGTK3();
        
        void Sync() override;
        void Flush() override;
        
        std::shared_ptr< CGUICursor > NewCursor(CGUICursor::ECursorType type) override;
};

class CGUICursorGTK3 : public CGUICursor{
    friend class CGUIWidgetGTK3;
    protected:
        GdkCursor *DCursor;
    public:
        
        CGUICursorGTK3(GdkCursor *cursor);
        virtual ~CGUICursorGTK3();
        
        ECursorType Type() override;
};

class CGUIWidgetGTK3 : public virtual CGUIWidget, public std::enable_shared_from_this<CGUIWidget>{
    protected:
        GtkWidget *DWidget;
        
        TGUICalldata DActivateCalldata;
        TGUIActivateEventCallback DActivateCallback;
        
        TGUICalldata DButtonPressCalldata;
        TGUIButtonEventCallback DButtonPressCallback;
        
        TGUICalldata DButtonReleaseCalldata;
        TGUIButtonEventCallback DButtonReleaseCallback;
        
        TGUICalldata DDeleteCalldata;
        TGUIDeleteEventCallback DDeleteCallback;
        
        TGUICalldata DDestroyCalldata;
        TGUIDestroyEventCallback DDestroyCallback;
        
        TGUICalldata DMotionCalldata;
        TGUIMotionEventCallback DMotionCallback;
        
        TGUICalldata DKeyPressCalldata; 
        TGUIKeyEventCallback DKeyPressCallback;
        
        TGUICalldata DKeyReleaseCalldata; 
        TGUIKeyEventCallback DKeyReleaseCallback;
        
        TGUICalldata DExposeCalldata;
        TGUIConfigureEventCallback DExposeCallback;
        
        TGUICalldata DConfigureCalldata;
        TGUIConfigureEventCallback DConfigureCallback;
        
        TGUICalldata DDrawCalldata;
        TGUIDrawEventCallback DDrawCallback;
        
        static void ActivateEventCallback(GtkWidget *widget, gpointer data);
        static gboolean ButtonPressEventCallback(GtkWidget *widget, GdkEventButton *event, gpointer data);
        static gboolean ButtonReleaseEventCallback(GtkWidget *widget, GdkEventButton *event, gpointer data);
        static gboolean DeleteEventCallback(GtkWidget *widget, GdkEvent *event, gpointer data);
        static void DestroyEventCallback(GtkWidget *widget, gpointer data);
        static gboolean MotionNotifyEventCallback(GtkWidget *widget, GdkEventMotion *event, gpointer data);
        static gboolean KeyPressEventCallback(GtkWidget *widget, GdkEventKey *event, gpointer data);
        static gboolean KeyReleaseEventCallback(GtkWidget *widget, GdkEventKey *event, gpointer data);
        static gboolean ExposeEventCallback(GtkWidget *widget, GdkEventExpose *event, gpointer data);
        static gboolean ConfigureEventCallback(GtkWidget *widget, GdkEventConfigure *event, gpointer data);
        static gboolean DrawEventCallback(GtkWidget *widget, cairo_t *cr, gpointer data);
        
    public:
        CGUIWidgetGTK3(GtkWidget *widget, bool reference = false);
        virtual ~CGUIWidgetGTK3();
        
        GtkWidget *Widget(){
            return DWidget;  
        };
        
        virtual void Show() override;
        virtual void ShowAll() override;
        
        virtual void Hide() override;
        
        virtual int AllocatedWidth() override;
        virtual int AllocatedHeight() override;
        
        virtual void Invalidate() override;
        
        virtual void SetCursor(std::shared_ptr< CGUICursor > cursor) override;
        
        virtual std::shared_ptr<CGraphicSurface> CreateSimilarSurface(int width, int height) override;
        virtual std::shared_ptr<CGraphicResourceContext> CreateResourceContext() override;
        
        virtual void EnableEvent(EGUIEvent event) override;
        virtual void DisableEvent(EGUIEvent event) override;
        
        virtual void SetActivateEventCallback(TGUICalldata calldata, TGUIActivateEventCallback callback) override;
        virtual void SetButtonPressEventCallback(TGUICalldata calldata, TGUIButtonEventCallback callback) override;
        virtual void SetButtonReleaseEventCallback(TGUICalldata calldata, TGUIButtonEventCallback callback) override;
        virtual void SetDeleteEventCallback(TGUICalldata calldata, TGUIDeleteEventCallback callback) override;
        virtual void SetDestroyEventCallback(TGUICalldata calldata, TGUIDestroyEventCallback callback) override;
        virtual void SetMotionEventCallback(TGUICalldata calldata, TGUIMotionEventCallback callback) override;
        virtual void SetKeyPressEventCallback(TGUICalldata calldata, TGUIKeyEventCallback callback) override;
        virtual void SetKeyReleaseEventCallback(TGUICalldata calldata, TGUIKeyEventCallback callback) override;
        virtual void SetExposeEventCallback(TGUICalldata calldata, TGUIConfigureEventCallback callback) override;
        virtual void SetConfigureEventCallback(TGUICalldata calldata, TGUIConfigureEventCallback callback) override;
        virtual void SetDrawEventCallback(TGUICalldata calldata, TGUIDrawEventCallback callback) override;
};

class CGUIContainerGTK3 : public virtual CGUIContainer, public CGUIWidgetGTK3{
    public:
        CGUIContainerGTK3(GtkWidget *widget, bool reference = false);
        virtual ~CGUIContainerGTK3();
        
        void SetBorderWidth(int width) override;
        void Add(std::shared_ptr<CGUIWidget> widget) override;
        void Remove(std::shared_ptr<CGUIWidget> widget) override;
};

class CGUILabelGTK3 : public virtual CGUILabel, public CGUIWidgetGTK3{
    public:
        CGUILabelGTK3(GtkWidget *widget, bool reference = false);
        virtual ~CGUILabelGTK3();
        
        std::string GetText() override;
        void SetText(const std::string &str) override;
        void SetMarkup(const std::string &str) override;
};

class CGUIDrawingAreaGTK3 : public virtual CGUIDrawingArea, public CGUIWidgetGTK3{
    public:
        CGUIDrawingAreaGTK3(GtkWidget *widget, bool reference = false);
        virtual ~CGUIDrawingAreaGTK3();
};

class CGUIBoxGTK3 : public virtual CGUIBox, public CGUIContainerGTK3{
    public:
        CGUIBoxGTK3(GtkWidget *widget, bool reference = false);
        virtual ~CGUIBoxGTK3();
        
        void PackStart(std::shared_ptr<CGUIWidget> widget, bool expand, bool fill, int padding) override;
        void PackEnd(std::shared_ptr<CGUIWidget> widget, bool expand, bool fill, int padding) override;
};

class CGUIMenuShellGTK3 : public virtual CGUIMenuShell, public CGUIContainerGTK3{
    public:
        CGUIMenuShellGTK3(GtkWidget *widget, bool reference = false);
        virtual ~CGUIMenuShellGTK3();
        
        void Append(std::shared_ptr<CGUIWidget> widget) override;
};

class CGUIMenuGTK3 : public virtual CGUIMenu, public CGUIMenuShellGTK3{
    public:
        CGUIMenuGTK3(GtkWidget *widget, bool reference = false);
        virtual ~CGUIMenuGTK3();
        
};

class CGUIMenuBarGTK3 : public virtual CGUIMenuBar, public CGUIMenuShellGTK3{
    public:
        CGUIMenuBarGTK3(GtkWidget *widget, bool reference = false);
        virtual ~CGUIMenuBarGTK3();
        
};

class CGUIMenuItemGTK3 : public virtual CGUIMenuItem, public CGUIContainerGTK3{
    public:
        CGUIMenuItemGTK3(GtkWidget *widget, bool reference = false);
        virtual ~CGUIMenuItemGTK3();
        
        std::shared_ptr<CGUILabel> GetLabel() override;
        void SetSubMenu(std::shared_ptr<CGUIWidget> widget) override;
};

class CGUIWindowGTK3 : public virtual CGUIWindow, public CGUIContainerGTK3{
    public:
        CGUIWindowGTK3(GtkWidget *widget, bool reference = false);
        virtual ~CGUIWindowGTK3();
        
        void SetMinSize(int width, int height) override;
        void SetMaxSize(int width, int height) override;
        void SetMinSize(std::shared_ptr<CGUIWidget> widget, int width, int height) override;
        void SetMaxSize(std::shared_ptr<CGUIWidget> widget, int width, int height) override;
        void SetTitle(const std::string &title) override;
        void Close() override;
};

class CGUIFileFilterGTK3 : public virtual CGUIFileFilter{
    friend class CGUIFileChooserDialogGTK3;
    protected:
        GtkFileFilter *DFilter;
        
    public:
        CGUIFileFilterGTK3(GtkFileFilter *filter, bool reference = false);
        virtual ~CGUIFileFilterGTK3();
                
        void AddPattern(const std::string &pattern) override;
};

class CGUIFileChooserDialogGTK3 : public virtual CGUIFileChooserDialog, public CGUIWindowGTK3{
    public:
        CGUIFileChooserDialogGTK3(GtkWidget *widget, bool reference = false);
        virtual ~CGUIFileChooserDialogGTK3();
        
        void SetFilter(std::shared_ptr<CGUIFileFilter> filter) override;
        void SetCurrentFolder(const std::string &folder) override;
        std::string GetFilename() override;
        
        int Run() override;
};

#endif

