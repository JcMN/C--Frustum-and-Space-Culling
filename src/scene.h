#pragma once
#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H
#include "../wolf/wolf.h"
#include <cassert>
#include <vector>
#include "node.h"
#include "camera.h"
#include "frustum.h"
#include "quadTree.h"

class Scene{
    public:
        static void createInstance();
        static void deleteInstance();
        static Scene* instance();

        void setActiveCamera(Camera* p_pCamera);
        Camera* getActiveCamera() const;

        void addNode(Node* p_pNode);
        void removeNode(Node* p_pNode);

        void createQuadTree();
        std::vector<Node*> doCulling();
        void recursionTime(Node* node);

        void render(int p_iWidth, int p_iHeight);
        void uptade(float p_fDT);
        void init(wolf::App* p_pApp);

        void renderUI();
    
    private:
        Scene();
        ~Scene();

        static Scene* m_pSceneSingleton;
        Camera* m_pCamera;
        Frustum* m_pFrustum;
        QuadTree* m_pQuadTree;

        float m_fDt = 0;

        int m_iHeight, m_iWidth;

        std::vector<Node*> m_vTopNodes;
        std::vector<Node*> m_vNodesToRender;
        int m_iSizeofNode;
};

#endif