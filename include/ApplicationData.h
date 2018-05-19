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
#ifndef APPLICATIONDATA_H
#define APPLICATIONDATA_H

#include "GUIFactory.h"
#include "GraphicFactory.h"
#include "FontTileset.h"
#include "GameModel.h"
#include "AIPlayer.h"
#include "ViewportRenderer.h"
#include "MiniMapRenderer.h"
#include "ResourceRenderer.h"
#include "UnitActionRenderer.h"
#include "UnitDescriptionRenderer.h"
#include "CursorSet.h"
#include "SoundEventRenderer.h"
#include "ButtonRenderer.h"
#include "EditRenderer.h"
#include "ListViewRenderer.h"
#include "ApplicationMode.h"

typedef void (*TButtonCallbackFunction)(void *calldata);
typedef bool (*TEditTextValidationCallbackFunction)(const std::string &text);

class CApplicationData : public std::enable_shared_from_this<CApplicationData>{
    friend class CAssetLoader;
    friend class CButtonMenuMode;
    friend class CServerConnectOptionMode;
    friend class CMainMenuMode;
    friend class COptionsMenuMode;
    friend class CInGameMenuMode;
    friend class CConnectionSelectionMenuMode;
    friend class CLogInOptionsMode;
    friend class CMultiPlayerOptionsMenuMode;
    friend class CEditOptionsMode;
    friend class CEndOfBattleMode;
    friend class CSoundOptionsMode;
    friend class CGameSelectionMode;
    friend class CNetworkOptionsMode;
    friend class CMapSelectionMode;
    friend class CPlayerAIColorSelectMode;
    friend class CHostGameOptionsMode;
    friend class CBattleMode;
    friend class CPlayerData; 
    struct SPrivateApplicationType{};
    protected:
        typedef enum{
            ctPointer = 0,
            ctInspect,
            ctArrowN,
            ctArrowE,
            ctArrowS,
            ctArrowW,
            ctTargetOff,
            ctTargetOn,
            ctMax
        } ECursorType, *ECursorTypeRef;

        typedef enum{
            uictNone = 0,
            uictViewport,
            uictViewportBevelN,
            uictViewportBevelE,
            uictViewportBevelS,
            uictViewportBevelW,
            uictMiniMap,
            uictUserDescription,
            uictUserAction,
            uictMenuButton
        } EUIComponentType, *EUIComponentTypeRef;

        typedef enum{
            gstSinglePlayer,
            gstMultiPlayerHost,
            gstMultiPlayerClient
        } EGameSessionType, *EGameSessionTypeRef;

        typedef enum{
            ptNone = 0,
            ptHuman,
            ptAIEasy,
            ptAIMedium,
            ptAIHard
        } EPlayerType, *EPlayerTypeRef;

        static std::shared_ptr< CApplicationData > DApplicationDataPointer;
        bool DDeleted;
        EGameSessionType DGameSessionType;
        float DSoundVolume;
        float DMusicVolume;
        std::string DUsername;
        std::string DPassword;
        // std::string DRemoteHostname;
        std::string DLANIPAddress;
        // int DMultiplayerPort;
        int DLANPort;
        std::shared_ptr<CGUIApplication> DApplication;
        std::shared_ptr<CGUIWindow> DMainWindow;
        std::shared_ptr<CGUIDrawingArea> DDrawingArea;
        std::shared_ptr<CGUICursor> DBlankCursor;
        std::shared_ptr<CGraphicSurface> DDoubleBufferSurface;
        std::shared_ptr<CGraphicSurface> DWorkingBufferSurface;
        std::shared_ptr<CGraphicSurface> DMiniMapSurface;
        std::shared_ptr<CGraphicSurface> DViewportSurface;
        std::shared_ptr<CGraphicSurface> DViewportTypeSurface;
        std::shared_ptr<CGraphicSurface> DUnitDescriptionSurface;
        std::shared_ptr<CGraphicSurface> DUnitActionSurface;
        std::shared_ptr<CGraphicSurface> DResourceSurface;
        std::shared_ptr<CGraphicSurface> DMapSelectListViewSurface;
        uint32_t DMiniMapViewportColor;

        int DBorderWidth;
        int DPanningSpeed;
        int DViewportXOffset, DViewportYOffset;
        int DMiniMapXOffset, DMiniMapYOffset;
        int DUnitDescriptionXOffset, DUnitDescriptionYOffset;
        int DUnitActionXOffset, DUnitActionYOffset;
        int DMenuButtonXOffset, DMenuButtonYOffset;
        int DMapSelectListViewXOffset, DMapSelectListViewYOffset;

        bool DMapConfirmed;
        int DSelectedMapIndex;
        std::shared_ptr< CAssetDecoratedMap > DSelectedMap;
        int DOptionsEditSelected;
        int DOptionsEditSelectedCharacter;
        std::vector< SRectangle > DOptionsEditLocations;
        std::vector< std::string > DOptionsEditTitles;
        std::vector< std::string > DOptionsEditText;
        std::vector< TEditTextValidationCallbackFunction > DOptionsEditValidationFunctions;

        std::shared_ptr< CMapRenderer > DMapRenderer;
        std::shared_ptr< CCursorSet > DCursorSet;
        int DCursorIndices[ctMax];
        ECursorType DCursorType;
        std::shared_ptr< CSoundLibraryMixer > DSoundLibraryMixer;
        std::shared_ptr< CSoundEventRenderer > DSoundEventRenderer;
        std::array<std::shared_ptr< CFontTileset >, to_underlying(CUnitDescriptionRenderer::EFontSize::Max) > DFonts;
        int DTotalLoadingSteps;
        int DCurrentLoadingStep;
        std::shared_ptr< CGraphicResourceContext > DLoadingResourceContext;
        std::shared_ptr< CGraphicTileset > DSplashTileset;
        std::shared_ptr< CGraphicTileset > DMarkerTileset;
        std::shared_ptr< CGraphicTileset > DBackgroundTileset;
        std::shared_ptr< CGraphicTileset > DMiniBevelTileset;
        std::shared_ptr< CGraphicTileset > DInnerBevelTileset;
        std::shared_ptr< CGraphicTileset > DOuterBevelTileset;
        std::shared_ptr< CGraphicTileset > DListViewIconTileset;
        std::shared_ptr< CBevel > DMiniBevel;
        std::shared_ptr< CBevel > DInnerBevel;
        std::shared_ptr< CBevel > DOuterBevel;
        std::vector< char > DMapRendererConfigurationData;
        std::shared_ptr< CGraphicTileset > DTerrainTileset;
        std::shared_ptr< CGraphicTileset > DFogTileset;
        std::shared_ptr< CGraphicRecolorMap > DAssetRecolorMap;
        std::shared_ptr< CGraphicRecolorMap > DButtonRecolorMap;
        std::shared_ptr< CGraphicRecolorMap > DFontRecolorMap;
        std::shared_ptr< CGraphicRecolorMap > DPlayerRecolorMap;
        std::shared_ptr< CGraphicMulticolorTileset > DIconTileset;
        std::shared_ptr< CGraphicTileset > DMiniIconTileset;
        std::vector< std::shared_ptr< CGraphicMulticolorTileset > > DAssetTilesets;
        std::vector< std::shared_ptr< CGraphicTileset > > DFireTilesets;
        std::shared_ptr< CGraphicTileset > DBuildingDeathTileset;
        std::shared_ptr< CGraphicTileset > DCorpseTileset;
        std::shared_ptr< CGraphicTileset > DArrowTileset;
        std::shared_ptr< CAssetRenderer > DAssetRenderer;
        std::shared_ptr< CFogRenderer > DFogRenderer;
        std::shared_ptr< CViewportRenderer > DViewportRenderer;
        std::shared_ptr< CMiniMapRenderer > DMiniMapRenderer;
        std::shared_ptr< CUnitDescriptionRenderer > DUnitDescriptionRenderer;
        std::shared_ptr< CUnitActionRenderer > DUnitActionRenderer;
        std::shared_ptr< CResourceRenderer > DResourceRenderer;
        std::shared_ptr< CButtonRenderer > DMenuButtonRenderer;
        std::shared_ptr< CButtonRenderer > DButtonRenderer;
        std::shared_ptr< CListViewRenderer > DMapSelectListViewRenderer;
        std::shared_ptr< CEditRenderer > DOptionsEditRenderer;

        // Model
        EPlayerColor DPlayerColor;
        std::shared_ptr< CGameModel > DGameModel;
        std::array< SPlayerCommandRequest, to_underlying(EPlayerColor::Max) > DPlayerCommands;
        std::array< std::shared_ptr< CAIPlayer >, to_underlying(EPlayerColor::Max) > DAIPlayers;
        std::array< EPlayerType, to_underlying(EPlayerColor::Max) > DLoadingPlayerTypes;
        std::array< EPlayerColor, to_underlying(EPlayerColor::Max) > DLoadingPlayerColors;
        std::array< std::string, to_underlying(EPlayerColor::Max) > DPlayerNames;

        std::shared_ptr< CApplicationMode > DApplicationMode;
        std::shared_ptr< CApplicationMode > DNextApplicationMode;

        std::unordered_map < uint32_t, std::list< std::weak_ptr< CPlayerAsset > > > DGroupHotKeyMap; 
        std::unordered_map< uint32_t, EAssetCapabilityType > DUnitHotKeyMap;
        std::unordered_map< uint32_t, EAssetCapabilityType > DBuildHotKeyMap;
        std::unordered_map< uint32_t, EAssetCapabilityType > DTrainHotKeyMap;
        std::list< std::weak_ptr< CPlayerAsset > > DSelectedPlayerAssets;
        EAssetCapabilityType DCurrentAssetCapability;
        std::vector< uint32_t > DPressedKeys;
        std::vector< uint32_t > DReleasedKeys;
        int DCurrentX;
        int DCurrentY;
        int DTime;  
        CPixelPosition DMouseDown;
        int DLeftClick;
        int DRightClick;
        bool DLeftDown;
        bool DRightDown;
        CButtonRenderer::EButtonState DMenuButtonState;

        static void ActivateCallback(TGUICalldata data);
        static bool TimeoutCallback(TGUICalldata data);
        static bool MainWindowDeleteEventCallback(std::shared_ptr<CGUIWidget> widget, TGUICalldata data);
        static void MainWindowDestroyCallback(std::shared_ptr<CGUIWidget> widget, TGUICalldata data);
        static bool MainWindowKeyPressEventCallback(std::shared_ptr<CGUIWidget> widget, SGUIKeyEvent &event, TGUICalldata data);
        static bool MainWindowKeyReleaseEventCallback(std::shared_ptr<CGUIWidget> widget, SGUIKeyEvent &event, TGUICalldata data);
        static bool MainWindowConfigureEventCallback(std::shared_ptr<CGUIWidget> widget, SGUIConfigureEvent &event, TGUICalldata data);
        static bool DrawingAreaDrawCallback(std::shared_ptr<CGUIWidget> widget, std::shared_ptr<CGraphicResourceContext> rc, TGUICalldata data);
        static bool DrawingAreaButtonPressEventCallback(std::shared_ptr<CGUIWidget> widget, SGUIButtonEvent &event, TGUICalldata data);
        static bool DrawingAreaButtonReleaseEventCallback(std::shared_ptr<CGUIWidget> widget, SGUIButtonEvent &event, TGUICalldata data);
        static bool DrawingAreaMotionNotifyEventCallback(std::shared_ptr<CGUIWidget> widget, SGUIMotionEvent &event, TGUICalldata data);
        
        static int TestLua();
        
        void Activate();
        bool Timeout();
        bool MainWindowDeleteEvent(std::shared_ptr<CGUIWidget> widget);
        void MainWindowDestroy(std::shared_ptr<CGUIWidget> widget);
        bool MainWindowKeyPressEvent(std::shared_ptr<CGUIWidget> widget, SGUIKeyEvent &event);
        bool MainWindowKeyReleaseEvent(std::shared_ptr<CGUIWidget> widget, SGUIKeyEvent &event);
        bool MainWindowConfigureEvent(std::shared_ptr<CGUIWidget> widget, SGUIConfigureEvent &event);
        bool DrawingAreaDraw(std::shared_ptr<CGUIWidget> widget, std::shared_ptr<CGraphicResourceContext> rc);
        bool DrawingAreaButtonPressEvent(std::shared_ptr<CGUIWidget> widget, SGUIButtonEvent &event);
        bool DrawingAreaButtonReleaseEvent(std::shared_ptr<CGUIWidget> widget, SGUIButtonEvent &event);
        bool DrawingAreaMotionNotifyEvent(std::shared_ptr<CGUIWidget> widget, SGUIMotionEvent &event);

        EUIComponentType FindUIComponentType(const CPixelPosition &pos);
        CPixelPosition ScreenToViewport(const CPixelPosition &pos);
        CPixelPosition ScreenToMiniMap(const CPixelPosition &pos);
        CPixelPosition ScreenToDetailedMap(const CPixelPosition &pos);
        CPixelPosition ScreenToUnitDescription(const CPixelPosition &pos);
        CPixelPosition ScreenToUnitAction(const CPixelPosition &pos);
        CPixelPosition ViewportToDetailedMap(const CPixelPosition &pos);
        CPixelPosition MiniMapToDetailedMap(const CPixelPosition &pos);

        // Output
        void RenderMenuTitle(const std::string &title, int &titlebottomy, int &pagewidth, int &pageheight);
        void RenderSplashStep();
        static void SoundLoadingCallback(TSoundLibraryLoadingCalldata data);


        void ChangeApplicationMode(std::shared_ptr< CApplicationMode > mode);
        bool ModeIsChanging();

        void LoadGameMap(int index);
        bool LoadSoundOptions(std::shared_ptr< CDataContainer > container);
        // bool LoadNetworkOptions(std::shared_ptr< CDataContainer > container);
        void ResetPlayerColors();
        void ResizeCanvases();

    public:
        static std::map <int, std::shared_ptr< CPlayerAsset > > DAssetObjectIDMap;

        int GetTime() {
            return DTime;
        }
        
	explicit CApplicationData(const std::string &appname, const SPrivateApplicationType &key);
        ~CApplicationData();

	static void AddAssetID(std::shared_ptr<     CPlayerAsset > asset, int id);
        static std::shared_ptr< CApplicationData > Instance(const std::string &appname);

        int Run(int argc, char *argv[]);

};
#endif
