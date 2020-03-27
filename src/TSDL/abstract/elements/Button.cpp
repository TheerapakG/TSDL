#include "TSDL/abstract/elements/Button.hpp"
#include "TSDL/abstract/elements/EventloopAdapter.hpp"
#include "TSDL/TSDL_Meta.hpp"

#include <algorithm>
#include <cmath>

namespace TSDL::elements
{
    Button::Button(EventloopAdapter& evloop, TSDL_Renderer& renderer, const point_2d& _size):
        impl::_Button<Button>(evloop, renderer, _size) {}

    Button::Button(EventloopAdapter& evloop, TSDL_Renderer& renderer, const point_2d& _size, const attrs::ListenerMap& listeners):
        impl::_Button<Button>(evloop, renderer, _size, listeners) {}

    Button::Button(EventloopAdapter& evloop, TSDL_Renderer& renderer, const point_2d& _size, attrs::ListenerMap&& listeners):
        impl::_Button<Button>(evloop, renderer, _size, listeners) {}

    Button& Button::normal(const std::shared_ptr<RenderSizedElement>& element)
    {
        impl::_Button<Button>::normal(element);
        return *this;
    }

    Button& Button::hover(const std::shared_ptr<RenderSizedElement>& element)
    {
        impl::_Button<Button>::hover(element);
        return *this;
    }

    Button& Button::clicked(const std::shared_ptr<RenderSizedElement>& element)
    {
        impl::_Button<Button>::clicked(element);
        return *this;
    }

    Button& Button::front(optional_reference<attrs::sizable<RenderSizedElement>> front)
    {
        impl::_Button<Button>::front(front);
        return *this;
    }
}