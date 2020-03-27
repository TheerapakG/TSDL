#ifndef TSDL_ELEMENTS_SCENE_
#define TSDL_ELEMENTS_SCENE_

#include "TSDL/abstract/elements/Element.hpp"
#include "TSDL/abstract/elements/attrs/EventForwarder.hpp"

namespace TSDL::elements
{
    /*
    * Scene is a class that automatically bind target to EventloopAdapter immediately after constructed.
    * After destructed, associated EventloopAdapter's source is std::nullopt
    */
    class Scene: public attrs::eventforwarder<DependentElement>
    {
        Scene(EventloopAdapter& evloop, TSDL_Renderer& renderer, DependentElement& target);
        ~Scene();
    };    
}

#endif