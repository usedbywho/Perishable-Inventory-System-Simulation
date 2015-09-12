/**
 * The header file for the PIS class. All the operations in the inventory system are executed here.
 * This class utilize the EventList to store and execute events, mainly demand arrival and order arrival.
 * The data structures used are mainly integers, doubles and arrays.
 */
#ifndef PIS_H
#define PIS_H

#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include "math.h"
#include "eventlist.h"

using namespace std;

class PIS
{
public:
	/**
	 * Default constructor
	 */
	PIS();

	/**
	 * The constructor with arguments
	 */
	PIS(int simLength, int expiration, double arrivalRate, int leadTime, int startingInventory);

	/**
	 * Determines if there is any events to execute in this period
	 */
	bool eventsAvailable(double currTime);

	/**
	 * Returns the time of the next event to execute
	 */
	double nextEventTime();

	/**
	 * Returns the quantity change of the next event to execute
	 */
	double nextEventQt();

	/**
	 * Execute the next event in the event list
	 */
	void executeNextEvent();

	/**
	 * Schedule the demand event in the event list
	 */
	void scheduleDemand(double currTime);

	/**
	 * Receives demand
	 */
	void receiveDemand(int currPeriod, int q);

	/**
	 * Updates the backlog list
	 */
	void updateBacklogs(int currPeriod);

	/**
	 * Schedule the order arrival event in the event list
	 */
	void scheduleOrderArrival(double currTime, int orderQt);

	/**
	 * Receives order arrival
	 */
	void receiveOrderArrival(int q);

	/**
	 * Updates inventory list
	 */
	void updateInventories(int currPeriod);

	/**
	 * Serves the demand according to the rationing policy specified by rationingType
	 * 1 - First in first out (FIFO)
	 * 2 - First in last out (FILO)
	 * 3 - Random
	 */
	void rationing(int currPeriod, int rationingType);

	/**
	 * Makes order
	 * 1 - Base stock policy
	 * 2 - Base stock policy with consideration of future disposals
	 */
	void ordering(int currPeriod, int orderingType);

	/**
	 * Returns the order quantity in this period
	 */
	int periodOrder(int currPeriod);

	/**
	 * Returns the total inventory in the system
	 */
	int periodInventory();

	/**
	 * Returns the disposal quantity in this period
	 */
	int periodDisposal();

	/**
	 * Returns the total demands received in the given interval
	 */
	int calcDemands(int startPeriod, int endPeriod);

	/**
	 * Returns the total orders placed in the given interval
	 */
	int calcOrders(int startPeriod, int endPeriod);

	/**
	 * Returns the total inventories held in the given interval
	 */
	int calcInventories(int startPeriod, int endPeriod);

	/**
	 * Returns the total backlogs induced in the given interval
	 */
	int calcBacklogs(int startPeriod, int endPeriod);

	/**
	 * Returns the total disposals made in the given interval
	 */
	int calcDisposals(int startPeriod, int endPeriod);

	/**
	 * Returns the total sales made in the given interval
	 */
	int calcSales(int startPeriod, int endPeriod);

	/**
	 * Returns the mean of an array
	 */
	double calcMean(int* & values, int num);

	/**
	 * Returns the standard deviation of an array
	 */
	double calcStdDev(int* & values, int num);

	/**
	 * Print out running data for demand, backlogs and orders
	 */
	void printData(int runNum);



private:
	/**
	 * The number of periods being simulated
	 */
	int l;

	/**
	 * The time before the product expires
	 */
	int m;

	/**
	 * Arrival rate for the product
	 */
	double lambda;

	/**
	 * Lead time of orders
	 */
	int L;

	/**
	 * The initial and desired inventory position
	 */
	int IP0;

	/**
	 * The current inventory position
	 */
	int IPt;

	/**
	 * The EventList for holding events
	 */
	EventList events;

	/**
	 * The array for storing demands in each period
	 */
	int* demands;

	/**
	 * The array for storing backlogs in each period
	 */
	int* backlogs;

	/**
	 * The array for storing orders in each period
	 */
	int* orders;

	/**
	 * The array for storing inventories of each possible age
	 */
	int* inventories;

	/**
	 * The array for storing current inventories at the end of each period
	 */
	int* currInventories;

	/**
	 * The array for storing disposals in each period
	 */
	int* disposals;

	/**
	 * The array for storing sales in each period
	 */
	int* sales;

	/**
	 * The total inventories in the system
	 */
	int totalInventories;

	/**
	 * The total disposal of the system
	 */
	int totalDisposal;

	/**
	 * The disposal amount in the current period
	 */
	int currDisposal;

	/**
	 * The disposal amount in the lead time period from now
	 */
	int futureDisposal;

	/**
	 * Private helper function for initializing lists
	 */
	void initialize();

	/**
	 * Private helper function for determining interarrival time
	 */
	double interarrivalTime(double r);
};

#endif