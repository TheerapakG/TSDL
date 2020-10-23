//-----------------------------------------------------------------------------
// This file is subject to the license terms in the LICENSE file found in the 
// top-level directory of this distribution.
//
// No part of this software, including this file, may be copied, modified, 
// propagated, or distributed except according to the terms contained in the 
// LICENSE file.
//-----------------------------------------------------------------------------

#include "TSDL/visuals/elements/Scene.hpp"
#include "TSDL/visuals/elements/EventloopAdapter.hpp"

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