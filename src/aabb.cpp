#include "aabb.h"

AABB::AABB(const glm::vec3 &p_vMinCor, const glm::vec3 &p_vMaxCor){
    m_vMinCorner = p_vMinCor;
    m_vMaxCorner = p_vMaxCor;
}

AABB::~AABB(){
}

BoundingType AABB::getType() const{
    return BoundingType::AABB;
}

glm::vec3 AABB::getMinCorner() const{
    return m_vMinCorner;
}

glm::vec3 AABB::getMaxCorner() const{
    return m_vMaxCorner;
}

bool AABB::intersectionAABBSphere(const BoundingSphere* p_rSphere) const{

    glm::vec3 closestPoint(
        glm::clamp(p_rSphere->m_vCenter.x, m_vMinCorner.x, m_vMaxCorner.x),
        glm::clamp(p_rSphere->m_vCenter.y, m_vMinCorner.y, m_vMaxCorner.y),
        glm::clamp(p_rSphere->m_vCenter.z, m_vMinCorner.z, m_vMaxCorner.z)
    );
    
    return glm::distance(p_rSphere->m_vCenter, closestPoint) <= p_rSphere->m_fRadius;
}
