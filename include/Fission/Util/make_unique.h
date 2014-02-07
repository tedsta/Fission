#ifndef FISSION_MAKE_UNIQUE_H_INCLUDED
#define FISSION_MAKE_UNIQUE_H_INCLUDED

#include <memory>

namespace fsn
{
    template<typename T, typename C = T, typename... Args>
    std::unique_ptr<T> make_unique(Args&&... args)
    {
        return std::unique_ptr<T>(new C(std::forward<Args>(args)...));
    }
}


#endif // FISSION_MAKE_UNIQUE_H_INCLUDED
