#pragma once
#ifndef AABB_H
#define AABB_H
#include "boundingVolume.h"
#include "boundingSphere.h"
#include "../wolf/wolf.h"
#include <iostream>

class AABB : public BoundingVolume{
    public:
        AABB(const glm::vec3& p_vMinCor, const glm::vec3& p_vMaxCor);
        ~AABB();

        BoundingType getType() const override;

        glm::vec3 getMinCorner() const;
        glm::vec3 getMaxCorner() const;

        bool intersectionAABBSphere(const BoundingSphere* p_rSphere) const;

    private:
        glm::vec3 m_vMinCorner;
        glm::vec3 m_vMaxCorner;
};

#endif