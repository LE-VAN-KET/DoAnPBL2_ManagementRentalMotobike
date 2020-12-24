#pragma once
#include <string.h>
#include "Date.h"
class Rental
{
	int MaRental;
	Date RentDay, ReturnDay;
	double ThanhTien;
	bool Status;
	static int RentalID;
public:
	void scan();
	//void add();
	//void remove();
	//void update();
	void read();
	//bool checkAvailableStatus();
	//void getAvailableStatus();
	bool operator==(const Rental&);
public:
	Rental() = default;
	Rental(int, int, int, int, int, int, double, bool);
	void setMaRental(int);
	int getMaRental();
	void setRentDay(int, int, int);
	Date getRentDay();
	void setReturnDay(int, int, int);
	Date getReturnDay();
	void setThanhTien(double);
	double getThanhTien();
	void setStatus(bool);
	bool getStatus();
	void setRentalID(int);
	int getRentalID();
	~Rental();
};

