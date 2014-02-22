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

class TestListener
{
    public:
        TestListener() : test1(false), test2(false) {}

        void onTestEvent(const TestEvent& event)
        {
            if (event.str == "hello" && event.data == 42)
                test1 = true;
        }

        void onTestEvent2(const TestEvent& event)
        {
            if (event.str == "hello" && event.data == 42)
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

    eventMgr.addListener<TestEvent, TestListener, &TestListener::onTestEvent>(listener1);
    eventMgr.addListener<TestEvent, TestListener, &TestListener::onTestEvent2>(listener2);

    eventMgr.fireEvent(TestEvent("hello", 42));

    CHECK(listener1.test1 && !listener1.test2);
    CHECK(!listener2.test1 && listener2.test2);
}
