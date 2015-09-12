/**
 * The main cpp file for the simulation. This program creates PIS objects and run simulatioin with them.
 * 
 * The program can read parameters from a .txt file from an outside location. The format of the Parameters.txt and the default parameters are
 * Simulation Length:10000
 * Expiration Time:10
 * Arrival Rate:2
 * Lead Time:5
 * Starting Inventory:100
 * Number of Runs:2
 * Rationing Type:1
 * Ordering Type:1
 * 
 * After each run of the simulation is finished, the program calls the printDate function in PIS class to print
 * order, demands, backlogs, inventories, sales, and disposals data to .txt files.
 * The program will also print the summary statistics of each run in both command line and Results.txt,
 * including total demand, total sales, total ordering cost, total holding cost, total backlog cost,
 * total disposal cost, total cost, total revenue, and total profit.
 * 
 * After all runs of the simulation are finished, the program will print the summary statistics of the entire simulation
 * in both command line and Results.txt, including both average and standard deviation of demand, sales, ordering cost,
 * holding cost, backlog cost, disposal cost, cost, revenue, and profit.
 */
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include "eventlist.h"
#include "pis.h"

using namespace std;

int countNumLine(string file)
{
	int numL = 0;
	ifstream fin(file.c_str());
	string line;
	while (getline(fin, line))
	{
		++numL;
	}
	return numL;
}

void readParam(string file, vector<int> & params)
{
	int numLine = countNumLine(file);
	if (numLine != 8)
	{
		cout << "The number of parameters is not correct!" << endl;
		exit(1);
	}
	params.clear();
	params.resize(numLine);
	ifstream fin(file.c_str());
	string line;
	for (int lNum = 0; getline(fin, line); lNum++)
	{
		size_t splitPos = line.find_first_of(":");
		params[lNum] = atoi(line.substr(splitPos + 1).c_str());
	}
}

int calcOptimIP(double arrivalRate, int orderingCost, int price)
{
	double p = (double) orderingCost / price;
	int i = 0;
	int factorial = 1;
	double sum = exp(-arrivalRate) * pow(arrivalRate, i) / factorial;
	while (sum < p)
	{
		i = i + 1;
		factorial = factorial * i;
		sum = sum + exp(-arrivalRate) * pow(arrivalRate, i) / factorial;
	}
	return i;
}

double calcMean(int* & values, int num)
{
	double sum = 0;
	for (int i = 0; i < num; i++)
	{
		sum = sum + values[i];
	}
	return (sum / num);
}

double calcStdDev(int* & values, int num)
{
	double mean = calcMean(values, num);
	double temp = 0;
	for (int i = 0; i < num; i++)
	{
		temp = temp + (values[i] - mean) * (values[i] - mean);
	}
	return sqrt(temp / num);
}

int main(int argc, char* argv[])
{
	srand(time(NULL));
	// int simLength = 10000;
	// int expiration = 10;
	// double arrivalRate = 2.0;
	// int leadTime = 5;
	int simLength;
	int expiration;
	double arrivalRate;
	int leadTime;
	int startingInventory;
	int runs;
	int rationingType;
	int orderingType;
	// cout << "Please enter the length of the simulation" << endl;
	// cin >> simLength;
	// cout << "Please enter the expiration time of the product" << endl;
	// cin >> expiration;
	// cout << "Please enter the arrivalRate of the demand" << endl;
	// cin >> arrivalRate;
	// cout << "Please enter the lead time of the order" << endl;
	// cin >> leadTime;
	// cout << "Please enter the number of runs" << endl;
	// cin >> runs;
	vector<int> params;
	readParam(argv[1], params);
	simLength = params[0];
	expiration = params[1];
	arrivalRate = params[2];
	leadTime = params[3];
	startingInventory = params[4];
	runs = params[5];
	rationingType = params[6];
	orderingType = params[7];
	int* demands = new int[runs];
	int* sales = new int[runs];
	int* orderingCosts = new int[runs];
	int* holdingCosts = new int[runs];
	int* backlogCosts = new int[runs];
	int* disposalCosts = new int[runs];
	int* totalCosts = new int[runs];
	int* totalRevenues = new int[runs];
	int* totalProfits = new int[runs];
	cout << "The following data is recorded for period " << 0.3 * simLength + 1 << " through period " << simLength << endl;
	ofstream fout("Results.txt");
	fout << "The following data is recorded for period " << 0.3 * simLength + 1 << " through period " << simLength << endl;
	fout.close();
	fout.clear();
	for (int count = 0; count < runs; count++)
	{
		PIS pis(simLength, expiration, arrivalRate, leadTime, startingInventory);
		int i;
		double tck;
		int orderingCost = 1;
		int holdingCost = 1;
		int backlogCost = 1;
		int disposalCost = 1;
		int price = 2;
		int totalOrderingCost = 0;
		int totalHoldingCost = 0;
		int totalDisposalCost = 0;
		int totalDemand = 0;
		for (i = 0; i < simLength; i++)
		{
			while (pis.eventsAvailable(i))
			{
				tck = pis.nextEventTime();
				int qt = pis.nextEventQt();
				if (qt > 0)
				{
					pis.receiveOrderArrival(qt);
				}
				else if (qt < 0)
				{
					pis.receiveDemand(i, abs(qt));
					pis.scheduleDemand(tck);
					totalDemand++;
				}
				pis.executeNextEvent();
			}
			pis.updateBacklogs(i);
			pis.rationing(i, rationingType);
			pis.updateInventories(i);
			pis.ordering(i, orderingType);
		}
		// for (i = 0.3 * simLength + 1; i < simLength; i++)
		// {
		// 	pis.updateInventories(i);
		// 	totalHoldingCost = totalHoldingCost + holdingCost * pis.periodInventory();
		// 	while (pis.eventsAvailable(i))
		// 	{
		// 		tck = pis.nextEventTime();
		// 		int qt = pis.nextEventQt();
		// 		if (qt > 0)
		// 		{
		// 			pis.receiveOrderArrival(qt);
		// 		}
		// 		else if (qt < 0)
		// 		{
		// 			pis.receiveDemand(i, abs(qt));
		// 			pis.scheduleDemand(tck);
		// 			totalDemand++;
		// 		}
		// 		pis.executeNextEvent();
		// 	}
		// 	pis.updateBacklogs(i);
		// 	pis.rationing(i, rationingType);
		// 	pis.ordering(i, orderingType);
		// 	totalDisposalCost = totalDisposalCost + disposalCost * pis.periodDisposal();
		// 	totalOrderingCost = totalOrderingCost + orderingCost * pis.periodOrder(i);
		// }
		cout << "Run " << count + 1 << endl;
		pis.printData(count + 1);
		demands[count] = pis.calcDemands(0.3 * simLength + 1, simLength);
		sales[count] = pis.calcSales(0.3 * simLength + 1, simLength);
		orderingCosts[count] = orderingCost * pis.calcOrders(0.3 * simLength + 1, simLength);
		holdingCosts[count] = holdingCost * pis.calcInventories(0.3 * simLength + 1, simLength);
		backlogCosts[count] = backlogCost * pis.calcBacklogs(0.3 * simLength + 1, simLength);
		disposalCosts[count] = disposalCost * pis.calcDisposals(0.3 * simLength + 1, simLength);
		totalCosts[count] = orderingCosts[count] + holdingCosts[count] + backlogCosts[count] + disposalCosts[count];
		totalRevenues[count] = price * sales[count];
		totalProfits[count] = totalRevenues[count] - totalCosts[count];
		cout << "The total demand is " << demands[count] << endl;
		cout << "The total sales is" << sales[count] << endl;
		cout << "The total ordering cost is " << orderingCosts[count] << endl;
		cout << "The total holding cost is " << holdingCosts[count] << endl;
		cout << "The total backlog cost is " << backlogCosts[count] << endl;
		cout << "The total disposal cost is " << disposalCosts[count] << endl;
		cout << "The total cost is " << totalCosts[count] << endl;
		cout << "The total revenue is " << totalRevenues[count] << endl;
		cout << "The total profit is " << totalProfits[count] << endl;
		fout.open("Results.txt", ios::app);
		fout << "Run " << count << endl;
		fout << "Total Demand:" << demands[count] << '\n';
		fout << "Total Sales:" << sales[count] << '\n';
		fout << "Total Ordering Cost:" << orderingCosts[count] << '\n';
		fout << "Total Holding Cost:" << holdingCosts[count] << '\n';
		fout << "Total Backlog Cost:" << backlogCosts[count] << '\n';
		fout << "Total Disposal Cost:" << disposalCosts[count]<< '\n';
		fout << "Total Cost:" << totalCosts[count]<< '\n';
		fout << "Total Revenue:" << totalRevenues[count]<< '\n';
		fout << "Total Profit:" << totalProfits[count]<< '\n';
		fout.close();
		fout.clear();
	}
	cout << "Summary" << endl;
	double aveDemand = calcMean(demands,runs);
	double stdDevDemand = calcStdDev(demands,runs);
	double aveSales = calcMean(sales,runs);
	double stdDevSales = calcStdDev(sales,runs);
	double aveOrderingCost = calcMean(orderingCosts,runs);
	double stdDevOrderingCost = calcStdDev(orderingCosts,runs);
	double aveHoldingCost = calcMean(holdingCosts,runs);
	double stdDevHoldingCost = calcStdDev(holdingCosts,runs);
	double aveBacklogCost = calcMean(backlogCosts,runs);
	double stdDevBacklogCost = calcStdDev(backlogCosts,runs);
	double aveDisposalCost = calcMean(disposalCosts,runs);
	double stdDevDisposalCost = calcStdDev(disposalCosts,runs);
	double aveTotalCost = calcMean(totalCosts,runs);
	double stdDevTotalCost = calcStdDev(totalCosts,runs);
	double aveTotalRevenue = calcMean(totalRevenues,runs);
	double stdDevTotalRevenue = calcStdDev(totalRevenues,runs);
	double aveTotalProfit = calcMean(totalProfits,runs);
	double stdDevTotalProfit = calcStdDev(totalProfits,runs);
	cout << "The average demand is " << aveDemand << endl;
	cout << "The standard deviation of the demands is " << stdDevDemand << endl;
	cout << "The average sales is " << aveSales << endl;
	cout << "The standard deviation of the sales is " << stdDevSales << endl;
	cout << "The average ordering cost is " << aveOrderingCost << endl;
	cout << "The standard deviation of the ordering costs is " << stdDevOrderingCost << endl;
	cout << "The average holding cost is " << aveHoldingCost << endl;
	cout << "The standard deviation of the holdingCosts costs is " << stdDevHoldingCost << endl;
	cout << "The average backlog cost is " << aveBacklogCost << endl;
	cout << "The standard deviation of the backlog costs is " << stdDevBacklogCost << endl;
	cout << "The average disposal cost is " << aveDisposalCost << endl;
	cout << "The standard deviation of the disposal costs is " << stdDevDisposalCost << endl;
	cout << "The average total cost is " << aveTotalCost << endl;
	cout << "The standard deviation of the total costs is " << stdDevTotalCost << endl;
	cout << "The average total revenue is " << aveTotalRevenue << endl;
	cout << "The standard deviation of the total revenues is " << stdDevTotalRevenue << endl;
	cout << "The average total profit is " << aveTotalProfit << endl;
	cout << "The standard deviation of the total profits is " << stdDevTotalProfit << endl;
	fout.open("Results.txt", ios::app);
	fout << "Average Demand:" << aveDemand << '\n';
	fout << "Standard Deviation of Demands:" << stdDevDemand << '\n';
	fout << "Average Sales:" << aveSales << '\n';
	fout << "Standard Deviation of Sales:" << stdDevSales << '\n';
	fout << "Average Ordering Cost:" << aveOrderingCost << '\n';
	fout << "Standard Deviation of Ordering Costs:" << stdDevOrderingCost << '\n';
	fout << "Average Holding Cost:" << aveHoldingCost << '\n';
	fout << "Standard Deviation of Holding Costs:" << stdDevHoldingCost << '\n';
	fout << "Average Backlog Cost:" << aveBacklogCost << '\n';
	fout << "Standard Deviation of Backlog Costs:" << stdDevBacklogCost << '\n';
	fout << "Average Disposal Cost:" << aveDisposalCost << '\n';
	fout << "Standard Deviation of Disposal Costs:" << stdDevDisposalCost << '\n';
	fout << "Average Total Cost:" << aveTotalCost << '\n';
	fout << "Standard Deviation of Total Costs:" << stdDevTotalCost << '\n';
	fout << "Average Total Revenue:" << aveTotalRevenue << '\n';
	fout << "Standard Deviation of Total Revenue:" << stdDevTotalRevenue << '\n';
	fout << "Average Total Profit:" << aveTotalProfit << '\n';
	fout << "Standard Deviation of Total Profit:" << stdDevTotalProfit << '\n';
	fout.close();
	fout.clear();
}