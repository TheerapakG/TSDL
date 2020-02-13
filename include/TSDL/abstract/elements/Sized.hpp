#ifndef TSDL_ELEMENTS_SIZED_
#define TSDL_ELEMENTS_SIZED_

#include "TSDL/abstract/elements/Element.hpp"
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
            const point_2d& size();
        };

        template <class T>
        class sized: public std::enable_if_t<std::is_base_of_v<Element, T>, T>, public Sized
        {
            public:
            template <typename ...Args>
            sized(const point_2d& size, Args... args): T(args...), Sized(size) {}

			using Sized::size;

            virtual void size(const point_2d& size) final
            {
                Sized::size(size);
				T::dispatch_event(Caller(*this, {0, 0}), ::TSDL::events::EventType::ElementResized, ::TSDL::null_event);
            }
        };
    }
}

#endif