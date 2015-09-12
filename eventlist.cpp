/**
 * The cpp file for the EventList class. This class is for storing and executing events for the inventory system.
 * The EventList object uses linked list to store events.
 */
#include <stdio.h>
#include <stdlib.h>
#include "eventlist.h"

EventList::EventList()
{
	head = NULL;
}

bool EventList::emptyEventList()
{
	return (head == NULL);
}

void EventList::addEvent(double t, int q)
{
	if (head == NULL)
	{
		head = new EventListNode(t, q);
		return;
	}
	EventListNode* newNode = new EventListNode(t, q);
	if (newNode->eventTime <= head->eventTime)
	{
		newNode->next = head;
		head->prev = newNode;
		head = newNode;
		return;
	}
	EventListNode* curr = head;
	while (curr->next != NULL)
	{
		if (newNode->eventTime <= curr->eventTime)
		{
			curr->prev->next = newNode;
			newNode->prev = curr->prev;
			newNode->next = curr;
			curr->prev = newNode;
			return;
		}
		else
		{
			curr = curr->next;
		}
	}
	if (newNode->eventTime <= curr->eventTime)
	{
		curr->prev->next = newNode;
		newNode->prev = curr->prev;
		newNode->next = curr;
		curr->prev = newNode;
		return;
	}
	curr->next = newNode;
	newNode->prev = curr;
}

// void EventList::addEvent(double t, int q)
// {
// 	addEvent(head, t, q);
// }

double EventList::findNextEventTime()
{
	if (head != NULL)
	{
		return head->eventTime;
	}
	else
	{
		return 0;
	}
}

int EventList::findNextEventQt()
{
	if (head != NULL)
	{
		return head->eventQt;
	}
	else
	{
		return 0;
	}
}

void EventList::executeNextEvent()
{
	if (head->next != NULL)
	{
		EventListNode* newHead = head->next;
		newHead->prev = NULL;
		delete head;
		head = newHead;
	}
	else
	{
		delete head;
		head = NULL;
	}
}

// void EventList::executeNextEvent()
// {
// 	deleteEvent(head);
// }

EventList::EventListNode::EventListNode()
{
	eventTime = 0;
	eventQt = 0;
	next = NULL;
	prev = NULL;
}

EventList::EventListNode::EventListNode(double t, int q)
{
	eventTime = t;
	eventQt = q;
	next = NULL;
	prev = NULL;
}

// void EventList::addEvent(EventListNode* & curr, double t, int q)
// {
// 	if (curr == NULL)
// 	{
// 		curr = new EventListNode(t, q);
// 		return;
// 	}
// 	if (t < curr->eventTime)
// 	{
// 		EventListNode* prev = curr->prev;
// 		if (prev == NULL)
// 		{
// 			EventListNode* newNode = new EventListNode(t, q);
// 			newNode->next = curr;
// 			curr->prev = newNode;
// 		}
// 		else
// 		{
// 			if (prev->eventTime <= t)
// 			{
// 				EventListNode* newNode = new EventListNode(t, q);
// 				prev->next = newNode;
// 				newNode->prev = prev;
// 				newNode->next = curr;
// 				curr->prev = newNode;
// 			}
// 			else
// 			{
// 				addEvent(prev, t, q);
// 			}
// 		}
// 	}
// 	else
// 	{
// 		EventListNode* next = curr->next;
// 		if (next == NULL)
// 		{
// 			EventListNode* newNode = new EventListNode(t, q);
// 			newNode->prev = curr;
// 			curr->next = newNode;
// 		}
// 		else
// 		{
// 			if (next->eventTime >= t)
// 			{
// 				EventListNode* newNode = new EventListNode(t, q);
// 				curr->next = newNode;
// 				newNode->prev = curr;
// 				newNode->next = next;
// 				next->prev = newNode;
// 			}
// 			else
// 			{
// 				addEvent(curr->next, t, q);
// 			}
// 		}
// 	}
// }

// EventListNode* EventList::findEvent(EventListNode* & curr, double t)
// {
// 	if (curr == NULL || curr->eventTime == t)
// 		return curr;
// 	else
// 		return findEvent(curr->next, t);
// }

// EventListNode* EventList::findPreviousEvent(EventListNode* & curr, double t)
// {
// 	if (curr->next == NULL)
// 		return NULL;
// 	else if (curr->next->eventTime == t)
// 	{
// 		return curr;
// 	}
// 	else
// 	{
// 		return findPreviousEvent(curr->next, t);
// 	}

// }

// void EventList::deleteEvent(EventListNode* & curr)
// {
// 	if (curr != NULL)
// 	{
// 		if (curr->prev != NULL)
// 		{
// 			if (curr->next != NULL)
// 			{
// 				curr->prev->next = curr->next;
// 			}
// 			else
// 			{
// 				curr->prev->next = NULL;
// 			}
// 		}
// 		if (curr->next != NULL)
// 		{
// 			if (curr->prev != NULL)
// 			{
// 				curr->next->prev = curr->prev;
// 			}
// 			else
// 			{
// 				curr->next->prev = NULL;
// 			}
// 		}
// 		delete curr;
// 	}
// }