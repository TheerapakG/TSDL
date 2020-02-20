#ifndef TSDL_ELEMENTS_SIZED_
#define TSDL_ELEMENTS_SIZED_

#include "TSDL/abstract/elements/EventDispatcher.hpp"
#include "TSDL/TSDL_Main.hpp"
#include "TSDL/TSDL_Utility.hpp"

namespace TSDL
{
    namespace elements
    {
        class Sized
        {        
            private:
            point_2d _size;

            public:
            Sized(const point_2d& size);

            virtual void size(const point_2d& size);
            const point_2d& size() const;
        };

        template <class T>
        class sized: public std::enable_if_t<std::is_base_of_v<Element, T>, T>, public Sized
        {
            public:
            template <typename ...Args>
            sized(TSDL_Renderer& renderer, const point_2d& size, Args... args): T(renderer, args...), Sized(size) {}

            using Sized::size;

            template <typename U = T>
            typename std::enable_if_t<!std::is_base_of_v<EventDispatcher, U>> _size(const point_2d& size)
            {
                Sized::size(size);
            }

            template <typename U = T>
            typename std::enable_if_t<std::is_base_of_v<EventDispatcher, U>> _size(const point_2d& size)
            {
                Sized::size(size);
                T::dispatch_event(Caller(*this, {0, 0}), ::TSDL::events::EventType::ElementResized, ::TSDL::null_event);
            }

            virtual void size(const point_2d& size) final
            {
                _size(size);
            }
        };
    }
}

#endif