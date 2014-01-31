#ifndef __EVENT_H__
#define __EVENT_H__

namespace fsn
{
    typedef unsigned int EventID;

    /// \brief A tag class for event data
    struct IEventData
    {
    };

    class IEventListener
    {
        public:
            virtual bool handleEvent(const std::string& ID, const IEventData& event) = 0;
    };
}


#endif
