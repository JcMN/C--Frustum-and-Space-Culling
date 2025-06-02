#pragma once
#ifndef CAMERA_H
#define CAMERA_H
#include "node.h"
#include "../wolf/wolf.h"
//Based on the LearnOpenGL first person camera

//Default Camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.0f;
const float SENSITIVITY = 0.6f;

class Camera : public Node{

    public:
        Camera(wolf::App* p_pApp, const glm::vec3& p_vPos);
        ~Camera();

        glm::mat4 getViewMatrix() const;
        glm::mat4 getProjMatrix(int p_iWidth, int p_iHeight) const;

        void update(float p_fDT) override;
        void render(const glm::mat4& p_mView, const glm::mat4& p_mProj) override;
        void updateBoundingVolume() override;

        void setFOV(float p_fFOV);
        void setNearPlane(float p_fNearPlane);
        void setFarPlane(float p_fFarPlane);

        float getFov() const;
        float getNearPlane() const;
        float getFarPlane() const;

    private:
        void updateCameraVectors();
        void keyboardMovement(float p_fDT);
        void mouseMovement(GLboolean p_bConstraintPitch = true);

        glm::vec3 m_vFront;
        glm::vec3 m_vUp;
        glm::vec3 m_vRight;
        glm::vec2 m_vLastMousePos;

        float m_fYaw;
        float m_fPitch;

        float m_fMovementSpeed;
        float m_fMouseSensitivity;

        float m_fFOV;
        float m_fNearPlane;
        float m_fFarPlane;

        wolf::App* m_pApp;

        bool m_bToggle = true;
};

#endif