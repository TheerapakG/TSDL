#ifndef TSDL_ELEMENTS_ATTRS_SIZABLE_
#define TSDL_ELEMENTS_ATTRS_SIZABLE_

#include "TSDL/abstract/elements/attrs/EventDispatcher.hpp"
#include "TSDL/TSDL_Main.hpp"
#include "TSDL/TSDL_Utility.hpp"

namespace TSDL
{
    namespace elements
    {
        namespace attrs
        {
            class Sizable
            {        
                private:
                point_2d _size;

                public:
                Sizable(const point_2d& size);

                virtual void size(const point_2d& size);
                const point_2d& size() const;
            };

            template <class T>
            class sizable: public std::enable_if_t<std::is_base_of_v<DependentElement, T>, T>, public Sizable
            {
                public:
                template <typename ...Args>
                sizable(EventloopAdapter& evloop, TSDL_Renderer& renderer, const point_2d& size, Args... args): T(evloop, renderer, args...), Sizable(size) {}

                using Sizable::size;

                template <typename U = T>
                typename std::enable_if_t<!std::is_base_of_v<EventDispatcher, U>> _size(const point_2d& size)
                {
                    Sizable::size(size);
                }

                template <typename U = T>
                typename std::enable_if_t<std::is_base_of_v<EventDispatcher, U>> _size(const point_2d& size)
                {
                    Sizable::size(size);
                    dispatch_event(Caller(*this, {0, 0}), ::TSDL::events::EventType::ElementResized, ::TSDL::null_event);
                }

                virtual void size(const point_2d& size) final
                {
                    _size(size);
                }
            };
        }
    }
}

#endif