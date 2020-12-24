#include "Motobike.h"
#include <iostream>
#include <iomanip>
using namespace std;

int Motobike::MotobikeID = 0;

Motobike::Motobike(char* tenxe, char* bienso, char* color, double pricedate, bool status) {
	this->MaXe = 0;
	strcpy_s(this->TenXe, 50, tenxe);
	strcpy_s(this->BienSo, 50, bienso);
	strcpy_s(this->Color, 50, color);
	this->PriceDate = pricedate;
	this->status = status;
}

Motobike::~Motobike() {
	for (unsigned int i = 0; i < rental.size(); ++i) {
		Rental* pRental = rental.at(i);
		delete pRental;
	}
	this->rental.clear();
}

void Motobike::scan() {
	cout << "\t\t-> Thong tin Motobike:" << endl;
	cout << "\tTenXe: ";
	cin.getline(this->TenXe, 50);
	cout << "\tBienSo: ";
	cin.getline(this->BienSo, 50);
	cout << "\tColor: ";
	cin.getline(this->Color, 50);
	cout << "\tPrice/date: ";
	cin >> this->PriceDate;
	this->status = false;
	cin.ignore();
}

void Motobike::read() {
	cout << "Ma Xe: " << this->MaXe << endl;
	cout << "Ten Xe: " << this->TenXe << endl;
	cout << "Bien So: " << this->BienSo << endl;
	cout << "Color: " << this->Color << endl;
	cout << "Price/date " << setprecision(3) << fixed << this->PriceDate << endl;
	cout << "Status: " << this->status << endl;
}

void Motobike::setMaXe(int maxe) {
	this->MaXe = maxe;
}
int Motobike::getMaXe() {
	return this->MaXe;
}
void Motobike::setTenXe(char* tenxe) {
	strcpy_s(this->TenXe, 50, tenxe);
}
char* Motobike::getTenXe() {
	return this->TenXe;
}
void Motobike::setBienSo(char* bienso) {
	strcpy_s(this->BienSo, 50, bienso);
}
char* Motobike::getBienSo() {
	return this->BienSo;
}
void Motobike::setColor(char* color) {
	strcpy_s(this->Color, 50, color);
}
char* Motobike::getColor() {
	return this->Color;
}
double Motobike::getPriceDate() {
	return this->PriceDate;
}
bool Motobike::getStatus() {
	return this->status;
}

void Motobike::addRental(Rental* rental) {
	this->rental.push_back(rental);
}

void Motobike::setPriceDate(double Price) {
	this->PriceDate = Price;
}
void Motobike::setStatus(bool status) {
	this->status = status;
}

vector<Rental*>& Motobike::getListRental() {
	return this->rental;
}

void Motobike::removeRental(int pos_rental) {
	for (unsigned int i = pos_rental; i < rental.size() - 1; ++i) {
		this->rental[i] = this->rental[i + 1];
	}
	this->rental.resize(rental.size() - 1);
}

int Motobike::getMotobikeID() {
	return Motobike::MotobikeID;
}

void Motobike::setMotobikeID(int ID) {
	Motobike::MotobikeID = ID;
}