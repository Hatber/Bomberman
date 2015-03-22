#ifndef __CREATION__
#define __CREATION__

#include "Mixins/MCoordinates.hpp"
#include "Mixins/MHitPoints.hpp"

const float defaultMaxHP = 10;

class Creation : public MHitPoints<float>, public MCoordinates {
public :
    Creation() :  MHitPoints(defaultMaxHP) { }
    Creation(float maxHP) : MHitPoints(maxHP) { }
    Creation(float maxHP, float currentHP, int xPosition, int yPosition) :
        MHitPoints(maxHP, currentHP),
        MCoordinates(xPosition, yPosition)
    { }
};

#endif // __CREATION__
