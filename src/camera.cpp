#include "camera.h"

Camera::Camera(wolf::App* p_pApp, const glm::vec3& p_vPos) : 
Node(){
    m_vPos = p_vPos;
    m_pApp = p_pApp;
    m_vFront = glm::vec3(0.0f, 0.0f, 1.0f);
    m_vRight = glm::vec3(1.0f, 0.0f, 0.0f);
    m_vUp = glm::vec3(0.0f, 1.0f, 1.0f);
    m_fMovementSpeed = SPEED;
    m_fMouseSensitivity = SENSITIVITY;
    m_fYaw = YAW;
    m_fPitch = PITCH;
    //Default values
    m_fFOV = 45.0f;
    m_fNearPlane = 0.1f;
    m_fFarPlane = 1000.0f;

    m_sName = "Camera" + std::to_string(m_iID);

     m_vLastMousePos = m_pApp->getMousePos();

    updateCameraVectors();
}

Camera::~Camera(){
    delete m_pApp;
}

glm::mat4 Camera::getViewMatrix() const{
    return glm::lookAt(m_vPos, m_vPos + m_vFront, m_vUp);
}
glm::mat4 Camera::getProjMatrix(int p_iWidth, int p_iHeight) const{
    return glm::perspective(glm::radians(m_fFOV), (float) p_iWidth / (float) p_iHeight, m_fNearPlane, m_fFarPlane);
}

void Camera::update(float p_fDT){
    if (m_pApp->isKeyJustDown('P') || m_pApp-> isKeyJustDown('p')){
        m_bToggle = !m_bToggle;
    }
     else if (m_bToggle){
        m_pApp->captureCursor();
        updateCameraVectors();
        mouseMovement(true);
        m_vLastMousePos = m_pApp->getMousePos();
        keyboardMovement(p_fDT);
    } else if(!m_bToggle){
        m_pApp->stopCaptureCursor();
    }
}

void Camera::render(const glm::mat4 &p_mView, const glm::mat4 &p_mProj){
    //Does not do anything but maybe later on work as a debug renderer for the frustum
    // Could be used to render frustum maybe at some point?
}

void Camera::updateBoundingVolume(){
    //Does not do anything but implementation needed
}

void Camera::setFOV(float p_fFOV){
    m_fFOV = p_fFOV;
}

void Camera::setNearPlane(float p_fNearPlane){
    m_fNearPlane = p_fNearPlane;
}

void Camera::setFarPlane(float p_fFarPlane){
    m_fFarPlane = p_fFarPlane;
}

float Camera::getFov() const{
    return m_fFOV;
}

float Camera::getNearPlane() const{
    return m_fNearPlane;
}

float Camera::getFarPlane() const{
    return m_fFarPlane;
}

void Camera::updateCameraVectors(){
    glm::vec3 front;
    front.x = cos(glm::radians(m_fYaw)) * cos(glm::radians(m_fPitch));
    front.y = sin(glm::radians(m_fPitch));
    front.z = sin(glm::radians(m_fYaw)) * cos(glm::radians(m_fPitch));
    m_vFront = glm::normalize(front);

    m_vRight = glm::normalize(glm::cross(m_vFront, glm::vec3(0.0f, 1.0f, 0.0f)));
    m_vUp = glm::normalize(glm::cross(m_vRight, m_vFront));
}

void Camera::keyboardMovement(float p_fDT){
    float currentVelocity = m_fMovementSpeed * p_fDT;

    if(m_pApp->isKeyDown(340)){
        if (m_pApp->isKeyDown('w') || m_pApp->isKeyDown('W')){
            m_vPos += m_vFront * currentVelocity * 20.0f;
        }
        if (m_pApp->isKeyDown('s') || m_pApp->isKeyDown('S')){
            m_vPos -= m_vFront * currentVelocity * 20.0f;
        }
        if (m_pApp->isKeyDown('a') || m_pApp->isKeyDown('A')){
            m_vPos -= m_vRight * currentVelocity * 20.0f;
        }
        if (m_pApp->isKeyDown('d') || m_pApp->isKeyDown('D')){
            m_vPos += m_vRight * currentVelocity * 20.0f;
        } 
    }
    if (m_pApp->isKeyDown('w') || m_pApp->isKeyDown('W')){
        m_vPos += m_vFront * currentVelocity;
    }
    if (m_pApp->isKeyDown('s') || m_pApp->isKeyDown('S')){
        m_vPos -= m_vFront * currentVelocity;
    }
    if (m_pApp->isKeyDown('a') || m_pApp->isKeyDown('A')){
        m_vPos -= m_vRight * currentVelocity;
    }
    if (m_pApp->isKeyDown('d') || m_pApp->isKeyDown('D')){
        m_vPos += m_vRight * currentVelocity;
    }
}

void Camera::mouseMovement(GLboolean p_bConstraintPitch){
    glm::vec2 mousePosition = m_pApp->getMousePos();

    float xoffset = (mousePosition.x - m_vLastMousePos.x) * m_fMouseSensitivity;
    float yoffset = (mousePosition.y - m_vLastMousePos.y) * m_fMouseSensitivity;

    m_fYaw += xoffset;
    m_fPitch += yoffset;

    if (p_bConstraintPitch){
        if(m_fPitch > 89.0f)
            m_fPitch = 89.0f;
        if(m_fPitch < -89.0f)
            m_fPitch = -89.0f;
    }
    updateCameraVectors();
}
