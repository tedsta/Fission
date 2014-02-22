#ifndef __EVENT_MANAGER_H__
#define __EVENT_MANAGER_H__

#include <map>
#include <typeinfo>

#include <Nano/nano_signal_slot.hpp>

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

            template <typename Evt_T, typename T, void (T::*meth_ptr)(const Evt_T&)>
            void addListener(T& listener)
            {
                auto type = typeid(Evt_T).hash_code();

                if (mSignals.find(type) == mSignals.end())
                    mSignals[type] = make_unique<SignalWrapper<Evt_T>>();

                Nano::signal<void (const Evt_T&)>& signal = static_cast<SignalWrapper<Evt_T>*>(mSignals[type].get())->signal;
                signal.template connect<T, meth_ptr>(&listener);
            }

            template <typename Evt_T>
            void fireEvent(const Evt_T& event)
            {
                mSignals[typeid(Evt_T).hash_code()]->emit(event);
            }

        private:
            class ISignalWrapper
            {
                public:
                    virtual void emit(const EventData& event) = 0;
            };

            template<typename Evt_T>
            class SignalWrapper : public ISignalWrapper
            {
                public:
                    void emit(const EventData& event)
                    {
                        signal.emit(static_cast<const Evt_T&>(event));
                    }

                    Nano::signal<void (const Evt_T&)> signal;
            };

            std::map<std::size_t, std::unique_ptr<ISignalWrapper>> mSignals;
    };
}

#endif
