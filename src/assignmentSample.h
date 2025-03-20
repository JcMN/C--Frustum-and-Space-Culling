#pragma once
#include "../wolf/wolf.h"
#include "../samplefw/Sample.h"
#include "scene.h"
#include "debugCube.h"
#include <cstdlib>
#include <ctime>


class Assignment : public Sample{
    public:
        Assignment(wolf::App* pApp) : Sample(pApp, "Culling Assignment"){}
        ~Assignment();

        void init() override;
        void update(float dt) override;
        void render(int width, int height) override;
        bool m_bIDK = true;
};