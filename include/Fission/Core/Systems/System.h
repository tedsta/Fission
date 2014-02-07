#ifndef SYSTEM_H
#define SYSTEM_H

namespace fsn
{
    /// \brief Abstract base class of all entity processing systems.
    class System
    {
        public:
            virtual void update(const float dt) = 0;
    };
}


#endif // SYSTEM_H
