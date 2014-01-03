#ifndef __EVENT_H__
#define __EVENT_H__

namespace fission
{
    typedef unsigned int EventID;

    class IEventData
    {
        public:
            IEventData(EventID ID) : mID(ID) {}

            EventID getID() const { return mID; }

        private:
            EventID mID;
    };

    class IEventListener
    {
        public:
            virtual bool handleEvent(const IEventData &event) = 0;
    };
}


#endif
