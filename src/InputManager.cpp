#include "InputManager.h"
#include "InputButton.h"
#include "Engine.h"


/**
 * \brief GLFW key callback.
 */
void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    static ugly::Engine* engine = ugly::Engine::getInstance();

    engine->getInputManager()->processKeyChange(key, action);
}


/**
 *  \brief Constructor.
 */
ugly::InputManager::InputManager()
{
}


/**
 * \brief Initialize manager.
 * \return False if error
 */
bool ugly::InputManager::initialize()
{
    LOG_INFO << "Initialize input manager...";
    
    // Register input callbacks
    glfwSetKeyCallback(ugly::Engine::getInstance()->getWindow(), glfwKeyCallback);

    return true;
}


/**
 * \brief Shutdown.
 */
void ugly::InputManager::shutdown()
{
    LOG_INFO << "Shutdown input manager...";
}


/**
 * \brief Update state manager.
 *
 * This function updates state(for exemple pass key state from released to none).
 * It must be call after event processing and before polling events.
 */
void ugly::InputManager::update()
{
    for(auto const &button_itor : m_buttons)
    {
        auto button = (button_itor.second).get();
        if(button->getAction() == InputAction::pressed || button->getAction() == InputAction::repeated || button->getAction() == InputAction::released)
        {
            LOG_DEBUG << "Button: " << button_itor.first << "action is set to NONE";
            button->setAction(InputAction::none);
        }
    }
}


/**
 * \brief Process a key change.
 * 
 * This method is mainly used by GLFW key callback.
 * If key is not bind, nothing happens.
 * \param key_name  GLFW key name
 * \param action    GLFW action
 */
void ugly::InputManager::processKeyChange(int key_name, int action)
{
    // Check if key is binded
    auto key_button_mapping_itor = m_key_button_mapping.find(key_name);
    if(key_button_mapping_itor == m_key_button_mapping.end())
        return;

    // Update button
    auto button = (m_buttons[key_button_mapping_itor->second]).get();
    if(action == GLFW_PRESS)
    {
        LOG_DEBUG << "Button: " << key_button_mapping_itor->second << " is pressed";
        button->setState(InputState::press);
        button->setAction(InputAction::pressed);
    }
    else if(action == GLFW_REPEAT)
    {
        LOG_DEBUG << "Button: " << key_button_mapping_itor->second << " is repeated";
        button->setAction(InputAction::repeated);
    }
    else //(action == GLFW_RELEASE)
    {
        LOG_DEBUG << "Button: " << key_button_mapping_itor->second << " is released";
        button->setState(InputState::release);
        button->setAction(InputAction::released);
    }
}


/**
 * \brief Create a button.
 * 
 * \param button_name   InputButton name
 */
void ugly::InputManager::createButton(const std::string& button_name)
{
    PLOG_INFO << "Create input button: " << button_name;

    auto button_map_itor = m_buttons.find(button_name);
    if( button_map_itor != m_buttons.end())
    {
        PLOG_ERROR << "Trying to create a button already existing: " << button_name;
        return;
    }

    m_buttons.insert(std::make_pair(button_name, std::make_unique<InputButton>()));
}


/**
 * \brief Bind a key to a button.
 * 
 * \param key_name      GFLW key name
 * \param button_name   Button name
 */
void ugly::InputManager::bindKeyToButton(int key_name, const std::string& button_name)
{
    LOG_INFO << "Bind key: " <<  key_name << " to input button: " << button_name; 

    auto key_button_mapping_itor = m_key_button_mapping.find(key_name);
    if(key_button_mapping_itor != m_key_button_mapping.end())
    {
        LOG_ERROR << "Key is already bind to button: " << key_button_mapping_itor->second;
        return;
    }

    auto button_itor = m_buttons.find(button_name);
    if(button_itor == m_buttons.end())
    {
        LOG_ERROR << "Button: " << button_name << " not found. Creating it";
        createButton(button_name);
    }

    m_key_button_mapping[key_name] = button_name;
}


/**
 * \brief Get a button state.
 * 
 * \param button_name   InputButton name
 * \return InputButton state
 */
const ugly::InputState& ugly::InputManager::getButtonState(const std::string& button_name)
{
    auto button_map_itor = m_buttons.find(button_name);
    if(button_map_itor == m_buttons.end())
    {
        LOG_WARNING << "InputButton: " << button_name << " not found.";
        createButton(button_name);
        button_map_itor = m_buttons.find(button_name);
    }

    return button_map_itor->second.get()->getState();
}


/**
 * \brief Get a button action.
 * 
 * \param button_name   InputButton name
 * \return InputButton last action
 */
const ugly::InputAction& ugly::InputManager::getButtonAction(const std::string& button_name)
{
    auto button_map_itor = m_buttons.find(button_name);
    if (button_map_itor == m_buttons.end())
    {
        LOG_WARNING << "InputButton: " << button_name << " not found.";
        createButton(button_name);
        button_map_itor = m_buttons.find(button_name);
    }

    return button_map_itor->second.get()->getAction();
}

