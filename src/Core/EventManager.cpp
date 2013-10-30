#include "Core/EventManager.h"
#include <assert.h>
#include <iostream>

EventManager::EventManager()
{
    //ctor
}

EventManager::~EventManager()
{
	removeAllListeners();
}

void EventManager::addListener( IEventListener *listener, EventType type )
{
	// Add the type to the listeners map if it doesn't exist.
	EventListenerMap::iterator it = mListeners.find(type);
	if (it == mListeners.end())
		it = mListeners.insert(EventListenerMapPair(type, EventListenerList())).first;

	EventListenerList &list = it->second;

	// Add the listener to the map.
	list.push_back(listener);
}

void EventManager::removeListener( IEventListener *listener, EventType type )
{
	EventListenerMap::iterator it = mListeners.find(type);
	if (it != mListeners.end())
	{
		EventListenerList &list = it->second;
		for (EventListenerList::iterator it1 = list.begin(); it1 != list.end(); it1++)
		{
			if (*it1 == listener)
			{
				list.erase(it1);
				break;
			}
		}
	}
}

void EventManager::addGlobalListener(IEventListener *listener)
{
	mGlobals.push_back(listener);
}

void EventManager::removeGlobalListener(IEventListener *listener)
{
	EventListenerList &list = mGlobals;
	for (EventListenerList::iterator it = list.begin(); it != list.end(); it++)
	{
		if (*it == listener)
		{
			mGlobals.erase(it);
			return;
		}
	}
}

void EventManager::removeAllListeners()
{
	for (EventListenerMap::iterator it = mListeners.begin(); it != mListeners.end(); it++)
		it->second.clear();

	mListeners.clear();
	mGlobals.clear();
}

bool EventManager::fireEvent(IEventData const& evt) const
{
	// Notify all global listeners.
	for (EventListenerList::const_iterator it = mGlobals.begin(); it != mGlobals.end(); ++it)
	{
		if( (*it)->handleEvent( evt ) )
			return true;
	}

	EventListenerMap::const_iterator it = mListeners.find(evt.getType());
	if (it != mListeners.end())
	{
		const EventListenerList &list = it->second;
		for (EventListenerList::const_iterator it2 = list.begin(); it2 != list.end(); it2++)
		{
			if ((*it2)->handleEvent(evt))
				return true;
		}
	}

	return false;
}
