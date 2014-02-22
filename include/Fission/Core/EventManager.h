#ifndef FISSION_EVENT_MANAGER_H
#define FISSION_EVENT_MANAGER_H

#include <unordered_map>
#include <typeinfo>
#include <typeindex>
#include <functional>
#include <vector>

#include "Fission/Util/make_unique.h"

namespace fsn
{
    /// \brief A tag class for event data structs to inherit from.
    struct EventData
    {
    };

    class EventManager
    {
        public:
            EventManager();

            template <typename Evt_T, typename T>
            void addListener(void (T::*meth_ptr)(const Evt_T&), T& listener)
            {
                auto type = std::type_index(typeid(Evt_T));

                if (mListeners.find(type) == mListeners.end())
                    mListeners[type] = std::vector<std::unique_ptr<ICallbackWrapper>>();

                mListeners[type].push_back(make_unique<CallbackWrapper<Evt_T>>(std::bind(meth_ptr, &listener, std::placeholders::_1)));
            }

            template <typename Evt_T>
            void fireEvent(const Evt_T& event)
            {
                for (auto& callback : mListeners[std::type_index(typeid(Evt_T))])
                    callback->call(event);
            }

        private:
            class ICallbackWrapper
            {
                public:
                    virtual ~ICallbackWrapper() {}

                    virtual void call(const EventData& event) = 0;
            };

            template <typename Evt_T>
            class CallbackWrapper : public ICallbackWrapper
            {
                public:
                    CallbackWrapper(const std::function<void(const Evt_T&)>& func) : func(func) {}

                    void call(const EventData& event)
                    {
                        func(static_cast<const Evt_T&>(event));
                    }

                    std::function<void(const Evt_T&)> func;
            };

            std::unordered_map<std::type_index, std::vector<std::unique_ptr<ICallbackWrapper>>> mListeners;
    };
}

#endif
