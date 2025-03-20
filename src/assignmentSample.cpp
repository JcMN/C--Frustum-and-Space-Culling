#include "assignmentSample.h"

Assignment::~Assignment(){
    std::cout << "Destroyin something sample \n";
    Scene::deleteInstance();
}

void Assignment::init(){
    if (m_bIDK){
        Scene::createInstance();
        Scene::instance()->init(m_pApp);
        srand(static_cast<unsigned>(time(0)));
        int numberCubes = 75 + (rand() % 21);
        std::vector<Node*> whatCubes;

        for (size_t i = 0; i < numberCubes; i++){  
            DebugCube* cube = new DebugCube();

            float scaleX = 1.0f + static_cast<float>(rand()) / (RAND_MAX - 1) * 3.0f;
            float scaleY = 1.0f + static_cast<float>(rand()) / (RAND_MAX - 1) * 3.0f;
            float scaleZ = 1.0f + static_cast<float>(rand()) / (RAND_MAX - 1) * 3.0f;

            float posX = -100.0f + static_cast<float>(rand()) / RAND_MAX * 200.0f;
            float posY = static_cast<float>(rand()) / RAND_MAX * 10.0f;
            float posZ = -100.0f + static_cast<float>(rand()) / RAND_MAX * 200.0f;

            cube->setPos(glm::vec3(posX, posY, posZ));
            cube->setScale(glm::vec3(scaleX, scaleY, scaleZ));

            Scene::instance()->addNode(cube);
            whatCubes.push_back(cube);
        }
        
        //Childs
        for(const auto& cube : whatCubes){
            if(rand() % 100 < 75){
                int childrenNum = 1 + (rand() % 4);
                for (size_t i = 0; i < childrenNum; i++){
                    DebugCube* child = new DebugCube();

                    float cScaleX = 1.0f + static_cast<float>(rand()) / (RAND_MAX) * 1.0f;
                    float cScaleY = 1.0f + static_cast<float>(rand()) / (RAND_MAX) * 1.0f;
                    float cScaleZ = 1.0f + static_cast<float>(rand()) / (RAND_MAX) * 1.0f;

                    glm::vec3 offset = glm::vec3(
                        -1.0f + static_cast<float>(rand()) / RAND_MAX * 2.0f,
                        0.5f + static_cast<float>(rand()) / RAND_MAX * 1.5f,
                        -1.0f + static_cast<float>(rand()) / RAND_MAX * 2.0f
                    );

                    child->setScale(glm::vec3(cScaleX, cScaleY, cScaleZ));
                    child->setPos(offset);

                    cube->addChild(child);
                    Scene::instance()->addNode(child);
                    child->setColour(glm::vec3(0.0f, 1.0f, 0.0f));
                }
            }
        }
        Scene::instance()->createQuadTree();
        m_pApp->captureCursor();
        m_bIDK = false;
    }
}

void Assignment::update(float dt){
    Scene::instance()->uptade(dt);
}

void Assignment::render(int width, int height){
    glEnable(GL_DEPTH_TEST);
	glClearColor(0.3f, 0.3f, 1.0f, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    Scene::instance()->render(width, height);
}
