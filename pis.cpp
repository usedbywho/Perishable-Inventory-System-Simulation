/**
 * The cpp file for the PIS class. All the operations in the inventory system are executed here.
 * This class utilize the EventList to store and execute events, mainly demand arrival and order arrival.
 * The data structures used are mainly integers, doubles and arrays.
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "pis.h"

PIS::PIS()
{
	l = 10000;
	m = 10;
	lambda = 2.0;
	L = 5;
	demands = new int[l];
	backlogs = new int[l];
	orders = new int[l];
	inventories = new int[m];
	currInventories = new int[l];
	disposals = new int[l];
	sales = new int[l];
	totalInventories = 0;
	totalDisposal = 0;
	currDisposal = 0;
	futureDisposal = 0;
	initialize();
	scheduleDemand(0);
}

PIS::PIS(int simLength, int expiration, double arrivalRate, int leadTime, int startingInventory)
{
	l = simLength;
	m = expiration;
	lambda = arrivalRate;
	L = leadTime;
	demands = new int[l];
	backlogs = new int[l];
	orders = new int[l];
	inventories = new int[m];
	currInventories = new int[l];
	disposals = new int[l];
	sales = new int[l];
	inventories[0] = startingInventory;
	totalInventories = startingInventory;
	IP0 = startingInventory;
	IPt = startingInventory;
	totalDisposal = 0;
	currDisposal = 0;
	futureDisposal = 0;
	initialize();
	scheduleDemand(0);
}

bool PIS::eventsAvailable(double currTime)
{
	return !events.emptyEventList() && (events.findNextEventTime() <= currTime);
}

double PIS::nextEventTime()
{
	return events.findNextEventTime();
}

double PIS::nextEventQt()
{
	return events.findNextEventQt();
}

void PIS::executeNextEvent()
{
	events.executeNextEvent();
}

void PIS::scheduleDemand(double currTime)
{
	double intArrTime = 1;
	while (intArrTime == 1)
	{
		intArrTime = interarrivalTime(((double) rand() / (RAND_MAX)));
	}
	events.addEvent(currTime + intArrTime, -1);
}

void PIS::receiveDemand(int currPeriod, int q)
{
	demands[currPeriod] = demands[currPeriod] + q;
	IPt = IPt - q;
}

void PIS::updateBacklogs(int currPeriod)
{
	backlogs[currPeriod] = backlogs[currPeriod] + demands[currPeriod];
}

void PIS::scheduleOrderArrival(double currTime, int orderQt)
{
	events.addEvent(currTime + L, orderQt);
	IPt = IPt + orderQt;
}

void PIS::receiveOrderArrival(int q)
{
	inventories[0] = inventories[0] + q;
	totalInventories = totalInventories + q;
	IPt = IPt - q;
	IPt = IPt + q;
}

void PIS::updateInventories(int currPeriod)
{
	totalInventories = totalInventories - inventories[m - 1];
	currInventories[currPeriod] = totalInventories;
	IPt = IPt - inventories[m - 1];
	currDisposal = inventories[m - 1];
	disposals[currPeriod] = currDisposal;
	totalDisposal = totalDisposal + currDisposal;
	for (int i = m - 1; i > 0; i--)
	{
		inventories[i] = inventories[i - 1];
	}
	inventories[0] = 0;
	futureDisposal = 0;
	if (currPeriod < l - m)
	{
		for (int i = m - 1; i >= m - L; i--)
		{
			futureDisposal = futureDisposal + inventories[i];
		}
	}
}

void PIS::rationing(int currPeriod, int rationingType)
{
	if (rationingType == 1)
	{
		int servedDemand = min(backlogs[currPeriod], totalInventories);
		if (totalInventories >= backlogs[currPeriod])
		{
			for (int i = m - 1; i >= 0; i--)
			{
				if (servedDemand == 0)
				{
					break;
				}
				if (servedDemand >= inventories[i])
				{
					servedDemand = servedDemand - inventories[i];
					inventories[i] = 0;
				}
				else
				{
					inventories[i] = inventories[i] - servedDemand;
					servedDemand = 0;
				}
			}
			sales[currPeriod] = backlogs[currPeriod];
			totalInventories = totalInventories - backlogs[currPeriod];
			backlogs[currPeriod] = 0;
		}
		else
		{
			for (int i = 0; i < m; i++)
			{
				inventories[i] = 0;
			}
			sales[currPeriod] = totalInventories;
			backlogs[currPeriod] = backlogs[currPeriod] - totalInventories;
			if (currPeriod + 1 < l)
			{
				backlogs[currPeriod + 1] = backlogs[currPeriod];
			}
			totalInventories = 0;
		}
	}
	else if (rationingType == 2)
	{
		int servedDemand = min(backlogs[currPeriod], totalInventories);
		if (totalInventories >= backlogs[currPeriod])
		{
			for (int i = 0; i < m; i++)
			{
				if (servedDemand == 0)
				{
					break;
				}
				if (servedDemand >= inventories[i])
				{
					servedDemand = servedDemand - inventories[i];
					inventories[i] = 0;
				}
				else
				{
					inventories[i] = inventories[i] - servedDemand;
					servedDemand = 0;
				}
			}
			sales[currPeriod] = backlogs[currPeriod];
			totalInventories = totalInventories - backlogs[currPeriod];
			backlogs[currPeriod] = 0;
		}
		else
		{
			for (int i = 0; i < m; i++)
			{
				inventories[i] = 0;
			}
			sales[currPeriod] = totalInventories;
			backlogs[currPeriod] = backlogs[currPeriod] - totalInventories;
			if (currPeriod + 1 < l)
			{
				backlogs[currPeriod + 1] = backlogs[currPeriod];
			}
			totalInventories = 0;
		}
	}
	else if (rationingType == 3)
	{
		int servedDemand = min(backlogs[currPeriod], totalInventories);
		if (totalInventories >= backlogs[currPeriod])
		{
			while (servedDemand != 0)
			{
				int i = (int) ((double) rand() / (RAND_MAX) * m);
				if (inventories[i] > 0)
				{
					inventories[i] = inventories[i] - 1;
					servedDemand = servedDemand - 1;
				}
			}
			sales[currPeriod] = backlogs[currPeriod];
			totalInventories = totalInventories - backlogs[currPeriod];
			backlogs[currPeriod] = 0;
		}
		else
		{
			for (int i = 0; i < m; i++)
			{
				inventories[i] = 0;
			}
			sales[currPeriod] = totalInventories;
			backlogs[currPeriod] = backlogs[currPeriod] - totalInventories;
			if (currPeriod + 1 < l)
			{
				backlogs[currPeriod + 1] = backlogs[currPeriod];
			}
			totalInventories = 0;
		}
	}
}

void PIS::ordering(int currPeriod, int orderingType)
{
	if (orderingType == 1)
	{
		int orderQt = demands[currPeriod] + currDisposal;
		scheduleOrderArrival(currPeriod + L, orderQt);
		orders[currPeriod] = orders[currPeriod] + orderQt;
	}
	else if (orderingType == 2)
	{
		int orderQt = demands[currPeriod] + currDisposal + max((IP0 - (IPt - futureDisposal)), 0);
		scheduleOrderArrival(currPeriod + L, orderQt);
		orders[currPeriod] = orders[currPeriod] + orderQt;
	}
}

int PIS::periodOrder(int currPeriod)
{
	return orders[currPeriod];
}

int PIS::periodInventory()
{
	return totalInventories;
}

int PIS::periodDisposal()
{
	return currDisposal;
}

int PIS::calcDemands(int startPeriod, int endPeriod)
{
	int sum = 0;
	for (int i = startPeriod; i <= endPeriod; i++)
	{
		sum = sum + demands[i];
	}
	return sum;
}

int PIS::calcOrders(int startPeriod, int endPeriod)
{
	int sum = 0;
	for (int i = startPeriod; i <= endPeriod; i++)
	{
		sum = sum + orders[i];
	}
	return sum;
}

int PIS::calcInventories(int startPeriod, int endPeriod)
{
	int sum = 0;
	for (int i = startPeriod; i <= endPeriod; i++)
	{
		sum = sum + currInventories[i];
	}
	return sum;
}

int PIS::calcBacklogs(int startPeriod, int endPeriod)
{
	int sum = 0;
	for (int i = startPeriod; i <= endPeriod; i++)
	{
		sum = sum + backlogs[i];
	}
	return sum;
}

int PIS::calcDisposals(int startPeriod, int endPeriod)
{
	int sum = 0;
	for (int i = startPeriod; i <= endPeriod; i++)
	{
		sum = sum + disposals[i];
	}
	return sum;
}

int PIS::calcSales(int startPeriod, int endPeriod)
{
	int sum = 0;
	for (int i = startPeriod; i <= endPeriod; i++)
	{
		sum = sum + sales[i];
	}
	return sum;
}

double PIS::calcMean(int* & values, int num)
{
	double sum = 0;
	for (int i = 0; i < num; i++)
	{
		sum = sum + values[i];
	}
	return (sum / num);
}

double PIS::calcStdDev(int* & values, int num)
{
	double mean = calcMean(values, num);
	double temp = 0;
	for (int i = 0; i < num; i++)
	{
		temp = temp + (values[i] - mean) * (values[i] - mean);
	}
	return (temp / num);
}

void PIS::printData(int runNum)
{
	ofstream fout;
	fout.open("Demands" + std::to_string(runNum) + ".txt");
	for (int i = 0; i < l; i++)
	{
		fout << demands[i] << '\n';
	}
	fout.close();
	fout.clear();
	fout.open("Backlogs" + std::to_string(runNum) + ".txt");
	for (int i = 0; i < l; i++)
	{
		fout << backlogs[i] << '\n';
	}
	fout.close();
	fout.clear();
	fout.open("Orders" + std::to_string(runNum) + ".txt");
	for (int i = 0; i < l; i++)
	{
		fout << orders[i] << '\n';
	}
	fout.close();
	fout.clear();
	fout.open("Inventories" + std::to_string(runNum) + ".txt");
	for (int i = 0; i < l; i++)
	{
		fout << currInventories[i] << '\n';
	}
	fout.close();
	fout.clear();
	fout.open("Disposals" + std::to_string(runNum) + ".txt");
	for (int i = 0; i < l; i++)
	{
		fout << disposals[i] << '\n';
	}
	fout.close();
	fout.clear();
	fout.open("Sales" + std::to_string(runNum) + ".txt");
	for (int i = 0; i < l; i++)
	{
		fout << sales[i] << '\n';
	}
	fout.close();
	fout.clear();
}

void PIS::initialize()
{
	for (int i = 0; i < l; i++)
	{
		demands[i] = 0;
		backlogs[i] = 0;
		orders[i] = 0;
	}
	for (int i = 0; i < m; i++)
	{
		inventories[i] = 0;
	}
}

double PIS::interarrivalTime(double r)
{
	return -log(1 - r) / lambda;
}