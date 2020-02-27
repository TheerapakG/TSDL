#include "TSDL/abstract/elements/ElementHolder.hpp"
#include "TSDL/abstract/elements/EventloopAdapter.hpp"
#include "TSDL/TSDL_Meta.hpp"
#include <algorithm>

bool ::TSDL::elements::Subelement::operator==(const ::TSDL::elements::Subelement& other)
{
    return element == other.element && dimension == other.dimension;
}

TSDL::elements::ElementHolder::ElementHolder(EventloopAdapter& evloop, TSDL_Renderer& renderer): 
    DependentElement(evloop, renderer) {}

void TSDL::elements::ElementHolder::add_child(DependentElement& subelement, const point_2d& topleft, const point_2d& bottomright)
{
    std::pair el_loc(topleft, bottomright);
    Subelement el_all{&subelement, el_loc};

    _subelements_order.push_back(el_all);
    _subelements_info[&subelement] = el_all;

    subelement._holders.emplace_back(*this);

    update();
}

void TSDL::elements::ElementHolder::add_child(DependentElement& subelement, const point_2d& topleft, const point_2d& bottomright, int order)
{
    std::pair el_loc(topleft, bottomright);
    Subelement el_all{&subelement, el_loc};

    auto it = _subelements_order.begin();
    _subelements_order.insert(it + order, el_all);
    _subelements_info[&subelement] = el_all;

    subelement._holders.emplace_back(*this);

    update();
}

void TSDL::elements::ElementHolder::reorder_child(DependentElement& subelement, int order)
{
    std::swap(
        std::find(_subelements_order.begin(), _subelements_order.end(), _subelements_info[&subelement]),
        _subelements_order.begin() + order
    );

    update();
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

const std::vector<TSDL::elements::Subelement>& TSDL::elements::ElementHolder::get_child_order()
{
    return _subelements_order;
}

std::optional<TSDL::elements::Subelement> TSDL::elements::ElementHolder::highest_child(const point_2d& point)
{
    for(Subelement& subelement: ::TSDL::reverse(_subelements_order))
    {
        auto& [topleft, bottomright] = subelement.dimension;
        if(topleft.x < point.x && point.x < bottomright.x &&
            topleft.y < point.y && point.y < bottomright.y) return subelement;
    }

    return std::optional<::TSDL::elements::Subelement>();
}

bool TSDL::elements::ElementHolder::need_update() const
{
    return DependentElement::need_update() ||
        std::any_of(
            _subelements_order.cbegin(), 
            _subelements_order.cend(), 
            [](const ::TSDL::elements::Subelement& subel){ return subel.element->need_update(); }
        );
}

void TSDL::elements::ElementHolder::render(const ::TSDL::point_2d& dist)
{
    ::TSDL::TSDL_Renderer& _renderer = renderer();

    _renderer.clear({0xFF, 0xFF, 0xFF, 0xFF});
    for(Subelement& subelement: _subelements_order)
    {
        auto& [el_ptr, dim, sizable] = subelement;
        auto el = el_ptr;
        el->render(dim.first);
    }
    _renderer.update();

    not_update();
}

bool TSDL::elements::operator==(const ::TSDL::elements::ElementHolder& lhs, const ::TSDL::elements::ElementHolder& rhs)
{
    return static_cast<const ::TSDL::elements::Element&>(lhs) == static_cast<const ::TSDL::elements::Element&>(rhs);
}