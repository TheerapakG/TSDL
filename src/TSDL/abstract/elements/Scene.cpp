#include "TSDL/abstract/elements/Scene.hpp"
#include "TSDL/abstract/elements/EventloopAdapter.hpp"

namespace TSDL::elements
{
    Scene::Scene(Scene&& other): _target(other._target)
    {
        other._unbind = false;
    }

    Scene& Scene::operator=(Scene&& other)
    {
        _target = other._target;
        other._unbind = false;
        return *this;
    }

    Scene::~Scene()
    {
        const DependentElement& _t_target = get_ref(_target);
        if (_unbind && 
            _t_target == _t_target.eventloop().template src<DependentElement>()) 
            _t_target.eventloop().src(std::nullopt);
    }
}