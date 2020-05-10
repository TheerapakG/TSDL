//-----------------------------------------------------------------------------
// This file is subject to the license terms in the LICENSE file found in the 
// top-level directory of this distribution.
//
// No part of this software, including this file, may be copied, modified, 
// propagated, or distributed except according to the terms contained in the 
// LICENSE file.
//-----------------------------------------------------------------------------

#ifndef TSDL_EVENTS_EVENTTYPE_
#define TSDL_EVENTS_EVENTTYPE_

#include <type_traits>
#include <variant>

namespace TSDL
{
    namespace events
    {
        enum class EventType
        {
            WindowShown,
            WindowHidden,
            WindowExposed,
            WindowMoved,
            WindowResized,
            WindowMinimized,
            WindowMaximized,
            WindowRestored,
            WindowClose,
            KeyDown,
            KeyUp,
            TextInput,
            MouseMotion,
            MouseIn,
            MouseOut,
            LeftDown,
            LeftUp,
            LeftUp_Inside, // LeftDown originated in this element and LeftUp originated in this element
            LeftUp_Outside, // LeftDown originated in this element but LeftUp not originated in this element
            RightDown,
            RightUp,
            RightUp_Inside, // Like Left*
            RightUp_Outside, // Like Left*
            MiddleDown,
            MiddleUp,            
            MiddleUp_Inside, // Like Left*
            MiddleUp_Outside, // Like Left*
            MouseWheel,
            ElementResized,
            ButtonActivated,
            Dragged // Dragable dragged
        };
        // If added event here, also add it in EventLookup
    }
}

#endif