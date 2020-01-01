#ifndef TSDL_PIXELACCESSER_
#define TSDL_PIXELACCESSER_

#include "TSDL/TSDL_Meta.hpp"

namespace TSDL
{
    template <typename LockFunc, typename AccessFunc, typename UnlockFunc, typename Cls = get_argument_t<UnlockFunc, 0>>
    class PixelAccesser
    {
        private:
        LockFunc _lock;
        AccessFunc _access;
        UnlockFunc _unlock;
        Cls& _locked_cls;

        public:
        template <typename ...Args>
        PixelAccesser(LockFunc locker, AccessFunc accesser, UnlockFunc unlocker, Cls& cls, Args... args) : _lock(locker), _access(accesser), _unlock(unlocker)
        {
            _lock(cls, args...);
            _locked_cls = cls;
        }

        template <typename ...Args>
        function_traits<AccessFunc>::return_type access(Args... args)
        {
            return _access(_locked_cls, args...);
        }

        ~PixelAccesser()
        {
            _unlock(_locked_cls);
        }
    };
}

#endif