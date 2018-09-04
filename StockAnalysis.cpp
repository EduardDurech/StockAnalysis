// I declare that this assignment is my own work and that I have correctly acknowledged the
// work of others.  I acknowledged that I have read and followed the Academic Honesty and
// Integrity related policies as outlined in the syllabus.
//
// _____Eduard Durech____      _____21 July 2018______________

/**
*  @file    lab7.cpp
*  @author  Eduard Durech
*  @date    21 July 2018
*
*  @section ENSC 251 Lab03
*
*  This program will read a .csv file of AAPL stocks 2008-2018, determine the yearly high, low, average,
*  monthly averages, and create a high->low and low->high sorted list
*
*/

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>

using namespace std;


/**
*  @brief The allStocks class holds the information of every stock (date, high)
*/
class allStocks
{
private:
	long double price;
	int month;
	int day;
	int year;
public:
	// Constructors
	allStocks()	
	{
		price = 0.0;
		month = 0;
		day = 0;
		year = 0;
	}
	allStocks( int dclMonth, int dclDay, int dclYear, long double dclPrice )
	{
		price = dclPrice;
		month = dclMonth;
		day = dclDay;
		year = dclYear;
	}

	// Accessors
	double getPrice()
	{
		return price;
	}
	int getMonth()
	{
		return month;
	}
	int getDay()
	{
		return day;
	}
	int getYear()
	{
		return year;
	}

	// Overloaded Operator
	friend bool operator <(const allStocks& leftStock, const allStocks& rightStock)
	{
		return( leftStock.price < rightStock.price );
	}
};

/**
*  @brief The StockYear class holds the high, low, average for each year and an array of monthly averages
*/
class StockYear
{
private:
	long double monthCount[12];
	long double monthTotal[12];
	long double monthAvg[12];

	int monthLow;
	int dayLow;
	int yrLow;
	int monthHigh;
	int dayHigh;
	int yrHigh;

	long double count;
	long double yearTotal;
	long double yearAvg;
	long double yearHigh;
	long double yearLow;
public:
	// Constructors
	StockYear()
	{
		count = 0.0;
		yearTotal = 0.0;
		yearAvg = 0.0;
		yearHigh = 0.0;
		yearLow = 0.0;

		monthLow = 0;
	    dayLow = 0;
		yearLow = 0;
		monthHigh = 0;
		dayHigh = 0;
		yearHigh = 0;

		for( int i = 0; i < 12; i++ )
		{
			monthCount[i] = 0.0;
			monthTotal[i] = 0.0;
			monthAvg[i] = 0.0;
		}
	}

	// When reading in a new stock, adds to total for the month and year and finds if high or low value
	void addStock(int currentMonth, int currentDay, int currentYear, long double currentPrice)
	{
		monthTotal[currentMonth-1] = monthTotal[currentMonth-1] + currentPrice;
		monthCount[currentMonth-1]++;

		if( count == 0 )
		{
			yearLow = currentPrice;

			monthLow = currentMonth;
			dayLow = currentDay;
			yrLow = currentYear;
		}

		yearTotal = yearTotal + currentPrice;
		count++;

		if( currentPrice > yearHigh )
		{
			yearHigh = currentPrice;

			monthHigh = currentMonth;
			dayHigh = currentDay;
			yrHigh = currentYear;
		}
		if(  currentPrice < yearLow )
		{
			yearLow = currentPrice;

			monthLow = currentMonth;
			dayLow = currentDay;
			yrLow = currentYear;
		}
	}

	// Takes average of every month and of year
	void countAvg()
	{
		for( int i = 0; i < 12; i++ )
		{
			if( monthCount[i] != 0 )
			{
				monthAvg[i] = monthTotal[i]/monthCount[i];
			}
			else
			{
				monthCount[i] = 0.0;
			}

			if( count != 0 )
			{
				yearAvg = yearTotal/count;
			}
		}
	}

	// Displays the yearly and monthly information
	void dispInfrmtn()
	{
		cout << " the high, low, and average was: " << endl
			 << yearHigh << "		" << yearLow << "		" << yearAvg << endl 
			 << monthHigh << "/" << dayHigh << "/" << yrHigh << "	"
			 << monthLow << "/" << dayLow << "/" << yrLow << endl <<endl;

		for( int i = 0; i < 12; i++ )
		{
			cout << "	For month " << i + 1 << " the average was " << monthAvg[i] << endl;
		}
	}
};


/**
*  @brief The main function will initialize the containers and read in the file information as well as write
*/
int main()
{
	StockYear years[11];
	vector<allStocks> stocks;
	string waste = "";
	char breaker = '\0';
	int month = 0;
	int day = 0;
	int year = 0;
	long double currentPrice = 0.0;
	ifstream inFile;
	ofstream outFile;

	cout << "Stock Price Records for AAPL 2008-2018 (all dates in MM/DD/YYYY format)" << endl << endl;

	// Open file
	inFile.open("StockPrices.csv");
	getline(inFile, waste, '\n');
	waste = "";

	// Read in file and add, only columns 1 and 3 read in
	do
	{
		inFile >> month >> breaker >> day >> breaker >> year >> breaker;
		getline(inFile, waste, ',');
		inFile >> currentPrice;
		getline(inFile, waste, '\n');

		if( inFile.eof() )
		{
			break;
		}

		years[year-2008].addStock(month, day, year, currentPrice);
		stocks.push_back(allStocks(month, day, year, currentPrice));
	}
	while( ! inFile.eof() );

	// Calculate averages
	for( int i = 0; i < 11; i++ )
	{
		years[i].countAvg();

		cout << endl << "For year " << 2008 + i;
		years[i].dispInfrmtn();
	}

	// Sort vector, create output file, and write to it
	sort( stocks.begin(), stocks.end() );

	outFile.open("LowtoHigh.txt");

	outFile << "Date         High Price" << endl;

	for( allStocks &stock : stocks )
	{
		outFile << stock.getMonth() << "/" << stock.getDay() << "/" << stock.getYear() 
			    << "    " << stock.getPrice() << endl;
	}

	outFile.close();

	reverse( stocks.begin(), stocks.end() );

	outFile.open("HighToLow.txt");

	outFile << "Date              High Price" << endl;

	for( allStocks &stock : stocks )
	{
		outFile << stock.getMonth() << "/" << stock.getDay() << "/" << stock.getYear() 
			    << "    " << stock.getPrice() << endl;
	}
	
	cout << endl << endl << "Done generating HighToLow.txt and LowToHigh.txt !";

	inFile.close();
	outFile.close();

	return 0;
}
