#pragma once
#include <string.h>
#include "Date.h"
#include "Rental.h"
#include <vector>
#include <iostream>
using namespace std;
class Customers
{
	char fullName[50], country[50], city[50], CMND[50], SDT[50], email[50];
	int maKH;
	bool gender;
	Date birthDay;
	vector<Rental*> rental;
	static int CustomerID;
public:
	void scan();
	void addRental(Rental*);
	void removeRental(int);
	vector<Rental*>& getListRental();
	bool isValidCustomer(const Customers&);
	void validateInput(const string&);
	bool isCharacter(const char);
	bool isNumber(const char);
	bool isValidPhone(const char*);
	bool isValidEmailAddress(const char*);
	void isValidGender(const string&);
	friend ostream& operator<<(ostream&, const Customers&);
public:
	Customers() = default;
	Customers(int, int, int, char*, char*, char*, char*, char*, char*, bool);

	int getCustomerID();
	void setCustomerID(int);
	void setMaKH(int);
	int getMaKH();
	void setFullName(char*);
	char* getFullName();
	void setCountry(char*);
	char* getCountry();
	void setCity(char*);
	char* getCity();
	void setCMND(char*);
	char* getCMND();
	void setSDT(char*);
	char* getSDT();
	void setEmail(char*);
	char* getEmail();
	void setGender(bool);
	bool getGender();
	void setBirthday(int, int, int);
	Date getBirthday();

	~Customers();
};

