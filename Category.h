#pragma once
#include<string.h>
#include <iostream>
#include "Motobike.h"
#include <vector>
using namespace std;
class Category
{
	//protected:
private:
	int MaLoaiXe;
	char TenLoaiXe[50];
	int Number;
	vector<Motobike*> Moto;
	static int CategoryID;
public:
	// add a new object Category
	void scan();
	void addMotobike(Motobike*);
	void readMotobike();
	vector<Motobike*>& getListMoto();
	void removeMoto(int);
	// Update attribute object Category
	void update();
	// Delete object Category
	void remove();
public:
	Category();
	Category(char* TenLX, int = 0);
	~Category();

	void setMaLoaiXe(int);
	int getMaLoaiXe();
	void setTenLoaiXe(char*);
	char* getTenLoaiXe();
	void setNumber(int);
	int getNumber();
	void setCategoryID(int);
	int getCategoryID();

	friend ostream& operator<<(ostream&, const Category&);
};