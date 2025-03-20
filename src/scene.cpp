#include "scene.h"

Scene* Scene::m_pSceneSingleton = nullptr;

void Scene::createInstance(){
    //For debuggin purposes
    assert(m_pSceneSingleton == nullptr);
    m_pSceneSingleton = new Scene();
}

void Scene::deleteInstance(){
    //For debuggin purposes
    assert(m_pSceneSingleton != nullptr);
    delete m_pSceneSingleton;
    m_pSceneSingleton = nullptr;
}

Scene* Scene::instance(){
    //For debuggin purposes
    assert(m_pSceneSingleton);
    return m_pSceneSingleton;
}

void Scene::setActiveCamera(Camera* p_pCamera){
    m_pCamera = p_pCamera;
}

Camera *Scene::getActiveCamera() const{
    return m_pCamera;
}

void Scene::addNode(Node *p_pNode){
    m_vTopNodes.push_back(p_pNode);
}

void Scene::removeNode(Node *p_pNode){
}

void Scene::createQuadTree(){
    if (m_vTopNodes.empty()) return; //No nodes then no quadtree

    //So I need to force one update to actually subdivide the tree
    for(Node* node : m_vTopNodes){
        node->update(1.0f);
        for(Node* child : node->getChildren()){
            child->update(1.0f);
        }
    }
    AABB* boundsQuadTree = new AABB(glm::vec3(-200, -1000, -200), glm::vec3(200, 1000, 200));
    m_pQuadTree = new QuadTree(0, boundsQuadTree);

    for(Node* node : m_vTopNodes){
        m_pQuadTree->insertNode(node);
    }
    QuadTree::init(m_pQuadTree);
}

std::vector<Node*> Scene::doCulling(){
    glm::mat4 proj = m_pCamera->getProjMatrix(m_iWidth, m_iHeight);
    glm::mat4 view = m_pCamera->getViewMatrix();
    glm::mat4 viewPorj = proj*view;
    m_pFrustum->extractPlanes(viewPorj, true);

    m_vNodesToRender.clear();
    std::vector<Node*> visibleNodes;

    m_pQuadTree->query(*m_pFrustum, visibleNodes);
    m_iSizeofNode = visibleNodes.size();

    for (Node* node : visibleNodes){
        recursionTime(node);
    }
    return visibleNodes;
}

void Scene::recursionTime(Node *node){
    if(!node) return;

    m_vNodesToRender.push_back(node);

    const std::vector<Node*>& children = node->getChildren();
    if(children.empty()) return;

    for (Node* child : children){
        BoundingSphere* sphere = dynamic_cast<BoundingSphere*>(child->getBoundingVolume());
        if (sphere || m_pFrustum->isSphereInside(*sphere)){
            recursionTime(child);
         }     
    }
}

void Scene::render(int p_iWidth, int p_iHeight){
    m_iHeight = p_iHeight;
    m_iWidth = p_iWidth;
    glm::mat4 proj = m_pCamera->getProjMatrix(p_iWidth, p_iHeight);
    glm::mat4 view = m_pCamera->getViewMatrix();
    
    for(Node* node : m_vNodesToRender){
        node->render(view, proj);
    }
    m_pQuadTree->render(view, proj);

    //Start ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    
    renderUI();

    //Render ImGui
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Scene::uptade(float p_fDT){
    m_fDt = p_fDT;
    //Update active camera
    m_pCamera->update(p_fDT);

    //Update all nodes 
    for(const auto& node : m_vTopNodes){
        node->update(p_fDT);
    }
    m_vNodesToRender = doCulling();
}

void Scene::init(wolf::App* p_pApp){
    if (m_pCamera == nullptr){
        m_pCamera = new Camera(p_pApp, glm::vec3(0.0f, 30.0f, 0.0f));
        setActiveCamera(m_pCamera);
        m_pFrustum = new Frustum();
        m_vNodesToRender = {};
    }
}

void Scene::renderUI(){

    ImGui::Begin("Debug");
    ImGui::Text("Frame Time: %.3f ms (%.1f FPS)", m_fDt * 1000.0f, 1.0f / m_fDt);
    ImGui::Text("Total Scene Objects: %d", m_vTopNodes.size());
    ImGui::Text("Total Nodes in Frustum: %d", m_iSizeofNode);
    ImGui::End();

    ImGui::Begin("Debug Menu Camera");
    ImGui::Text("Camera Debugging");

    float fov = m_pCamera->getFov();
    float nearPlane = m_pCamera->getNearPlane();
    float farPlane = m_pCamera->getFarPlane();


    if (ImGui::SliderFloat("FOV", &fov, 45.0f, 100.0f)){
        m_pCamera->setFOV(fov);
    }
    
    if (ImGui::SliderFloat("Near Plane", &nearPlane, 0.1f, 10.0f)){
        m_pCamera->setNearPlane(nearPlane);
    }
    
    if (ImGui::SliderFloat("Far Plane", &farPlane, 10.0f, 1000.0f)){
        m_pCamera->setFarPlane(farPlane);
    }
    ImGui::End();

    ImGui::Begin("Toggles");
    ImGui::Text("Press P to enable mouse, BEWARE IT IS WONKY");
    if (ImGui::Button("Toggle Quadtree Rendering")){
        QuadTree::toggleRender();
    }
    if (ImGui::Button("Toggle Volume Rendering")){
        for (auto& node : m_vTopNodes){
            node->setIsVisible(!node->geIsVisible());
        }
    }   
    ImGui::End();
}

Scene::Scene(){
    m_pCamera = nullptr;
    m_pFrustum = nullptr;
    m_pQuadTree = nullptr;
}

Scene::~Scene(){
    std::cout << "Destroying Scene \n";
}

