#include "TSDL/abstract/elements/ElementHolder.hpp"
#include "TSDL/TSDL_Meta.hpp"
#include <algorithm>

bool TSDL::elements::Subelement::operator==(const TSDL::elements::Subelement& other)
{
    return element == other.element && dimension == other.dimension;
}

TSDL::elements::ElementHolder::ElementHolder(TSDL_Renderer& renderer, const point_2d& size): 
    Element(renderer), _surface(size.x, size.y), _rendered(static_cast<_SDL_Renderer>(renderer), _surface) {}

void TSDL::elements::ElementHolder::add_child(Element&& subelement, const point_2d& topleft, const point_2d& bottomright)
{
    std::pair el_loc(topleft, bottomright);
    Subelement el_all{&subelement, el_loc};

    _subelements_order.push_back(el_all);
    _subelements_info[&subelement] = el_all;
}

void TSDL::elements::ElementHolder::add_child(Element&& subelement, const point_2d& topleft, const point_2d& bottomright, int order)
{
    std::pair el_loc(topleft, bottomright);
    Subelement el_all{&subelement, el_loc};

    auto it = _subelements_order.begin();
    _subelements_order.insert(it + order, el_all);
    _subelements_info[&subelement] = el_all;
}

void TSDL::elements::ElementHolder::reorder_child(Element&& subelement, int order)
{
    std::swap(
        std::find(_subelements_order.begin(), _subelements_order.end(), _subelements_info[&subelement]),
        _subelements_order.begin() + order
    );
}

void TSDL::elements::ElementHolder::move_child(Element&& subelement, const point_2d& destination)
{
    auto el = std::find(_subelements_order.begin(), _subelements_order.end(), _subelements_info[&subelement]);
    el->dimension = {destination, el->dimension.second + destination - el->dimension.first};
    _subelements_info[&subelement].dimension = el->dimension;
}

void TSDL::elements::ElementHolder::remove_child(Element&& subelement)
{
    auto el_all_node = _subelements_info.extract(&subelement);
    _subelements_order.erase(
        std::find(_subelements_order.begin(), _subelements_order.end(), el_all_node.mapped())
    );
}

const std::vector<TSDL::elements::Subelement>& TSDL::elements::ElementHolder::get_child_order()
{
    return _subelements_order;
}

TSDL::elements::Subelement& TSDL::elements::ElementHolder::highest_child(const point_2d& point)
{
    for(Subelement& subelement: TSDL::reverse(_subelements_order))
    {
        auto& [topleft, bottomright] = subelement.dimension;
        if(topleft.x < point.x && point.x < bottomright.x &&
            topleft.y < point.y && point.y < bottomright.y) return subelement;
    }
}

void TSDL::elements::ElementHolder::render()
{
    for(Subelement& subelement: _subelements_order)
    {
        auto& [el_ptr, dim] = subelement;
        auto el = el_ptr;
        if (el->need_update())
        {
            el->render();
            el->not_update();
        }
    }

    auto& _renderer = renderer();
    auto prev_target = _renderer.target();
    _renderer.target(make_optional_ref<_SDL_Texture>(_rendered));
    _renderer.clear(color_rgba(0xFF, 0xFF, 0xFF, 0xFF));
    for(Subelement& subelement: _subelements_order)
    {
        auto& [el_ptr, dim] = subelement;
        auto& el = *el_ptr;
        _renderer.copy_from(el.get_texture(), make_optional_const_ref<TSDL::rect>(), make_optional_const_ref<TSDL::rect>(dim));
    }
    _renderer.update();

    if(prev_target) _renderer.target(prev_target.value());
    else _renderer.target(make_optional_ref<_SDL_Texture>());
}

TSDL::TSDL_Texture& TSDL::elements::ElementHolder::get_texture()
{
    return _rendered;
}
