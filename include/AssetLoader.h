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
#ifndef ASSETLOADER_H
#define ASSETLOADER_H
#include "ApplicationData.h"
#include "Debug.h"

class CAssetLoader{
    protected:
        std::shared_ptr< CApplicationData> AppData;
        std::shared_ptr< CDataContainer > TempDataContainer;
        std::shared_ptr< CDataContainer > ImageDirectory;
        std::shared_ptr< CDataContainerIterator > FileIterator;
        std::shared_ptr< CDataSource > TempDataSource;
    public:
        CAssetLoader(std::shared_ptr< CApplicationData> PassedAppData, std::shared_ptr< CDataContainer >, std::shared_ptr< CDataContainer >);
        void LoadIconAsset();
        void LoadMiniIcons();
        void LoadCorpse();
        void LoadFireSmall();
        void LoadFireLarge();
        void LoadBuildingDeath();
        void LoadArrow();
        void LoadAssetColor();
        void LoadPeasant();
        void LoadFootman();
        void LoadArcher();
        void LoadRanger();
        void LoadGoldMine();
        void LoadTownHall();
        void LoadKeep();
        void LoadCastle();
        void LoadFarm();
        void LoadBarracks();
        void LoadBlacksmith();
        void LoadLumberMill();
        void LoadScoutTower();
        void LoadGuardTower();
        void LoadCannonTower();
};

#endif

