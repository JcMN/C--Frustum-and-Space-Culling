#pragma once
#ifndef BOUNDINGVOLUME_H
#define BOUNDINGVOLUME_H
#include "extras.h"

class BoundingVolume{
    public:
        BoundingVolume(){};
        virtual ~BoundingVolume() = default;
        virtual BoundingType getType() const = 0;
};

#endif