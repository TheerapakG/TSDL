#include "TSDL/abstract/elements/attrs/Gridded.hpp"

namespace TSDL::elements::attrs
{
    Gridded::Gridded(EventloopAdapter& evloop, TSDL_Renderer& renderer): _grid(evloop, renderer) {}

    Gridded::Gridded(EventloopAdapter& evloop, TSDL_Renderer& renderer, const ListenerMap& listeners): _grid(evloop, renderer, listeners) {}

    Gridded::Gridded(EventloopAdapter& evloop, TSDL_Renderer& renderer, ListenerMap&& listeners): _grid(evloop, renderer, listeners) {}

    Grid& Gridded::grid()
    {
        return _grid;
    }

    const Grid& Gridded::grid() const
    {
        return _grid;
    }
}