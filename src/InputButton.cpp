#include "InputButton.h"

/**
 * \brief Constructor.
 */
ugly::InputButton::InputButton()
{
}


/**
 * \brief Get current state.
 * 
 * \return Current state.
 */
const ugly::InputState& ugly::InputButton::getState() const
{
    return m_state;
}


/**
 * \brief Set state.
 * 
 * \param state State.
 */
void ugly::InputButton::setState(ugly::InputState state)
{
    m_state = state;
}


/**
 * \brief Get current action.
 * 
 * \return Currrent action.
 */
const ugly::InputAction& ugly::InputButton::getAction() const
{
    return m_action;
}


/**
 * \brief Set action.
 * 
 * \param action Action.
 */
void ugly::InputButton::setAction(ugly::InputAction action)
{
    m_action = action;
}