#ifndef __EVENT_H__
#define __EVENT_H__

typedef unsigned int EventType;

class IEventData
{
    public:
        IEventData(EventType type) : mType(type) {}

        EventType getType() const { return mType; }

    private:
        EventType mType;
};

class IEventListener
{
    public:
        virtual bool handleEvent(const IEventData &event) = 0;
};

#endif
