#ifndef TSDL_EVENTS_EVENTTYPE_
#define TSDL_EVENTS_EVENTTYPE_

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
            RightDown,
            RightUp,
            MiddleDown,
            MiddleUp,
            MouseWheel
        };
    }
}

#endif