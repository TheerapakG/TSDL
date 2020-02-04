#ifndef TSDL_EVENTS_EVENTTYPE_
#define TSDL_EVENTS_EVENTTYPE_

namespace TSDL
{
    namespace events
    {
        enum EventType
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
            KeyDown,
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