#ifndef TSDL_ELEMENTS_ATTRS_SIZABLE_
#define TSDL_ELEMENTS_ATTRS_SIZABLE_

#include "TSDL/abstract/elements/attrs/EventDispatcher.hpp"
#include "TSDL/TSDL_Main.hpp"
#include "TSDL/TSDL_Utility.hpp"

namespace TSDL::elements::attrs
{
    class Sized
    {
        public:
        virtual point_2d size() const = 0;
    };

    class Sizable: virtual public Sized
    {        
        private:
        point_2d _size;

        public:
        Sizable(const point_2d& size);

        virtual void size(const point_2d& size);
        virtual point_2d size() const override;
    };

    template <class T>
    class sized: public std::enable_if_t<std::is_base_of_v<DependentElement, T>, T>, virtual public Sized
    {
        public:
        template <typename ...Args>
        sized(EventloopAdapter& evloop, TSDL_Renderer& renderer, Args... args): T(evloop, renderer, args...) {}
    };

    template <class T>
    class sizable: public sized<T>, public Sizable
    {
        public:
        template <typename ...Args>
        sizable(EventloopAdapter& evloop, TSDL_Renderer& renderer, const point_2d& size, Args... args): sized<T>(evloop, renderer, args...), Sizable(size) {}

        template <typename U = T>
        typename std::enable_if_t<!std::is_base_of_v<EventDispatcher, U>> _size(const point_2d& size)
        {
            Sizable::size(size);
        }

        template <typename U = T>
        typename std::enable_if_t<std::is_base_of_v<EventDispatcher, U>> _size(const point_2d& size)
        {
            Sizable::size(size);
            dispatch_event(Caller(*this, {0, 0}), events::EventType::ElementResized, ::TSDL::null_event);
        }

        virtual void size(const point_2d& size) override
        {
            _size(size);
        }

        virtual point_2d size() const final
        {
            return Sizable::size();
        }
    };
}

#endif