#include "TSDL/abstract/elements/Scene.hpp"
#include "TSDL/abstract/elements/EventloopAdapter.hpp"

#include <cassert>

namespace TSDL::elements
{
    Scene::Scene(Scene&& other): _target(other._target), _window(other._window)
    {
        other._unbind = false;
    }

    WindowAdapter& Scene::bounded_window()
    {
        return _window;
    }

    Scene::~Scene()
    {
        const DependentElement& _t_target = _target;
        if (_unbind && 
            _t_target == _window.template src<DependentElement>()) 
            _window.src(std::nullopt);
    }
}