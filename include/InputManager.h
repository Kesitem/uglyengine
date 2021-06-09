#pragma once

#include "Core.h"
#include "InputStats.h"
#include "InputButton.h"

namespace ugly
{

/**
 * \class InputManager
 * \brief Input manager
 */
class InputManager
{
public:

    /**
     *  \brief Constructor.
     */
    InputManager();

    /**
     * \brief Initialize manager.
     * \return False if error
     */
    bool initialize();

    /**
     * \brief Shutdown.
     */
    void shutdown();

    /**
     * \brief Update state manager.
     *
     * This function updates state(for exemple pass key state from released to none).
     * It must be call after event processing and before polling events.
     */
    void update();

    /**
     * \brief Process a key change.
     * 
     * This method is mainly used by GLFW key callback.
     * If key is not bind, nothing happens.
     * \param key_name  GLFW key name
     * \param action    GLFW action
     */
    void processKeyChange(int key_name, int action);

    /**
     * \brief Create a button.
     * 
     * \param button_name   InputButton name
     */
    void createButton(const std::string& button_name);

    /**
     * \brief Bind a key to a button.
     * 
     * \param key_name      GFLW key name
     * \param button_name   Button name
     */
    void bindKeyToButton(int key_name, const std::string& button_name);

    /**
     * \brief Get a button state.
     * 
     * \param button_name   InputButton name
     * \return InputButton state
     */
    const InputState& getButtonState(const std::string& button_name);

    /**
     * \brief Get a button action.
     * 
     * \param button_name   InputButton name
     * \return InputButton last action
     */
    const InputAction& getButtonAction(const std::string& button_name);

private:

    /*! InputButton list. */
    std::map<std::string, std::unique_ptr<InputButton>> m_buttons;

    /*! Key to button mapping*/
    std::map<int, std::string> m_key_button_mapping;
};

}//namespace ugly