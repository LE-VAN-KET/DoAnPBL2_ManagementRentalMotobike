#pragma once
#include<string.h>
#include "Rental.h"
#include <iostream>
#include <vector>
using namespace std;
class Motobike
{
	char TenXe[50], BienSo[50], Color[50];
	int MaXe;
	double PriceDate;
	bool status; // true-thue roi, false-ranh
	vector<Rental*> rental;
	static int MotobikeID;
public:
	void scan();
	void addRental(Rental*);
	void insert();
	void remove();
	void update();
	void read();
	bool checkAvailableStatus();
	void getAvailableStatus();
	void removeRental(int);
	vector<Rental*>& getListRental();
public:
	Motobike() = default;
	Motobike(char*, char*, char*, double, bool);

	void setMaXe(int);
	int getMaXe();
	void setTenXe(char*);
	char* getTenXe();
	void setBienSo(char*);
	char* getBienSo();
	void setColor(char*);
	char* getColor();
	double getPriceDate();
	void setPriceDate(double);
	void setStatus(bool);
	bool getStatus();
	void setMotobikeID(int);
	int getMotobikeID();
	~Motobike();
};

