#pragma once

#include "Core.h"

namespace ugly
{

    /*! State enumeration */
    enum class InputState
    {
        press,      /*! InputButton is press. */
        release     /*! InputButton is release. */
    };


    /*! Action enumeration */
    enum class InputAction
    {
        none,       /*! No button action. */
        pressed,    /*! The button was pressed. */
        repeated,   /*! The button as press Repeated. */
        released    /*! The button was released. */
    };

}//namespace ugly