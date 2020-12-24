#include "Category.h"
#include<conio.h>
#include<string.h>
#include <string> 
#include <iomanip>

int Category::CategoryID = 0;

void Category::setCategoryID(int ID) {
	Category::CategoryID = ID;
}
int Category::getCategoryID() {
	return Category::CategoryID;
}

Category::Category()
{
	this->MaLoaiXe = 0;
	strcpy_s(TenLoaiXe, "");
	this->Number = 0;
}

Category::Category(char* TenLX, int Soluong)
{
	this->MaLoaiXe = 0;
	strcpy_s(this->TenLoaiXe, 50, TenLX);
	this->Number = Soluong;
}

void Category::setMaLoaiXe(int MaLoaiXe) {
	this->MaLoaiXe = MaLoaiXe;
}

int Category::getMaLoaiXe()
{
	return this->MaLoaiXe;
}

void Category::setTenLoaiXe(char* TenLoaiXe) {
	strcpy_s(this->TenLoaiXe, 50, TenLoaiXe);
}

char* Category::getTenLoaiXe()
{
	return this->TenLoaiXe;
}

void Category::setNumber(int Number) {
	this->Number = Number;
}

int Category::getNumber()
{
	return this->Number;
}


Category::~Category()
{
	for (unsigned int i = 0; i < Moto.size(); ++i) {
		Motobike* pMoto = Moto.at(i);
		delete pMoto;
	}
	this->Moto.clear();
}
//=================== NHAP THONG TIN Category Moto =======================
void Category::scan() {
	cout << "\t\t\tTen Loai Xe: ";
	cin.getline(this->TenLoaiXe, 50);
}

void Category::addMotobike(Motobike* moto) {
	this->Moto.push_back(moto);
}

ostream& operator<<(ostream& out, const Category& type) {
	out << "\t" << std::left << setw(20) << type.MaLoaiXe << "|\t" << setw(20) << type.TenLoaiXe << "|\t" << setw(20) << type.Number << endl;
	return out;
}

void Category::readMotobike() {
	for (unsigned int i = 0; i < this->Moto.size(); ++i) {
		this->Moto[i]->read();
	}
}

vector<Motobike*>& Category::getListMoto() {
	return this->Moto;
}

void Category::removeMoto(int pos_moto) {
	//delete Moto[pos_moto];
	for (int i = pos_moto; i < Moto.size() - 1; ++i) {
		Moto[i] = Moto[i + 1];
	}
	Moto.resize(Moto.size() - 1);
}

