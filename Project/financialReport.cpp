/*
	Name:    Marco Martinez
	Project: financialReport.cpp
	Purpose: Tax program intended to output financial details.

	IPO Chart
	=================================================================================================
	Variable		Input(s)	Processed	Output(s)
	-------------------------------------------------------------------------------

	lastName(S)		Yes			No			Yes
	firstName(S)	Yes			No			Yes
	payRate(R)		Yes			No			Yes
	hrsWkd	(R)		Yes			No			Yes
	grossPay(R)		No			Yes			Yes
	taxAmt(R)		No			Yes			Yes
	netPay(R)		No			Yes			Yes
	empNum(I)		No			Yes			No
	totPayRate(R)	No			Yes			Yes
	totHrsWkd(R)	No			Yes			Yes
	totGrossPay(R)	No			Yes			Yes
	totTaxAmt(R)	No			Yes			Yes
	totNetPay(R)	No			Yes			Yes
	EMPMAX(IC)		No			Yes			No
	REGULARHOURS(IC)No			Yes			No
	TAXRATE(RC)		No			Yes			No
	OVERTIMERATE(RC)No			Yes			No


	Design Decomposition
	=================================================================================================

	3.0 Payroll
		3.1 PrintReportHeadings (inout myFile as File)
		3.2 InitializeAccumulators (inout totals as Record)
		3.3 InputEmployeeData (inout myEmployees as Array, inout empNum as Integer)
			3.3.1 ValidateString (inout name as String)
			3.3.2 ValidateFloat (inout value as Real)
		3.4 CalculateGross (inout myEmployees as Array, in empNum as Integer)
		3.5 CalculateTax (inout myEmployees as Array, in empNum as Integer)
		3.6 CalculateNetPay (inout myEmployees as Array, in empNum as Integer)
		3.7 Accumulate(in myEmployees as Array, in empNum as Integer, inout totals as Record)
		3.8 SortData (inout myEmployees as Array, in empNum as Integer)
		3.9 PrintReport (in myEmployees as Array, in empNum as Integer, in totals as Record, inout myFile as File)

	Date:    8/30/2018
	
	Software Change Record
	Date            Who                   What
*/

/*---Libraries---*/
#include <iostream>      // Allows cin, cout
#include <iomanip>		 // Allows setw, setprecision, fixed, left, right
#include <string>		 // Allows strings
#include <cctype>        // Allows isalpha
#include <algorithm>     // Allows all_of
#include <fstream>       // Allows for file manipulation

/*---Constant Definitions---*/
#define EMPMAX 1000          // Controls max amount of employees to enter
#define REGULARHOURS 40      // Number of hours in a normal work day
#define TAXRATE 0.15F        // Rate of tax as subject to law
#define OVERTIMERATE 1.5F    // 150% pay amount for overtime work

/*---Structure Definitions---*/
typedef struct employeeRecord {
	std::string	firstName,
				lastName;
	float		payRate,
				hrsWkd,
				grossPay,
				taxAmt,
				netPay;
} employeeRecord;

typedef struct totalRecord {
	float	totPayRate,
			totHrsWkd,
			totGrossPay,
			totTaxAmt,
			totNetPay;
} totalRecord;

/*---Function Declarations---*/
void printReportHeadings(std::ofstream& myFile);
void initializeAccumulators(totalRecord& totals);
void inputEmployeeData(employeeRecord myEmployees[], unsigned short int& empNum);
std::string validateString(std::string name);
float validateFloat(float value);
void calculateGross(employeeRecord myEmployees[], unsigned short int empNum);
void calculateTax(employeeRecord myEmployees[], unsigned short int empNum);
void calculateNet(employeeRecord myEmployees[], unsigned short int empNum);
void accumulate(employeeRecord myEmployees[], unsigned short int empNum, totalRecord& totals);
void sortData(employeeRecord myEmployees[], unsigned short int empNum);
void printReport(employeeRecord myEmployees[], unsigned short int empNum, totalRecord totals, std::ofstream& myFile);

//3.0 Payroll
int main() {
	employeeRecord myEmployees[EMPMAX];
	totalRecord totals;
	std::ofstream myFile("report.txt");
	unsigned short int empNum = 0;

	printReportHeadings(myFile);
	initializeAccumulators(totals);
	inputEmployeeData(myEmployees, empNum);
	calculateGross(myEmployees, empNum);
	calculateTax(myEmployees, empNum);
	calculateNet(myEmployees, empNum);
	accumulate(myEmployees, empNum, totals);
	sortData(myEmployees, empNum);
	printReport(myEmployees, empNum, totals, myFile);
}

	/*---Function Definitions---*/
//3.1 PrintReportHeadings (inout myFile as File)
void printReportHeadings(std::ofstream& myFile) {
	if (myFile.is_open()) {
		myFile << "==================================================================================================" << std::endl;
		myFile << "                                  YOUR FINANCIAL REPORT ANALYSIS                                  " << std::endl;
		myFile << "==================================================================================================" << std::endl;
		myFile << std::endl;
	}
	else {
		std::cout << "Error. File inaccesible." << std::endl;
		std::cout << "Please confirm \"report.txt\" access." << std::endl;
		std::cin.get();
		std::exit(0);
	}
}

//3.2 InitializeAccumulators (inout totals as Record)
void initializeAccumulators(totalRecord& totals) {
	totals.totPayRate = 0.00F;
	totals.totHrsWkd = 0.00F;
	totals.totGrossPay = 0.00F;
	totals.totTaxAmt = 0.00F;
	totals.totNetPay = 0.00F;
}

//3.3 InputEmployeeData (inout myEmployees as Array, inout empNum as Integer)
void inputEmployeeData(employeeRecord myEmployees[], unsigned short int& empNum) {
	char c = 'y';

	while ( c != 'n' && c != 'N') {
		std::cout << "Please enter the employee's first name here: ";
		std::cin >> myEmployees[empNum].firstName;
		myEmployees[empNum].firstName = validateString(myEmployees[empNum].firstName);

		std::cout << "Please enter the employee's last name here: ";
		std::cin >> myEmployees[empNum].lastName;
		myEmployees[empNum].lastName = validateString(myEmployees[empNum].lastName);

		std::cout << "Please enter the employee's hourly pay here: ";
		std::cin >> myEmployees[empNum].payRate;
		myEmployees[empNum].payRate = validateFloat(myEmployees[empNum].payRate);

		std::cout << "Please enter the employee's number of worked hours: ";
		std::cin >> myEmployees[empNum].hrsWkd;
		myEmployees[empNum].hrsWkd = validateFloat(myEmployees[empNum].hrsWkd);

		empNum++;

		if (empNum >= EMPMAX - 1) {
			std::cout << "You have hit the maximum amount of employees to enter." << std::endl;
			return;
		}

		std::cout << "Would you like to continue? (Y/N) ";
		std::cin >> c;

		while (c != 'n' && c != 'N' && c != 'y' && c != 'Y') {
			std::cout << "Invalid input." << std::endl;
			std::cout << "Please enter either a 'y' or 'n': ";
			std::cin >> c;
		}

		std::cout << std::endl;
	}
}

//3.3.1 ValidateString (inout name as String)
//REQUIRES C++11 COMPILER COMPATIBILITY
std::string validateString(std::string name) {
	unsigned short int i = 0;

	while (!(std::all_of(name.begin(), name.end(), [](char c) { return (std::isalpha(c)); }))) {
		std::cout << "Please enter with alphabetical characters only: ";
		std::cin >> name;

		if ((std::all_of(name.begin(), name.end(), [](char c) { return (std::isalpha(c)); })))
			return name;
		else if (i >= 2)
			return name = "Default";

		i++;
	}

	return name;
}

//3.3.2 ValidateFloat (inout value as Real)
float validateFloat(float value) {
	unsigned short int i = 0;

	while (value < 0) {
		std::cout << "Please enter a value greater than or equal to zero: ";
		std::cin >> value;

		if (value >= 0)
			return value;
		else if (i >= 2)
			return value = 0.00F;

		i++;
	}

	return value;
}

//3.4 CalculateGross (inout myEmployees as Array, in empNum as Integer)
void calculateGross(employeeRecord myEmployees[], unsigned short int empNum) {
	for (unsigned short int i = 0; i < empNum; i++) {
		if (myEmployees[i].hrsWkd <= REGULARHOURS) {
			myEmployees[i].grossPay = myEmployees[i].payRate * myEmployees[i].hrsWkd;
		}
		else {
			myEmployees[i].grossPay = REGULARHOURS * myEmployees[i].payRate;
			myEmployees[i].grossPay += (myEmployees[i].hrsWkd - REGULARHOURS) * myEmployees[i].payRate * OVERTIMERATE;
		}
	}
}

//3.5 CalculateTax (inout myEmployees as Array, in empNum as Integer)
void calculateTax(employeeRecord myEmployees[], unsigned short int empNum) {
	for (unsigned short int i = 0; i < empNum; i++) {
		myEmployees[i].taxAmt = myEmployees[i].grossPay * TAXRATE;
	}
}

//3.6 CalculateNetPay (inout myEmployees as Array, in empNum as Integer)
void calculateNet(employeeRecord myEmployees[], unsigned short int empNum) {
	for (unsigned short int i = 0; i < empNum; i++) {
		myEmployees[i].netPay = myEmployees[i].grossPay - myEmployees[i].taxAmt;
	}
}

//3.7 Accumulate(in myEmployees as Array, in empNum as Integer, inout totals as Record)
void accumulate(employeeRecord myEmployees[], unsigned short int empNum, totalRecord& totals) {
	for (unsigned short int i = 0; i < empNum; i++) {
		totals.totPayRate += myEmployees[i].payRate;
		totals.totHrsWkd += myEmployees[i].hrsWkd;
		totals.totGrossPay += myEmployees[i].grossPay;
		totals.totTaxAmt += myEmployees[i].taxAmt;
		totals.totNetPay += myEmployees[i].netPay;
	}
}

//3.8 SortData (inout myEmployees as Array, in empNum as Integer)
void sortData(employeeRecord myEmployees[], unsigned short int empNum) {
	employeeRecord temp;

	for (unsigned short int i = 1; i < empNum; i++)
	{
		for (unsigned short int j = 0; j < empNum - 1; j++)
		{
			if (myEmployees[j].lastName > myEmployees[j + 1].lastName)
			{
				temp = myEmployees[j];
				myEmployees[j] = myEmployees[j + 1];
				myEmployees[j + 1] = temp;
			}
		}
	}
}

//3.9 PrintReport (in myEmployees as Array, in empNum as Integer, in totals as Record, inout myFile as File)
void printReport(employeeRecord myEmployees[], unsigned short int empNum, totalRecord totals, std::ofstream& myFile) {
	if (myFile.is_open()) {
		myFile << std::left << std::setw(38) << "Employee" << std::setw(12) << "        Pay" << std::setw(12) << "      Hours" << std::setw(12) << "      Gross" << std::setw(12) << "      Tax" << std::setw(12) << "       Net" << std::endl;
		myFile << std::left << std::setw(38) << "Name" << std::setw(12) << "        Rate" << std::setw(12) << "      Worked" << std::setw(12) << "      Pay" << std::setw(12) << "      Amount" << std::setw(12) << "       Pay" << std::endl;
		myFile << std::left << std::setw(38) << "========" << std::right << std::setw(12) << "====" << std::setw(12) << "======" << std::setw(12) << "===== " << std::setw(12) << "======" << std::setw(12) << "======" << std::endl;
		myFile << std::endl;

		for (unsigned short int i = 0; i < empNum; i++) {
			myFile << std::left << std::setw(38) << myEmployees[i].lastName + ", " + myEmployees[i].firstName << std::setprecision(2) << std::fixed << std::right
				<< std::setw(12) << myEmployees[i].payRate
				<< std::setw(12) << myEmployees[i].hrsWkd
				<< std::setw(12) << myEmployees[i].grossPay
				<< std::setw(12) << myEmployees[i].taxAmt
				<< std::setw(12) << myEmployees[i].netPay << std::endl;
		}
		myFile << std::endl;

		myFile << std::left << std::setw(38) << "Totals " << std::fixed << std::right
			<< std::setw(12) << totals.totPayRate
			<< std::setw(12) << totals.totHrsWkd
			<< std::setw(12) << totals.totGrossPay
			<< std::setw(12) << totals.totTaxAmt
			<< std::setw(12) << totals.totNetPay << std::endl;

		myFile << std::left << std::setw(38) << "Averages" << std::fixed << std::right
			<< std::setw(12) << totals.totPayRate / empNum
			<< std::setw(12) << totals.totHrsWkd / empNum
			<< std::setw(12) << totals.totGrossPay / empNum
			<< std::setw(12) << totals.totTaxAmt / empNum
			<< std::setw(12) << totals.totNetPay / empNum << std::endl;
		myFile.close();

		std::cout << "Report creation sucessful." << std::endl;
		std::cout << "Thank you for using this program!" << std::endl;
		std::cin.get();
		std::cin.ignore();
	}
	else {
		std::cout << "Error. File inaccesible." << std::endl;
		std::cout << "Please confirm \"report.txt\" access." << std::endl;
		std::cin.get();
		std::cin.ignore();
	}
}
