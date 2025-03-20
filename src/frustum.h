#pragma once
#ifndef FRUSTUM_H
#define FRUSTUM_H
#include "../wolf/wolf.h"
#include "extras.h"
#include <array>
#include "boundingSphere.h"
#include "aabb.h"
#include <iostream>

class Frustum{
    public:
        Frustum();
        ~Frustum();

        void extractPlanes(const glm::mat4& p_mProjView, bool p_bNormalize = true);
        void normalizePlane(Plane& p_pPlane);

        bool isSphereInside(const BoundingSphere& p_rSphere);
        bool isAABBInside(const AABB& p_rAABB);

    private:
        std::array<Plane, 6> m_aPlanes{};

};

#endif