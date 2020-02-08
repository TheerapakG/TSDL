#include "abstract/elements/ElementHolder.hpp"
#include "TSDL_Meta.hpp"
#include <algorithm>

TSDL::elements::ElementHolder::ElementHolder(TSDL_Renderer& renderer, const point_2d& size): 
    Element(renderer), _rendered(static_cast<_SDL_Renderer>(renderer), size, SDL_TEXTUREACCESS_TARGET) {}

void TSDL::elements::ElementHolder::add_child(Element&& subelement, const point_2d& topleft, const point_2d& bottomright)
{
    std::reference_wrapper <Element> el_ref(subelement);
    std::pair el_loc(topleft, bottomright);
    Subelement el_all(el_ref, el_loc);

    _subelements_order.push_back(el_all);
    _subelements_info[el_ref] = el_all;
}

void TSDL::elements::ElementHolder::add_child(Element&& subelement, const point_2d& topleft, const point_2d& bottomright, int order)
{
    std::reference_wrapper <Element> el_ref(subelement);
    std::pair el_loc(topleft, bottomright);
    Subelement el_all(el_ref, el_loc);

    auto it = _subelements_order.begin();
    _subelements_order.insert(it + order, el_all);
    _subelements_info[el_ref] = el_all;
}

void TSDL::elements::ElementHolder::reorder_child(Element&& subelement, int order)
{
    std::swap(
        std::find(_subelements_order.begin(), _subelements_order.end(), _subelements_info[subelement]),
        _subelements_order.begin() + order
    );
}

void TSDL::elements::ElementHolder::move_child(Element&& subelement, const point_2d& destination)
{
    std::reference_wrapper <Element> el_ref(subelement);
    auto el = std::find(_subelements_order.begin(), _subelements_order.end(), _subelements_info[el_ref]);
    el->second = {destination, el->second.second + destination - el->second.first};
    _subelements_info[el_ref].second = el->second;
}

void TSDL::elements::ElementHolder::remove_child(Element&& subelement)
{
    std::reference_wrapper <Element> el_ref(subelement);
    auto el_all_node = _subelements_info.extract(el_ref);
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
        auto& [topleft, bottomright] = subelement.second;
        if(topleft.x < point.x && point.x < bottomright.x &&
            topleft.y < point.y && point.y < bottomright.y) return subelement;
    }
}

void TSDL::elements::ElementHolder::render()
{
    auto _renderer = renderer();
    auto prev_target = _renderer.target();
    for(Subelement& subelement: _subelements_order)
    {
        auto& [el_ref, dim] = subelement;
        auto& el = el_ref.get();
        if (el.need_update())
        {
            _renderer.update();
            el.render();
            el.not_update();
        }        
        _renderer.copy_from(el.get_texture(), std::optional<TSDL::rect>(), TSDL::rect(dim));
    }
    _renderer.update();
    _renderer.target(prev_target);
}

TSDL::TSDL_Texture& TSDL::elements::ElementHolder::get_texture()
{
    return _rendered;
}