#include "quadTree.h"

wolf::VertexBuffer* QuadTree::m_pVb = nullptr;
wolf::VertexDeclaration* QuadTree::m_pDecl = nullptr;
wolf::Material* QuadTree::m_pMat = nullptr;
std::vector<glm::vec3> QuadTree::m_vLineVertices;
bool QuadTree::m_bRenderCheck = true;

QuadTree::QuadTree(int p_iLevel, AABB *p_pBounds){
    m_iLevel = p_iLevel;
    m_pBounds = p_pBounds;
}

QuadTree::~QuadTree(){
    delete m_pBounds;
    m_pBounds = nullptr;
    for (size_t i = 0; i < 4; i++){
        if (m_pChildren[i]){
            delete m_pChildren[i];
            m_pChildren[i] = nullptr;
        }
    }
    m_vNodes.clear();
}

//This is not perfect at all but it works
void QuadTree::insertNode(Node* p_pNode){
    BoundingVolume* volume = p_pNode->getBoundingVolume();
    BoundingSphere* sphere = dynamic_cast<BoundingSphere*>(volume);
    if(!m_pBounds->intersectionAABBSphere(sphere)){
        return;
    }

    if (m_vNodes.size() < MAX_OBJECTS || m_iLevel >= MAX_LEVELS){
        m_vNodes.push_back(p_pNode);
        return;
    }

    if(!m_pChildren[0]){
        subDivide();
    }
    
    bool inserted = false;
    for (size_t i = 0; i < 4; i++){
        if (m_pChildren[i]->m_pBounds->intersectionAABBSphere(sphere)){
            m_pChildren[i]->insertNode(p_pNode);
            inserted = true;
            break;
        }
    }
    if(!inserted){
        m_vNodes.push_back(p_pNode);
    }
    
}

void QuadTree::subDivide(){
    glm::vec3 min = m_pBounds->getMinCorner();
    glm::vec3 max = m_pBounds->getMaxCorner();
    glm::vec3 center = (m_pBounds->getMinCorner() + m_pBounds->getMaxCorner()) * 0.5f;
    //REMEMBER TO PUT Y AS A HIGH VALUE
    m_pChildren[0] = new QuadTree((m_iLevel + 1), new AABB({min.x, min.z, min.z}, {center.x, max.y, center.z}));
    m_pChildren[1] = new QuadTree((m_iLevel + 1), new AABB({center.x, min.y, min.z}, {max.x, max.y, center.z}));
    m_pChildren[2] = new QuadTree((m_iLevel + 1), new AABB({min.x, min.y, center.z}, {center.x, max.y, max.z}));
    m_pChildren[3] = new QuadTree((m_iLevel + 1), new AABB(center, max));

    std::cout << "Subdividing node at level " << m_iLevel 
          << " with " << m_vNodes.size() << " objects." << std::endl;
}

void QuadTree::init(QuadTree* quadtree){
    m_vLineVertices.clear();
    quadtree->collectLines(m_vLineVertices);

    if(!m_pMat){
        m_pMat = wolf::MaterialManager::CreateMaterial("quadTree_mat");
        m_pMat->SetProgram("data/quadtree.vsh", "data/quadtree.fsh");

        m_pVb = wolf::BufferManager::CreateVertexBuffer(m_vLineVertices.data(), sizeof(glm::vec3) * m_vLineVertices.size());

        m_pDecl = new wolf::VertexDeclaration();
        m_pDecl->Begin();
        m_pDecl->AppendAttribute(wolf::AT_Position, 3, wolf::CT_Float);
        m_pDecl->SetVertexBuffer(m_pVb);
        m_pDecl->End();
    }
}

void QuadTree::toggleRender(){
    m_bRenderCheck = !m_bRenderCheck;
}

void QuadTree::collectLines(std::vector<glm::vec3>& lines){
    if(!m_pBounds){
        return;
    }

    glm::vec3 min = m_pBounds->getMinCorner();
    glm::vec3 max = m_pBounds->getMaxCorner();

    lines.push_back(glm::vec3(min.x, 0.0f, min.z));
    lines.push_back(glm::vec3(max.x, 0.0f, min.z));
    lines.push_back(glm::vec3(max.x, 0.0f, min.z));
    lines.push_back(glm::vec3(max.x, 0.0f, max.z));
    lines.push_back(glm::vec3(max.x, 0.0f, max.z));
    lines.push_back(glm::vec3(min.x, 0.0f, max.z));
    lines.push_back(glm::vec3(min.x, 0.0f, max.z));
    lines.push_back(glm::vec3(min.x, 0.0f, min.z));

    for (size_t i = 0; i < 4; i++){
        if(m_pChildren[i]){
            m_pChildren[i]->collectLines(lines);
        }
    }

}

void QuadTree::render(const glm::mat4& p_mView, const glm::mat4 p_mProj){
    if(!m_bRenderCheck) return;

    m_pMat->SetUniform("proj", p_mProj);
    m_pMat->SetUniform("view", p_mView);
    m_pMat->Apply();

    m_pDecl->Bind();
    
    glDrawArrays(GL_LINES, 0, m_vLineVertices.size());

    if (m_pChildren[0]){
        for (size_t i = 0; i < 4; i++){
            m_pChildren[i]->render(p_mView, p_mProj);
        }
    }
}

void QuadTree::query(Frustum &p_pFrustum, std::vector<Node *>& p_vVisibleNodes){
    if(!p_pFrustum.isAABBInside(*m_pBounds)) return; //Early rejection baby

    for (Node* node : m_vNodes){
        BoundingSphere* sphere = dynamic_cast<BoundingSphere*>(node->getBoundingVolume());
        if (p_pFrustum.isSphereInside(*sphere)){
            p_vVisibleNodes.push_back(node); //Visible baby
        }
    }

    for (size_t i = 0; i < 4; i++){
        if(m_pChildren[i] != nullptr){
            m_pChildren[i]->query(p_pFrustum, p_vVisibleNodes);
        }
    }
}
