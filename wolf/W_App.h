#pragma once
#define GLEW_NO_GLU
#include "W_Types.h"
#include <string>
#include "../thirdparty/imgui/imgui.h"
#include "../thirdparty/imgui/imgui_impl_glfw.h"
#include "../thirdparty/imgui/imgui_impl_opengl3.h"

namespace wolf
{
    class App
    {
    public:
        App(const std::string &name);
        virtual ~App();

        virtual void run();
        virtual void update(float dt) = 0;
        virtual void render() = 0;

        bool isKeyDown(int key) const;
        bool isKeyJustDown(int key) const;
        bool isLMBDown() const;
        bool isRMBDown() const;
        bool isMMBDown() const;
        glm::vec2 getMousePos() const;
        glm::vec2 getScreenSize() const;
        glm::vec2 getMouseScroll() const { return m_mouseScroll; }
        GLFWwindow* getWindow() const { return m_pWindow; }

        // Used internally but needs to be public
        void _setMouseScroll(const glm::vec2 &scroll);

        //Capture the cursor for mouse movement
        void captureCursor();
        

    protected:
        int m_width = 0;
        int m_height = 0;

    private:
        void _init();
        void _internalUpdate(float dt);

        GLFWwindow *m_pWindow = nullptr;
        std::string m_name;
        glm::vec2 m_mouseScroll;

        glm::vec2 m_vLastMousePos;

        bool m_bMouseCapture = true;

        static const int NUM_KEYS = GLFW_KEY_LAST - GLFW_KEY_SPACE;
        bool m_lastKeysDown[NUM_KEYS];
    };
}