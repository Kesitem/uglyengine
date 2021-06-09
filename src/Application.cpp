#include "Application.h"
#include "Engine.h"

/**
 * \brief Constructor.
 */
ugly::Application::Application()
{
}


/**
 * \brief Destructor.
 */
ugly::Application::~Application()
{
}


/**
 * \brief Initialize application.
 * 
 * \return False if error
 */
bool ugly::Application::initialize()
{
    Engine::getInstance()->getInputManager()->createButton("quit");
    Engine::getInstance()->getInputManager()->bindKeyToButton(GLFW_KEY_ESCAPE, "quit");

    return true;
}


/**
 * \brief Shutdown application.
 */
void ugly::Application::shutdown()
{
}


/**
 * \brief Update application.
 */
void ugly::Application::update()
{
    if(Engine::getInstance()->getInputManager()->getButtonAction("quit") == InputAction::released)
        Engine::getInstance()->quit();
}


/**
 * \brief Get application name.
 * 
 * \return Application name
 */
const std::string &ugly::Application::getName() const
{
    return m_name;
}