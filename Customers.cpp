#include "Customers.h"
#include <exception>
#include <string>
#include<iomanip>
int Customers::CustomerID = 0;

Customers::Customers(int day, int month, int year, char* fullName, char* country, char* city,
	char* CMND, char* SDT, char* email, bool gender)
	: birthDay(month, day, year) {
	strcpy_s(this->fullName, 50, fullName);
	strcpy_s(this->country, 50, country);
	strcpy_s(this->city, 50, city);
	strcpy_s(this->CMND, 50, CMND);
	strcpy_s(this->SDT, 50, SDT);
	strcpy_s(this->email, 50, email);
	this->gender = gender;
	this->maKH = 0;
}

Customers::~Customers() {
	for (unsigned int i = 0; i < rental.size(); ++i) {
		Rental* pRental = rental.at(i);
		delete pRental;
	}
	this->rental.clear();
}

void Customers::addRental(Rental* rental) {
	this->rental.push_back(rental);
}

void Customers::scan() {
	bool check = false;
	string GioiTinh;
	do {
		try {
			cout << "\t\t\t\tNhap thong tin Customer" << endl;
			cout << "\t\t\tFull Name: ";
			cin.getline(this->fullName, 50);
			cout << "\t\t\tCountry: ";
			cin.getline(this->country, 50);
			cout << "\t\t\tCity: ";
			cin.getline(this->city, 50);
			cout << "\t\t\tCMND: ";
			cin.getline(this->CMND, 50);
			cout << "\t\t\tSDT: ";
			cin.getline(this->SDT, 50);
			cout << "\t\t\tEmail: ";
			cin.getline(this->email, 50);
			cout << "\t\t\tGender: ";
			getline(cin, GioiTinh);
			cout << "\t\t\tBirthDay";
			this->birthDay.scan();
			this->validateInput(GioiTinh);
			this->birthDay.isValid();
			this->gender = (GioiTinh == "nam") ? 1 : 0;
			check = true;
		}
		catch (invalid_argument& exception) {
			cout << "\t\t\tError: " << exception.what() << endl;
			check = false;
		}
	} while (!check);
}

void Customers::isValidGender(const string& gioitinh) {
	if (!(gioitinh == "nam" || gioitinh == "nu")) throw invalid_argument("Gender must is nam or nu !");
}

void Customers::setMaKH(int makh) {
	this->maKH = makh;
}
int Customers::getMaKH() {
	return this->maKH;
}
void Customers::setFullName(char* fullname) {
	strcpy_s(this->fullName, 50, fullname);
}
char* Customers::getCountry() {
	return this->country;
}
void Customers::setCity(char* city) {
	strcpy_s(this->city, 50, city);
}
char* Customers::getCity() {
	return this->city;
}
void Customers::setCMND(char* CMND) {
	strcpy_s(this->CMND, 50, CMND);
}
char* Customers::getCMND() {
	return this->CMND;
}
void Customers::setSDT(char* SDT) {
	strcpy_s(this->SDT, 50, SDT);
}
char* Customers::getSDT() {
	return this->SDT;
}
void Customers::setEmail(char* email) {
	strcpy_s(this->email, 50, email);
}
char* Customers::getEmail() {
	return this->email;
}
void Customers::setGender(bool gender) {
	this->gender = gender;
}
bool Customers::getGender() {
	return this->gender;
}
void Customers::setBirthday(int day, int month, int year) {
	this->birthDay.setDate(day, month, year);
}
Date Customers::getBirthday() {
	return this->birthDay;
}

char* Customers::getFullName() {
	return this->fullName;
}
void Customers::setCountry(char* country) {
	strcpy_s(this->country, 50, country);
}

ostream& operator<<(ostream& out, const Customers& customer) {
	//out << "\t->Thong tin Customer:" << endl;
	//out << "Ma Khach Hang: " << customer.maKH << endl;
	//out << "Full Name: " << customer.fullName << endl;
	//out << "Country: " << customer.country << endl;
	//out << "City: " << customer.city << endl;
	//out << "CMND: " << customer.CMND << endl;
	//out << "Phone: " << customer.SDT << endl;
	//out << "Address Email: " << customer.email << endl;
	//out << "Gender: " << ((customer.gender) ? "nam" : "nu") << endl;
	//out << "BirthDay: " << customer.birthDay << endl;
	out << std::left << setw(10) << customer.maKH << "|" << setw(25) << customer.fullName << "|" << setw(15) << customer.country
		<< "|" << setw(15) << customer.city << "|" << setw(15) << customer.CMND << "|" << setw(35) << customer.email << "|" << setw(10) << customer.SDT
		<< "|" << setw(10) << ((customer.gender) ? "nam" : "nu") << "|" << customer.birthDay << endl; ;
	cout << "----------+-------------------------+---------------+---------------+---------------+-----------------------------------+------"
		"----+----------+--------------------" << endl;
	return out;
}

vector<Rental*>& Customers::getListRental() {
	return this->rental;
}

bool Customers::isValidCustomer(const Customers& KH) {
	return (this->CMND == KH.CMND && this->birthDay == KH.birthDay && this->gender == KH.gender) ? true : false;
}

int Customers::getCustomerID() {
	return Customers::CustomerID;
}
void Customers::setCustomerID(int ID) {
	Customers::CustomerID = ID;
}

void Customers::validateInput(const string& GioiTinh) {
	if (!(strlen(CMND) == 9 || strlen(CMND) == 12)) throw invalid_argument("CMND must length 9 or 12 number!");
	if (!isValidPhone(SDT)) throw invalid_argument("Phone must is number!");
	if (!isValidEmailAddress(email)) throw invalid_argument("Email must begin character 'a' -> 'z' or 'A' -> 'Z' and '@' must is before the dot !");
	this->isValidGender(GioiTinh);
}

bool Customers::isCharacter(const char Character)
{
	return ((Character >= 'a' && Character <= 'z') || (Character >= 'A' && Character <= 'Z'));
	//Checks if a Character is a Valid A-Z, a-z Character
}

bool Customers::isNumber(const char Character)
{
	return (Character >= '0' && Character <= '9') ? true : false;
}

bool Customers::isValidPhone(const char* phone) {
	for (unsigned int i = 0; i < strlen(phone); ++i) {
		if (!isNumber(phone[i])) return false;
	}
	return true;
}

bool Customers::isValidEmailAddress(const char* EmailAddress)
{
	if (!isCharacter(EmailAddress[0])) // If the First character is not A-Z, a-z
		return 0;
	int AtOffset = -1;
	int DotOffset = -1;
	unsigned int Length = strlen(EmailAddress); // Length = StringLength (strlen) of EmailAddress
	for (unsigned int i = 0; i < Length; i++)
	{
		if (EmailAddress[i] == '@') // If one of the characters is @, store it's position in AtOffset
			AtOffset = (int)i;
		else if (EmailAddress[i] == '.')
			DotOffset = (int)i;
	}
	if (AtOffset == -1 || DotOffset == -1) // If cannot find a Dot or a @
		return 0;
	if (AtOffset > DotOffset) // If the @ is after the Dot
		return 0;
	return !(DotOffset >= ((int)Length - 1)); //Chech there is some other letters after the Dot
}

void Customers::removeRental(int pos_rental) {
	for (unsigned int i = pos_rental; i < this->rental.size() - 1; ++i) {
		this->rental[i] = this->rental[i + 1];
	}
	this->rental.resize(rental.size() - 1);
}