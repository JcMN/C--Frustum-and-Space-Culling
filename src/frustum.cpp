#include "frustum.h"

Frustum::Frustum(){
    for(auto& plane : m_aPlanes){
        plane = Plane();                
    }
}

Frustum::~Frustum(){ }

//There are multiple ways to extract the frustum of a plane
//This is just one of the most simple ones
void Frustum::extractPlanes(const glm::mat4 &p_mProjView, bool p_bNormalize){
    //All my troubles silved by transpose
    glm::mat4 transposed = glm::transpose(p_mProjView);
    //Left Plane
    m_aPlanes[0].a = transposed[3][0] + transposed[0][0];
    m_aPlanes[0].b = transposed[3][1] + transposed[0][1];
    m_aPlanes[0].c = transposed[3][2] + transposed[0][2];
    m_aPlanes[0].d = transposed[3][3] + transposed[0][3];
    //Right Plane
    m_aPlanes[1].a = transposed[3][0] - transposed[0][0];
    m_aPlanes[1].b = transposed[3][1] - transposed[0][1];
    m_aPlanes[1].c = transposed[3][2] - transposed[0][2];
    m_aPlanes[1].d = transposed[3][3] - transposed[0][3];
    //Top Plane
    m_aPlanes[2].a = transposed[3][0] - transposed[1][0];
    m_aPlanes[2].b = transposed[3][1] - transposed[1][1];
    m_aPlanes[2].c = transposed[3][2] - transposed[1][2];
    m_aPlanes[2].d = transposed[3][3] - transposed[1][3];
    //Bottom Plane
    m_aPlanes[3].a = transposed[3][0] + transposed[1][0];
    m_aPlanes[3].b = transposed[3][1] + transposed[1][1];
    m_aPlanes[3].c = transposed[3][2] + transposed[1][2];
    m_aPlanes[3].d = transposed[3][3] + transposed[1][3];
    //Near Plane
    m_aPlanes[4].a = transposed[3][0] + transposed[2][0];
    m_aPlanes[4].b = transposed[3][1] + transposed[2][1];
    m_aPlanes[4].c = transposed[3][2] + transposed[2][2];
    m_aPlanes[4].d = transposed[3][3] + transposed[2][3];
    //Far Plane
    m_aPlanes[5].a = transposed[3][0] - transposed[2][0];
    m_aPlanes[5].b = transposed[3][1] - transposed[2][1];
    m_aPlanes[5].c = transposed[3][2] - transposed[2][2];
    m_aPlanes[5].d = transposed[3][3] - transposed[2][3];

    if(p_bNormalize){
        for (size_t i = 0; i < 6; i++){
            normalizePlane(m_aPlanes[i]);
        }
    }        
}

void Frustum::normalizePlane(Plane &p_pPlane){
    float magnitude;

    magnitude = sqrt(p_pPlane.a * p_pPlane.a + p_pPlane.b * p_pPlane.b + p_pPlane.c * p_pPlane.c);
    p_pPlane.a /= magnitude;
    p_pPlane.b /= magnitude;
    p_pPlane.c /= magnitude;
    p_pPlane.d /= magnitude;
}

//Same as below :P
bool Frustum::isSphereInside(const BoundingSphere &p_rSphere){
    bool inside = true;
    for (const auto& plane : m_aPlanes){
        float distance = plane.getDistance(p_rSphere.m_vCenter);
        if (distance < -p_rSphere.m_fRadius){
            inside = false;
        }
    }
    return inside;
}

//Magic methods found online
bool Frustum::isAABBInside(const AABB &p_rAABB){
    for(const auto& plane: m_aPlanes){
        glm::vec3 positiveVer = p_rAABB.getMinCorner();
        glm::vec3 negativeVer = p_rAABB.getMaxCorner();

        if (plane.a >= 0){
            positiveVer.x = p_rAABB.getMaxCorner().x;
            negativeVer.x = p_rAABB.getMinCorner().x;
        }

        if (plane.b >= 0){
            positiveVer.y = p_rAABB.getMaxCorner().y;
            negativeVer.y = p_rAABB.getMinCorner().y;
        }
        
        if (plane.c >= 0){
            positiveVer.z = p_rAABB.getMaxCorner().z;
            negativeVer.z = p_rAABB.getMinCorner().z;
        }
        
        if (plane.getDistance(positiveVer) < 0 && plane.getDistance(negativeVer) < 0){
            return false;
        }
    }
    return true;
}
