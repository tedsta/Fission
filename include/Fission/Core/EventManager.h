#ifndef __EVENT_MANAGER_H__
#define __EVENT_MANAGER_H__

#include <map>
#include <list>

#include <SFML/System/Mutex.hpp>

#include <Fission/Core/Event.h>

namespace fsn
{
    class IEventManager
    {
        public:
            virtual ~IEventManager(){}

            /// \brief Add a listener for a given event type.
            virtual void addListener(IEventListener *listener, EventID type) = 0;

            /// \brief Remove a listener for a given event type.
            virtual void removeListener(IEventListener *listener, EventID type) = 0;

            /// \brief Add a global listener. This type of listener receives all types of events.
            /// \note A listener that is registered as global as well as for event types will receive
            /// duplicate events.
            virtual void addGlobalListener(IEventListener *listener) = 0;

            /// \brief Remove a global listener.
            virtual void removeGlobalListener(IEventListener *listener) = 0;

            /// \brief Remove all listeners for all event types.
            virtual void removeAllListeners() = 0;

            /// \brief Dispatch an event.
            virtual bool fireEvent(const IEventData& evt) = 0;
    };

    class EventManager : public IEventManager
    {
        friend class Engine;

        public:
            ~EventManager();

            /// \brief Add a listener for a given event type.
            void addListener(IEventListener *listener, EventID type);

            /// \brief Remove a listener for a given event type.
            void removeListener(IEventListener *listener, EventID type);

            /// \brief Add a global listener. This type of listener receives all types of events.
            /// \note A listener that is registered as global as well as for event types will receive
            /// duplicate events.
            void addGlobalListener(IEventListener *listener);

            /// \brief Remove a global listener.
            void removeGlobalListener(IEventListener *listener);

            /// \brief Remove all listeners for all event types.
            void removeAllListeners();

            /// \brief Dispatch an event.
            bool fireEvent(const IEventData& evt);

        private:
            EventManager(); // Only Engine can instantiate an event manager.

            typedef std::list<IEventListener*> EventListenerList;
            typedef std::map<EventID, EventListenerList> EventListenerMap;
            typedef std::pair<EventID, EventListenerList> EventListenerMapPair;

            // A map of all the listeners connected to specific events.
            std::map<EventID, EventListenerList> mListeners;

            // A list of global event listeners. These listeners receive all events regardless of type.
            EventListenerList mGlobals;

            // Mutex for threadsafe event firing
            sf::Mutex mMutex;
    };
}

#endif
