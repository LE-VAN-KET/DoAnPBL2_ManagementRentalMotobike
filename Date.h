#pragma once
#include <iostream>
using namespace std;
class Date
{
	int month;
	int day;
	int year;
public:
	Date(int = 1, int = 1, int = 2000);
	Date(const Date&);
	~Date();
	void setDate(int, int, int);
	void showDate();
	bool sameDate(const Date&);
	void isValid();
	void scan();
	int dayofmonth(int);
	bool operator==(const Date&) const;

	void setDay(int);
	int getDay();
	void setMonth(int);
	int getMonth();
	void setYear(int);
	int getYear();
	friend ostream& operator<<(ostream& out, const Date& date);
};