#include "TSDL/abstract/elements/Scene.hpp"
#include "TSDL/abstract/elements/EventloopAdapter.hpp"

namespace TSDL::elements
{
    Scene::Scene(EventloopAdapter& evloop, TSDL_Renderer& renderer, DependentElement& target): 
        attrs::eventforwarder<DependentElement>(evloop, renderer, target)
    {
        evloop.src(target);
    }

    Scene::~Scene()
    {
        eventloop().src(std::nullopt);
    }
}