#ifndef TSDL_ELEMENTS_SCROLLBAR_
#define TSDL_ELEMENTS_SCROLLBAR_

#include "TSDL/abstract/elements/attrs/EventDispatcher.hpp"
#include "TSDL/abstract/elements/attrs/Gridded.hpp"
#include "TSDL/abstract/elements/attrs/Dragable.hpp"
#include "TSDL/abstract/elements/Grid.hpp"
#include "TSDL/abstract/elements/Button.hpp"

#include "TSDL/TSDL_Meta.hpp"
#include <memory>

namespace TSDL::elements
{
    namespace _using_guard
    {
        using namespace std::placeholders;

        class BaseHorizontalScrollbar: public attrs::gridded<attrs::sizable<attrs::eventdispatcher<DependentElement>>>
        {
            int _content_length;

            virtual int _bar_length();

            point_2d _bar_movement_calc(const ::TSDL::point_2d& start, const ::TSDL::point_2d& dist);

            attrs::dragable<Button> _bar{
                eventloop(), renderer(),
                std::bind(std::mem_fn(&BaseHorizontalScrollbar::_bar_movement_calc), this, _1, _2),
                point_2d{_bar_length(), size().y}
            };

            public:
            BaseHorizontalScrollbar(EventloopAdapter& evloop, TSDL_Renderer& renderer, int content_length, const point_2d& size);
            BaseHorizontalScrollbar(EventloopAdapter& evloop, TSDL_Renderer& renderer, int content_length, const point_2d& size, const attrs::ListenerMap& listeners);
            BaseHorizontalScrollbar(EventloopAdapter& evloop, TSDL_Renderer& renderer, int content_length, const point_2d& size, attrs::ListenerMap&& listeners);

            /*
            Query if parent need to update this element on the next cycle
            */
            virtual bool need_update() const override;

            /*
            Re-render this element
            */
            virtual void render(const ::TSDL::point_2d& dist) override;
        };
    }
    using _using_guard::BaseHorizontalScrollbar;
}

#endif