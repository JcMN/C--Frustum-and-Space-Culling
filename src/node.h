#pragma once
#ifndef NODE_H
#define NODE_H
#include "../wolf/wolf.h"
#include <iostream>
#include <vector>
#include "boundingVolume.h"

class Node{
    public:
        Node();
        virtual ~Node();

        void setScale(const glm::vec3& p_vScale);
        void setPos(const glm::vec3& p_vPos);
        void setRot(const glm::vec3& p_vRot);

        void scale(const glm::vec3& p_vScale);
        void rotate(const glm::vec3& p_vRot);
        void translate(const glm::vec3& p_vPos);

        glm::mat4 getWorldTransform();
        glm::mat4 getTransform();

        glm::vec3 getScale() const;
        glm::vec3 getRot() const;
        glm::vec3 getPos() const;
        glm::vec3 getWorldPos();

        Node* getParent() const;
        std::vector<Node*>& getChildren() ;

        int getID() const;
        const std::string getName() const;

        void setIsVisible(bool p_bIsVisible);
        bool geIsVisible() const;

        BoundingVolume* getBoundingVolume() const;
        virtual void updateBoundingVolume() = 0;

        void addChild(Node* m_pChild);
        void removeChild(Node* m_pChild);

        virtual void update(float p_fDT);
        virtual void render(const glm::mat4& p_mView, const glm::mat4& p_mProj) = 0;

    protected:
        glm::vec3 m_vPos;
        glm::vec3 m_vRot;
        glm::vec3 m_vSca;
        std::string m_sName;
        std::vector<Node*> m_vpChildren;
        BoundingVolume* m_pBoundingVolume = nullptr;
        int m_iID;
        bool m_bIsVisible = false;

    private:
        static int generateUniqueID();

        glm::mat4 m_mWorldTransform;

        Node* m_pParent = nullptr;

        bool m_bDirty;
};

#endif