#include "Rental.h"
#include <iostream>
#include <iomanip>
using namespace std;

int Rental::RentalID = 0;

Rental::Rental(int day1, int month1, int year1, int day2, int month2, int year2, double Thanhtien, bool status)
	: RentDay(day1, month1, year1), ReturnDay(day2, month2, year2)
{
	this->MaRental = 0;
	this->ThanhTien = Thanhtien;
	this->Status = status;
}

void Rental::scan() {
	cout << "\tNgay Thue";
	this->RentDay.scan();
	cout << "\tNgay Tra";
	this->ReturnDay.scan();
	this->ThanhTien = 0;
	this->Status = 0;
}

Rental::~Rental() {
}

void Rental::setMaRental(int maRental) {
	this->MaRental = maRental;
}
int Rental::getMaRental() {
	return this->MaRental;
}
void Rental::setRentDay(int day, int month, int year) {
	this->RentDay.setDate(day, month, year);
}
Date Rental::getRentDay() {
	return this->RentDay;
}
void Rental::setReturnDay(int day, int month, int year) {
	this->ReturnDay.setDate(day, month, year);
}
Date Rental::getReturnDay() {
	return this->ReturnDay;
}
void Rental::setThanhTien(double money) {
	this->ThanhTien = money;
}
double Rental::getThanhTien() {
	return this->ThanhTien;
}
void Rental::setStatus(bool status) {
	this->Status = status;
}
bool Rental::getStatus() {
	return this->Status;
}
void Rental::read() {
	cout << "\t\t->Thong tin Rental\n";
	cout << "Ma Rental: " << this->MaRental << endl;
	cout << "Rent day: ";
	this->RentDay.showDate();
	cout << "Return day: ";
	this->ReturnDay.showDate();
	cout << "Thanh Tien: " << setprecision(3) << fixed << this->ThanhTien << endl;
	cout << "Status: " << this->Status << endl;
}

bool Rental::operator==(const Rental& rental) {
	return (this->MaRental == rental.MaRental && this->RentDay == rental.RentDay && this->ReturnDay == rental.ReturnDay && this->Status == rental.Status && this->ThanhTien == rental.ThanhTien) ? true : false;
}

void Rental::setRentalID(int ID) {
	Rental::RentalID = ID;
}

int Rental::getRentalID() {
	return Rental::RentalID;
}