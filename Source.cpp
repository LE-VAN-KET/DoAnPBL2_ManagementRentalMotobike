
//include the below additional libraries
#include "QL_RentalMoto.h"
#include <iostream>
#include <string.h>
using namespace std;
void menu_Category(QL_RentalMoto*& db) {
	cout << "\t\t1. Add Category Motobike" << endl;
	cout << "\t\t2. Read All Category Motobike" << endl;
	cout << "\t\t3. Update Category Motobike" << endl;
	cout << "\t\t4. Delete Category Motobike" << endl;
	cout << "\t\t5. Search Category Motobike" << endl;
	cout << "\t\t-------------+-------------" << endl;
	int choose;
	bool check = false;
	do {
		cout << "\t=>Choose: ";
		cin >> choose;
		cin.ignore();
		switch (choose)
		{
		case 1:
			db->addCategory();
			check = true;
			break;
		case 2:
			db->readCategory();
			check = true;
			break;
		case 5:
		{
			db->searchCategory();
			check = true;
			break;
		}
		case 3:
		{
			db->updateCategoryMotobike();
			check = true;
			break;
		}
		default:
			cout << "\t\tError: select invalid!" << endl;
			break;
		}
	} while (!check);
}

void menu_Motobikes(QL_RentalMoto*& db) {
	cout << "\t\t1. Add Motobike" << endl;
	cout << "\t\t2. Read All Motobike" << endl;
	cout << "\t\t3. Update Motobike" << endl;
	cout << "\t\t4. Delete Motobike" << endl;
	cout << "\t\t5. Search Motobike" << endl;
	cout << "\t\t-------------+-------------" << endl;
	int choose;
	bool check = false;
	do {
		cout << "\t=>Choose: ";
		cin >> choose;
		cin.ignore();
		switch (choose)
		{
		case 1:
			db->addMotobike();
			check = true;
			break;
		case 2:
			db->readMotobike();
			check = true;
			break;
		case 3:
			db->updateMotobike();
			check = true;
			break;
		case 5:
			db->searchMotobike();
			check = true;
			break;
		default:
			cout << "\t\tError: select invalid!" << endl;
			break;
		}
	} while (!check);
}

void menu_Customers(QL_RentalMoto*& db) {
	cout << "\t\t1. Add Information Customer" << endl;
	cout << "\t\t2. Read All Information Customer" << endl;
	cout << "\t\t3. Update Information Customer" << endl;
	cout << "\t\t4. Delete Customer" << endl;
	cout << "\t\t5. Search Customer" << endl;
	cout << "\t\t-------------+-------------" << endl;
	int choose;
	bool check = false;
	do {
		cout << "\t=>Choose: ";
		cin >> choose;
		cin.ignore();
		switch (choose)
		{
		case 1:
			db->addCustomers();
			check = true;
			break;
		case 2:
			db->readCustomer();
			check = true;
			break;
		case 3:
			db->updateCustomer();
			check = true;
			break;
		case 5:
			db->searchCustomer();
			check = true;
			break;
		case 4:
		{
			db->deleteCustomers();
			check = true;
			break;
		}
		default:
			cout << "\t\tError: select invalid!" << endl;
			break;
		}
	} while (!check);
}

void menu_Rental(QL_RentalMoto*& db) {
	cout << "\t\t1. Add Rental Motobike" << endl;
	cout << "\t\t2. Read All Rental Motobike" << endl;
	cout << "\t\t3. Update Rental Motobike" << endl;
	cout << "\t\t4. Delete Rental Motobike" << endl;
	cout << "\t\t5. Search Rental Motobike" << endl;
	cout << "\t\t-------------+-------------" << endl;
	int choose;
	bool check = false;
	do {
		cout << "\t=>Choose: ";
		cin >> choose;
		cin.ignore();
		switch (choose)
		{
		case 1:
			db->addRental();
			check = true;
			break;
		case 2:
			db->readAllRental();
			check = true;
			break;
		case 3:
			db->updateRental();
			check = true;
			break;
		case 5:
			db->searchRental();
			check = true;
			break;
		default:
			cout << "\t\tError: select invalid!" << endl;
			break;
		}
	} while (!check);
}
void Menu(QL_RentalMoto*& db) {
	int selection;
	bool flash = false;
	do {
		cout << "\t\t========MENU========" << endl;
		cout << "\t\t1. Book Motobike" << endl;
		cout << "\t\t2. Return Motobike" << endl;
		cout << "\t\t3. Categorys Motobikes" << endl;
		cout << "\t\t4. Motobikes" << endl;
		cout << "\t\t5. Customers" << endl;
		cout << "\t\t6. Rentals" << endl;
		cout << "\t\t0. Exit(0)" << endl;
		cout << "\t\t====================" << endl;
		cout << "\t=>Selection: ";
		cin >> selection;
		cin.ignore();
		switch (selection)
		{
		case 1:
			db->BookMotobike();
			break;
		case 2:
			db->ReturnMotobike();
			break;
		case 3:
			menu_Category(db);
			break;
		case 4:
			menu_Motobikes(db);
			break;
		case 5:
			menu_Customers(db);
			break;
		case 6:
			menu_Rental(db);
			break;
		case 0:
			flash = true;
			break;
		default:
			cout << "\tErr: selection invalid !" << endl;
			break;
		}
	} while (!flash);
}

int main()
{
	QL_RentalMoto* database = QL_RentalMoto::getInstance();
	database->init();
	database->selectCategory();
	database->selectCustomers();
	database->selectMotobike();
	database->selectRental();
	Menu(database);
	////database->addCategory();
	////database->updateCategoryMotobike();
	////database->addMotobike();
	////database->searchMotobike();
	//database->readCustomer();
	//database->readAllRental();
	//database->ReturnMotobike();
	//database->readAllRental();
	//database->readMotobike();
	database->close();
	return 0;
}