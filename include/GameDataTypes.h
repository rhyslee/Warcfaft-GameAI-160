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
#ifndef GAMEDATATYPES_H
#define GAMEDATATYPES_H

#include <type_traits>
enum class EPlayerColor : int{
    None = 0,
    Red,
    Blue,
    Green,
    Purple,
    Orange,
    Yellow,
    Black,
    White,
    Max 
};

enum class EAssetAction : int{
    None = 0,
    Construct,
    Build,
    Repair,
    Walk,
    StandGround,
    Attack,
    HarvestLumber,
    MineGold,
    QuarryStone,
    ConveyLumber,
    ConveyGold,
    ConveyStone,
    Death,
    Decay,
    Capability
};

enum class EAssetCapabilityType : int{
    None = 0,
    BuildPeasant,
    BuildFootman,
    BuildArcher,
    BuildRanger,
    BuildKnight,
    BuildFarm,
    BuildTownHall,
    BuildBarracks,
    BuildLumberMill,
    BuildBlacksmith,
    BuildKeep,
    BuildCastle,
    BuildScoutTower,
    BuildGuardTower,
    BuildCannonTower,
    Move,
    Repair,
    Mine,
    BuildSimple,
    BuildAdvanced,
    Convey,
    Cancel,
    BuildWall,
    Attack,
    StandGround,
    Patrol,
    WeaponUpgrade1,
    WeaponUpgrade2,
    WeaponUpgrade3,
    ArrowUpgrade1,
    ArrowUpgrade2,
    ArrowUpgrade3,
    ArmorUpgrade1,
    ArmorUpgrade2,
    ArmorUpgrade3,
    Longbow,
    RangerScouting,
    Marksmanship,
    Max
};

enum class EAssetType : int{
    None = 0,
    Peasant,
    Footman,
    Archer,
    Ranger,
    GoldMine,
    TownHall,//6
    Keep,//7
    Castle,//8
    Farm,//9
    Barracks,//10
    LumberMill,//11
    Blacksmith,//12
    ScoutTower,//13
    GuardTower,//14
    CannonTower,//15
    Wall,//16
    Max
};

enum class EDirection : int{
    North = 0,
    NorthEast,
    East,
    SouthEast,
    South,
    SouthWest,
    West,
    NorthWest,
    Max
};


template<typename E>
constexpr typename std::underlying_type<E>::type to_underlying(E enumerator) noexcept{
    return static_cast<typename std::underlying_type<E>::type>(enumerator);
}

#define DirectionOpposite(dir)      static_cast<EDirection>( (to_underlying(dir) + to_underlying(EDirection::Max) / 2) % to_underlying(EDirection::Max))



#endif

