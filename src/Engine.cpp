#include "Engine.h"
#include "LogFormatter.h"

/**
 * \brief Constructor.
 */
ugly::Engine::Engine()
{
}


/**
 * \brief Destructor.
 */
ugly::Engine::~Engine()
{
}


/**
 * \brief Run the application.
 * Engine take ownership of the application. It will destroy application.
 * 
 * \param _application  Application to run
 * \return 0 if ok
 */
int ugly::Engine::run(Application *_application)
{
    initializePLog();

    if(_application == nullptr)
    {
        LOG_ERROR << "Invalid application";
        return -1;
    }

    m_application.reset(_application);
    LOG_INFO << "Run application: " << m_application->getName();

    if(!initialize())
    {
        PLOG_ERROR << "Failed to initialize engine";
        shutdown();
        return -2;
    }

    if(!mainLoop())
    {
        PLOG_ERROR << "Failed to run main loop";
        shutdown();
        return -3;
    }

    shutdown();

    return 0;
}


/**
 * \brief Request application to quit.
 */
void ugly::Engine::quit()
{
    m_quit = true;
}


/**
 * \brief Get input manager.
 *
 * \return Input manager
 */
ugly::InputManager* ugly::Engine::getInputManager() const
{
    return m_input_manager.get();
}


/**
 * \brief Get GLFW window.
 *
 * \return GLFW window.
 */
GLFWwindow* ugly::Engine::getWindow() const
{
    return m_window;
}


/**
 * \brief Initialize plog.
 */
void ugly::Engine::initializePLog()
{
    // Remove log file if exists
    struct stat buffer;
    if (stat(LOG_FILENAME.c_str(), &buffer) == 0)
    {
        if (remove(LOG_FILENAME.c_str()) != 0)
        {
            LOG_ERROR << "Cannnot remove log file";
        }
    }

    // Create log
    static plog::RollingFileAppender<plog::LogFormatter> fileAppender(LOG_FILENAME.c_str(), 0, 0);
    static plog::ConsoleAppender<plog::LogFormatter> consoleAppender;

    plog::init(plog::debug, &consoleAppender).addAppender(&fileAppender);

    PLOG_INFO << "----- UglyEngine Log";
    PLOG_INFO << "----- Version: " << ugly::version::FULLVERSION_STRING;
}


/**
 * \brief Initialize engine.
 * 
 * \return false if error
 */
bool ugly::Engine::initialize()
{
    PLOG_INFO << "--- Initialize engine";

    if(!glfwInit())
    {
        PLOG_ERROR << "Failed to initialize GLFW";
        return false;
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    PLOG_INFO << "Display size: " << m_display_size.x << "*" << m_display_size.y;
    m_window = glfwCreateWindow(m_display_size.x, m_display_size.y, m_application->getName().c_str(), NULL, NULL);
    if(m_window == nullptr)
    {
        PLOG_ERROR << "Failed to create GLFW window";
        return false;
    }

    m_input_manager.reset(new InputManager());
    if(!m_input_manager->initialize())
    {
        LOG_ERROR << "Failed to init imput manager";
        return false;
    }

    m_vulkan_manager.reset(new VulkanManager());
    if(!m_vulkan_manager->initialize())
    {
        LOG_ERROR << "Failed to init vulkan manager";
        return false;
    }

    if(!m_application->initialize())
    {
        LOG_ERROR << "Failed to initialize application";
        return false;
    }

    return true;
}


/**
 * \breif Shutdown engine.
 */
void ugly::Engine::shutdown()
{
    if(m_application.get() != nullptr)
    {
        m_application->shutdown();
        m_application.reset(nullptr);
    }

    if(m_vulkan_manager.get() != nullptr)
    {
        m_vulkan_manager->shutdown();
        m_vulkan_manager.reset(nullptr);
    }

    if(m_input_manager.get() != nullptr)
    {
        m_input_manager->shutdown();
        m_input_manager.reset(nullptr);
    }

    PLOG_INFO << "--- Shutdown engine";
    m_window = nullptr;
    glfwTerminate();
}


/**
 * \brief Execute main loop.
 * 
 * \return false if error
 */
bool ugly::Engine::mainLoop()
{
    while(!m_quit)
    {
        if(glfwWindowShouldClose(m_window))
            m_quit = true;

        m_application->update();
        m_input_manager->update();

        glfwSwapBuffers(m_window);
        glfwPollEvents();    
    }

    return true;
}