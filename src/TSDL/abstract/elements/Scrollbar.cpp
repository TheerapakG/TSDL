#include "TSDL/abstract/elements/Scrollbar.hpp"

namespace TSDL::elements
{
    BaseHorizontalScrollbar::BaseHorizontalScrollbar(EventloopAdapter& evloop, TSDL_Renderer& renderer, int content_width, const point_2d& size):
        impl::_BaseHorizontalScrollbar<BaseHorizontalScrollbar>(evloop, renderer, content_width, size) {}
    BaseHorizontalScrollbar::BaseHorizontalScrollbar(EventloopAdapter& evloop, TSDL_Renderer& renderer, int content_width, const point_2d& size, const attrs::ListenerMap& listeners):
        impl::_BaseHorizontalScrollbar<BaseHorizontalScrollbar>(evloop, renderer, content_width, size, listeners) {}
    BaseHorizontalScrollbar::BaseHorizontalScrollbar(EventloopAdapter& evloop, TSDL_Renderer& renderer, int content_width, const point_2d& size, attrs::ListenerMap&& listeners):
        impl::_BaseHorizontalScrollbar<BaseHorizontalScrollbar>(evloop, renderer, content_width, size, listeners) {}

    BaseVerticalScrollbar::BaseVerticalScrollbar(EventloopAdapter& evloop, TSDL_Renderer& renderer, int content_height, const point_2d& size):
        impl::_BaseVerticalScrollbar<BaseVerticalScrollbar>(evloop, renderer, content_height, size) {}
    BaseVerticalScrollbar::BaseVerticalScrollbar(EventloopAdapter& evloop, TSDL_Renderer& renderer, int content_height, const point_2d& size, const attrs::ListenerMap& listeners):
        impl::_BaseVerticalScrollbar<BaseVerticalScrollbar>(evloop, renderer, content_height, size, listeners) {}
    BaseVerticalScrollbar::BaseVerticalScrollbar(EventloopAdapter& evloop, TSDL_Renderer& renderer, int content_height, const point_2d& size, attrs::ListenerMap&& listeners):
        impl::_BaseVerticalScrollbar<BaseVerticalScrollbar>(evloop, renderer, content_height, size, listeners) {}
}