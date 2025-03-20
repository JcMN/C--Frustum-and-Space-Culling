#pragma once
#ifndef BOUNDINGSPHERE_H
#define BOUNDINGSPHERE_H
#include "boundingVolume.h"
#include "../wolf/wolf.h"


class BoundingSphere : public BoundingVolume{
    public:
        BoundingSphere(const float& p_fRadius, const glm::vec3& p_fCenter);
        ~BoundingSphere();

        BoundingType getType() const override;
        
        float m_fRadius;
        glm::vec3 m_vCenter;

    private:


};
#endif