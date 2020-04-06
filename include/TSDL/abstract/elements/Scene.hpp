#ifndef TSDL_ELEMENTS_SCENE_
#define TSDL_ELEMENTS_SCENE_

#include "TSDL/abstract/elements/Element.hpp"
#include "TSDL/abstract/elements/attrs/EventLookup.hpp"

namespace TSDL::elements
{
    /*
    * Scene is a class that automatically unbind target as eventloop's source when destructed
    */
    class Scene
    {
        private:
        optional_const_reference<DependentElement> _target;
        bool _unbind = false;

        public:
        Scene() = default;
        
        template <typename T, typename U = std::enable_if_t<
            _and_v<
                std::is_base_of_v<DependentElement, T>,
                std::is_base_of_v<attrs::EventLookupable, T>
            >
        >>
        Scene(T& target): _target(make_optional_const_ref(target)), _unbind(true)
        {
            target.eventloop().src(target);
        }

        Scene(Scene&& other);
        
        Scene& operator=(Scene&& other);

        virtual ~Scene();
    };
}

#endif