#include "boundingSphere.h"

BoundingSphere::BoundingSphere(const float &p_fRadius, const glm::vec3 &p_vCenter){
    m_fRadius = p_fRadius;
    m_vCenter = p_vCenter;
}

BoundingSphere::~BoundingSphere(){
}

BoundingType BoundingSphere::getType() const{
    return BoundingType::SPHERE;
}
