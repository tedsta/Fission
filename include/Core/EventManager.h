#ifndef __EVENT_MANAGER_H__
#define __EVENT_MANAGER_H__

#include <map>
#include <list>

#include "Core/Event.h"

class EventManager
{
	public:
		EventManager();
		virtual ~EventManager();

		/// \brief Add a listener for a given event type.
		void addListener(IEventListener *listener, EventType type);

		/// \brief Remove a listener for a given event type.
		void removeListener(IEventListener *listener, EventType type);

		/// \brief Add a global listener. This type of listener receives all types of events.
		/// \note A listener that is registered as global as well as for event types will receive
		/// duplicate events.
		void addGlobalListener(IEventListener *listener);

		/// \brief Remove a global listener.
		void removeGlobalListener(IEventListener *listener);

		/// \brief Remove all listeners for all event types.
		void removeAllListeners();

		/// \brief Dispatch an event.
		bool fireEvent(IEventData const& evt) const;

	private:
		typedef std::list<IEventListener*> EventListenerList;
		typedef std::map<EventType, EventListenerList> EventListenerMap;
		typedef std::pair<EventType, EventListenerList> EventListenerMapPair;

		// A map of all the listeners connected to specific events.
		std::map<EventType, EventListenerList> mListeners;

		// A list of global event listeners. These listeners receive all events regardless of type.
		EventListenerList mGlobals;
};

#endif
