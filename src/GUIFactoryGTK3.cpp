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
#include "GUIFactoryGTK3.h"
#include "GraphicFactoryCairo.h"
#include <glib.h>
#include <glib/gi18n.h>
#include "Debug.h"

//The next four declarations assign unsigned integers to track mouse actions.

//Declares an unsigned integer to track if the mouse is clicked.
const uint32_t SGUIButtonEventType::DButtonPress = GDK_BUTTON_PRESS;

//Declares an unsigned integer to track if the mouse is double clicked.
const uint32_t SGUIButtonEventType::DDoubleButtonPress = GDK_2BUTTON_PRESS;

//Declares an unsigned integer to track if the mouse is clicked three times in quick succession.
const uint32_t SGUIButtonEventType::DTripleButtonPress = GDK_3BUTTON_PRESS;

//Declares an unsigned integer to track if the mouse button is released.
const uint32_t SGUIButtonEventType::DButtonRelease = GDK_BUTTON_RELEASE;

/**
* Returns true if mouse button is pressed.
*
* @param[in] Nothing.
*
* @return bool Tracks if mouse button has been pressed.
*
*/
bool SGUIButtonEventType::IsButtonPress() const{
    return DType == DButtonPress;
}

/**
* Returns true if mouse button is pressed twice.
*
* @param[in] Nothing.
*
* @return bool Tracks if mouse button has been pressed twice.
*
*/
bool SGUIButtonEventType::IsDoubleButtonPress() const{
    return DType == DDoubleButtonPress;
}

/**
* Returns true if mouse button is pressed three times.
*
* @param[in] Nothing.
*
* @return bool Tracks if mouse button has been pressed three times.
*
*/
bool SGUIButtonEventType::IsTripleButtonPress() const{
    return DType == DTripleButtonPress;
}

/**
* Returns true if mouse button is released.
*
* @param[in] Nothing.
*
* @return bool Tracks if mouse button has been released.
*
*/

bool SGUIButtonEventType::IsButtonRelease() const{
    return DType == DButtonRelease;
}

//The next seven declarations assign more unsigned integers to deal with actions that modify other actions.

//Assigns an unsigned int to track if the Shift key is used to modify another key press.
const uint32_t SGUIModifierType::Shift = GDK_SHIFT_MASK;

//Assigns an unsigned int to track if the Caps Lock key is used to modify another key press.
const uint32_t SGUIModifierType::Lock = GDK_LOCK_MASK;

//Assigns an unsigned int to track if the CTRL key is used to modify another key press.
const uint32_t SGUIModifierType::Control = GDK_CONTROL_MASK;

//Assigns an unsigned int to track if the ALT key is used to modify another key press.
const uint32_t SGUIModifierType::Alt = GDK_MOD1_MASK;

//Assigns an unsigned int to track if the first mouse button is used to modify another key press.
const uint32_t SGUIModifierType::Button1 = GDK_BUTTON1_MASK;

//Assigns an unsigned int to track if the second mouse button is used to modify another key press.
const uint32_t SGUIModifierType::Button2 = GDK_BUTTON2_MASK;

//Assigns an unsigned int to track if the third mouse button is used to modify another key press.
const uint32_t SGUIModifierType::Button3 = GDK_BUTTON3_MASK;

/*
* Checks if a certain modifier has been set for key presses. 
*
* @param[in] val A modifier being tested to see if it's currently active.
*
* @return bool Returns true if val is a currently active modifier.
*
*/
bool SGUIModifierType::ModifierIsSet(uint32_t val) const{
    return DState & val;
}

/*
* Sets a modifier as active.
*
* @param[in] val The modifier to be set.
*
* @return Nothing.
*/
void SGUIModifierType::SetModifier(uint32_t val){
    DState |= val;
}

/*
*
* This function removes a modifier from the list of modifiers present in the current state.
*
* @param[in] val The modifier to be removed.
*
* @return Nothing.
*/

void SGUIModifierType::ClearModifier(uint32_t val){
    DState &= ~val;
}


//The next 78 unsigned integers assign values to the basic keys on the keyboard.

//Assigns an unsigned integer to track if the left shift button is pressed.
const uint32_t SGUIKeyType::LeftShift = GDK_KEY_Shift_L;

//Assigns an unsigned integer to track if the right shift button is pressed.
const uint32_t SGUIKeyType::RightShift = GDK_KEY_Shift_R;

//Assigns an unsigned integer to track if the left control button is pressed.
const uint32_t SGUIKeyType::LeftControl = GDK_KEY_Control_L;

//Assigns an unsigned integer to track if the right control button is pressed.
const uint32_t SGUIKeyType::RightControl = GDK_KEY_Control_R;

//Assigns an unsigned integer to track if the left alt button is pressed.
const uint32_t SGUIKeyType::LeftAlt = GDK_KEY_Alt_L;

//Assigns an unsigned integer to track if the right alt button is pressed.
const uint32_t SGUIKeyType::RightAlt = GDK_KEY_Alt_R;

//Assigns an unsigned integer to track if the Esc key is pressed.
const uint32_t SGUIKeyType::Escape = GDK_KEY_Escape;

//Assigns an unsigned integer to track if the Space key is pressed.
const uint32_t SGUIKeyType::Space = GDK_KEY_space;

//Assigns an unsigned integer to track if the delete key is pressed.
const uint32_t SGUIKeyType::Delete = GDK_KEY_Delete;

//Assigns an unsigned integer to track if the period key is pressed.
const uint32_t SGUIKeyType::Period = GDK_KEY_period;

//Assigns an unsigned integer to track if the backspace key is pressed.
const uint32_t SGUIKeyType::BackSpace = GDK_KEY_BackSpace;

//Assigns an unsigned integer to track if the up arrow key is pressed.
const uint32_t SGUIKeyType::UpArrow = GDK_KEY_Up;

//Assigns an unsigned integer to track if the down arrow key is pressed.
const uint32_t SGUIKeyType::DownArrow = GDK_KEY_Down;

//Assigns an unsigned integer to track if the left arrow key is pressed.
const uint32_t SGUIKeyType::LeftArrow = GDK_KEY_Left;

//Assigns an unsigned integer to track if the right arrow key is pressed.
const uint32_t SGUIKeyType::RightArrow = GDK_KEY_Right;

//Assigns an unsigned integer to track if the '0' key is pressed.
const uint32_t SGUIKeyType::Key0 = GDK_KEY_0;

//Assigns an unsigned integer to track if the '1' key is pressed.
const uint32_t SGUIKeyType::Key1 = GDK_KEY_1;

//Assigns an unsigned integer to track if the '2' key is pressed.
const uint32_t SGUIKeyType::Key2 = GDK_KEY_2;

//Assigns an unsigned integer to track if the '3' key is pressed.
const uint32_t SGUIKeyType::Key3 = GDK_KEY_3;

//Assigns an unsigned integer to track if the '4' key is pressed.
const uint32_t SGUIKeyType::Key4 = GDK_KEY_4;

//Assigns an unsigned integer to track if the '5' key is pressed.
const uint32_t SGUIKeyType::Key5 = GDK_KEY_5;

//Assigns an unsigned integer to track if the '6' key is pressed.
const uint32_t SGUIKeyType::Key6 = GDK_KEY_6;

//Assigns an unsigned integer to track if the '7' key is pressed.
const uint32_t SGUIKeyType::Key7 = GDK_KEY_7;

//Assigns an unsigned integer to track if the '8' key is pressed.
const uint32_t SGUIKeyType::Key8 = GDK_KEY_8;

//Assigns an unsigned integer to track if the '9' key is pressed.
const uint32_t SGUIKeyType::Key9 = GDK_KEY_9;

//Assigns an unsigned integer to track if the 'a' key is pressed with an uppercase modifier.
const uint32_t SGUIKeyType::KeyA = GDK_KEY_A;

//Assigns an unsigned integer to track if the 'b' key is pressed with an uppercase modifier.
const uint32_t SGUIKeyType::KeyB = GDK_KEY_B;

//Assigns an unsigned integer to track if the 'c' key is pressed with an uppercase modifier.
const uint32_t SGUIKeyType::KeyC = GDK_KEY_C;

//Assigns an unsigned integer to track if the 'd' key is pressed with an uppercase modifier.
const uint32_t SGUIKeyType::KeyD = GDK_KEY_D;

//Assigns an unsigned integer to track if the 'e' key is pressed with an uppercase modifier.
const uint32_t SGUIKeyType::KeyE = GDK_KEY_E;

//Assigns an unsigned integer to track if the 'f' key is pressed with an uppercase modifier.
const uint32_t SGUIKeyType::KeyF = GDK_KEY_F;

//Assigns an unsigned integer to track if the 'g' key is pressed with an uppercase modifier.
const uint32_t SGUIKeyType::KeyG = GDK_KEY_G;

//Assigns an unsigned integer to track if the 'h' key is pressed with an uppercase modifier.
const uint32_t SGUIKeyType::KeyH = GDK_KEY_H;

//Assigns an unsigned integer to track if the 'i' key is pressed with an uppercase modifier.
const uint32_t SGUIKeyType::KeyI = GDK_KEY_I;

//Assigns an unsigned integer to track if the 'j' key is pressed with an uppercase modifier.
const uint32_t SGUIKeyType::KeyJ = GDK_KEY_J;

//Assigns an unsigned integer to track if the 'k' key is pressed with an uppercase modifier.
const uint32_t SGUIKeyType::KeyK = GDK_KEY_K;

//Assigns an unsigned integer to track if the 'l' key is pressed with an uppercase modifier.
const uint32_t SGUIKeyType::KeyL = GDK_KEY_L;

//Assigns an unsigned integer to track if the 'm' key is pressed with an uppercase modifier.
const uint32_t SGUIKeyType::KeyM = GDK_KEY_M;

//Assigns an unsigned integer to track if the 'n' key is pressed with an uppercase modifier.
const uint32_t SGUIKeyType::KeyN = GDK_KEY_N;

//Assigns an unsigned integer to track if the 'o' key is pressed with an uppercase modifier.
const uint32_t SGUIKeyType::KeyO = GDK_KEY_O;

//Assigns an unsigned integer to track if the 'p' key is pressed with an uppercase modifier.
const uint32_t SGUIKeyType::KeyP = GDK_KEY_P;

//Assigns an unsigned integer to track if the 'q' key is pressed with an uppercase modifier.
const uint32_t SGUIKeyType::KeyQ = GDK_KEY_Q;

//Assigns an unsigned integer to track if the 'r' key is pressed with an uppercase modifier.
const uint32_t SGUIKeyType::KeyR = GDK_KEY_R;

//Assigns an unsigned integer to track if the 's' key is pressed with an uppercase modifier.
const uint32_t SGUIKeyType::KeyS = GDK_KEY_S;

//Assigns an unsigned integer to track if the 't' key is pressed with an uppercase modifier.
const uint32_t SGUIKeyType::KeyT = GDK_KEY_T;

//Assigns an unsigned integer to track if the 'u' key is pressed with an uppercase modifier.
const uint32_t SGUIKeyType::KeyU = GDK_KEY_U;

//Assigns an unsigned integer to track if the 'v' key is pressed with an uppercase modifier.
const uint32_t SGUIKeyType::KeyV = GDK_KEY_V;

//Assigns an unsigned integer to track if the 'w' key is pressed with an uppercase modifier.
const uint32_t SGUIKeyType::KeyW = GDK_KEY_W;

//Assigns an unsigned integer to track if the 'x' key is pressed with an uppercase modifier.
const uint32_t SGUIKeyType::KeyX = GDK_KEY_X;

//Assigns an unsigned integer to track if the 'y' key is pressed with an uppercase modifier.
const uint32_t SGUIKeyType::KeyY = GDK_KEY_Y;

//Assigns an unsigned integer to track if the 'z' key is pressed with an uppercase modifier.
const uint32_t SGUIKeyType::KeyZ = GDK_KEY_Z;

//Assigns an unsigned integer to track if the 'a' key is pressed.
const uint32_t SGUIKeyType::Keya = GDK_KEY_a;

//Assigns an unsigned integer to track if the 'b' key is pressed. 
const uint32_t SGUIKeyType::Keyb = GDK_KEY_b;

//Assigns an unsigned integer to track if the 'c' key is pressed.
const uint32_t SGUIKeyType::Keyc = GDK_KEY_c;

//Assigns an unsigned integer to track if the 'd' key is pressed.
const uint32_t SGUIKeyType::Keyd = GDK_KEY_d;

//Assigns an unsigned integer to track if the 'e' key is pressed.
const uint32_t SGUIKeyType::Keye = GDK_KEY_e;

//Assigns an unsigned integer to track if the 'f' key is pressed.
const uint32_t SGUIKeyType::Keyf = GDK_KEY_f;

//Assigns an unsigned integer to track if the 'g' key is pressed.
const uint32_t SGUIKeyType::Keyg = GDK_KEY_g;

//Assigns an unsigned integer to track if the 'h' key is pressed.
const uint32_t SGUIKeyType::Keyh = GDK_KEY_h;

//Assigns an unsigned integer to track if the 'i' key is pressed.
const uint32_t SGUIKeyType::Keyi = GDK_KEY_i;

//Assigns an unsigned integer to track if the 'j' key is pressed.
const uint32_t SGUIKeyType::Keyj = GDK_KEY_j;

//Assigns an unsigned integer to track if the 'k' key is pressed.
const uint32_t SGUIKeyType::Keyk = GDK_KEY_k;

//Assigns an unsigned integer to track if the 'l' key is pressed.
const uint32_t SGUIKeyType::Keyl = GDK_KEY_l;

//Assigns an unsigned integer to track if the 'm' key is pressed.
const uint32_t SGUIKeyType::Keym = GDK_KEY_m;

//Assigns an unsigned integer to track if the 'n' key is pressed.
const uint32_t SGUIKeyType::Keyn = GDK_KEY_n;

//Assigns an unsigned integer to track if the 'o' key is pressed.
const uint32_t SGUIKeyType::Keyo = GDK_KEY_o;

//Assigns an unsigned integer to track if the 'p' key is pressed.
const uint32_t SGUIKeyType::Keyp = GDK_KEY_p;

//Assigns an unsigned integer to track if the 'q' key is pressed.
const uint32_t SGUIKeyType::Keyq = GDK_KEY_q;

//Assigns an unsigned integer to track if the 'r' key is pressed.
const uint32_t SGUIKeyType::Keyr = GDK_KEY_r;

//Assigns an unsigned integer to track if the 's' key is pressed.
const uint32_t SGUIKeyType::Keys = GDK_KEY_s;

//Assigns an unsigned integer to track if the 't' key is pressed.
const uint32_t SGUIKeyType::Keyt = GDK_KEY_t;

//Assigns an unsigned integer to track if the 'u' key is pressed.
const uint32_t SGUIKeyType::Keyu = GDK_KEY_u;

//Assigns an unsigned integer to track if the 'v' key is pressed.
const uint32_t SGUIKeyType::Keyv = GDK_KEY_v;

//Assigns an unsigned integer to track if the 'w' key is pressed.
const uint32_t SGUIKeyType::Keyw = GDK_KEY_w;

//Assigns an unsigned integer to track if the 'x' key is pressed.
const uint32_t SGUIKeyType::Keyx = GDK_KEY_x;

//Assigns an unsigned integer to track if the 'y' key is pressed.
const uint32_t SGUIKeyType::Keyy = GDK_KEY_y;

//Assigns an unsigned integer to track if the 'z' key is pressed.
const uint32_t SGUIKeyType::Keyz = GDK_KEY_z;


/**
* This boolean is designed to test whether an unsigned integer is a specific key press. 
*
* @param[in] val The unsigned integer being tested.
*
* @return bool True if val is the key currently being looked at.
*
*/
bool SGUIKeyType::IsKey(uint32_t val) const{
    return DValue == val;
}

/**
* This sets DValue (the current key being looked at) to be a specific key.
*
* @param[in] val The unsigned integer being set as the current key.
*
* @return Nothing.
*
*/
void SGUIKeyType::SetKey(uint32_t val){
    DValue = val;
}

/**
* Checks if the most recent key press is an alphabetical character.
*
* @param[in] Nothing.
*
* @return bool Returns true if DValue is either an uppercase or lowercase alphabetical character.
*
*/
bool SGUIKeyType::IsAlpha() const{
    return IsUpper() || IsLower();
}

/**
* Checks if the most recent key press is an alphanumeric character.
*
* @param[in] Nothing.
*
* @return bool Returns true if DValue is either an alphabetical character or a digit.
*
*/
bool SGUIKeyType::IsAlphaNumeric() const{
    return IsAlpha() || IsDigit();
}

/**
* Checks if the most recent key press is a digit.
*
* @param[in] Nothing.
*
* @return bool Returns true if DValue has an ASCII value between 0 and 9.
*/
bool SGUIKeyType::IsDigit() const{
    return ((Key0 <= DValue)&&(Key9 >= DValue));
}

/**
* Checks if the most recent key press is an uppercase alphabetical character.
*
* @param[in] Nothing.
*
* @return bool Returns true if DValue has an ASCII value between 'A' and 'Z'.
*/
bool SGUIKeyType::IsUpper() const{
    return ((KeyA <= DValue)&&(KeyZ >= DValue));
}

/**
* Checks if the most recent key press is a lowercase alphabetical character.
*
* @param[in] Nothing.
*
* @return bool Returns true if DValue has an ASCII value between 'a' and 'z'.
*/
bool SGUIKeyType::IsLower() const{
    return ((Keya <= DValue)&&(Keyz >= DValue));
}

/**
* Checks if the most recent key press is an acceptable ASCII value.
*
* @param[in] Nothing.
*
* @return bool Returns true if DValue has an ASCII value
*/
bool SGUIKeyType::IsASCII() const{
    return 127 >= DValue;
}


std::shared_ptr<CGUIApplication> CGUIFactory::DApplicationPointer = nullptr;

/**
*If there isn't a pre-existing instance of the application's GUI, constructs and returns one.
*
*@param[in] appname The name of the application to be created.
*
*@return shared_ptr A shared_ptr with information about the application GUI.
*
*/
std::shared_ptr<CGUIApplication> CGUIFactory::ApplicationInstance(const std::string &appname){
    if(!DApplicationPointer && appname.length()){
        DApplicationPointer = std::make_shared<CGUIApplicationGTK3>(appname);
    }
    return DApplicationPointer;
}

/**
* Gives the default settings for a GUI display.
*
* @param[in] Nothing.
*
* @return shared_ptr A shared_ptr with the default settings for a GUI display.
*
*/

std::shared_ptr<CGUIDisplay> CGUIFactory::DefaultDisplay(){
    return std::make_shared<CGUIDisplayGTK3>(gdk_display_get_default());   
}

/**
* Creates the GUI for a drawing area.
*
* @param[in] Nothing.
*
* @return shared_ptr A shared_ptr to a new drawing area GUI.
*
*/
std::shared_ptr<CGUIDrawingArea> CGUIFactory::NewDrawingArea(){
    return std::make_shared<CGUIDrawingAreaGTK3>(gtk_drawing_area_new());
}

/**
* Creates the GUI for a new box.
*
* @param[in] orientation The orientation of the box (vertical or horizontal)
*
* @param[in] spacing The space given for the box.
*
* @return shared_ptr A shared_ptr to a new box GUI.
*
*/
std::shared_ptr<CGUIBox> CGUIFactory::NewBox(CGUIBox::EOrientation orientation, int spacing){
    return std::make_shared<CGUIBoxGTK3>(gtk_box_new(orientation == CGUIBox::EOrientation::Horizontal ? GTK_ORIENTATION_HORIZONTAL : GTK_ORIENTATION_VERTICAL, spacing));
}

/**
* Creates the GUI for a new menu.
*
* @param[in] Nothing.
*
* @return shared_ptr A shared_ptr to a new box GUI.
*
*/
std::shared_ptr<CGUIMenu> CGUIFactory::NewMenu(){
    return std::make_shared<CGUIMenuGTK3>(gtk_menu_new());
}

/**
* Creates the GUI for a new bar in a menu.
*
* @param[in] Nothing.
*
* @return shared_ptr A shared_ptr to a new menu bar.
*
*/
std::shared_ptr<CGUIMenuBar> CGUIFactory::NewMenuBar(){
    return std::make_shared<CGUIMenuBarGTK3>(gtk_menu_bar_new());
}

/**
* Creates a pointer to a GUI for an item (a button) present in a menu.
*
* @param[in] label A string holding the label for the item
*
* @return shared_ptr A shared_ptr to a new menu item.
*/
std::shared_ptr<CGUIMenuItem> CGUIFactory::NewMenuItem(const std::string &label){
    return std::make_shared<CGUIMenuItemGTK3>(gtk_menu_item_new_with_label(label.c_str()));
}
/**
* Creates a pointer to a GUI for a file filter.
*
* @param[in] Nothing.
*
* @return shared_ptr A shared_ptr to a new file filter.
*
*/
std::shared_ptr<CGUIFileFilter> CGUIFactory::NewFileFilter(){
    return std::make_shared<CGUIFileFilterGTK3>(gtk_file_filter_new());   
}
/**
* Gives GUI settings for creating a new file. If said file has a parent window,
* a pointer is set to the parent window, and a pointer stores the widget of the 
* parent window.
*
* If openfile is true, the function returns the options to open the file or cancel. If it is not true,
* the options returned are to save the file or cancel.
*
* @param[in] title A string storing the title of the file.
*
* @param[in] openfile A bool storing information on whether the user wants to open or save a file.
*
* @param[in] parent A shared_ptr leading to the parent of a file, if the file has a parent.
*
* @return shared_ptr Points to a dialogue window, asking whether the user wants to open (or save) a file, or cancel the action.
*
*/
std::shared_ptr<CGUIFileChooserDialog> CGUIFactory::NewFileChooserDialog(const std::string &title, bool openfile, std::shared_ptr<CGUIWindow> parent){
    GtkWindow *ParentWidget = NULL;
    if(parent){
        std::shared_ptr<CGUIWindowGTK3> ParentWindow = std::dynamic_pointer_cast<CGUIWindowGTK3>(parent);
    
        ParentWidget = GTK_WINDOW(ParentWindow->Widget());
    } 
    if(openfile){
        return std::make_shared<CGUIFileChooserDialogGTK3>(gtk_file_chooser_dialog_new(title.c_str(), ParentWidget, GTK_FILE_CHOOSER_ACTION_OPEN, _("_Cancel"), GTK_RESPONSE_CANCEL, _("_Open"), GTK_RESPONSE_ACCEPT, NULL));
    }
    else{
        return std::make_shared<CGUIFileChooserDialogGTK3>(gtk_file_chooser_dialog_new(title.c_str(), ParentWidget, GTK_FILE_CHOOSER_ACTION_SAVE, _("_Cancel"), GTK_RESPONSE_CANCEL, _("_Save"), GTK_RESPONSE_ACCEPT, NULL));
    }
}

/**
*
*@class CGUIApplicationGTK3
*
*@brief This class shows logic for generating an application GUI using the GTK3 library.
*
*@author Greg
*
*@version 3.0
*
*@date 10/23/2017 9:45:34
*
*contact: gjdost@ucdavis.edu
*
*/
CGUIApplicationGTK3::CGUIApplicationGTK3(const std::string &appname){
    DApplication = gtk_application_new (appname.c_str(), G_APPLICATION_FLAGS_NONE);
    //g_signal_connect(DApplication, "startup", G_CALLBACK (StartupCallback), NULL);
    g_signal_connect(DApplication, "activate", G_CALLBACK (ActivateCallback), this);
    
    DActivateCalldata = nullptr;
    DActivateCallback = nullptr;

    DTimerHandle = -1;
    DTimerCalldata = nullptr;
    DTimerCallback = nullptr;
    //DPeriodicTimeout = std::make_shared< CPeriodicTimeout > (50);
}

CGUIApplicationGTK3::~CGUIApplicationGTK3(){

}

/**
* Runs the callback to activate an application. 
*
* @param[in] app A pointer to the application being activated.
*
* @paran[in] data A pointer with information about the current gamestate.
*
* @return Nothing.
* 
*/
void CGUIApplicationGTK3::ActivateCallback(GtkApplication* app, gpointer data){
    CGUIApplicationGTK3 *App = static_cast<CGUIApplicationGTK3 *>(data);
    if(App->DActivateCallback){
        App->DActivateCallback(App->DActivateCalldata);
    }
}


/**
* Handles if an application is timing out.
*
* @param[in] data A pointer with information about the current gamestate.
*
* @return gbool A boolean equivalent, returns false if the program isn't timing out.
*
*/
gboolean CGUIApplicationGTK3::TimeoutCallback(gpointer data){
    CGUIApplicationGTK3 *App = static_cast<CGUIApplicationGTK3 *>(data);
    if(App->DTimerCallback){
        if(App->DTimerCallback(App->DTimerCalldata)){
            App->DTimerHandle = g_timeout_add(App->DPeriodicTimeout->MiliSecondsUntilDeadline(), TimeoutCallback, data);
        }
    }
    return FALSE;
}
/**
* Sets an applications calldata and callback values.
*
* @param[in] calldata The object to be used as the application's calldata.
* 
* @param[in] callback The object to be used as the application's callback.
*
* @return Nothing.
*
*/
void CGUIApplicationGTK3::SetActivateCallback(TGUICalldata calldata, TGUIApplicationCallback callback){
    DActivateCalldata = calldata;
    DActivateCallback = callback;
}

/**
*Creates a timer, setting a value for the time remaining.
*
*@param timems The amount of time remaining.
*
*@param calldata Tracks the data leading up to the timeout call
*
*@param callback Tracks how to return to the program after the timeout ends.
*
*@return None.
*
*/
void CGUIApplicationGTK3::SetTimer(int timems, TGUICalldata calldata, TGUITimeoutCallback callback){
    if(0 >= timems){
        if(0 <= DTimerHandle){
            g_source_remove(DTimerHandle);
            DTimerHandle = -1;
        }
        DTimerCalldata = nullptr;
        DTimerCallback = nullptr;
        return;
    }
    if(callback){
        if(0 <= DTimerHandle){
            g_source_remove(DTimerHandle);
            DTimerHandle = -1;
        }
        DTimerCalldata = calldata;
        DTimerCallback = callback;
        DPeriodicTimeout = std::make_shared< CPeriodicTimeout > (timems);
        DTimerHandle = g_timeout_add(DPeriodicTimeout->MiliSecondsUntilDeadline(), TimeoutCallback, this);
    }
}


/**
* Processeses an interation of GTK's main loop.
*
* @param[in] block A bool to decide whether to block or return (If no events are available).
*
* @return Nothing.
*
*/
void CGUIApplicationGTK3::ProcessEvents(bool block){
    gtk_main_iteration_do(block ? TRUE : FALSE);
}

/**
* Taking in user arguments as a parameter, runs the application.
*
* @param[in] argc An integer tracking the number of user arguments
*
* @param[in] argv An array of the user arguments.
*
* return int The exit status.
*/
int CGUIApplicationGTK3::Run(int argc, char *argv[]){
    return g_application_run(G_APPLICATION(DApplication), argc, argv);
}

/**
* Quits the application.
*
* @param[in] Nothing.
*
* @return Nothing.
*
*/
void CGUIApplicationGTK3::Quit(){
    g_application_quit(G_APPLICATION(DApplication));
}

/**
* Creates the gui for a new window.
*
* @param[in] Nothing.
*
* @return shared_ptr A shared_ptr to the created window.
*/
std::shared_ptr<CGUIWindow> CGUIApplicationGTK3::NewWindow(){
    return std::make_shared<CGUIWindowGTK3>(gtk_application_window_new(DApplication));
}

/**
*@class CGUIDisplayGTK3
*
*@brief This class gives logic for creating a display GUI using the GTK3 library.
*
*@author Greg
*
*@version 3.0
*
*@date 10/23/2017 9:45:34
*
*contact: gjdost@ucdavis.edu
*
*/
CGUIDisplayGTK3::CGUIDisplayGTK3(GdkDisplay *display) : CGUIDisplay(){
    DDisplay = display;
}

CGUIDisplayGTK3::~CGUIDisplayGTK3(){

}

/**
* Syncs the display
* 
* @param[in] Nothing.
*
* @return Nothing.
*
*/
void CGUIDisplayGTK3::Sync(){
    gdk_display_sync(DDisplay);
}
/**
* Flushes the display
*
* @param[in] Nothing.
*
* @param[in] Nothing.
*/
void CGUIDisplayGTK3::Flush(){
    gdk_display_flush(DDisplay);
}

/**
* Creates the GUI for a cursor.
*
* @param[in] type The type of cursor (right, left, center, or crosshair).
*
* @return shared_ptr A shared ptr to the cursor created.
*
*/
std::shared_ptr< CGUICursor > CGUIDisplayGTK3::NewCursor(CGUICursor::ECursorType type){
    GdkCursorType CursorType = GDK_BLANK_CURSOR;
    switch(type){
        case CGUICursor::ECursorType::RightPointer:     CursorType = GDK_RIGHT_PTR;
                                                        break;
        case CGUICursor::ECursorType::LeftPointer:      CursorType = GDK_LEFT_PTR;
                                                        break;
        case CGUICursor::ECursorType::CenterPointer:    CursorType = GDK_CENTER_PTR;
                                                        break;
        case CGUICursor::ECursorType::Crosshair:        CursorType = GDK_CROSSHAIR;
                                                        break;
        default:                                        break;
    }
    
    return std::make_shared<CGUICursorGTK3>(gdk_cursor_new_for_display(DDisplay, CursorType));
}

/**
*@class CGUICursorGTK3
*
*@brief This class gives logic for generating a cursor in a GUI using the GTK3 library.
*
*@author Greg
*
*@version 3.0
*
*@date 10/23/2017 9:47:14
*
*contact: gjdost@ucdavis.edu
*
*/        
CGUICursorGTK3::CGUICursorGTK3(GdkCursor *cursor) : CGUICursor(){
    DCursor = cursor;
}

CGUICursorGTK3::~CGUICursorGTK3(){
    
}

/**
* Tests a cursor to find what type it is.
*
* @param[in] Nothing.
*
* @return enum The type of cursor.
*/
CGUICursor::ECursorType CGUICursorGTK3::Type(){
    switch(gdk_cursor_get_cursor_type(DCursor)){
        case GDK_CENTER_PTR:    return ECursorType::CenterPointer;
        case GDK_LEFT_PTR:      return ECursorType::LeftPointer;
        case GDK_RIGHT_PTR:     return ECursorType::RightPointer;
        case GDK_CROSSHAIR:     return ECursorType::Crosshair;
        default:                return ECursorType::Blank;
    }
}

/**
*@class CGUIWidgetGTK3
*
*@brief This class gives logic for generating a widget in a GUI using the GTK3 library.
*
*@author Greg
*
*@version 3.0
*
*@date 10/23/2017 9:51:30
*
*contact: gjdost@ucdavis.edu
*/
CGUIWidgetGTK3::CGUIWidgetGTK3(GtkWidget *widget, bool reference){
    DWidget = widget;
    //if(reference){ // Seems to fail on exit if not always referenced.
        g_object_ref(DWidget);
    //}
    
    DActivateCalldata = nullptr;
    DActivateCallback = nullptr;
    
    DButtonPressCalldata = nullptr;
    DButtonPressCallback = nullptr;
    
    DButtonReleaseCalldata = nullptr;
    DButtonReleaseCallback = nullptr;
    
    DMotionCalldata = nullptr;
    DMotionCallback = nullptr;
    
    DKeyPressCalldata = nullptr; 
    DKeyPressCallback = nullptr;
    
    DKeyReleaseCalldata = nullptr; 
    DKeyReleaseCallback = nullptr;
    
    DExposeCalldata = nullptr;
    DExposeCallback = nullptr;
    
    DConfigureCalldata = nullptr;
    DConfigureCallback = nullptr;
    
    DDrawCalldata = nullptr;
    DDrawCallback = nullptr;
}

CGUIWidgetGTK3::~CGUIWidgetGTK3(){
    if(DWidget){
        gtk_widget_destroy(DWidget);
    }
}

/**
* Runs the callback to activate a widget.
*
* @param[in] widget The widget being activated.
*
* @param[in] data A pointer with information about the current state of the game.
*
* @return Nothing.
*
*/
void CGUIWidgetGTK3::ActivateEventCallback(GtkWidget *widget, gpointer data){
    CGUIWidgetGTK3 *Widget = static_cast<CGUIWidgetGTK3 *>(data); 

    if(Widget->DActivateCallback){
        Widget->DActivateCallback(Widget->shared_from_this(),Widget->DActivateCalldata);
    }
}
/**
* Returns true if a button is pressed, otherwise returns false. As well, creates events depending on 
* stored values relating to the button and widget.
*
* @param[in] widget The widget in which we are checking for a button press.
*
* @param[in] event The event we are checking for (A button press event).
*
* @param[in] data A pointer with information about the current state of the game.
*
* @return gbool Returns false when there is no button press event left to deal with.
*
*/
gboolean CGUIWidgetGTK3::ButtonPressEventCallback(GtkWidget *widget, GdkEventButton *event, gpointer data){
    CGUIWidgetGTK3 *Widget = static_cast<CGUIWidgetGTK3 *>(data); 

    if(Widget->DButtonPressCallback){
        SGUIButtonEvent TempEvent;
        TempEvent.DType.DType = event->type;
        TempEvent.DWindowX = event->x;
        TempEvent.DWindowY = event->y;
        TempEvent.DModifier.DState = event->state;
        TempEvent.DButton = event->button;
        TempEvent.DScreenX = event->x_root;
        TempEvent.DScreenY = event->y_root;

        return Widget->DButtonPressCallback(Widget->shared_from_this(), TempEvent, Widget->DButtonPressCalldata);
    }
    return FALSE;
}

/**
* Returns true if a button is released, otherwise returns false. As well, creates events depending on 
* stored values relating to the button and widget.
*
* @param[in] widget The widget in which we are checking for a button release.
*
* @param[in] event The event we are checking for (A button release event).
*
* @param[in] data A pointer with information about the current state of the game.
*
* @return gbool Returns false when there is no button release event left to deal with.
*
*/

gboolean CGUIWidgetGTK3::ButtonReleaseEventCallback(GtkWidget *widget, GdkEventButton *event, gpointer data){
    CGUIWidgetGTK3 *Widget = static_cast<CGUIWidgetGTK3 *>(data); 

    if(Widget->DButtonPressCallback){
        SGUIButtonEvent TempEvent;
        TempEvent.DType.DType = event->type;
        TempEvent.DWindowX = event->x;
        TempEvent.DWindowY = event->y;
        TempEvent.DModifier.DState = event->state;
        TempEvent.DButton = event->button;
        TempEvent.DScreenX = event->x_root;
        TempEvent.DScreenY = event->y_root;

        return Widget->DButtonReleaseCallback(Widget->shared_from_this(), TempEvent, Widget->DButtonReleaseCalldata);
    }
    return FALSE;
}

/**
* Provides functionality for deleting a widgets call data to an event.
*
* @param[in] widget The widget where call data is being deleted.
*
* @param[in] event The event whose call data is being deleted
*
* @param[in] data A pointer with information about the current state of the game.
*
* @return gbool Returns false when there is no more need to delete the event callback.
*/
gboolean CGUIWidgetGTK3::DeleteEventCallback(GtkWidget *widget, GdkEvent *event, gpointer data){
    CGUIWidgetGTK3 *Widget = static_cast<CGUIWidgetGTK3 *>(data); 

    PrintDebug(DEBUG_HIGH, "CGUIWidgetGTK3::DeleteEventCallback\n");
    if(Widget->DDeleteCallback){
        Widget->DDeleteCallback(Widget->shared_from_this(), Widget->DDeleteCalldata);
    }
    return FALSE;
}

/**
* Provides functionality for deleting all of a widget's call data.
*
* @param[in] widget The widget where call data is being deleted.
* 
* @param[in] data A pointer with information about the current state of the game.
*
* @return Nothing.
*/
void CGUIWidgetGTK3::DestroyEventCallback(GtkWidget *widget, gpointer data){
    CGUIWidgetGTK3 *Widget = static_cast<CGUIWidgetGTK3 *>(data); 

    PrintDebug(DEBUG_HIGH, "CGUIWidgetGTK3::DestroyEventCallback\n");
    if(Widget->DDestroyCallback){
        Widget->DDestroyCallback(Widget->shared_from_this(), Widget->DDestroyCalldata);
    }
}

/**
* Provides functionality for if the window of a widget is moved :- calls DMotionCallBack with the new location of the widget.
*
* @param[in] widget The widget being moved.
*
* @param[in] event The description of the widget's motion.
*
* @param[in] data A pointer with information about the current state of the game.
*
* @return gbool Returns false when there is no more motion to handle (calls DMotionCallBack).
*/
gboolean CGUIWidgetGTK3::MotionNotifyEventCallback(GtkWidget *widget, GdkEventMotion *event, gpointer data){
    CGUIWidgetGTK3 *Widget = static_cast<CGUIWidgetGTK3 *>(data); 

    if(Widget->DMotionCallback){
        SGUIMotionEvent TempEvent;

        TempEvent.DWindowX = event->x;
        TempEvent.DWindowY = event->y;
        TempEvent.DModifier.DState = event->state;
        TempEvent.DScreenX = event->x_root;
        TempEvent.DScreenY = event->y_root;
        
        return Widget->DMotionCallback(Widget->shared_from_this(), TempEvent, Widget->DMotionCalldata);
    }
    return FALSE;
}

/**
* If a key has been pressed, stores the key that was pressed and calls DKeyPressCallback to deal with it.
*
* @param[in] widget The widget in which the key was pressed.
*
* @param[in] event The key press event.
*
* @param[in] data A pointer with information about the current state of the game.
*
* @return gbool Returns false when there is no more key press to handle (calls DKeyPressCallBack). 
*
*/
gboolean CGUIWidgetGTK3::KeyPressEventCallback(GtkWidget *widget, GdkEventKey *event, gpointer data){
    CGUIWidgetGTK3 *Widget = static_cast<CGUIWidgetGTK3 *>(data); 

    if(Widget->DKeyPressCallback){
        SGUIKeyEvent TempEvent;

        TempEvent.DValue.DValue = event->keyval;
        TempEvent.DModifier.DState = event->state;
        
        return Widget->DKeyPressCallback(Widget->shared_from_this(), TempEvent, Widget->DKeyPressCalldata);
    }
    return FALSE;
}

/**
* If a key has been released, stores the key that was released and calls DKeyReleaseCallback to deal with it.
*
* @param[in] widget The widget in which the key is being released.
*
* @param[in] event The key release event.
*
* @param[in] data A pointer with information about the current state of the game.
*
* @return gbool Returns false when there is no more key release to handle (calls DKeyReleaseCallBack).
*/
gboolean CGUIWidgetGTK3::KeyReleaseEventCallback(GtkWidget *widget, GdkEventKey *event, gpointer data){
    CGUIWidgetGTK3 *Widget = static_cast<CGUIWidgetGTK3 *>(data); 

    if(Widget->DKeyReleaseCallback){
        SGUIKeyEvent TempEvent;

        TempEvent.DValue.DValue = event->keyval;
        TempEvent.DModifier.DState = event->state;
        
        return Widget->DKeyReleaseCallback(Widget->shared_from_this(), TempEvent, Widget->DKeyReleaseCalldata);
    }
    return FALSE;
}

/**
* Gives the callback for an event that occurrs in a widget.
* 
* @param[in] widget The widget where the event occured.
*
* @param[in] event The event whose calldata is being exposed.
*
* @param[in] data A pointer with information about the current state of the game.
*
* @return gbool Returns false when there is no more callback to expose (calls DExposeCallBack).
*
*/
gboolean CGUIWidgetGTK3::ExposeEventCallback(GtkWidget *widget, GdkEventExpose *event, gpointer data){
    CGUIWidgetGTK3 *Widget = static_cast<CGUIWidgetGTK3 *>(data); 

    if(Widget->DExposeCallback){
        SGUIConfigureEvent TempEvent;

        TempEvent.DX = event->area.x;
        TempEvent.DY = event->area.y;
        TempEvent.DWidth = event->area.width;
        TempEvent.DHeight = event->area.height;
        
        return Widget->DExposeCallback(Widget->shared_from_this(), TempEvent, Widget->DExposeCalldata);
    }
    return FALSE;
}

/**
* Configures the callback data for an event.
*
* @param[in] widget The widget where the event is occuring.
*
* @param[in] event The event being configured.
*
* @param[in] data A pointer with information about the current state of the game.
*
*/
gboolean CGUIWidgetGTK3::ConfigureEventCallback(GtkWidget *widget, GdkEventConfigure *event, gpointer data){
    CGUIWidgetGTK3 *Widget = static_cast<CGUIWidgetGTK3 *>(data); 

    if(Widget->DConfigureCallback){
        SGUIConfigureEvent TempEvent;

        TempEvent.DX = event->x;
        TempEvent.DY = event->y;
        TempEvent.DWidth = event->width;
        TempEvent.DHeight = event->height;
        
        return Widget->DConfigureCallback(Widget->shared_from_this(), TempEvent, Widget->DConfigureCalldata);
    }
    return FALSE;
}

/**
* Draws the callback data for an event.
*
* @param[in] widget The widget where the event is occuring.
*
* @param[in] cr The cairo drawing context.
*
* @param[in] data A pointer with information about the current state of the game.
*
* @return gbool Returns false when the event is done drawing (calls DDrawCallback), or when DDrawCallback returns false.
*
*/
gboolean CGUIWidgetGTK3::DrawEventCallback(GtkWidget *widget, cairo_t *cr, gpointer data){
    CGUIWidgetGTK3 *Widget = static_cast<CGUIWidgetGTK3 *>(data); 

    if(Widget->DDrawCallback){
        return Widget->DDrawCallback(Widget->shared_from_this(), std::make_shared<CGraphicResourceContextCairo>(cr, true), Widget->DDrawCalldata);
    }
    return FALSE;    
}

/**
* Gives a function for showing the current widget.
*
* @param[in] Nothing.
*
* @return Nothing.
*
*/
void CGUIWidgetGTK3::Show(){
    gtk_widget_show(DWidget);
}

/**
* Gives a function for showing all current widgets.
*
* @param[in] Nothing.
*
* @return Nothing.
*
*/
void CGUIWidgetGTK3::ShowAll(){
    gtk_widget_show_all(DWidget);
}

/**
* Gives a function for hiding the current widget.
*
* @param[in] Nothing.
*
* @return Nothing.
*/
void CGUIWidgetGTK3::Hide(){
    gtk_widget_hide(DWidget);
}

/**
* Gives the allocated width for the current widget.
*
* @param[in] Nothing.
*
* @return The width of the current widget.
*/
int CGUIWidgetGTK3::AllocatedWidth(){
    GtkAllocation AllocatedArea;

    gtk_widget_get_allocation(DWidget, &AllocatedArea);
    return AllocatedArea.width;
}

/**
* Gives the allocated height for the current widget.
*
* @param[in] Nothing.
*
* @return The height of the current widget.
*
*/
int CGUIWidgetGTK3::AllocatedHeight(){
    GtkAllocation AllocatedArea;

    gtk_widget_get_allocation(DWidget, &AllocatedArea);
    return AllocatedArea.height;
}

/**
* Invalidates the widget, making it recieve a draw on the next frame.
*
* @param[in] Nothing.
*
* @return Nothing.
*
*/
void CGUIWidgetGTK3::Invalidate(){
    GtkAllocation AllocatedArea;

    gtk_widget_get_allocation(DWidget, &AllocatedArea);
    
    gtk_widget_queue_draw_area(DWidget, 0, 0, AllocatedArea.width, AllocatedArea.height);
}

/**
* Puts a cursor inside the widget window.
*
* @param[in] cursor The cursor being placed in the window.
*
* @return Nothing.
*/
void CGUIWidgetGTK3::SetCursor(std::shared_ptr< CGUICursor > cursor){
    std::shared_ptr<CGUICursorGTK3> Cursor = std::dynamic_pointer_cast<CGUICursorGTK3>(cursor);
    
    gdk_window_set_cursor(gtk_widget_get_window(DWidget), Cursor->DCursor);
}

/**
* Creates a graphic surface similar to that of the current widget, but with different width and height.
*
* @param[in] width The width of the surface to be created.
*
* @param[in] height The height of the surface to be created.
*
* @return shared_ptr A shared_ptr to the created grahpics surface.
*/
std::shared_ptr<CGraphicSurface> CGUIWidgetGTK3::CreateSimilarSurface(int width, int height){
    return std::make_shared<CGraphicSurfaceCairo>(gdk_window_create_similar_image_surface(gtk_widget_get_window(DWidget),
                                                       CAIRO_FORMAT_ARGB32,
                                                       width,
                                                       height, 0));
}
/**
* Gives the drawing context of the current window.
*
* @param[in] Nothing.
*
* @return shared_ptr A shared_ptr to the drawing context given by the current window.
*
*/
std::shared_ptr<CGraphicResourceContext> CGUIWidgetGTK3::CreateResourceContext(){
#if GTK_CHECK_VERSION(3, 22, 0) //(GDK_VERSION_MAX_ALLOWED >= GDK_VERSION_3_22)
    GtkAllocation AllocatedArea;

    gtk_widget_get_allocation(DWidget, &AllocatedArea);
    cairo_rectangle_int_t WidgetRectangle{AllocatedArea.x, AllocatedArea.y, AllocatedArea.width, AllocatedArea.height};
    cairo_region_t *Region = cairo_region_create_rectangle(&WidgetRectangle);
    GdkWindow *Window = gtk_widget_get_window(DWidget);
    GdkDrawingContext *DrawingContext = gdk_window_begin_draw_frame(Window, Region);
    cairo_region_destroy(Region);
    
    return std::make_shared<CGraphicResourceContextCairo>(Window, DrawingContext);
#else
    GdkWindow *Window = gtk_widget_get_window(DWidget);
    return std::make_shared<CGraphicResourceContextCairo>(gdk_cairo_create(Window));
#endif
}

/**
* Sets an event.
*
* @param[in] event The event to be set.
*
* @return Nothing.
*/ 
void CGUIWidgetGTK3::EnableEvent(EGUIEvent event){
    gint CurrentMask = gtk_widget_get_events(DWidget);
    switch(event){
        case EGUIEvent::Expose:         CurrentMask |= GDK_EXPOSURE_MASK;
                                        break;
        case EGUIEvent::Motion:         CurrentMask |= GDK_POINTER_MOTION_MASK;
                                        break;
        case EGUIEvent::ButtonPress:    CurrentMask |= GDK_BUTTON_PRESS_MASK;
                                        break;
        case EGUIEvent::ButtonRelease:  CurrentMask |= GDK_BUTTON_RELEASE_MASK;
                                        break;
        case EGUIEvent::KeyPress:       CurrentMask |= GDK_KEY_PRESS_MASK;
                                        break;
        case EGUIEvent::KeyRelease:     CurrentMask |= GDK_KEY_RELEASE_MASK;
                                        break;
        default:                        break;
    }
    gtk_widget_set_events(DWidget, CurrentMask);
}

/**
* Removes an event from the list of currently active events.
*
* @param[in] event The event to be removed.
*
* @return Nothing.
*/
void CGUIWidgetGTK3::DisableEvent(EGUIEvent event){
    gint CurrentMask = gtk_widget_get_events(DWidget);
    switch(event){
        case EGUIEvent::Expose:         CurrentMask &= ~GDK_EXPOSURE_MASK;
                                        break;
        case EGUIEvent::Motion:         CurrentMask &= ~GDK_POINTER_MOTION_MASK;
                                        break;
        case EGUIEvent::ButtonPress:    CurrentMask &= ~GDK_BUTTON_PRESS_MASK;
                                        break;
        case EGUIEvent::ButtonRelease:  CurrentMask &= ~GDK_BUTTON_RELEASE_MASK;
                                        break;
        case EGUIEvent::KeyPress:       CurrentMask &= ~GDK_KEY_PRESS_MASK;
                                        break;
        case EGUIEvent::KeyRelease:     CurrentMask &= ~GDK_KEY_RELEASE_MASK;
                                        break;
        default:                        break;
    }
    gtk_widget_set_events(DWidget, CurrentMask);
}

/**
* Gives event callback functionality to an activation event.
*
* @param[in] calldata The calldata to be set for an activation event.
*
* @param[in] callback The callback to be set for an activation event.
*
* @return Nothing.
*/
void CGUIWidgetGTK3::SetActivateEventCallback(TGUICalldata calldata, TGUIActivateEventCallback callback){
    DActivateCalldata = calldata;
    DActivateCallback = callback;
    if(callback){
        g_signal_connect(DWidget, "activate", G_CALLBACK(ActivateEventCallback), this);
    }
}

/**
* Gives event callback functionality to a button press event.
*
* @param[in] calldata The calldata to be set for a button press event.
*
* @param[in] callback The callback to be set for a button press event.
*
* @return Nothing.
*/                                       
void CGUIWidgetGTK3::SetButtonPressEventCallback(TGUICalldata calldata, TGUIButtonEventCallback callback){
    DButtonPressCalldata = calldata;
    DButtonPressCallback = callback;
    if(callback){
        g_signal_connect(DWidget, "button-press-event", G_CALLBACK(ButtonPressEventCallback), this);
    }
}

/**
* Gives event callback functionality to a button release event.
*
* @param[in] calldata The calldata to be set for a button release event.
*
* @param[in] callback The callback to be set for a button release event.
*
* @return Nothing.
*/
void CGUIWidgetGTK3::SetButtonReleaseEventCallback(TGUICalldata calldata, TGUIButtonEventCallback callback){
    DButtonReleaseCalldata = calldata;
    DButtonReleaseCallback = callback;
    if(callback){
        g_signal_connect(DWidget, "button-release-event", G_CALLBACK(ButtonReleaseEventCallback), this);
    }
}

/**
* Gives event callback functionality to a deletion event.
*
* @param[in] calldata The calldata to be set for a deletion event.
*
* @param[in] callback The callback to be set for a deletion event.
*
* @return Nothing.
*/
void CGUIWidgetGTK3::SetDeleteEventCallback(TGUICalldata calldata, TGUIDeleteEventCallback callback){
    DDeleteCalldata = calldata;
    DDeleteCallback = callback;
    if(callback){
        g_signal_connect(DWidget, "delete-event", G_CALLBACK(DeleteEventCallback), this);
    }
}

/**
* Gives event callback functionality to a destroy event.
*
* @param[in] calldata The calldata to be set for a destroy event.
*
* @param[in] callback The callback to be set for a destroy event.
*
* @return Nothing.
*/
void CGUIWidgetGTK3::SetDestroyEventCallback(TGUICalldata calldata, TGUIDestroyEventCallback callback){
    DDestroyCalldata = calldata;
    DDestroyCallback = callback;
    if(callback){
        g_signal_connect(DWidget, "destroy", G_CALLBACK(DestroyEventCallback), this);
    }
}

/**
* Gives event callback functionality to a motion event.
*
* @param[in] calldata The calldata to be set for a motion event.
*
* @param[in] callback The callback to be set for a motion event.
*
* @return Nothing.
*/
void CGUIWidgetGTK3::SetMotionEventCallback(TGUICalldata calldata, TGUIMotionEventCallback callback){
    DMotionCalldata = calldata;
    DMotionCallback = callback;
    if(callback){
        g_signal_connect(DWidget, "motion-notify-event", G_CALLBACK(MotionNotifyEventCallback), this);
    }
}                                                                   

/**
* Gives event callback functionality to a key press event.
*
* @param[in] calldata The calldata to be set for a key press event.
*
* @param[in] callback The callback to be set for a key press event.
*
* @return Nothing.
*/
void CGUIWidgetGTK3::SetKeyPressEventCallback(TGUICalldata calldata, TGUIKeyEventCallback callback){
    DKeyPressCalldata = calldata;
    DKeyPressCallback = callback;
    if(callback){
        g_signal_connect(DWidget, "key-press-event", G_CALLBACK(KeyPressEventCallback), this);
    }
}

/**
* Gives event callback functionality to a key release event.
*   
* @param[in] calldata The calldata to be set for a key release event.
*   
* @param[in] callback The callback to be set for a key release event.
*
* @return Nothing.
*/
void CGUIWidgetGTK3::SetKeyReleaseEventCallback(TGUICalldata calldata, TGUIKeyEventCallback callback){
    DKeyReleaseCalldata = calldata;
    DKeyReleaseCallback = callback;
    if(callback){
        g_signal_connect(DWidget, "key-release-event", G_CALLBACK(KeyReleaseEventCallback), this);
    }
}

/**
* Gives event callback functionality to an expose event.
*   
* @param[in] calldata The calldata to be set for an expose event.
*   
* @param[in] callback The callback to be set for an expose event.
*
* @return Nothing.
*/
void CGUIWidgetGTK3::SetExposeEventCallback(TGUICalldata calldata, TGUIConfigureEventCallback callback){
    DExposeCalldata = calldata;
    DExposeCallback = callback;
    if(callback){
        g_signal_connect(DWidget, "expose-event", G_CALLBACK(ExposeEventCallback), this);
    }
}

/**
* Gives event callback functionality to a configure event.
*   
* @param[in] calldata The calldata to be set for a configure event.
*   
* @param[in] callback The callback to be set for a configure event.
*
* @return Nothing.
*/
void CGUIWidgetGTK3::SetConfigureEventCallback(TGUICalldata calldata, TGUIConfigureEventCallback callback){
    DConfigureCalldata = calldata;
    DConfigureCallback = callback;
    if(callback){
        g_signal_connect(DWidget, "configure-event", G_CALLBACK(ConfigureEventCallback), this);
    }
}

/**
* Gives event callback functionality to a draw event.
*   
* @param[in] calldata The calldata to be set for a draw event.
*   
* @param[in] callback The callback to be set for a draw event.
*
* @return Nothing.
*/
void CGUIWidgetGTK3::SetDrawEventCallback(TGUICalldata calldata, TGUIDrawEventCallback callback){
    DDrawCalldata = calldata;
    DDrawCallback = callback;
    if(callback){
        g_signal_connect(DWidget, "draw", G_CALLBACK(DrawEventCallback), this);
    }    
}

/**
*
*@class CGUIContainerGTK3
*
*@brief This class gives logic for generating a container GUI using the GTK3 library.
*
*@author Greg
*
*@version 3.0
*
*@date 10/23/2017 9:52:53
*
*contact: gjdost@ucdavis.edu
*/

CGUIContainerGTK3::CGUIContainerGTK3(GtkWidget *widget, bool reference) : CGUIWidgetGTK3(widget, reference){
    
}

CGUIContainerGTK3::~CGUIContainerGTK3(){
    
}

/**
* Sets the border width of a widget to a given value.
*
* @param[in] width The given width of the widget.
*
* @return Nothing.
*
*/
void CGUIContainerGTK3::SetBorderWidth(int width){
    gtk_container_set_border_width(GTK_CONTAINER(DWidget), width);
}

/**
* Adds a widget to the current application.
*
* @param[in] widget The widget being added to the application.
*
* @return Nothing.
*
*/
void CGUIContainerGTK3::Add(std::shared_ptr<CGUIWidget> widget){
    std::shared_ptr<CGUIWidgetGTK3> WidgetToAdd = std::dynamic_pointer_cast<CGUIWidgetGTK3>(widget);
    
    gtk_container_add(GTK_CONTAINER(DWidget), WidgetToAdd->Widget());
}

/**
* Removes a widget from the current application.
*
* @param widget The widget being removed from the application.
*
* @return Nothing.
*
*/
void CGUIContainerGTK3::Remove(std::shared_ptr<CGUIWidget> widget){
    std::shared_ptr<CGUIWidgetGTK3> WidgetToAdd = std::dynamic_pointer_cast<CGUIWidgetGTK3>(widget);
    
    gtk_container_remove(GTK_CONTAINER(DWidget), WidgetToAdd->Widget());    
}

/**
*
*@class CGUILabelGTK3
*
*@brief This class gives logic for generating a label in a GUI widget using the GTK3 library.
*
*@author Greg
*
*@version 3.0
*
*@date 10/23/2017 9:52:53
*
*contact: gjdost@ucdavis.edu
*/

CGUILabelGTK3::CGUILabelGTK3(GtkWidget *widget, bool reference) : CGUIWidgetGTK3(widget, reference){
    
}

CGUILabelGTK3::~CGUILabelGTK3(){
    
}

/**
* Gets the text from a widget's label.
*
* @param[in] Nothing.
*
* @return string The label of the widget.
*
*/
std::string CGUILabelGTK3::GetText(){
    return gtk_label_get_text(GTK_LABEL(DWidget));
}

/**
* Sets the text for a widget's label.
*
* @param[in] str The string being set as the label.
*
* @return Nothing.
*
*/
void CGUILabelGTK3::SetText(const std::string &str){
    gtk_label_set_text(GTK_LABEL(DWidget), str.c_str());
}

/**
* Sets a markup on the label for a widget.
*
* @param[in] str The string being set as the markup.
*
* @return Nothing.
*
*/
void CGUILabelGTK3::SetMarkup(const std::string &str){
    gtk_label_set_markup(GTK_LABEL(DWidget), str.c_str());
}

/**
*
*@class CGUIDrawingAreaGTK3
*
*@brief This class gives logic for generating a drawing area in a GUI widget using the GTK3 library.
*
*@author Greg
*
*@version 3.0
*
*@date 10/23/2017 9:55:01
*
*contact: gjdost@ucdavis.edu
*/


CGUIDrawingAreaGTK3::CGUIDrawingAreaGTK3(GtkWidget *widget, bool reference) : CGUIWidgetGTK3(widget, reference){
    
}

CGUIDrawingAreaGTK3::~CGUIDrawingAreaGTK3(){

}

/**
*
*@class CGUIBoxGTK3
*
*@brief This class gives logic for generating a box in a GUI widget using the GTK3 library.
*
*@author Greg
*
*@version 3.0
*
*@date 10/23/2017 9:56:11
*
*contact: gjdost@ucdavis.edu
*/



CGUIBoxGTK3::CGUIBoxGTK3(GtkWidget *widget, bool reference) : CGUIContainerGTK3(widget, reference){

}

CGUIBoxGTK3::~CGUIBoxGTK3(){

}

/**
* Adds a widget to a box, with a reference to the start of the box.
*
* @param[in] widget The widget to be added.
*
* @param[in] expand A bool, true if the widget should get extra space in the box.
*
* @param[in] fill A bool, true if the space from expand should be allocated to the widget, rather than just used as padding.
*
* @param[in] padding An int, the amount of space that should be between this widget and other items in the box.
*
* @return Nothing.
*/ 
void CGUIBoxGTK3::PackStart(std::shared_ptr<CGUIWidget> widget, bool expand, bool fill, int padding){
    std::shared_ptr<CGUIWidgetGTK3> WidgetToAdd = std::dynamic_pointer_cast<CGUIWidgetGTK3>(widget);
    
    gtk_box_pack_start(GTK_BOX(DWidget), WidgetToAdd->Widget(), expand, fill, padding);
}

/**
* Adds a widget to a box, with a reference to the end of the box.
*
* @param[in] widget The widget to be added.
*
* @param[in] expand A bool, true if the widget should get extra space in the box.
*
* @param[in] fill A bool, true if the space from expand should be allocated to the widget, rather than just used as padding.
*
* @param[in] padding An int, the amount of space that should be between this widget and other items in the box.
*
* @return Nothing.
*/
void CGUIBoxGTK3::PackEnd(std::shared_ptr<CGUIWidget> widget, bool expand, bool fill, int padding){
    std::shared_ptr<CGUIWidgetGTK3> WidgetToAdd = std::dynamic_pointer_cast<CGUIWidgetGTK3>(widget);
    
    gtk_box_pack_end(GTK_BOX(DWidget), WidgetToAdd->Widget(), expand, fill, padding);
}

/**
*
*@class CGUIMenuShellGTK3
*
*@brief This class gives logic for generating the shell of a menu GUI using the GTK3 library.
*
*@author Greg
*
*@version 3.0
*
*@date 10/23/2017 9:57:41
*
*contact: gjdost@ucdavis.edu
*/


CGUIMenuShellGTK3::CGUIMenuShellGTK3(GtkWidget *widget, bool reference) : CGUIContainerGTK3(widget, reference){
    
}

CGUIMenuShellGTK3::~CGUIMenuShellGTK3(){
   
}

/**
* Adds a new widget to the end of a menu shell's item list.
*
* @param[in] widget The widget to be added.
*
* @return Nothing.
*/
void CGUIMenuShellGTK3::Append(std::shared_ptr<CGUIWidget> widget){
    std::shared_ptr<CGUIWidgetGTK3> WidgetToAdd = std::dynamic_pointer_cast<CGUIWidgetGTK3>(widget);
    
    gtk_menu_shell_append(GTK_MENU_SHELL(DWidget), WidgetToAdd->Widget());
}

/**
*
*@class CGUIMenuGTK3
*
*@brief This class gives logic for generating a menu GUI using the GTK3 library.
*
*@author Greg
*
*@version 3.0
*
*@date 10/23/2017 9:59:43
*
*contact: gjdost@ucdavis.edu
*/

CGUIMenuGTK3::CGUIMenuGTK3(GtkWidget *widget, bool reference) : CGUIMenuShellGTK3(widget, reference){
    
}

CGUIMenuGTK3::~CGUIMenuGTK3(){
        
}

/**
*
*@class CGUIMenuBarGTK3
*
*@brief This class gives logic for generating a bar in a menu GUI using the GTK3 library.
*
*@author Greg
*
*@version 3.0
*
*@date 10/23/2017 10:00:14
*
*contact: gjdost@ucdavis.edu
*/

CGUIMenuBarGTK3::CGUIMenuBarGTK3(GtkWidget *widget, bool reference) : CGUIMenuShellGTK3(widget, reference){
    
}

CGUIMenuBarGTK3::~CGUIMenuBarGTK3(){
    
}

/**
*
*@class CGUIMenuItemGTK3
*
*@brief This class gives logic for generating an item in a menu GUI using the GTK3 library.
*
*@author Greg
*
*@version 3.0
*
*@date 10/23/2017 10:00:54
*
*contact: gjdost@ucdavis.edu
*/

CGUIMenuItemGTK3::CGUIMenuItemGTK3(GtkWidget *widget, bool reference) : CGUIContainerGTK3(widget, reference){
    
}

CGUIMenuItemGTK3::~CGUIMenuItemGTK3(){
    
}

/**
* Gets the labels for all widgets in the current menu.
*
* @param[in] Nothing.
*
* @return shared_ptr A shared_ptr to a list of the labels.
*/
std::shared_ptr<CGUILabel> CGUIMenuItemGTK3::GetLabel(){
    GList *List = gtk_container_get_children(GTK_CONTAINER(DWidget));
    
    return std::make_shared<CGUILabelGTK3>(GTK_WIDGET(List->data), true);
}

/**
* Gives the current menu a submenu.
*
* @param[in] widget A widget to be placed in the new submenu
*
* @return Nothing. 
*
*/
void CGUIMenuItemGTK3::SetSubMenu(std::shared_ptr<CGUIWidget> widget){
    std::shared_ptr<CGUIWidgetGTK3> WidgetToAdd = std::dynamic_pointer_cast<CGUIWidgetGTK3>(widget);
    
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(DWidget), WidgetToAdd->Widget());
}

/**
*
*@class CGUIWindowGTK3
*
*@brief This class gives logic for generating a window GUI using the GTK3 library.
*
*@author Greg
*
*@version 3.0
*
*@date 10/23/2017 10:01:43
*
*contact: gjdost@ucdavis.edu
*/

CGUIWindowGTK3::CGUIWindowGTK3(GtkWidget *widget, bool reference) : CGUIContainerGTK3(widget, reference){

}

CGUIWindowGTK3::~CGUIWindowGTK3(){
    
}

/**
* Sets a minimum possible size for widgets.
*
* @param[in] width The minimum width.
*
* @param[in] height The minimum height.
*
* @return Nothing.
*/
void CGUIWindowGTK3::SetMinSize(int width, int height){
    GdkGeometry Geometry;
    Geometry.min_width = width;
    Geometry.min_height = height;
    gtk_window_set_geometry_hints(GTK_WINDOW(DWidget), NULL, &Geometry, GDK_HINT_MIN_SIZE);
}

/**
* Sets a maximum possible size for widgets.
*
* @param[in] width The maximum width.
*
* @param[in] height The maximum height.
*
* @return Nothing.
*/
void CGUIWindowGTK3::SetMaxSize(int width, int height){
    GdkGeometry Geometry;
    Geometry.max_width = width;
    Geometry.max_height = height;
    gtk_window_set_geometry_hints(GTK_WINDOW(DWidget), NULL, &Geometry, GDK_HINT_MAX_SIZE);    
}

/**
* Sets a minimum possible size for a specific widget.
*
* @param[in] width The minimum width.
*
* @param[in] height The minimum height.
*
* @return Nothing.
*/
void CGUIWindowGTK3::SetMinSize(std::shared_ptr<CGUIWidget> widget, int width, int height){
    std::shared_ptr<CGUIWidgetGTK3> WidgetToSize = std::dynamic_pointer_cast<CGUIWidgetGTK3>(widget);
    GdkGeometry Geometry;
    Geometry.min_width = width;
    Geometry.min_height = height;
    gtk_window_set_geometry_hints(GTK_WINDOW(DWidget), WidgetToSize->Widget(), &Geometry, GDK_HINT_MIN_SIZE);
}


/**
* Sets a maximum possible size for a specific widget.
*
* @param[in] width The maximum width.
*
* @param[in] height The maximum height.
*
* @return Nothing.
*/
void CGUIWindowGTK3::SetMaxSize(std::shared_ptr<CGUIWidget> widget, int width, int height){
    std::shared_ptr<CGUIWidgetGTK3> WidgetToSize = std::dynamic_pointer_cast<CGUIWidgetGTK3>(widget);
    GdkGeometry Geometry;
    Geometry.max_width = width;
    Geometry.max_height = height;
    gtk_window_set_geometry_hints(GTK_WINDOW(DWidget), WidgetToSize->Widget(), &Geometry, GDK_HINT_MAX_SIZE);    
}

/**
* Gives a title to a GUI window.
*
* @param[in] title The title being given to the GUI window.
*
* @return Nothing.
*/
void CGUIWindowGTK3::SetTitle(const std::string &title){
    gtk_window_set_title(GTK_WINDOW(DWidget), title.c_str());
}

/**
* Closes a GUI window.
*
* @param[in] Nothing.
*
* @return Nothing.
*/
void CGUIWindowGTK3::Close(){
    gtk_window_close(GTK_WINDOW(DWidget));
}

/**
*
*@class CGUIFileFilterGTK3
*
*@brief This class gives logic for generating a file filter GUI using the GTK3 library.
*
*@author Greg
*
*@version 3.0
*
*@date 10/23/2017 10:03:41
*
*contact: gjdost@ucdavis.edu
*/


CGUIFileFilterGTK3::CGUIFileFilterGTK3(GtkFileFilter *filter, bool reference){
    DFilter = filter;
    //if(reference){
        g_object_ref(DFilter);   
    //}
}

CGUIFileFilterGTK3::~CGUIFileFilterGTK3(){
    g_object_unref(DFilter);    
}

/**
* Adds a rule allowing a shell style glob to a filter
*
* @param[in] pattern A shell style glob being allowed to a filter.
*
* @return Nothing.
*
*/
void CGUIFileFilterGTK3::AddPattern(const std::string &pattern){
    gtk_file_filter_add_pattern(DFilter, pattern.c_str());
}

/**
*
*@class CGUIFileChooserDialogGTK3
*
*@brief This class gives logic for generating a file choosing window GUI using the GTK3 library.
*
*@author Greg
*
*@version 3.0
*
*@date 10/23/2017 10:05:11
*
*contact: gjdost@ucdavis.edu
*/

CGUIFileChooserDialogGTK3::CGUIFileChooserDialogGTK3(GtkWidget *widget, bool reference) : CGUIWindowGTK3(widget, reference){
    
}

CGUIFileChooserDialogGTK3::~CGUIFileChooserDialogGTK3(){

}

/**
* Sets the current filter for files.
*
* @param[in] filter The shell style glob being set as the current filter.
*
* @return Nothing.
*
*/
void CGUIFileChooserDialogGTK3::SetFilter(std::shared_ptr<CGUIFileFilter> filter){
    std::shared_ptr<CGUIFileFilterGTK3> FilterToAdd = std::dynamic_pointer_cast<CGUIFileFilterGTK3>(filter);
    
    gtk_file_chooser_set_filter(GTK_FILE_CHOOSER(DWidget), FilterToAdd->DFilter);
}

/**
* Sets the current folder.
*
* @param[in] folder A string that gives the filepath to the current folder.
*
* @return Nothing.
*/
void CGUIFileChooserDialogGTK3::SetCurrentFolder(const std::string &folder){
    gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(DWidget), folder.c_str());    
}

/**
* Gets the name of the file for the current widget.
*
* @param[in] Nothing.
*
* @return string The name of the file in which the current widget is.
*/
std::string CGUIFileChooserDialogGTK3::GetFilename(){
    gchar *Filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(DWidget));
    std::string ReturnName((char *)Filename);
    g_free(Filename);
    return ReturnName;
}

/**
* Runs the file chooser.
*
* @param[in] Nothing.
*
* @return int Returns a 0 or 1 depending on whether or not the user chose to cancel the file chooser. 
*/
int CGUIFileChooserDialogGTK3::Run(){
    return GTK_RESPONSE_CANCEL == gtk_dialog_run(GTK_DIALOG(DWidget)) ? 0 : 1;
}

