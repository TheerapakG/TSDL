#include "TSDL/abstract/elements/Scrollbar.hpp"

namespace TSDL::elements
{
    BaseHorizontalScrollbar::BaseHorizontalScrollbar(int content_width, const point_2d& size):
        impl::_BaseHorizontalScrollbar<BaseHorizontalScrollbar>(content_width, size) {}
    BaseHorizontalScrollbar::BaseHorizontalScrollbar(int content_width, const point_2d& size, const attrs::ListenerMap& listeners):
        impl::_BaseHorizontalScrollbar<BaseHorizontalScrollbar>(content_width, size, listeners) {}
    BaseHorizontalScrollbar::BaseHorizontalScrollbar(int content_width, const point_2d& size, attrs::ListenerMap&& listeners):
        impl::_BaseHorizontalScrollbar<BaseHorizontalScrollbar>(content_width, size, listeners) {}

    BaseVerticalScrollbar::BaseVerticalScrollbar(int content_height, const point_2d& size):
        impl::_BaseVerticalScrollbar<BaseVerticalScrollbar>(content_height, size) {}
    BaseVerticalScrollbar::BaseVerticalScrollbar(int content_height, const point_2d& size, const attrs::ListenerMap& listeners):
        impl::_BaseVerticalScrollbar<BaseVerticalScrollbar>(content_height, size, listeners) {}
    BaseVerticalScrollbar::BaseVerticalScrollbar(int content_height, const point_2d& size, attrs::ListenerMap&& listeners):
        impl::_BaseVerticalScrollbar<BaseVerticalScrollbar>(content_height, size, listeners) {}
}