#include "debugcube.h"

DebugCube::DebugCube() : Node (){
    m_sName = "Cube" + std::to_string(m_iID);
	this->setRot(glm::vec3(0.0f, 0.0f, 0.0f));
	this->m_pBoundingVolume = new BoundingSphere(1.0f, glm::vec3(0.0f));
	m_vColour = glm::vec3(0.0f, 0.0, 1.0f);

    if(!DebugCube::m_pMat){
        glEnable(GL_DEPTH_TEST);
	
		m_pMat = wolf::MaterialManager::CreateMaterial("cube_mat");
        m_pMat->SetProgram("data/cube.vsh", "data/cube.fsh");		
        m_pMat->SetDepthTest(true);
        m_pMat->SetDepthWrite(true);

		m_pVb = wolf::BufferManager::CreateVertexBuffer(DebugCube::gs_cubeVertices, sizeof(Vertex) * 36);

        m_pDecl = new wolf::VertexDeclaration();
        m_pDecl->Begin();
        m_pDecl->AppendAttribute(wolf::AT_Position, 3, wolf::CT_Float);
        m_pDecl->AppendAttribute(wolf::AT_Color, 4, wolf::CT_UByte);
        m_pDecl->SetVertexBuffer(m_pVb);
        m_pDecl->End();
    }
}

DebugCube::~DebugCube(){
	std::cout << "Destroying material and everything that needs destruction \n";
	wolf::MaterialManager::DestroyMaterial(m_pMat);
	wolf::MaterialManager::DestroyMaterial(m_pMatSphere);
}

void DebugCube::setColour(const glm::vec3 &p_vCol){
    m_vColour = p_vCol;
}

glm::vec3 DebugCube::getColour() const{
    return m_vColour;
}


void DebugCube::updateBoundingVolume(){

	BoundingType type = m_pBoundingVolume->getType();
	delete m_pBoundingVolume;
	m_pBoundingVolume = nullptr;
		
	glm::vec3 pos = glm::vec3(getWorldTransform()[3]);
	glm::mat3 rotationScale = glm::mat3(getWorldTransform());

	glm::vec3 scale(
		glm::length(rotationScale[0]), //X axis
		glm::length(rotationScale[1]), //Y axis
		glm::length(rotationScale[2])  //Z axis
	);
		
	glm::mat3 rotation = glm::mat3(
		rotationScale[0] / scale.x, rotationScale[1] / scale.y, rotationScale[2] / scale.z 
	);

	glm::vec3 halfExtents = scale * 0.5f;

	switch (type){
		//AABB Are not really implemented here yet
		case BoundingType::AABB:
			m_pBoundingVolume = new AABB((pos - halfExtents), (pos + halfExtents));
			break;
		case BoundingType::SPHERE:
			m_pBoundingVolume = new BoundingSphere(glm::length(halfExtents), pos);
			break;
	}

	BoundingSphere* parentSphere = static_cast<BoundingSphere*>(m_pBoundingVolume);
	glm::vec3 parentCenter = parentSphere->m_vCenter;
	float parentRadius = parentSphere->m_fRadius;

	std::vector<glm::vec3> centers;
	std::vector<float> radiuses;//radiia? whatever

	centers.push_back(parentCenter);
	radiuses.push_back(parentRadius);

	for (auto& child : m_vpChildren){
		child->updateBoundingVolume();

		//Like I said before only spheres are working so....
		if (child->getBoundingVolume()->getType() == BoundingType::SPHERE){
			BoundingSphere* childSphere = static_cast<BoundingSphere*>(child->getBoundingVolume());
			centers.push_back(childSphere->m_vCenter);
			radiuses.push_back(childSphere->m_fRadius);
		}
	}
	if(centers.size() > 1){
		glm::vec3 newCenter = parentCenter;
		float newRadius = parentRadius;

		for (size_t i = 0; i < centers.size(); i++){
			float dist = glm::distance(newCenter, centers[i]) + radiuses[i];

			if (dist > newRadius){
				float shift = (dist - newRadius) * 0.5f;
				newCenter += glm::normalize(centers[i] - newCenter) * shift;
				newRadius += shift;
			}
		}
		parentSphere->m_vCenter = newCenter;
		parentSphere->m_fRadius = newRadius;
	}
}

void DebugCube::render(const glm::mat4 &p_mView, const glm::mat4 &p_mProj) {
    m_pMat->SetUniform("proj", p_mProj);
    m_pMat->SetUniform("view", p_mView);
    m_pMat->SetUniform("world", getWorldTransform());
    m_pMat->SetUniform("u_color", m_vColour);
	m_pMat->Apply();

    m_pDecl->Bind();

    glDrawArrays(GL_TRIANGLES, 0, 36);

	if (m_bIsVisible && m_pBoundingVolume && m_pBoundingVolume->getType() == BoundingType::SPHERE) {
        BoundingSphere* sphere = dynamic_cast<BoundingSphere*>(m_pBoundingVolume);
        if (sphere) {
            renderSphereVolume(sphere, p_mView, p_mProj);
        }
	}

	for (const auto& child : m_vpChildren){
		child->render(p_mView, p_mProj);
	}
}

//I was told this was the way to create simple spheres
void DebugCube::setUpSphereLines(){
	if (m_pMatSphere) return;

	std::vector<float> vertices;
	int segments = 32;
	for (size_t i = 0; i <= segments; i++){
		float theta = (2.0f * glm::pi<float>()) * (float(i) / float(segments));
		float x = cos(theta);
		float z = sin(theta);

		vertices.push_back(x);
		vertices.push_back(0.0f);
		vertices.push_back(z);
	}

		for (size_t i = 0; i <= segments; i++){
		float theta = (2.0f * glm::pi<float>()) * (float(i) / float(segments));
		float x = cos(theta);
		float z = sin(theta);

		vertices.push_back(0.0f);
		vertices.push_back(x);
		vertices.push_back(z);
	}

		for (size_t i = 0; i <= segments; i++){
		float theta = (2.0f * glm::pi<float>()) * (float(i) / float(segments));
		float x = cos(theta);
		float z = sin(theta);

		vertices.push_back(x);
		vertices.push_back(z);
		vertices.push_back(0.0f);
	}

	m_pMatSphere = wolf::MaterialManager::CreateMaterial("sphere_mat");
    m_pMatSphere->SetProgram("data/sphere.vsh", "data/sphere.fsh");		

	m_pVbSphere = wolf::BufferManager::CreateVertexBuffer(vertices.data(), vertices.size() * sizeof(float));

    m_pDeclSphere = new wolf::VertexDeclaration();
    m_pDeclSphere->Begin();
    m_pDeclSphere->AppendAttribute(wolf::AT_Position, 3, wolf::CT_Float);
    m_pDeclSphere->SetVertexBuffer(m_pVbSphere);
    m_pDeclSphere->End();
	
}

void DebugCube::renderSphereVolume(BoundingSphere *p_pBoundingSphere, const glm::mat4& p_mView, const glm::mat4& p_mProj){
	if(!m_pMatSphere) setUpSphereLines();

	glm::mat4 world = glm::translate(glm::mat4(1.0f), p_pBoundingSphere->m_vCenter) *
					glm::scale(glm::mat4(1.0f), glm::vec3(p_pBoundingSphere->m_fRadius));
	
	m_pMatSphere->SetUniform("proj", p_mProj);
    m_pMatSphere->SetUniform("view", p_mView);
    m_pMatSphere->SetUniform("world", world);
	m_pMatSphere->Apply();

    m_pDeclSphere->Bind();

//So I found out this to render the bounding volumes, idk
    glDrawArrays(GL_LINE_STRIP, 0, 33);
	glDrawArrays(GL_LINE_STRIP, 33, 33);
	glDrawArrays(GL_LINE_STRIP, 66, 33);
}

const Vertex DebugCube::gs_cubeVertices[] = {
// Front
	{ -0.5f, -0.5f, 0.5f, 255, 0, 0, 255 },
	{ -0.5f,  0.5f, 0.5f, 255, 0, 0, 255 },
	{  0.5f,  0.5f, 0.5f, 255, 0, 0, 255 },
	{  0.5f,  0.5f, 0.5f, 255, 0, 0, 255 },
	{  0.5f, -0.5f, 0.5f, 255, 0, 0, 255 },
	{ -0.5f, -0.5f, 0.5f, 255, 0, 0, 255 },

	// Back
	{  0.5f,  0.5f,-0.5f, 128, 0, 0, 255 },
	{ -0.5f,  0.5f,-0.5f, 128, 0, 0, 255 },
	{ -0.5f, -0.5f,-0.5f, 128, 0, 0, 255 },
	{ -0.5f, -0.5f,-0.5f, 128, 0, 0, 255 },
	{  0.5f, -0.5f,-0.5f, 128, 0, 0, 255 },
	{  0.5f,  0.5f,-0.5f, 128, 0, 0, 255 },

	// Left
	{ -0.5f,  0.5f,-0.5f, 0, 255, 0, 255 },
	{ -0.5f,  0.5f, 0.5f, 0, 255, 0, 255 },
	{ -0.5f, -0.5f, 0.5f, 0, 255, 0, 255 },
	{ -0.5f, -0.5f, 0.5f, 0, 255, 0, 255 },
	{ -0.5f, -0.5f,-0.5f, 0, 255, 0, 255 },
	{ -0.5f,  0.5f,-0.5f, 0, 255, 0, 255 },

	// Right
	{  0.5f,  0.5f, 0.5f, 0, 128, 0, 255 },
	{  0.5f,  0.5f,-0.5f, 0, 128, 0, 255 },
	{  0.5f, -0.5f,-0.5f, 0, 128, 0, 255 },
	{  0.5f, -0.5f,-0.5f, 0, 128, 0, 255 },
	{  0.5f, -0.5f, 0.5f, 0, 128, 0, 255 },
	{  0.5f,  0.5f, 0.5f, 0, 128, 0, 255 },

	// Top
	{ -0.5f,  0.5f, 0.5f, 0, 0, 255, 255 },
	{ -0.5f,  0.5f,-0.5f, 0, 0, 255, 255 },
	{  0.5f,  0.5f,-0.5f, 0, 0, 255, 255 },
	{  0.5f,  0.5f,-0.5f, 0, 0, 255, 255 },
	{  0.5f,  0.5f, 0.5f, 0, 0, 255, 255 },
	{ -0.5f,  0.5f, 0.5f, 0, 0, 255, 255 },

	// Bottom
	{ -0.5f, -0.5f, 0.5f, 0, 0, 128, 255 },
	{  0.5f, -0.5f, 0.5f, 0, 0, 128, 255 },
	{  0.5f, -0.5f,-0.5f, 0, 0, 128, 255 },
	{  0.5f, -0.5f,-0.5f, 0, 0, 128, 255 },
	{ -0.5f, -0.5f,-0.5f, 0, 0, 128, 255 },
	{ -0.5f, -0.5f, 0.5f, 0, 0, 128, 255 },
};