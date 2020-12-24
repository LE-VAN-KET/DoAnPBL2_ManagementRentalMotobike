#pragma once
#include <iostream>
#include <vector>
#include <SQLAPI.h>
#include <string>
#include "Category.h"
#include "Customers.h"
using namespace std;

class QL_RentalMoto
{
	SAConnection con;
	vector<Category*> TypeMoto;
	vector<Customers*> Customer;
	QL_RentalMoto();
	static QL_RentalMoto* instance;
public:
	static QL_RentalMoto* getInstance();
public:
	void init();
	//friend ostream& operator<<(ostream&, const T*);
	void BookMotobike();
	void ReturnMotobike();
	void addCategory();
	void addMotobike();
	void addCustomers();
	void insertCustomer(Customers*);
	void addRental();
	void insertRental(Rental*, int, int);
	/*
	// Tim kiem ten hang xe, ma loai xe
	// return vi tri i trong list loai xe
	*/
	int searchNameCategory(const string&);
	int searchMaCategory(int);
	void searchCategory();
	int searchCategoryOfMotobike(const char*); // return postion category
	/*
	// Tim kiem ten khach hang, ma khach hang
	// return vi tri i trong list khach hang
	*/
	int searchCustomerByCMNDAndName(const string&, const string&);
	int searchMaCustomer(int);
	int searchCMNDCustomer(const string&);
	int searchSDTCustomer(const string&);
	Customers* searchCustomer(const string& = "search");
	Rental* searchRental(const string & = "search");
	/*
	// Tim kiem ten xe
	// return ma xe;
	*/
	int searchNameMotobike(const string&);
	void selectMotobike();
	void selectCategory();
	void selectCustomers();
	void selectRental();
	void readCategory();
	void readMotobike();
	void readCustomer();
	void readAllRental();
	Motobike* searchMotobike(const string& = "search");
	/*
	// check khach hang exist
	// return true(exist) || false(no exist);
	*/
	bool checkExistCustomers(const Customers&);
	/*UPDATE
	// Update Category Motobikes
	// Update Motobikes
	// Update Customers
	// Update Rentals
	*/
	void updateCategoryMotobike();
	void updateMotobike();
	void updateCustomer();
	void updateRental();
	/*validate*/
	bool isValidEmailSame(const char*); // check same email
	bool isValidCMNDSame(const char*); //check same CMND
	bool isValidSDTSame(const char*); //check same SDT
	bool isValidNameCategorySame(const char*); // check same Name Category
	bool isValidNameMotobikeSame(const char*); // check same name motobike 
	bool isValidBienSoMotobikeSame(const char*);
	/*DELETE*/
	void deleteCustomers();
	void close();
	~QL_RentalMoto();
};
