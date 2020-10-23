//-----------------------------------------------------------------------------
// This file is subject to the license terms in the LICENSE file found in the 
// top-level directory of this distribution.
//
// No part of this software, including this file, may be copied, modified, 
// propagated, or distributed except according to the terms contained in the 
// LICENSE file.
//-----------------------------------------------------------------------------

#ifndef TSDL_ELEMENTS_SCENE_
#define TSDL_ELEMENTS_SCENE_

#include "TSDL/visuals/elements/Element.hpp"
#include "TSDL/visuals/elements/WindowAdapter.hpp"
#include "TSDL/visuals/elements/attrs/EventLookup.hpp"

namespace TSDL::elements
{
    /*
    * Scene is a class that automatically unbind target as eventloop's source when destructed
    */
    class Scene
    {
        private:
        std::reference_wrapper<const DependentElement> _target;
        WindowAdapter& _window;
        bool _unbind = false;

        public:        
        template <typename T, typename U = std::enable_if_t<
            _and_v<
                std::is_base_of_v<DependentElement, T>,
                std::is_base_of_v<attrs::EventLookupable, T>
            >
        >>
        Scene(WindowAdapter& window, T& target): _target(target), _window(window), _unbind(true)
        {
            window.src(target);
        }

        Scene(Scene&& other);

        template <typename T, typename U = std::enable_if_t<
            _and_v<
                std::is_base_of_v<DependentElement, T>,
                std::is_base_of_v<attrs::EventLookupable, T>
            >
        >>
        Scene& operator=(T& target)
        {
            _target = target;
            window.src(target);
            return *this;
        }

        WindowAdapter& bounded_window();

        virtual ~Scene();
    };
}

#endif