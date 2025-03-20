#include "node.h"

int Node::generateUniqueID(){
    static int counter = 0;
    return counter++;
}

Node::Node() : m_vpChildren(){
    this->m_pParent = nullptr;
    m_iID = generateUniqueID();
    m_sName = "Node" + std::to_string(m_iID);
}

Node::~Node(){
    delete m_pParent;
    m_pParent = nullptr;
    m_vpChildren.clear();
}

void Node::setScale(const glm::vec3& p_vScale){
    m_vSca = p_vScale;
    m_bDirty = true;
}

void Node::setPos(const glm::vec3& p_vPos){
    m_vPos = p_vPos;
    m_bDirty = true;
}

void Node::setRot(const glm::vec3& p_vRot){
    m_vRot = p_vRot;
    m_bDirty = true;
}

void Node::scale(const glm::vec3& p_vScale){
    m_vSca *= p_vScale;
    m_bDirty = true;
}

void Node::rotate(const glm::vec3& p_vRot){
    m_vRot += p_vRot;
    m_bDirty = true;
}

void Node::translate(const glm::vec3& p_vPos){
    m_vPos += p_vPos;
    m_bDirty = true;
}

glm::vec3 Node::getScale() const{
    return m_vSca;
}

glm::vec3 Node::getRot() const{
    return m_vRot;
}

glm::vec3 Node::getPos() const{
    return m_vPos;
}

glm::vec3 Node::getWorldPos(){
    return glm::vec3();
}

Node *Node::getParent() const{
    return this->m_pParent;
}

std::vector<Node *>& Node::getChildren() {
    return this->m_vpChildren;
}

int Node::getID() const{
    return m_iID;
}

const std::string Node::getName() const{
    return m_sName;
}

void Node::setIsVisible(bool p_bIsVisible){
    m_bIsVisible = p_bIsVisible;
}

bool Node::geIsVisible() const{
    return m_bIsVisible;
}

BoundingVolume *Node::getBoundingVolume() const{
    return m_pBoundingVolume;
}

void Node::addChild(Node *m_pChild){
    m_pChild->m_pParent = this;
    this->m_vpChildren.push_back(m_pChild);
}

void Node::removeChild(Node *m_pChild){
    m_pChild->m_pParent = nullptr;
    this->m_vpChildren.erase(std::remove(m_vpChildren.begin(), m_vpChildren.end(), m_pChild), m_vpChildren.end());
}

glm::mat4 Node::getTransform(){
     if (m_bDirty){
        glm::mat4 transform = glm::mat4(1.0f);

        transform = glm::translate(transform, this->getPos());
        transform = glm::rotate(transform, this->getRot().x, glm::vec3(1.0f, 0.0f, 0.0f));
        transform = glm::rotate(transform, this->getRot().y, glm::vec3(0.0f, 1.0f, 0.0f));
        transform = glm::rotate(transform, this->getRot().z, glm::vec3(0.0f, 0.0f, 1.0f));
        transform = glm::scale(transform, this->getScale()); 

        this->m_mWorldTransform = transform;
        // this->m_bDirty = false; 
    }
    return m_mWorldTransform;
}

glm::mat4 Node::getWorldTransform(){
    if(m_pParent){
        return getTransform() * m_pParent->getWorldTransform();
    }
    return getTransform();
}

void Node::update(float p_fDT){
    if (m_bDirty){
        m_mWorldTransform = getWorldTransform();
        updateBoundingVolume();
        m_bDirty = false;
    }

    for (auto& child : m_vpChildren){
        child->m_mWorldTransform = child->getTransform();
        child->update(p_fDT);
    }
}