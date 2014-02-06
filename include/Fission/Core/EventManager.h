#ifndef __EVENT_MANAGER_H__
#define __EVENT_MANAGER_H__

#include <map>
#include <list>
#include <string>

#include <SFML/System/Mutex.hpp>

#include <Fission/Core/Event.h>

namespace fsn
{
    class IEventManager
    {
        public:
            /// \brief Add a listener for a given event type.
            virtual void addListener(IEventListener* listener, const std::string& ID) = 0;

            /// \brief Remove a listener for a given event type.
            virtual void removeListener(IEventListener* listener, const std::string& ID) = 0;

            /// \brief Add a global listener. This type of listener receives all types of events.
            /// \note A listener that is registered as global as well as for event types will receive
            /// duplicate events.
            virtual void addGlobalListener(IEventListener* listener) = 0;

            /// \brief Remove a global listener.
            virtual void removeGlobalListener(IEventListener* listener) = 0;

            /// \brief Remove all listeners for all event types.
            virtual void removeAllListeners() = 0;

            /// \brief Dispatch an event.
            virtual bool fireEvent(const std::string& ID, const IEventData& evt) = 0;
    };

    class EventManager : public IEventManager
    {
        public:
            EventManager();

            /// \brief Add a listener for a given event type.
            void addListener(IEventListener *listener, const std::string& ID);

            /// \brief Remove a listener for a given event type.
            void removeListener(IEventListener *listener, const std::string& ID);

            /// \brief Add a global listener. This type of listener receives all types of events.
            /// \note A listener that is registered as global as well as for event types will receive
            /// duplicate events.
            void addGlobalListener(IEventListener *listener);

            /// \brief Remove a global listener.
            void removeGlobalListener(IEventListener *listener);

            /// \brief Remove all listeners for all event types.
            void removeAllListeners();

            /// \brief Dispatch an event.
            bool fireEvent(const std::string& ID, const IEventData& evt);

        private:
            typedef std::list<IEventListener*> EventListenerList;
            typedef std::map<std::string, EventListenerList> EventListenerMap;
            typedef std::pair<std::string, EventListenerList> EventListenerMapPair;

            // A map of all the listeners connected to specific events.
            EventListenerMap mListeners;

            // A list of global event listeners. These listeners receive all events regardless of type.
            EventListenerList mGlobals;
    };
}

#endif
