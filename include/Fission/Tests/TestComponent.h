#ifndef TESTCOMPONENT_H
#define TESTCOMPONENT_H

#include "Fission/Core/Component.h"

class TestComponent : public fsn::Component
{
    FISSION_COMPONENT

    public:
        TestComponent() : mData(0)
        {
        }

        void serialize(fsn::Packet& packet)
        {
            packet << mData;
            packet << mStr;
        }

        void deserialize(fsn::Packet& packet)
        {
            packet >> mData;
            packet >> mStr;
        }

        int mData;
        std::string mStr;
};

class Test2Component : public fsn::Component
{
    FISSION_COMPONENT

    public:
        Test2Component() : mData(0)
        {
        }

        void serialize(fsn::Packet& packet)
        {
            packet << mData;
        }

        void deserialize(fsn::Packet& packet)
        {
            packet >> mData;
        }

        int mData;
};

#endif // TESTCOMPONENT_H
