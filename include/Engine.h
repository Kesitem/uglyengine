#pragma once

#include "Core.h"
#include "Application.h"
#include "InputManager.h"
#include "VulkanManager.h"

namespace ugly
{

/**
 * \class Engine
 * \brief Main class of the engine.
 * Engine is a singleton to easily use it anywhere.
 */
class Engine
{
public:

    /**
     * \brief Get the instance of the engine.
     */
    static Engine *const getInstance()
    {
        static Engine engine;
        return &engine;
    }

    /**
     * \brief Run the application.
     * Engine take ownership of the application. It will destroy application.
     * 
     * \param _application  Application to run
     * \return 0 if ok
     */
    int run(Application *_application);

    /**
     * \brief Request application to quit.
     */
    void quit();

    /**
     * \brief Get input manager.
     *
     * \return Input manager
     */
    InputManager* getInputManager() const;

    /**
     * \brief Get GLFW window.
     *
     * \return GLFW window.
     */
    GLFWwindow* getWindow() const;

private:

    /**
     * \brief Constructor.
     */
    Engine();

    /**
     * \brief Destructor.
     */
    virtual ~Engine();

    /**
     * \brief Initialize plog.
     */
    void initializePLog();

    /**
     * \brief Initialize engine.
     * 
     * \return false if error
     */
    bool initialize();

    /**
     * \brief Shutdown engine.
     */
    void shutdown();

    /**
     * \brief Execute main loop.
     * 
     * \return false if error
     */
    bool mainLoop();

private:

    /*! Application */
    std::unique_ptr<Application> m_application {nullptr};

    /*! GLFW Window */
    GLFWwindow* m_window {nullptr};

    /*! Display size */
    glm::ivec2 m_display_size {glm::ivec2(1280, 720)};

    /*! Quit flag */
    bool m_quit {false};
    
    /*! Input manager */
    std::unique_ptr<InputManager> m_input_manager {nullptr};

    /*! Vulkan manager */
    std::unique_ptr<VulkanManager> m_vulkan_manager {nullptr};
};

}//namespace ugly