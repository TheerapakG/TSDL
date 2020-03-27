#include "TSDL/abstract/elements/ElementHolder.hpp"
#include "TSDL/abstract/elements/EventloopAdapter.hpp"
#include "TSDL/TSDL_Meta.hpp"
#include <algorithm>

bool ::TSDL::elements::Subelement::operator==(const ::TSDL::elements::Subelement& other)
{
    return *std::any_cast<DependentElement*>(element) == *std::any_cast<DependentElement*>(other.element) && 
        dimension == other.dimension;
}

TSDL::elements::ElementHolder::ElementHolder(EventloopAdapter& evloop, TSDL_Renderer& renderer): 
    attrs::sized<DependentElement>(evloop, renderer) {}

void TSDL::elements::ElementHolder::add_child(const Subelement& formed_subelement)
{
    _subelements_order.push_back(formed_subelement);
    _subelements_info[std::any_cast<DependentElement*>(formed_subelement.element)] = formed_subelement;

    std::any_cast<DependentElement*>(formed_subelement.element)->_holders.emplace_back(*this);

    update();
}

TSDL::elements::ElementHolder::Subelement_vector::iterator TSDL::elements::ElementHolder::add_child(const Subelement& formed_subelement, int order)
{
    auto it = _subelements_order.begin();
    auto rit = _subelements_order.insert(it + order, formed_subelement);
    _subelements_info[std::any_cast<DependentElement*>(formed_subelement.element)] = formed_subelement;

    std::any_cast<DependentElement*>(formed_subelement.element)->_holders.emplace_back(*this);

    update();

    return rit;
}

TSDL::elements::ElementHolder::Subelement_vector::iterator TSDL::elements::ElementHolder::reorder_child(DependentElement& subelement, int order)
{
    auto dit = _subelements_order.begin() + order;
    std::iter_swap(
        std::find(_subelements_order.begin(), _subelements_order.end(), _subelements_info[&subelement]),
        dit
    );

    update();

    return dit;
}

void TSDL::elements::ElementHolder::move_child(DependentElement& subelement, const point_2d& destination)
{
    auto el = std::find(_subelements_order.begin(), _subelements_order.end(), _subelements_info[&subelement]);
    el->dimension = {destination, el->dimension.second + destination - el->dimension.first};
    _subelements_info[&subelement].dimension = el->dimension;

    update();
}

void TSDL::elements::ElementHolder::remove_child(DependentElement& subelement)
{
    auto el_all_node = _subelements_info.extract(&subelement);
    _subelements_order.erase(
        std::find(_subelements_order.begin(), _subelements_order.end(), el_all_node.mapped())
    );

    subelement._holders.erase(
        std::find(subelement._holders.begin(), subelement._holders.end(), *this)
    );

    update();
}

TSDL::elements::Subelement TSDL::elements::ElementHolder::child_info(::TSDL::elements::DependentElement& subelement)
{
    return _subelements_info.at(&subelement);
}

::TSDL::rect TSDL::elements::ElementHolder::bound() const
{
    auto begin = _subelements_order.begin(), end = _subelements_order.end();
    if(begin == end) return {0, 0, 0, 0};

    int min_x = begin->dimension.first.x, min_y = begin->dimension.first.y, 
        max_x = begin->dimension.second.x, max_y = begin->dimension.second.y;

    try
    {
        auto _sizable = std::any_cast<attrs::Sizable*>(begin->element);
        std::tie(max_x, max_y) = static_cast<_point_2d>(_sizable->size());
        max_x += min_x;
        max_y += min_y;
    }
    catch(const std::bad_any_cast&) {}

    for(begin++; begin != end; begin++)
    {
        auto [topleft, bottomright] = begin->dimension;
        if(topleft.x < min_x) min_x = topleft.x;
        if(topleft.y < min_y) min_y = topleft.y;
        try
        {
            auto _sizable = std::any_cast<attrs::Sizable*>(begin->element);
            auto [width_x, width_y] = static_cast<_point_2d>(_sizable->size());
            if(topleft.x + width_x > max_x) max_x = topleft.x + width_x;
            if(topleft.y + width_y > max_y) max_y = topleft.y + width_y;
        }
        catch(const std::bad_any_cast&)
        {
            if(bottomright.x > max_x) max_x = bottomright.x;
            if(bottomright.y > max_y) max_y = bottomright.y;
        }     
    }

    return {_point_2d{min_x, min_y}, _point_2d{max_x, max_y}};
}

TSDL::point_2d TSDL::elements::ElementHolder::size() const
{
    auto[topleft, bottomright] = static_cast<_rect>(bound());
    return bottomright - topleft;
}

const std::vector<TSDL::elements::Subelement>& TSDL::elements::ElementHolder::get_child_order()
{
    return _subelements_order;
}

std::optional<TSDL::elements::Subelement> TSDL::elements::ElementHolder::highest_child(const point_2d& point)
{
    // TODO: handle sizable
    for(Subelement& subelement: ::TSDL::reverse(_subelements_order))
    {
        auto& [topleft, bottomright] = subelement.dimension;
        if(topleft.x < point.x && point.x < bottomright.x &&
            topleft.y < point.y && point.y < bottomright.y) return subelement;
    }

    return std::optional<::TSDL::elements::Subelement>();
}

void TSDL::elements::ElementHolder::render_position(const rect& position)
{
    _render_position = position;
    update();
}

::TSDL::rect TSDL::elements::ElementHolder::render_position()
{
    return _render_position;
}

bool TSDL::elements::ElementHolder::need_update() const
{
    return DependentElement::need_update() ||
        std::any_of(
            _subelements_order.cbegin(), 
            _subelements_order.cend(), 
            [](const ::TSDL::elements::Subelement& subel){ return std::any_cast<DependentElement*>(subel.element)->need_update(); }
        );
}

void TSDL::elements::ElementHolder::render(const ::TSDL::point_2d& dist)
{
    ::TSDL::point_2d r_topleft = _render_position.topleft();
    ::TSDL::point_2d r_bottomright = _render_position.bottomright();

    for(Subelement& subelement: _subelements_order)
    {
        auto& [el_ptr, dim] = subelement;
        auto el = std::any_cast<DependentElement*>(el_ptr);
        
        ::TSDL::point_2d topleft = dim.first;
        ::TSDL::point_2d bottomright = {0, 0};
        try
        {
            auto _sizable = std::any_cast<attrs::Sizable*>(el_ptr);
            bottomright += dim.first + _sizable->size();
        }
        catch(const std::bad_any_cast&)
        {
            bottomright += dim.second;
        }

        if( (bottomright.x > r_topleft.x || bottomright.y > r_topleft.y) && 
            (topleft.x < r_bottomright.x || topleft.y < r_bottomright.y)
        )
        {
            el->render(dist+topleft-r_topleft);
        }
    }
    not_update();
}

bool TSDL::elements::operator==(const ::TSDL::elements::ElementHolder& lhs, const ::TSDL::elements::ElementHolder& rhs)
{
    return static_cast<const ::TSDL::elements::Element&>(lhs) == static_cast<const ::TSDL::elements::Element&>(rhs);
}