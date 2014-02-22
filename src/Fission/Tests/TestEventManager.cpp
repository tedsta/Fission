#include <UnitTest++/UnitTest++.h>

#include <memory>

#include "Fission/Core/EventManager.h"

using namespace fsn;

struct TestEvent : public EventData
{
    TestEvent(const std::string& str, int data) : str(str), data(data) {}

    std::string str;
    int data;
};

struct TestEvent2 : public EventData
{
    TestEvent2(const std::string& str, const std::string& data) : str(str), data(data) {}

    std::string str;
    std::string data;
};

class TestListener
{
    public:
        TestListener() : test1(false), test2(false) {}

        void onTestEvent(const TestEvent& event)
        {
            if (event.str == "hello" && event.data == 42)
                test1 = true;
        }

        void onTestEvent2(const TestEvent2& event)
        {
            if (event.str == "hello" && event.data == "world")
                test2 = true;
        }

        bool test1;
        bool test2;
};

TEST(EventManager_fireEventMemberFunc)
{
    EventManager eventMgr;

    TestListener listener1;
    TestListener listener2;

    eventMgr.addListener(&TestListener::onTestEvent, listener1);
    eventMgr.addListener(&TestListener::onTestEvent2, listener2);

    eventMgr.fireEvent(TestEvent("hello", 42));
    eventMgr.fireEvent(TestEvent2("hello", "world"));

    CHECK(listener1.test1 && !listener1.test2);
    CHECK(!listener2.test1 && listener2.test2);
}
