#include <exception>
#include <string>
#include <iomanip>
#include "Date.h"
//  constructor that initializes all data members
Date::Date(int day, int month, int year) : month(month), day(day), year(year)
{
}

//  copy constructor
Date::Date(const Date& date)
{
    this->month = date.month;
    this->day = date.day;
    this->year = date.year;
}

Date::~Date() {
}

//  set the data members of the invoking object
void Date::setDate(int day, int month, int year)
{
    this->month = month;
    this->day = day;
    this->year = year;
}

//  print the invoking object in the format mm/dd/yyyy
void Date::showDate()
{
    cout << setw(15) << this->day << '/' << this->month << '/' << this->year << endl;
}

ostream& operator<<(ostream& out, const Date& date) {
    string result = to_string(date.day) + '/' + to_string(date.month) + '/' + to_string(date.year);
    out << result;
    //out << date.day << '/' << date.month << '/' << date.year ;
    return out;
}

//  check whether two Dates are the same
bool Date::sameDate(const Date& date)
{
    return (this->month == date.month && this->day == date.day && this->year == date.year) ? true : false;
}

void Date::isValid() {
    if (this->month < 0 || this->month > 12)
        throw invalid_argument("Month value is invalid!");
    if (this->dayofmonth(this->month) < this->day)
        throw invalid_argument("Day value is too limited invalid!");
    if (this->day < 1 || this->day > 31)
        throw invalid_argument("Day value is invalid!");
    if (this->year > 2020)
        throw invalid_argument("Year value is invalid!");
}

void Date::setDay(int day) {
    this->day = day;
}
int Date::getDay() {
    return this->day;
}
void Date::setMonth(int month) {
    this->month = month;
}
int Date::getMonth() {
    return this->month;
}
void Date::setYear(int year) {
    this->year = year;
}
int Date::getYear() {
    return this->year;
}

void Date::scan() {
    string date;
    cout << "(dd/mm/yyyy): ";
    getline(cin, date);
    int pos = 0, i = 0;
    string splitdate[3];
    while ((pos = date.find("/")) != string::npos) {
        splitdate[i] = date.substr(0, pos);
        date.erase(0, pos + 1);
        ++i;
    }
    splitdate[i] = date;
    for (int i = 0; i < 3; ++i) {
        if (splitdate[i] == "") throw invalid_argument("Error: day, month and year not null!");
    }
    this->day = atoi(splitdate[0].c_str());
    this->month = atoi(splitdate[1].c_str());
    this->year = atoi(splitdate[2].c_str());
}

int Date::dayofmonth(int month) {
    int day_of_month[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    return day_of_month[month];
}

bool Date::operator==(const Date& d) const {
    return (day == d.day && month == d.month && year == d.year);
}