/**
 * The header file for the EventList class. This class is for storing and executing events for the inventory system.
 * The EventList object uses linked list to store events.
 */
#ifndef EVENTLIST_H
#define EVENTLIST_H

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include "math.h"

using namespace std;

class EventList
{
public:
	/**
	 * Default constructor
	 */
	EventList();

	/**
	 * Checks if the event list is empty
	 */
	bool emptyEventList();

	/**
	 * Adds an event to the event list
	 */
	void addEvent(double t, int q);

	/**
	 * Determines and returns the time of the next event to execute
	 */
	double findNextEventTime();

	/**
	 * Determines and returns the quantity change of next event to execute
	 */
	int findNextEventQt();

	/**
	 * Execute the next event
	 */
	void executeNextEvent();

private:
	/**
	 * The class for EventListNode
	 */
	class EventListNode
	{
	public:
		/**
		 * Default constructor
		 */
		EventListNode();

		/**
		 * Constructor with arguments
		 */
		
		EventListNode(double t, int q);

		/**
		 * The time of the event
		 */
		double eventTime;

		/**
		 * The quantity change of the inventory
		 */
		int eventQt;

		/**
		 * A pointer pointing to the next event
		 */
		EventListNode* next;

		/**
		 * A pointer pointing to the previous event
		 */
		EventListNode* prev;
	};

	/**
	 * The first node of the event list
	 */
	EventListNode* head;

	/**
	 * Private helper function for adding and event to the event list
	 */
	//void addEvent(EventListNode* & curr, double t, int q);

	/**
	 * Private helper function for finding the event with a certain time
	 */
	//EventListNode* findEvent(EventListNode* & curr, double t);

	/**
	 * Private helper function for finding the previous event of time t
	 */
	//EventListNode* findPreviousEvent(EventListNode* & curr, double t);

	/**
	 * Private helper function for deleting an event from the event list
	 */
	//void deleteEvent(EventListNode* & curr);
};

#endif