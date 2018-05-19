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
#include "Position.h"

int CPosition::DTileWidth = 1;
int CPosition::DTileHeight = 1;
int CPosition::DHalfTileWidth = 0;
int CPosition::DHalfTileHeight = 0;
std::vector< std::vector < EDirection > > CPosition::DOctant{{EDirection::Max}};
std::vector< std::vector < EDirection > > CPosition::DTileDirections
    {{EDirection::NorthWest, EDirection::North, EDirection::NorthEast},
    {EDirection::West, EDirection::Max, EDirection::East},
    {EDirection::SouthWest, EDirection::South, EDirection::SouthEast}};

/**
* @author Wonhee
*
* @version 1.0
*
* @date 10/20/2017 23:00:00
*
* Contact: wonpark@ucdavis.edu 
*/

/**
* @brief constructor of CPosition. CPosition inherited by CPixelPosition and CTilePosition
*
* @param[in] int x
* @param[in] int y
*
* @return nbothing
*/
CPosition::CPosition(int x, int y){
    DX = x;
    DY = y;
}

/**
* @brief defines == operator
*
* @param[in] const position from CPosition
*
* @return boolean value of == assignment success/fail
*/
bool CPosition::operator==(const CPosition &pos) const{
    return (DX == pos.DX) && (DY == pos.DY);
}

/**
* @brief defines != operator
*
* @param[in] const position from CPosition
*
* @return boolean value of != assignment success/fail
*/
bool CPosition::operator!=(const CPosition &pos) const{
    return (DX != pos.DX) || (DY != pos.DY);
}

/**
* @brief States what tile octant to go to based on position.
*
* @param[in] const pos from CPosition
*
* @return enum EDirection type of TileOctant call
*/
EDirection CPosition::DirectionTo(const CPosition &pos) const{
    CPosition DeltaPosition(pos.DX - DX, pos.DY - DY);
    int DivX = DeltaPosition.DX / HalfTileWidth();
    int DivY = DeltaPosition.DY / HalfTileHeight();
    int Div;
    DivX = 0 > DivX ? -DivX : DivX;
    DivY = 0 > DivY ? -DivY : DivY;
    Div = DivX > DivY ? DivX : DivY;

    if(Div){
        DeltaPosition.DX /= Div;
        DeltaPosition.DY /= Div;
    }
    DeltaPosition.DX += HalfTileWidth();
    DeltaPosition.DY += HalfTileHeight();
    if(0 > DeltaPosition.DX){
        DeltaPosition.DX = 0;
    }
    if(0 > DeltaPosition.DY){
        DeltaPosition.DY = 0;
    }
    if(TileWidth() <= DeltaPosition.DX){
        DeltaPosition.DX = TileWidth() - 1;
    }
    if(TileHeight() <= DeltaPosition.DY){
        DeltaPosition.DY = TileHeight() - 1;
    }
    return DeltaPosition.TileOctant();
}

/**
* @brief Octant coordinates
*
* @param[in] nothing
*
* @return enum EDirection type of Octant coordinates
*/
EDirection CPosition::TileOctant() const{
    return DOctant[DY % DTileHeight][DX % DTileWidth];
}

/**
* @brief calculates distance squared based on X and Y coordinates of position
*
* @param[in] const pos from CPosition
*
* @return int 
*/
int CPosition::DistanceSquared(const CPosition &pos){
    int DeltaX = pos.DX - DX;
    int DeltaY = pos.DY - DY;
    
    return DeltaX * DeltaX + DeltaY * DeltaY;
}

/**
* @brief calculates distance of position
*
* @param[in] const pos from CPosition
*
* @return int 
*/
int CPosition::Distance(const CPosition &pos){
    // Code from http://www.codecodex.com/wiki/Calculate_an_integer_square_root
    unsigned int Op, Result, One;
    
    Op = DistanceSquared(pos);
    Result = 0;
    
    One = 1 << (sizeof(unsigned int) * 8 - 2);
    while(One > Op){
        One >>= 2;
    }
    while(0 != One){
        if(Op >= Result + One){
            Op -= Result + One;  
            Result += One << 1;  // <-- faster than 2 * one  
        }
        Result >>= 1;
        One >>= 2;
    }
    return Result;    
}

/**
* @brief sets tile dimensions (w*h)
*
* @param[in] int width, int height
*
* @return nothing
*/
void CPosition::SetTileDimensions(int width, int height){
    if((0 < width)&&(0 < height)){
        DTileWidth = width;   
        DTileHeight = height;
        DHalfTileWidth = width / 2;
        DHalfTileHeight = height / 2;
        
        DOctant.resize(DTileHeight);
        for(auto &Row : DOctant){
            Row.resize(DTileWidth);
        }
        for(int Y = 0; Y < DTileHeight; Y++){
            for(int X = 0; X < DTileWidth; X++){
                int XDistance = X - DHalfTileWidth;
                int YDistance = Y - DHalfTileHeight;
                bool NegativeX = XDistance < 0;
                bool NegativeY = YDistance > 0; // Top of screen is 0
                double SinSquared;
                
                XDistance *= XDistance;
                YDistance *= YDistance;
                
                if(0 == (XDistance + YDistance)){
                    DOctant[Y][X] = EDirection::Max;
                    continue;
                }
                SinSquared = (double)YDistance / (XDistance + YDistance);
                
                if(0.1464466094 > SinSquared){
                    // East or West
                    if(NegativeX){
                        DOctant[Y][X] = EDirection::West; // West   
                    }
                    else{
                        DOctant[Y][X] = EDirection::East; // East
                    }
                }
                else if(0.85355339059 > SinSquared){
                    // NE, SE, SW, NW
                    if(NegativeY){
                        if(NegativeX){
                            DOctant[Y][X] = EDirection::SouthWest; // SW
                        }
                        else{
                            DOctant[Y][X] = EDirection::SouthEast; // SE
                        }
                    }
                    else{
                        if(NegativeX){
                            DOctant[Y][X] = EDirection::NorthWest; // NW
                        }
                        else{
                            DOctant[Y][X] = EDirection::NorthEast; // NE
                        }
                    }
                }
                else{
                    // North or South
                    if(NegativeY){
                        DOctant[Y][X] = EDirection::South; // South    
                    }
                    else{
                        DOctant[Y][X] = EDirection::North; // North
                    }
                }
            }
        }  
    }
}

/**
* @brief constructor of CTilePosition. Inherited from Cposition class.
*
* @param[in] int x of CPosition
* @param[in] int y of CPosition
*
* @return nbothing
*/
CTilePosition::CTilePosition(int x, int y) : CPosition(x,y){
    
}

/**
* @brief defines == operator
*
* @param[in] const position from CTilePosition
*
* @return boolean value of == assignment success/fail
*/
bool CTilePosition::operator==(const CTilePosition &pos) const{
    return CPosition::operator==(pos);
}

/**
* @brief defines != operator
*
* @param[in] const position from CTilePosition
*
* @return boolean value of != assignment success/fail
*/
bool CTilePosition::operator!=(const CTilePosition &pos) const{
    return CPosition::operator!=(pos);    
}

/**
* @brief sets tile coordinates according to tile width and height
*
* @param[in] const position from CPixelPosition
*
* @return nothing
*/
void CTilePosition::SetFromPixel(const CPixelPosition &pos){
    DX = pos.X() / DTileWidth;
    DY = pos.Y() / DTileHeight;
}

/**
* @brief sets x coordinate of tile
*
* @param[in] int x
*
* @return nothing
*/
void CTilePosition::SetXFromPixel(int x){
    DX = x / DTileWidth;
}

/**
* @brief sets y coordinate of tile
*
* @param[in] int y
*
* @return nothing
*/
void CTilePosition::SetYFromPixel(int y){
    DY = y / DTileHeight;
}

/**
* @brief States direction of adjacent tile 
*
* @param[in] const pos from CTilePosition 
* @param[in] int object size
*
* @return enum EDirection type of AdjacentTileDirection call
*/
EDirection CTilePosition::AdjacentTileDirection(const CTilePosition &pos, int objsize) const{
    if(1 == objsize){
        int DeltaX = pos.DX - DX;
        int DeltaY = pos.DY - DY;
        
        if((1 < (DeltaX * DeltaX))||(1 < (DeltaY * DeltaY))){
            return EDirection::Max;   
        }
        
        return DTileDirections[DeltaY+1][DeltaX+1];
    }
    else{
         CPixelPosition ThisPixelPosition;
         CPixelPosition TargetPixelPosition;
         CTilePosition TargetTilePosition;
         
         ThisPixelPosition.SetFromTile(*this);
         TargetPixelPosition.SetFromTile(pos);
         
         TargetTilePosition.SetFromPixel( ThisPixelPosition.ClosestPosition(TargetPixelPosition, objsize) );
         return AdjacentTileDirection(TargetTilePosition, 1);
    }
}

/**
* @brief calculates distance squared of tile 
*
* @param[in] const pos from CTilePosition 
*
* @return int
*/
int CTilePosition::DistanceSquared(const CTilePosition &pos){
    return CPosition::DistanceSquared(pos);
}

/**
* @brief constructor of CPixelPosition. Inherited from Cposition class.
*
* @param[in] int x of CPosition
* @param[in] int y of CPosition
*
* @return nbothing
*/
CPixelPosition::CPixelPosition(int x, int y) : CPosition(x,y){
    
}

/**
* @brief defines == operator
*
* @param[in] const position from CPixelPosition
*
* @return boolean value of == assignment success/fail
*/
bool CPixelPosition::operator==(const CPixelPosition &pos) const{
    return CPosition::operator==(pos);
}

/**
* @brief defines != operator
*
* @param[in] const position from CPixelPosition
*
* @return boolean value of != assignment success/fail
*/
bool CPixelPosition::operator!=(const CPixelPosition &pos) const{
    return CPosition::operator!=(pos);    
}

/**
* @brief States direction to of pixel pos
*
* @param[in] const pos from CPixelPosition 
*
* @return enum EDirection type of DirectionTo call
*/
EDirection CPixelPosition::DirectionTo(const CPixelPosition &pos) const{
    return CPosition::DirectionTo(pos);
}

/**
* @brief States tile octant 
*
* @param[in] const pos from CPixelPosition 
*
* @return enum EDirection type of DirectionTo call
*/
EDirection CPixelPosition::TileOctant() const{
    return CPosition::TileOctant();   
}

/**
* @brief sets pixel coordinates according to tile width and height
*
* @param[in] const position from CTilePosition
*
* @return nothing
*/
void CPixelPosition::SetFromTile(const CTilePosition &pos){
    DX = pos.X() * DTileWidth + DHalfTileWidth;
    DY = pos.Y() * DTileHeight + DHalfTileHeight;
}

/**
* @brief sets pixel coordinates according to tile width 
*
* @param[in] int x 
*
* @return nothing
*/
void CPixelPosition::SetXFromTile(int x){
    DX = x * DTileWidth + DHalfTileWidth;
}

/**
* @brief sets pixel coordinates according to tile height
*
* @param[in] int y
*
* @return nothing
*/
void CPixelPosition::SetYFromTile(int y){
    DY = y * DTileHeight + DHalfTileHeight;
}

/**
* @brief States closest position of pixel
*
* @param[in] const objpos from CPixelPosition 
* @param[in] int object size
*
* @return enum CPixelPostion of BestPosition
*/
CPixelPosition CPixelPosition::ClosestPosition(const CPixelPosition &objpos, int objsize) const{
    CPixelPosition CurPosition(objpos);
    CPixelPosition BestPosition;
    int BestDistance = -1;
    for(int YIndex = 0; YIndex < objsize; YIndex++){
        for(int XIndex = 0; XIndex < objsize; XIndex++){
            int CurDistance = CurPosition.DistanceSquared(*this);
            if((-1 == BestDistance)||(CurDistance < BestDistance)){
                BestDistance = CurDistance;
                BestPosition = CurPosition;
            }
            CurPosition.IncrementX(CPosition::TileWidth());
        }
        CurPosition.X(objpos.X());
        CurPosition.IncrementY(CPosition::TileHeight());
    }
    return BestPosition;
}

/**
* @brief calculate distance squared of pixel
*
* @param[in] const position from CPixelPosition
*
* @return int
*/
int CPixelPosition::DistanceSquared(const CPixelPosition &pos){
    return CPosition::DistanceSquared(pos);
}

/**
* @brief calculate distance of pixel
*
* @param[in] const position from CPixelPosition
*
* @return int
*/
int CPixelPosition::Distance(const CPixelPosition &pos){
    return CPosition::Distance(pos);    
}