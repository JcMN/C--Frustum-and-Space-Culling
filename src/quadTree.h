#pragma once
#ifndef QUADTREE_H
#define QUADTREE_H
#include <vector>
#include "aabb.h"
#include "../wolf/wolf.h"
#include "node.h"
#include "frustum.h"


class QuadTree{
    public:
        QuadTree(int p_iLevel, AABB* p_pBounds);
        ~QuadTree();

        static const int MAX_OBJECTS = 5;
        static const int MAX_LEVELS = 4;

        void insertNode(Node* p_pNode);
        
        void subDivide();

        void render(const glm::mat4& p_mView, const glm::mat4 p_mProj);

        void query(Frustum& p_pFrustum, std::vector<Node*>& p_vVisibleNodes);

        static void init(QuadTree* quadtree);
        static void toggleRender();
        void collectLines(std::vector<glm::vec3>& lines);

    private:
        int m_iLevel;
        std::vector<Node*> m_vNodes;
        QuadTree* m_pChildren[4] = {nullptr, nullptr, nullptr, nullptr};
        AABB* m_pBounds;

        
        static wolf::VertexBuffer* m_pVb;
        static wolf::VertexDeclaration* m_pDecl;
        static wolf::Material* m_pMat;

        static std::vector<glm::vec3> m_vLineVertices;

        static bool m_bRenderCheck;
};

#endif
