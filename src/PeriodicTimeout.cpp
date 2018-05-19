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
#include "PeriodicTimeout.h"
#include <cstdint>
/*
int CTimeout::SecondsUntilDeadline(struct timeval deadline){
    struct timeval CurrentTime;
    
    gettimeofday(&CurrentTime, nullptr);
    
    return ((deadline.tv_sec * 1000 + deadline.tv_usec / 1000) - (CurrentTime.tv_sec * 1000 + CurrentTime.tv_usec / 1000)) / 1000;
};

int CTimeout::MiliSecondsUntilDeadline(struct timeval deadline){
    struct timeval CurrentTime;
    
    gettimeofday(&CurrentTime, nullptr);
    
    return ((deadline.tv_sec * 1000 + deadline.tv_usec / 1000) - (CurrentTime.tv_sec * 1000 + CurrentTime.tv_usec / 1000));
};
*/

CPeriodicTimeout::CPeriodicTimeout(int periodms){
    gettimeofday(&DNextExpectedTimeout, nullptr);
    if(0 >= periodms){
        DTimeoutInterval = 1000;        
    }
    else{
        DTimeoutInterval = periodms;    
    }
}

int CPeriodicTimeout::MiliSecondsUntilDeadline(){
    struct timeval CurrentTime;
    int64_t TimeDelta;
    
    gettimeofday(&CurrentTime, nullptr);
    TimeDelta = (DNextExpectedTimeout.tv_sec * 1000 + DNextExpectedTimeout.tv_usec / 1000) - (CurrentTime.tv_sec * 1000 + CurrentTime.tv_usec / 1000);
    while(0 >= TimeDelta){
        DNextExpectedTimeout.tv_usec += DTimeoutInterval * 1000;
        if(1000000 <= DNextExpectedTimeout.tv_usec){
            DNextExpectedTimeout.tv_usec %= 1000000;
            DNextExpectedTimeout.tv_sec++;
        }
        TimeDelta = (DNextExpectedTimeout.tv_sec * 1000 + DNextExpectedTimeout.tv_usec / 1000) - (CurrentTime.tv_sec * 1000 + CurrentTime.tv_usec / 1000);
    }
    return TimeDelta;
}
