#pragma once
#ifndef DEBUGCUBE_H
#define DEBUGCUBE_H
#include "node.h"
#include "extras.h"
#include "aabb.h"
#include "boundingSphere.h"


class DebugCube : public Node{

    public:
        DebugCube();
        ~DebugCube();

        void setColour(const glm::vec3& p_vCol);
        glm::vec3 getColour() const;

        void updateBoundingVolume() override;

        void render(const glm::mat4& p_mView, const glm::mat4& p_mProj) override;

        void setUpSphereLines();
        void renderSphereVolume(BoundingSphere* p_pBoundingSphere, const glm::mat4& p_mView, const glm::mat4& p_mProj);

        static const Vertex gs_cubeVertices[];
    private:
        glm::vec3 m_vColour;

        wolf::VertexBuffer* m_pVb = nullptr;
        wolf::VertexDeclaration* m_pDecl = nullptr;
        wolf::Material* m_pMat = nullptr;

        wolf::VertexBuffer* m_pVbSphere = nullptr;
        wolf::VertexDeclaration* m_pDeclSphere = nullptr;
        wolf::Material* m_pMatSphere = nullptr;

};


#endif
