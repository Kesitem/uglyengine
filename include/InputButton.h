#pragma once

#include "Core.h"
#include "InputStats.h"

namespace ugly
{

/**
 * \brief Input button.
 * 
 * The button can be use in two ways:
 *  - as a simple state: the button is press or release
 *  - as an action: the button was just pressed or released
 */
class InputButton
{
public:

    /**
     * \brief Constructor.
     */
    InputButton();

    /**
     * \brief Get current state.
     * 
     * \return Current state.
     */
    const InputState& getState() const;

    /**
     * \brief Set state.
     * 
     * \param state State.
     */
    void setState(InputState state);

    /**
     * \brief Get current action.
     * 
     * \return Currrent action.
     */
    const InputAction& getAction() const;

    /**
     * \brief Set action.
     * 
     * \param action Action.
     */
    void setAction(InputAction action);

private:

    /*! Current state. */
    InputState m_state {InputState::release};

    /*! Current action. */
    InputAction m_action {InputAction::none};
};

}//namespace ugly