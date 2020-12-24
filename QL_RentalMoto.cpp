#include "QL_RentalMoto.h"
#include<iomanip>
#include<conio.h>
#include<string.h>
#include <string> 
#include <exception>
QL_RentalMoto *QL_RentalMoto::instance = nullptr;

QL_RentalMoto* QL_RentalMoto::getInstance()
{
	if (instance == nullptr) instance = new QL_RentalMoto();
	return instance;
}

QL_RentalMoto::QL_RentalMoto() {
}

void QL_RentalMoto::init() {
	try {
		con.Connect(_TSA("PC\\SQLEXPRESS@MangementRentalMotobike"), _TSA("SQL_SERVER"), _TSA("1512001"), SA_SQLServer_Client);
	}
	catch (SAException& x) {
		con.Rollback();
		cout << x.ErrText().GetMultiByteChars();
	}
}

void QL_RentalMoto::close() {
	con.Disconnect();
}

void QL_RentalMoto::selectCategory() {
	//if there is a problem executing the query then exit application
	//else display query result
	SACommand select;
	select.setConnection(&con);
	select.setCommandText(_TSA("SELECT * FROM Category"));
	select.Execute();
	while (select.FetchNext()) {
		sa_int64_t  MaLoaiXe = select.Field("MaLoaiXe").asInt64();
		SAString  TenLoaiXe = select.Field("TenLoaiXe").asString();
		int Number = select.Field("Number").asLong();
		Category* typenew = new Category(&string(TenLoaiXe)[0], Number);
		typenew->setCategoryID(int(MaLoaiXe));
		typenew->setMaLoaiXe(int(MaLoaiXe));
		this->TypeMoto.push_back(typenew);
	}
}

QL_RentalMoto::~QL_RentalMoto() {
}

void QL_RentalMoto::addCategory() {
	Category *t = new Category();
	t->scan();
	SACommand cmd;
	cmd.setConnection(&con);
	cmd.setCommandText(_TSA("INSERT INTO Category VALUES(:TenLoaiXe, :Number)"));
	cmd.Param(_TSA("Number")).setAsInt64() = 0;
	cmd.Param(_TSA("TenLoaiXe")).setAsString() = _TSA(t->getTenLoaiXe());
	cmd.Execute();
	t->setCategoryID(t->getCategoryID() + 1);
	t->setMaLoaiXe(t->getCategoryID());
	if (TypeMoto.size() == 0) this->selectCategory();
	else {
		this->TypeMoto.push_back(t);
	}
}

int QL_RentalMoto::searchNameCategory(const string& name) {
	static int err = -1;
	for (unsigned int i = 0; i < TypeMoto.size(); ++i) {
		if (string((this->TypeMoto[i]->getTenLoaiXe())) == name) {
			return i;
		}
	}
	return -1;
}

int QL_RentalMoto::searchCustomerByCMNDAndName(const string& CMND, const string& name) {
	static int err = -1;
	for (unsigned int i = 0; i < Customer.size(); ++i) {
		if (string((this->Customer[i]->getFullName())) == name && string(Customer[i]->getCMND()) == CMND) {
			return i;
		}
	}
	return -1;
}

int QL_RentalMoto::searchNameMotobike(const string& name) {
	static int err = -1;
	for (unsigned int i = 0; i < TypeMoto.size(); ++i) {
		for (unsigned int j = 0; j < TypeMoto[i]->getListMoto().size(); ++j) {
			if (string(TypeMoto[i]->getListMoto()[j]->getTenXe()) == name) {
				return TypeMoto[i]->getListMoto()[j]->getMaXe();
			}
		}
	}
	return err;
}

int QL_RentalMoto::searchMaCategory(int maloaixe) {
	static int err = -1;
	for (unsigned int i = 0; i < TypeMoto.size(); ++i) {
		if (this->TypeMoto[i]->getMaLoaiXe() == maloaixe) {
			return i;
		}
	}
	return -1;
}

void QL_RentalMoto::addMotobike() {
	Motobike *moto = new Motobike();
	bool check = false;
	do {
		try {
			moto->scan();
			if (!isValidNameMotobikeSame(moto->getTenXe())) throw invalid_argument("Error: Ten Xe Exists!");
			if (!isValidBienSoMotobikeSame(moto->getBienSo())) throw invalid_argument("Error: Bien So Exist");
			check = true;
		}
		catch (invalid_argument& exception) {
			cout << "\t" << exception.what() << endl;
		}
	} while (!check);
	string nameType, MaCategory;
	bool flash = false;
	int pos = -1;
	do {
		try {
			cout << "\tTen Loai Xe: ";
			getline(cin, nameType);
			pos = this->searchNameCategory(&nameType[0]);
			if (pos == -1) throw invalid_argument("Error: Ten Loai Xe Not Exists!");
			flash = true;
		}
		catch (invalid_argument& exception) {
			cout << "\t" << exception.what() << endl;
		}
	} while (!flash);
	//cmd.setConnection(&con);
	//cmd.setCommandText(_TSA("SELECT MaLoaiXe FROM Category WHERE TenLoaiXe = :TenLoaiXe"));
	//cmd.Param(_TSA("TenLoaiXe")).setAsString() = _TSA(&nameType[0]);
	//cmd.Execute();
	//while (cmd.FetchNext()) {
	//	SAString  MaLoaiXe = cmd.Field("MaLoaiXe").asString();
	//	MaCategory = string(MaLoaiXe);
	//	cout << MaCategory << endl;
	//}
	SACommand insert;
	insert.setConnection(&con);
	insert.setCommandText(_TSA("INSERT INTO Motobike VALUES(:TenXe, :BienSo, :Color, :MaLoaiXe, :PriceDate, :TinhTrang)"));
	insert.Param(_TSA("TenXe")).setAsString() = _TSA(moto->getTenXe());
	insert.Param(_TSA("BienSo")).setAsString() = _TSA(moto->getBienSo());
	insert.Param(_TSA("Color")).setAsString() = _TSA(moto->getColor());
	insert.Param(_TSA("MaLoaiXe")).setAsInt64() = this->TypeMoto[pos]->getMaLoaiXe();
	insert.Param(_TSA("PriceDate")).setAsDouble() = moto->getPriceDate();
	insert.Param(_TSA("TinhTrang")).setAsBool() = moto->getStatus();
	insert.Execute();
	SACommand update;
	update.setConnection(&con);
	update.setCommandText(_TSA("UPDATE Category SET Number = Number + 1 WHERE MaLoaiXe = :MaLoaiXe"));
	update.Param(_TSA("MaLoaiXe")).setAsInt64() = this->TypeMoto[pos]->getMaLoaiXe();
	update.Execute();
	if (moto->getMotobikeID() == 0) this->selectMotobike();
	else this->TypeMoto[pos]->addMotobike(moto);
	moto->setMotobikeID(moto->getMotobikeID() + 1);
	moto->setMaXe(moto->getMotobikeID());
	this->TypeMoto[pos]->setNumber(TypeMoto[pos]->getNumber() + 1);
}

void QL_RentalMoto::selectMotobike() {
	SACommand cmd;
	cmd.setConnection(&con);
	cmd.setCommandText(_TSA("SELECT * FROM Motobike"));
	cmd.Execute();
	while (cmd.FetchNext()) {
		sa_int64_t  MaXe = cmd.Field("MaXe").asInt64();
		SAString  TenXe = cmd.Field("TenXe").asString();
		SAString  BienSo = cmd.Field("BienSo").asString();
		SAString  Color = cmd.Field("Color").asString();
		double PriceDate = cmd.Field("PriceDay").asDouble();
		bool Status = cmd.Field("TinhTrang").asBool();
		sa_int64_t MaLoaiXe = cmd.Field("MaLoaiXe").asInt64();
		int pos = this->searchMaCategory(int(MaLoaiXe));
		Motobike* moto = new Motobike(&string(TenXe)[0], &string(BienSo)[0], &string(Color)[0], PriceDate, Status);
		moto->setMotobikeID(int(MaXe));
		moto->setMaXe(int(MaXe));
		this->TypeMoto[pos]->addMotobike(moto);
	}
}

void QL_RentalMoto::readCategory() {
	cout << "\t" << std::left << setw(20) <<"Ma Loai Xe"<< "|\t" << setw(20) <<"Ten Loai Xe" << "|\t" << setw(20) << "Number" << endl;
	cout << std::right << setw(10) << "-" << "------------------+-----------------------+----------" << endl;
	for (unsigned int i = 0; i < this->TypeMoto.size(); ++i) {
		cout << *(this->TypeMoto[i]);
	}
}

void QL_RentalMoto::readMotobike() {
	for (unsigned int i = 0; i < this->TypeMoto.size(); ++i) {
		this->TypeMoto[i]->readMotobike();
	}
}

void QL_RentalMoto::insertCustomer(Customers* KhachHang) {
	SACommand cmd;
	SADateTime date(KhachHang->getBirthday().getYear(), KhachHang->getBirthday().getMonth(), KhachHang->getBirthday().getDay());
	cmd.setConnection(&con);
	cmd.setCommandText(_TSA("INSERT INTO Customers VALUES(:FullName, :Country, :City, :CMND, :SDT, :Email, :Gender, :BirthDay)"));
	cmd.Param(_TSA("FullName")).setAsString() = _TSA(KhachHang->getFullName());
	cmd.Param(_TSA("Country")).setAsString() = _TSA(KhachHang->getCountry());
	cmd.Param(_TSA("City")).setAsString() = _TSA(KhachHang->getCity());
	cmd.Param(_TSA("CMND")).setAsString() = _TSA(KhachHang->getCMND());
	cmd.Param(_TSA("SDT")).setAsString() = _TSA(KhachHang->getSDT());
	cmd.Param(_TSA("Email")).setAsString() = _TSA(KhachHang->getEmail());
	cmd.Param(_TSA("Gender")).setAsBool() = KhachHang->getGender();
	cmd.Param(_TSA("BirthDay")).setAsDateTime() = date;
	cmd.Execute();
	KhachHang->setCustomerID(KhachHang->getCustomerID() + 1);
	KhachHang->setMaKH(KhachHang->getCustomerID());
	if (Customer.size() == 0) this->selectCustomers();
	else this->Customer.push_back(KhachHang);
}

void QL_RentalMoto::addCustomers() {
	Customers* customer = new Customers();
	bool check = false;
	do {
		try {
			customer->scan();
			if (!isValidCMNDSame(customer->getCMND())) throw invalid_argument("Note: CMND really exists!");
			if (!isValidSDTSame(customer->getSDT())) throw invalid_argument("Note: SDT really exists");
			if (!isValidEmailSame(customer->getEmail())) throw invalid_argument("Note: Email really exists");
			check = true;
		}
		catch (invalid_argument& exception) {
			cout << "\t" << exception.what() << endl;
		}
	} while (!check);
	this->insertCustomer(customer);
}

void QL_RentalMoto::selectCustomers() {
	SACommand select;
	select.setConnection(&con);
	select.setCommandText(_TSA("SELECT * FROM Customers"));
	select.Execute();
	while (select.FetchNext()) {
		sa_int64_t  MaKH = select.Field("MaKH").asInt64();
		SAString  FullName = select.Field("FullName").asString();
		SAString  Country = select.Field("Country").asString();
		SAString  City = select.Field("City").asString();
		SAString  CMND = select.Field("CMND").asString();
		SAString  SDT = select.Field("SDT").asString();
		SAString  Email = select.Field("Email").asString();
		bool  Gender = select.Field("Gender").asBool();
		SADateTime BirthDay = select.Field("BirthDay").asDateTime();
		Customers* customerNew = new Customers(BirthDay.GetMonth(), BirthDay.GetDay(), BirthDay.GetYear(),
			&string(FullName)[0], &string(Country)[0], &string(City)[0], &string(CMND)[0],
			&string(SDT)[0], &string(Email)[0], Gender);
		/*cout << "Test: " <<  << endl;*/
		customerNew->setCustomerID(int(MaKH));
		customerNew->setMaKH(int(MaKH));
		this->Customer.push_back(customerNew);
	}
}

void QL_RentalMoto::readCustomer() {
	for (unsigned int i = 0; i < Customer.size(); ++i) {
		cout << *(this->Customer[i]);
	}
}

void QL_RentalMoto::insertRental(Rental* rental, int ma_motobike, int pos_customer) {
	SACommand cmd;
	SADateTime Rent_date(rental->getRentDay().getYear(), rental->getRentDay().getMonth(), rental->getRentDay().getDay());
	SADateTime Return_date(rental->getReturnDay().getYear(), rental->getReturnDay().getMonth(), rental->getReturnDay().getDay());
	cmd.setConnection(&con);
	cmd.setCommandText(_TSA("INSERT INTO Rental VALUES(:MaXe, :MaKH, :Rent_date, :Return_date, :ThanhTien, :TinhTrang)"));
	cmd.Param(_TSA("MaXe")).setAsInt64() = ma_motobike;
	cmd.Param(_TSA("MaKH")).setAsInt64() = this->Customer[pos_customer]->getMaKH();
	cmd.Param(_TSA("Rent_date")).setAsDateTime() = Rent_date;
	cmd.Param(_TSA("Return_date")).setAsDateTime() = Return_date;
	cmd.Param(_TSA("ThanhTien")).setAsDouble() = rental->getThanhTien();
	cmd.Param(_TSA("TinhTrang")).setAsBool() = rental->getStatus();
	cmd.Execute();
	SACommand update;
	update.setConnection(&con);
	update.setCommandText(_TSA("UPDATE Motobike SET TinhTrang = :TinhTrang WHERE MaXe = :MaXe"));
	update.Param(_TSA("MaXe")).setAsInt64() = ma_motobike;
	update.Param(_TSA("TinhTrang")).setAsBool() = true;
	update.Execute();
	if (rental->getRentalID() == 0) this->selectRental();
	else {
		rental->setRentalID(rental->getRentalID() + 1);
		rental->setMaRental(rental->getRentalID());
		this->Customer[pos_customer]->addRental(rental);
		for (unsigned int i = 0; i < TypeMoto.size(); ++i) {
			for (unsigned int j = 0; j < TypeMoto[i]->getListMoto().size(); ++j) {
				if (TypeMoto[i]->getListMoto()[j]->getMaXe() == ma_motobike) {
					this->TypeMoto[i]->getListMoto()[j]->addRental(rental);
					this->TypeMoto[i]->getListMoto()[j]->setStatus(true); // update status motobike
				}
			}
		}
	}
}

void QL_RentalMoto::addRental() {
	Rental* rental = new Rental();
	string nameKH, nameMotobike, CMND;
	bool check = false;
	int pos_customer = -1, ma_motobike = -1;
	do {
		try {
			cout << "\tFull Name Khach Hang: ";
			getline(cin, nameKH);
			cout << "\tCMND Khach Hang: ";
			getline(cin, CMND);
			cout << "\tTen Xe: ";
			getline(cin, nameMotobike);
			pos_customer = this->searchCustomerByCMNDAndName(CMND, nameKH);
			if (pos_customer == -1) throw invalid_argument("Note: Customer not exists!");
			ma_motobike = this->searchNameMotobike(nameMotobike);
			if (ma_motobike == -1) throw invalid_argument("Note: Ten Xe not exist!");
			rental->scan();
			check = true;
		}
		catch (invalid_argument& exception) {
			cout << "\t" << exception.what() << endl;
		}
	} while (!check);
	this->insertRental(rental, ma_motobike, pos_customer);
}

void QL_RentalMoto::selectRental() {
	SACommand select;
	select.setConnection(&con);
	select.setCommandText(_TSA("SELECT * FROM Rental"));
	select.Execute();
	while (select.FetchNext()) {
		sa_int64_t MaRental = select.Field("MaRental").asInt64();
		sa_int64_t MaXe = select.Field("MaXe").asInt64();
		sa_int64_t MaKH = select.Field("MaKH").asInt64();
		SADateTime Rent_date = select.Field("Rent_date").asDateTime();
		SADateTime Return_date = select.Field("Return_date").asDateTime();
		double ThanhTien = select.Field("ThanhTien").asDouble();
		bool Status = select.Field("TinhTrang").asBool();
		Rental* rentalNew = new Rental(Rent_date.GetDay(), Rent_date.GetMonth(), Rent_date.GetYear(),
			Return_date.GetDay(), Return_date.GetMonth(), Return_date.GetYear(), ThanhTien, Status);
		int pos_customer = this->searchMaCustomer(int(MaKH));
		rentalNew->setRentalID(int(MaRental));
		rentalNew->setMaRental(int(MaRental));
		this->Customer[pos_customer]->addRental(rentalNew);
		for (unsigned int i = 0; i < TypeMoto.size(); ++i) {
			for (unsigned int j = 0; j < TypeMoto[i]->getListMoto().size(); ++j) {
				if (TypeMoto[i]->getListMoto()[j]->getMaXe() == int(MaXe)) {
					this->TypeMoto[i]->getListMoto()[j]->addRental(rentalNew);
				}
			}
		}
	}
}

int QL_RentalMoto::searchMaCustomer(int maCustomer) {
	static int err = -1;
	for (unsigned int i = 0; i < Customer.size(); ++i) {
		if (Customer[i]->getMaKH() == maCustomer) {
			return i;
		}
	}
	return err;
}

void QL_RentalMoto::readAllRental() {
	for (unsigned int i = 0; i < Customer.size(); ++i) {
		for (unsigned int j = 0; j < Customer[i]->getListRental().size(); ++j) {
			Customer[i]->getListRental()[j]->read();
		}
	}
}

Motobike* QL_RentalMoto::searchMotobike(const string& method) {
	cout << "\t\t\tTim kiem Motobike\n";
	cout << "\t\t1. Tim kiem Ma Motobike\n";
	cout << "\t\t2. Tim kiem Ten Motobike\n";
	cout << "\t\t3. Tim kiem Bien So Motobike\n";
	if (method == "search") {
		cout << "\t\t4. Tim kiem Color Motobike\n";
		cout << "\t\t5. Tim kiem Price one day Motobike\n";
		cout << "\t\t6. Tim kiem status Motobike\n";
	}
	cout << "\t\t================================\n";
	int selection;
	bool check = false;
	do {
		cout << "\t=>selection: ";
		cin >> selection;
		cin.ignore();
		switch (selection)
		{
		case 1:
		{
			int maMotobike;
			cout << "\tMa Xe: ";
			cin >> maMotobike; cin.ignore();
			for (unsigned int i = 0; i < TypeMoto.size(); ++i) {
				for (unsigned int j = 0; j < TypeMoto[i]->getListMoto().size(); ++j) {
					if (TypeMoto[i]->getListMoto()[j]->getMaXe() == maMotobike) {
						return (TypeMoto[i]->getListMoto()[j]);
					}
				}
			}
			cout << "\tError: Ma Motobike Not Exists!" << endl;
			check = true;
			break;
		}
		case 2:
		{
			string TenXe;
			cout << "\tTen Xe: ";
			getline(cin, TenXe);
			for (unsigned int i = 0; i < TypeMoto.size(); ++i) {
				for (unsigned int j = 0; j < TypeMoto[i]->getListMoto().size(); ++j) {
					if (string(TypeMoto[i]->getListMoto()[j]->getTenXe()) == TenXe) {
						return TypeMoto[i]->getListMoto()[j];
					}
				}
			}
			cout << "\tError: Ten Xe Not Exist!" << endl;
			check = true;
			break;
		}
		case 3:
		{
			string BienSo;
			cout << "\tBien So: ";
			getline(cin, BienSo);
			for (unsigned int i = 0; i < TypeMoto.size(); ++i) {
				for (unsigned int j = 0; j < TypeMoto[i]->getListMoto().size(); ++j) {
					if (string(TypeMoto[i]->getListMoto()[j]->getBienSo()) == BienSo) {
						return TypeMoto[i]->getListMoto()[j];
					}
				}
			}
			cout << "\tError: Bien So Not Exist!" << endl;
			check = true;
			break;
		}
		case 4:
		{
			if (method != "search") {
				cout << "\tError: seclection invalid!" << endl;
				break;
			}
			string Color;
			cout << "\tColor: ";
			getline(cin, Color);
			for (unsigned int i = 0; i < TypeMoto.size(); ++i) {
				for (unsigned int j = 0; j < TypeMoto[i]->getListMoto().size(); ++j) {
					if (string(TypeMoto[i]->getListMoto()[j]->getColor()) == Color) TypeMoto[i]->getListMoto()[j]->read();
				}
			}
			check = true;
			break;
		}
		case 5:
		{
			if (method != "search") {
				cout << "\tError: seclection invalid!" << endl;
				break;
			}
			double PriceDate;
			cout << "\tPrice/date: ";
			cin >> PriceDate; cin.ignore();
			for (unsigned int i = 0; i < TypeMoto.size(); ++i) {
				for (unsigned int j = 0; j < TypeMoto[i]->getListMoto().size(); ++j) {
					if (TypeMoto[i]->getListMoto()[j]->getPriceDate() == PriceDate) TypeMoto[i]->getListMoto()[j]->read();
				}
			}
			check = true;
			break;
		}
		case 6:
		{
			if (method != "search") {
				cout << "\tError: seclection invalid!" << endl;
				break;
			}
			bool Status, flash = false;
			string TinhTrang;
			do {
				try {
					cout << "\tTinh Trang: ";
					getline(cin, TinhTrang);
					if (!(TinhTrang == "ranh" || TinhTrang == "ban")) throw invalid_argument("Tinh Trang must is ranh or ban");
					Status = (TinhTrang == "ranh") ? 0 : 1;
				}
				catch (invalid_argument& exception) {
					cout << "\t" << exception.what() << endl;
				}
			} while (!flash);
			for (unsigned int i = 0; i < TypeMoto.size(); ++i) {
				for (unsigned int j = 0; j < TypeMoto[i]->getListMoto().size(); ++j) {
					if (TypeMoto[i]->getListMoto()[j]->getStatus() == Status) TypeMoto[i]->getListMoto()[j]->read();
				}
			}
			check = true;
			break;
		}
		default:
			cout << "\tError: seclection invalid!" << endl;
			break;
		}
	} while (!check);
	return NULL;
}

void QL_RentalMoto::BookMotobike() {
	Customers* KH = new Customers();
	KH->scan();
	if (this->checkExistCustomers(*KH)) this->insertCustomer(KH);
	KH = Customer[this->searchCustomerByCMNDAndName(string(KH->getCMND()), string(KH->getFullName()))];
	string TenHangXe, TenXe;
	int MaXe = -1;
	bool success = false;
	do {
		try {
			cout << "\tTen Loai Xe: ";
			getline(cin, TenHangXe);
			cout << "\tTen Xe: ";
			getline(cin, TenXe);
			if (isValidNameCategorySame(TenHangXe.c_str())) throw invalid_argument("Error: Ten Loai Xe Not Exist!");
			if (isValidNameMotobikeSame(TenXe.c_str())) throw invalid_argument("Error: Ten Xe Not Exist!");
			int pos_Category = this->searchNameCategory(TenHangXe);
			bool message = false;
			for (unsigned int i = 0; i < TypeMoto[pos_Category]->getListMoto().size(); ++i) {
				if (string(TypeMoto[pos_Category]->getListMoto()[i]->getTenXe()) == TenXe) {
					MaXe = TypeMoto[pos_Category]->getListMoto()[i]->getMaXe();
					if (TypeMoto[pos_Category]->getListMoto()[i]->getStatus() == true)  throw invalid_argument("Error: Motobike has been rented!");
					message = true;
					break;
				}
			}
			if (!message) throw invalid_argument("Error: Motobike not exist!");
			success = true;
		}
		catch (invalid_argument& exception) {
			cout << "\t" << exception.what() << endl;
		}
	} while (!success);
	Rental* newRental = new Rental();
	newRental->scan();
	int pos_customer = this->searchCustomerByCMNDAndName(string(KH->getCMND()), string(KH->getFullName()));
	this->insertRental(newRental, MaXe, pos_customer);
}

bool QL_RentalMoto::checkExistCustomers(const Customers& KH) {
	for (unsigned int i = 0; i < Customer.size(); ++i) {
		if (Customer[i]->isValidCustomer(KH)) return true;
	}
	return false;
}

void QL_RentalMoto::updateCategoryMotobike() {
	cout << "\tUPDATE CATEGORY MOTOBIKE" << endl;
	cout << "\t--------------------------" << endl;
	cout << "\t|1. Update Ten Loai Xe   |" << endl;
	cout << "\t--------------------------" << endl;
	int choose;
	bool check = false;
	do {
		cout << "\t=>Choose: ";
		cin >> choose;
		cin.ignore();
		switch (choose)
		{
		case 1:
		{
			string TenLoaiXeOld, TenLoaiXeNew;
			bool flasg = false;
			do {
				try {
					cout << "\tTen Loai Xe Old: ";
					getline(cin, TenLoaiXeOld);
					int pos_Category = this->searchNameCategory(TenLoaiXeOld);
					if (pos_Category == -1) throw invalid_argument("Note: Ten Loai Xe Not Exist!");
					cout << "\tTen Loai Xe New: ";
					getline(cin, TenLoaiXeNew);
					if (!isValidNameCategorySame(TenLoaiXeNew.c_str())) throw invalid_argument("Note: Ten Loai Xe Trung Ten Loai Xe Old!");
					TypeMoto[pos_Category]->setTenLoaiXe(&TenLoaiXeNew[0]);
					flasg = true;
				}
				catch (invalid_argument& exception) {
					cout << "\t" << exception.what() << endl;
				}
			} while (!flasg);
			SACommand update;
			update.setConnection(&con);
			update.setCommandText(_TSA("UPDATE Category SET TenLoaiXe = :TenLoaiXeNew WHERE TenLoaiXe = :TenLoaiXeOld"));
			update.Param(_TSA("TenLoaiXeNew")).setAsString() = _TSA(&TenLoaiXeNew[0]);
			update.Param(_TSA("TenLoaiXeOld")).setAsString() = _TSA(&TenLoaiXeOld[0]);
			update.Execute();
			check = true;
			break;
		}
		default:
			cout << "\tError: You Choose invalid!" << endl;
			break;
		}
	} while (!check);
}

void QL_RentalMoto::updateMotobike() {
	Motobike* moto = this->searchMotobike("update");
	if (moto == NULL) return;
	int choose;
	bool check = false;
	do {
		cout << "\tUPDATE MOTOBIKE\n";
		cout << "\t1. Update Ten Xe" << endl;
		cout << "\t2. Update Bien So" << endl;
		cout << "\t3. Update Color" << endl;
		cout << "\t4. Update Price rent" << endl;
		cout << "\t5. Update Tinh Trang" << endl;
		cout << "\t6. Update Loai Xe" << endl;
		cout << "\t0. Exit(0)" << endl;
		cout << "\t=>Choose: ";
		cin >> choose;
		cin.ignore();
		switch (choose)
		{
		case 1:
		{
			string TenXe;
			cout << "\tNew Ten Xe: ";
			getline(cin, TenXe);
			SACommand update;
			update.setConnection(&con);
			update.setCommandText(_TSA("UPDATE Motobike SET TenXe = :TenXeNew WHERE TenXe = :TenXeOld AND MaXe = :MaXe"));
			update.Param(_TSA("TenXeOld")).setAsString() = _TSA(moto->getTenXe());
			update.Param(_TSA("MaXe")).setAsInt64() = moto->getMaXe();
			update.Param(_TSA("TenXeNew")).setAsString() = _TSA(&TenXe[0]);
			update.Execute();
			moto->setTenXe(&TenXe[0]);
			//check = true;
			break;
		}
		case 2:
		{
			string BienSo;
			cout << "\tNew Bien So: ";
			getline(cin, BienSo);
			SACommand update;
			update.setConnection(&con);
			update.setCommandText(_TSA("UPDATE Motobike SET BienSo = :BienSoNew WHERE BienSo = :BienSoOld AND MaXe = :MaXe"));
			update.Param(_TSA("BienSoNew")).setAsString() = _TSA(&BienSo[0]);
			update.Param(_TSA("MaXe")).setAsInt64() = moto->getMaXe();
			update.Param(_TSA("BienSoOld")).setAsString() = _TSA(moto->getBienSo());
			update.Execute();
			moto->setBienSo(&BienSo[0]);
			//check = true;
			break;
		}
		case 3:
		{
			string Color;
			cout << "\tNew Color: ";
			getline(cin, Color);
			SACommand update;
			update.setConnection(&con);
			update.setCommandText(_TSA("UPDATE Motobike SET Color = :ColorNew WHERE Color = :ColorOld AND MaXe = :MaXe"));
			update.Param(_TSA("ColorNew")).setAsString() = _TSA(&Color[0]);
			update.Param(_TSA("MaXe")).setAsInt64() = moto->getMaXe();
			update.Param(_TSA("ColorOld")).setAsString() = _TSA(moto->getColor());
			update.Execute();
			moto->setColor(&Color[0]);
			//check = true;
			break;
		}
		case 4:
		{
			double PriceRent;
			cout << "\tNew Price/Date: ";
			cin >> PriceRent;
			cin.ignore();
			SACommand update;
			update.setConnection(&con);
			update.setCommandText(_TSA("UPDATE Motobike SET PriceDay = :PriceDateNew WHERE PriceDay = :PriceDateOld AND MaXe = :MaXe"));
			update.Param(_TSA("PriceDateNew")).setAsDouble() = PriceRent;
			update.Param(_TSA("MaXe")).setAsInt64() = moto->getMaXe();
			update.Param(_TSA("PriceDateOld")).setAsDouble() = moto->getPriceDate();
			update.Execute();
			moto->setPriceDate(PriceRent);
			//check = true;
			break;
		}
		case 5:
		{
			bool StastusNew;
			cout << "\tNew Status: ";
			cin >> StastusNew;
			cin.ignore();
			SACommand update;
			update.setConnection(&con);
			update.setCommandText(_TSA("UPDATE Motobike SET TinhTrang = :StatusNew WHERE TinhTrang = :StatusOld AND MaXe = :MaXe"));
			update.Param(_TSA("StatusNew")).setAsBool() = StastusNew;
			update.Param(_TSA("MaXe")).setAsInt64() = moto->getMaXe();
			update.Param(_TSA("StatusOld")).setAsBool() = moto->getStatus();
			update.Execute();
			moto->setStatus(StastusNew);
			//check = true;
			break;
		}
		case 6:
		{
			string TenLoaiXeNew;
			bool success = false;
			int pos_Category = -1;
			do {
				try {
					cout << "Ten Loai Xe Moto: ";
					getline(cin, TenLoaiXeNew);
					pos_Category = this->searchNameCategory(TenLoaiXeNew); // validate pos
					if (pos_Category == -1)  throw invalid_argument("Note: Ten Loai Xe Not Exists!");
					success = true;
				}
				catch (invalid_argument& exception) {
					cout << "\t" << exception.what() << endl;
				}
			} while (!success);
			SACommand update;
			update.setConnection(&con);
			update.setCommandText(_TSA("UPDATE Motobike SET MaLoaiXe = :MaLoaiXeNew WHERE MaXe = :MaXe"));
			update.Param(_TSA("MaLoaiXeNew")).setAsInt64() = TypeMoto[pos_Category]->getMaLoaiXe();
			update.Param(_TSA("MaXe")).setAsInt64() = moto->getMaXe();
			update.Execute();
			for (unsigned int i = 0; i < TypeMoto.size(); ++i) {
				for (unsigned int j = 0; j < TypeMoto[i]->getListMoto().size(); ++j) {
					if (TypeMoto[i]->getListMoto()[j]->getMaXe() == moto->getMaXe()) {
						TypeMoto[pos_Category]->addMotobike(moto);
						TypeMoto[i]->removeMoto(j);
						return;
					}
				}
			}
			break;
		}
		case 0:
			check = true;
			break;
		default:
			cout << "\tError: You choose invalid !" << endl;
			break;
		}
	} while (!check);
}

int QL_RentalMoto::searchCMNDCustomer(const string& CMND) {
	static int err = -1;
	for (unsigned int i = 0; i < Customer.size(); ++i) {
		if (string(Customer[i]->getCMND()) == CMND) {
			return i;
		}
	}
	return err;
}

int QL_RentalMoto::searchSDTCustomer(const string& SDT) {
	static int err = -1;
	for (unsigned int i = 0; i < Customer.size(); ++i) {
		if (string(Customer[i]->getSDT()) == SDT) {
			return i;
		}
	}
	return err;
}

Customers* QL_RentalMoto::searchCustomer(const string& method) {
	cout << "\tSEACRH CUSTOMER" << endl;
	cout << "\t1. Tim Kiem Ma Khach Hang" << endl;
	cout << "\t2. Tim Kiem CMND Khach Hang" << endl;
	cout << "\t3. Tim Kiem SDT Khach Hang" << endl;
	if (method == "search") {
		cout << "\t4. Tim Kiem Ten Khach Hang" << endl;
		cout << "\t5. Tim Kiem Country Khach Hang" << endl;
		cout << "\t6. Tim Kiem City Khach Hang" << endl;
		cout << "\t7. Tim Kiem Gender Khach Hang" << endl;
		cout << "\t8. Tim kiem BirthDay Khach Hang" << endl;
	}
	int choose;
	bool flash = false;
	do {
		cout << "\t=>Choose: ";
		cin >> choose;
		cin.ignore();
		switch (choose)
		{
		case 1:
		{
			int maKH;
			cout << "\tSearch Ma Khach Hang: ";
			cin >> maKH;
			cin.ignore();
			try {
				int pos_maKH = this->searchMaCustomer(maKH);
				if (pos_maKH < 0) throw invalid_argument("Note: Ma Khach Hang Not Exist!");
				cout << *Customer[pos_maKH]; // validate pos
				flash = true;
				return Customer[pos_maKH];
			}
			catch (invalid_argument& exception) {
				cout << "\t" << exception.what() << endl;
			}
			break;
		}
		case 4:
		{
			if (method != "search") {
				cout << "\tError: You choose invalid !" << endl;
				break;
			}
			string fullnameKH;
			cout << "\tSearch Name Khach Hang: ";
			getline(cin, fullnameKH);
			for (unsigned int i = 0; i < Customer.size(); ++i) {
				if (string(Customer[i]->getFullName()) == fullnameKH) cout << *Customer[i];
			}
			break;
		}
		case 2:
		{
			string CMND;
			cout << "\tSearch CMND Khach Hang: ";
			getline(cin, CMND);
			int pos_CMND = this->searchCMNDCustomer(CMND);
			try {
				if (pos_CMND < 0) throw invalid_argument("Note: CMND Khach Hang Not Exist!");
				cout << *Customer[pos_CMND]; // validate pos
				flash = true;
				return Customer[pos_CMND];
			}
			catch (invalid_argument& exception) {
				cout << "\t" << exception.what() << endl;
			}
			break;
		}
		case 3:
		{
			string SDT; bool check = false;
			do {
				try {
					cout << "\tSearch SDT Khach Hang: ";
					getline(cin, SDT);
					if (!Customer[0]->isValidPhone(SDT.c_str())) throw invalid_argument("Phone must is number!");
					check = true;
				}
				catch (invalid_argument& exception) {
					cout << "\t" << exception.what() << endl;
				}
			} while (!check);
			int pos_SDT = this->searchSDTCustomer(SDT);
			try {
				if (pos_SDT < 0) throw invalid_argument("Note: SDT Khach Hang Not Exist!");
				cout << *Customer[pos_SDT]; // validate pos
				flash = true;
				return Customer[pos_SDT];
			}
			catch (invalid_argument& exception) {
				cout << "\t" << exception.what() << endl;
			}
			break;
		}
		case 5:
		{
			if (method != "search") {
				cout << "\tError: You choose invalid !" << endl;
				break;
			}
			string Country;
			cout << "\tSearch Country Khach Hang: ";
			getline(cin, Country);
			for (unsigned int i = 0; i < Customer.size(); ++i) {
				if (string(Customer[i]->getCountry()) == Country) {
					cout << *Customer[i];
				}
			}
			flash = true;
			break;
		}
		case 6:
		{
			if (method != "search") {
				cout << "\tError: You choose invalid !" << endl;
				break;
			}
			string City;
			cout << "\tSearch City Khach Hang: ";
			getline(cin, City);
			for (unsigned int i = 0; i < Customer.size(); ++i) {
				if (string(Customer[i]->getCity()) == City) {
					cout << *Customer[i];
				}
			}
			flash = true;
			break;
		}
		case 7:
		{
			if (method != "search") {
				cout << "\tError: You choose invalid !" << endl;
				break;
			}
			bool check = false;
			string GioiTinh;
			do {
				try {
					cout << "\tSearch Gender Khach Hang: ";
					getline(cin, GioiTinh);
					if (!(GioiTinh == "nam" || GioiTinh == "nu")) throw invalid_argument("Gender must is nam or nu!");
					check = true;
				}
				catch (invalid_argument& exception) {
					cout << "\t" << exception.what() << endl;
				}
			} while (!check);
			bool Gender = (GioiTinh == "nam") ? 1 : 0;
			for (unsigned int i = 0; i < Customer.size(); ++i) {
				if (Customer[i]->getGender() == Gender) {
					cout << *Customer[i];
				}
			}
			flash = true;
			break;
		}
		case 8:
		{
			if (method != "search") {
				cout << "\tError: You choose invalid !" << endl;
				break;
			}
			Date BirthDay;
			cout << "\tSearch BirthDay Khach Hang: ";
			bool check = false;
			do {
				try {
					BirthDay.scan();
					BirthDay.isValid();
					check = true;
				}
				catch (invalid_argument& exception) {
					cout << "\t" << exception.what() << endl;
				}
			} while (!check);
			for (unsigned int i = 0; i < Customer.size(); ++i) {
				if (Customer[i]->getBirthday() == BirthDay) {
					cout << *Customer[i];
				}
			}
			flash = true;
			break;
		}
		default:
			cout << "\tError: You choose invalid !" << endl;
			break;
		}
	} while (!flash);
	return NULL;
}

void QL_RentalMoto::updateCustomer() {
	Customers* KhachHang = this->searchCustomer("update");
	if (KhachHang == NULL) return;
	int choose;
	bool check = false;
	do {
		cout << "\tUPDATE CUSTOMER" << endl;
		cout << "\t1. Update Name Customer" << endl;
		cout << "\t2. Update Address Country Customer" << endl;
		cout << "\t3. Update Address City Customer" << endl;
		cout << "\t4. Update CMND Customer" << endl;
		cout << "\t5. Update SDT Customer" << endl;
		cout << "\t6. Update Email Customer" << endl;
		cout << "\t7. Update Birtday Customer" << endl;
		cout << "\t0. Exit(0)" << endl;
		cout << "\t=>Choose: ";
		cin >> choose;
		cin.ignore();
		switch (choose)
		{
		case 1:
		{
			string nameKH;
			cout << "\tNew Name Khach Hang: ";
			getline(cin, nameKH);
			SACommand update;
			update.setConnection(&con);
			update.setCommandText(_TSA("UPDATE Customers SET FullName = :NameKHNew WHERE FullName = :NameKHOld AND CMND = :CMND"));
			update.Param(_TSA("NameKHOld")).setAsString() = _TSA(KhachHang->getFullName());
			update.Param(_TSA("CMND")).setAsString() = _TSA(KhachHang->getCMND());
			update.Param(_TSA("NameKHNew")).setAsString() = _TSA(&nameKH[0]);
			update.Execute();
			KhachHang->setFullName(&nameKH[0]);
			break;
		}
		case 2:
		{
			string Country;
			cout << "\tNew Country Khach Hang: ";
			getline(cin, Country);
			SACommand update;
			update.setConnection(&con);
			update.setCommandText(_TSA("UPDATE Customers SET Country = :CountryNew WHERE Country = :CountryOld AND CMND = :CMND"));
			update.Param(_TSA("CountryOld")).setAsString() = _TSA(KhachHang->getCountry());
			update.Param(_TSA("CMND")).setAsString() = _TSA(KhachHang->getCMND());
			update.Param(_TSA("CountryNew")).setAsString() = _TSA(&Country[0]);
			update.Execute();
			KhachHang->setCountry(&Country[0]);
			break;
		}
		case 3:
		{
			string City;
			cout << "\tNew City Khach Hang: ";
			getline(cin, City);
			SACommand update;
			update.setConnection(&con);
			update.setCommandText(_TSA("UPDATE Customers SET City = :CityNew WHERE City = :CityOld AND CMND = :CMND"));
			update.Param(_TSA("CityOld")).setAsString() = _TSA(KhachHang->getCity());
			update.Param(_TSA("CMND")).setAsString() = _TSA(KhachHang->getCMND());
			update.Param(_TSA("CityNew")).setAsString() = _TSA(&City[0]);
			update.Execute();
			KhachHang->setCity(&City[0]);
			break;
		}
		case 4:
		{
			string CMND; bool check = false;
			do {
				try {
					cout << "\tNew CMND Khach Hang: ";
					getline(cin, CMND);
					if (!(CMND.length() == 9 || CMND.length() == 12)) throw invalid_argument("CMND must length 9 or 12 number!");
					check = true;
				}
				catch (invalid_argument& exception) {
					cout << "\t" << exception.what() << endl;
				}
			} while (!check);
			SACommand update;
			update.setConnection(&con);
			update.setCommandText(_TSA("UPDATE Customers SET CMND = :CMNDNew WHERE CMND = :CMNDOld"));
			update.Param(_TSA("CMNDOld")).setAsString() = _TSA(KhachHang->getCMND());
			update.Param(_TSA("CMNDNew")).setAsString() = _TSA(&CMND[0]);
			update.Execute();
			KhachHang->setCMND(&CMND[0]);
			break;
		}
		case 5:
		{
			string SDT; bool check = false;
			do {
				try {
					cout << "\tNew SDT Khach Hang: ";
					getline(cin, SDT);
					if (!KhachHang->isValidPhone(SDT.c_str())) throw invalid_argument("Phone must is number!");
					check = true;
				}
				catch (invalid_argument& exception) {
					cout << "\t" << exception.what() << endl;
				}
			} while (!check);
			SACommand update;
			update.setConnection(&con);
			update.setCommandText(_TSA("UPDATE Customers SET SDT = :SDTNew WHERE SDT = :SDTOld AND CMND = :CMND"));
			update.Param(_TSA("SDTOld")).setAsString() = _TSA(KhachHang->getSDT());
			update.Param(_TSA("CMND")).setAsString() = _TSA(KhachHang->getCMND());
			update.Param(_TSA("SDTNew")).setAsString() = _TSA(&SDT[0]);
			update.Execute();
			KhachHang->setSDT(&SDT[0]);
			break;
		}
		case 6:
		{
			string Email; bool check = false;
			do {
				try {
					cout << "\tNew Email Khach Hang: ";
					getline(cin, Email);
					if (!KhachHang->isValidEmailAddress(Email.c_str())) throw invalid_argument("Email must begin character 'a' -> 'z' or 'A' -> 'Z' and '@' must is before the dot !");
					check = true;
				}
				catch (invalid_argument& exception) {
					cout << "\t" << exception.what() << endl;
				}
			} while (!check);
			SACommand update;
			update.setConnection(&con);
			update.setCommandText(_TSA("UPDATE Customers SET Email = :EmailNew WHERE Email = :EmailOld AND CMND = :CMND"));
			update.Param(_TSA("EmailOld")).setAsString() = _TSA(KhachHang->getEmail());
			update.Param(_TSA("CMND")).setAsString() = _TSA(KhachHang->getCMND());
			update.Param(_TSA("EmailNew")).setAsString() = _TSA(&Email[0]);
			update.Execute();
			KhachHang->setEmail(&Email[0]);
			break;
		}
		case 7:
		{
			Date BirthDay; bool check = false;
			do {
				try {
					cout << "\tNew BirthDay Khach Hang: ";
					BirthDay.scan();
					BirthDay.isValid();
					check = true;
				}
				catch (invalid_argument& exception) {
					cout << "\t" << exception.what() << endl;
				}
			} while (!check);
			SADateTime dateOld(KhachHang->getBirthday().getYear(), KhachHang->getBirthday().getMonth(), KhachHang->getBirthday().getDay());
			SADateTime dateNew(BirthDay.getYear(), BirthDay.getMonth(), BirthDay.getDay());
			SACommand update;
			update.setConnection(&con);
			update.setCommandText(_TSA("UPDATE Customers SET BirthDay = :BirthDayNew WHERE BirthDay = :BirthDayOld AND CMND = :CMND"));
			update.Param(_TSA("BirthDayNew")).setAsDateTime() = dateNew;
			update.Param(_TSA("BirthDayOld")).setAsDateTime() = dateOld;
			update.Param(_TSA("CMND")).setAsString() = _TSA(KhachHang->getCMND());
			update.Execute();
			KhachHang->setBirthday(BirthDay.getDay(), BirthDay.getMonth(), BirthDay.getYear());
			break;
		}
		case 0:
			check = true;
			break;
		default:
			cout << "\tError: You choose invalid !" << endl;
			break;
		}
	} while (!check);
}

Rental* QL_RentalMoto::searchRental(const string& method) {
	cout << "\tSEARCH RENTAL" << endl;
	cout << "\t1. Search Ma Rental" << endl;
	cout << "\t2. Search Rental Motobike By Customer And Rent Day" << endl;
	cout << "\t3. Search Rental Motobike By Motobike And Rent Day" << endl;
	if (method == "search") {
		cout << "\t4. Search All Motobike Rented By Customer" << endl;
		cout << "\t5. Search All Customers Rental Motobike" << endl;
		cout << "\t6. Search All Rental Motobike By Rent Day" << endl;
		cout << "\t7. Search All Rental Motobike By Return Day" << endl;
		cout << "\t8. Search All Rental Motobike By Tinh Trang" << endl;
	}
	int choose;
	bool success = false;
	do {
		cout << "\t=>Choose: ";
		cin >> choose;
		cin.ignore();
		switch (choose)
		{
		case 1:
		{
			int MaRental;
			cout << "\tSearch Ma Rental: ";
			cin >> MaRental; cin.ignore();
			for (unsigned int i = 0; i < Customer.size(); ++i) {
				for (unsigned int j = 0; j < Customer[i]->getListRental().size(); ++j) {
					if (Customer[i]->getListRental()[j]->getMaRental() == MaRental) {
						Customer[i]->getListRental()[j]->read();
						return Customer[i]->getListRental()[j];
					}
				}
			}
			cout << "\tError: Rental not exist !" << endl;
			success = true;
			break;
		}
		case 2: case 3:
		{
			string TenXe, TenKH, CMND;
			Date RentDay;
			if (choose == 2) {
				cout << "\tTen Khach Hang: ";
				getline(cin, TenKH);
				cout << "\tCMND: ";
				getline(cin, CMND);
			}
			else {
				cout << "\tTen Xe: ";
				getline(cin, TenXe);
			}
			cout << "\tNgay Thue";
			RentDay.scan();
			if (choose == 2) {
				int pos_customer = this->searchCustomerByCMNDAndName(CMND, TenKH);
				for (unsigned int i = 0; i < Customer[pos_customer]->getListRental().size(); ++i) {
					if (Customer[pos_customer]->getListRental()[i]->getRentDay() == RentDay) {
						Customer[pos_customer]->getListRental()[i]->read();
						return Customer[pos_customer]->getListRental()[i];
					}
				}
			}
			else {
				int pos_category = this->searchCategoryOfMotobike(TenXe.c_str());
				for (unsigned int i = 0; i < TypeMoto[pos_category]->getListMoto().size(); ++i) {
					if (string(TypeMoto[pos_category]->getListMoto()[i]->getTenXe()) == TenXe) {
						for (unsigned int k = 0; k < TypeMoto[pos_category]->getListMoto()[i]->getListRental().size(); ++k) {
							if (TypeMoto[pos_category]->getListMoto()[i]->getListRental()[k]->getRentDay() == RentDay) {
								TypeMoto[pos_category]->getListMoto()[i]->getListRental()[k]->read();
								return TypeMoto[pos_category]->getListMoto()[i]->getListRental()[k];
							}
						}
					}
				}
			}
			cout << "\tError: Rental not exist !" << endl;
			success = true;
			break;
		}
		case 4:
		{
			if (method != "search") {
				cout << "\tError: You choose invalid !" << endl;
				break;
			}
			string TenKH;
			cout << "\tTen Khach Hang: ";
			getline(cin, TenKH);
			for (unsigned int i = 0; i < Customer.size(); ++i) {
				if (string(Customer[i]->getFullName()) == TenKH) {
					for (unsigned int j = 0; j < Customer[i]->getListRental().size(); ++j) {
						Customer[i]->getListRental()[j]->read();
					}
				}
			}
			success = true;
			break;
		}
		case 5:
		{
			if (method != "search") {
				cout << "\tError: You choose invalid !" << endl;
				break;
			}
			string TenXe;
			cout << "\tTen Xe: ";
			getline(cin, TenXe);
			int pos_category = this->searchCategoryOfMotobike(TenXe.c_str());
			for (unsigned int j = 0; j < TypeMoto[pos_category]->getListMoto().size(); ++j) {
				if (string(TypeMoto[pos_category]->getListMoto()[j]->getTenXe()) == TenXe) {
					for (unsigned int k = 0; k < TypeMoto[pos_category]->getListMoto()[j]->getListRental().size(); ++k) {
						TypeMoto[pos_category]->getListMoto()[j]->getListRental()[k]->read();
					}
				}
			}
			success = true;
			break;
		}
		case 6:
		{
			if (method != "search") {
				cout << "\tError: You choose invalid !" << endl;
				break;
			}
			Date Rent_date;
			cout << "\tRent Day";
			Rent_date.scan();
			for (unsigned int i = 0; i < Customer.size(); ++i) {
				for (unsigned int j = 0; j < Customer[i]->getListRental().size(); ++j) {
					if (Customer[i]->getListRental()[j]->getRentDay() == Rent_date) {
						Customer[i]->getListRental()[j]->read();
					}
				}
			}
			success = true;
			break;
		}
		case 7:
		{
			if (method != "search") {
				cout << "\tError: You choose invalid !" << endl;
				break;
			}
			Date Return_date;
			cout << "\tReturn Day";
			Return_date.scan();
			for (unsigned int i = 0; i < Customer.size(); ++i) {
				for (unsigned int j = 0; j < Customer[i]->getListRental().size(); ++j) {
					if (Customer[i]->getListRental()[j]->getReturnDay() == Return_date) {
						Customer[i]->getListRental()[j]->read();
					}
				}
			}
			success = true;
			break;
		}
		case 8:
		{
			if (method != "search") {
				cout << "\tError: You choose invalid !" << endl;
				break;
			}
			bool status, check = false;
			string TinhTrang;
			do {
				try {
					cout << "Tinh Trang: ";
					getline(cin, TinhTrang);
					if (!(TinhTrang == "da tra" || TinhTrang == "chua tra")) throw invalid_argument("Note: Tinh Trang phai la da tra hoac chua tra!");
					check = true;
				}
				catch (invalid_argument& exception) {
					cout << "\t" << exception.what() << endl;
				}
			} while (!check);
			status = (TinhTrang == "da tra") ? 1 : 0;
			for (unsigned int i = 0; i < Customer.size(); ++i) {
				for (unsigned int j = 0; j < Customer[i]->getListRental().size(); ++j) {
					if (Customer[i]->getListRental()[j]->getStatus() == status) {
						Customer[i]->getListRental()[j]->read();
					}
				}
			}
			success = true;
			break;
		}
		default:
			cout << "\tError: You choose invalid !" << endl;
			break;
		}
	} while (!success);
	return NULL;
}

void QL_RentalMoto::updateRental() {
	Rental* rentalmoto = this->searchRental("update");
	if (rentalmoto == NULL) return;
	int choose;
	bool check = false;
	do {
		cout << "\tUPDATE RENTAL" << endl;
		cout << "\t1. Update Xe Rental" << endl;
		cout << "\t2. Update Khach Hang Rental" << endl;
		cout << "\t3. Update Rent Day" << endl;
		cout << "\t4. Update Return Day" << endl;
		cout << "\t5. Update Tinh Trang" << endl;
		cout << "\t0. Exits(0)" << endl;
		cout << "\t=>Choose: ";
		cin >> choose;
		cin.ignore();
		switch (choose)
		{
		case 1:
		{
			string TenXe;
			bool flash = false;
			int MaXeOld = -1;
			try {
				cout << "\tTen Xe: ";
				getline(cin, TenXe);
				int MaXe = this->searchNameMotobike(TenXe);
				if (MaXe == -1) throw invalid_argument("Note: Motobike Not Exist!");
				else {
					int pos_category = this->searchCategoryOfMotobike(TenXe.c_str());
					for (unsigned int j = 0; j < TypeMoto[pos_category]->getListMoto().size(); ++j) {
						if (TypeMoto[pos_category]->getListMoto()[j]->getMaXe() == MaXe) {
							if (TypeMoto[pos_category]->getListMoto()[j]->getStatus() == 1) throw invalid_argument("Error: Motobike has been rented!");
							TypeMoto[pos_category]->getListMoto()[j]->addRental(rentalmoto); // add vao motobike new
							TypeMoto[pos_category]->getListMoto()[j]->setStatus(true); //set tinh trang da thue
							break;
						}
					}
					for (unsigned int j = 0; j < TypeMoto[pos_category]->getListMoto().size(); ++j) {
						if (flash) break;
						for (unsigned int k = 0; k < TypeMoto[pos_category]->getListMoto()[j]->getListRental().size(); ++k) {
							if ((*rentalmoto) == (*TypeMoto[pos_category]->getListMoto()[j]->getListRental()[k])) {
								TypeMoto[pos_category]->getListMoto()[j]->removeRental(k); // remove rental inside motobike old
								TypeMoto[pos_category]->getListMoto()[j]->setStatus(false);
								MaXeOld = TypeMoto[pos_category]->getListMoto()[j]->getMaXe();
								flash = true;
								break;
							}
						}
					}
				}
				SACommand update;
				SADateTime Rent_date(rentalmoto->getRentDay().getYear(), rentalmoto->getRentDay().getMonth(), rentalmoto->getRentDay().getDay());
				update.setConnection(&con);
				update.setCommandText(_TSA("UPDATE Rental SET MaXe = :MaXe WHERE MaRental = :MaRental AND Rent_date = :Rent_date"));
				update.Param(_TSA("MaXe")).setAsInt64() = MaXe;
				update.Param(_TSA("MaRental")).setAsInt64() = rentalmoto->getMaRental();
				update.Param("Rent_date").setAsDateTime() = Rent_date;
				update.Execute();
				SACommand cmd;
				cmd.setConnection(&con);
				cmd.setCommandText(_TSA("UPDATE Motobike SET TinhTrang = 0 WHERE MaXe = :MaXeOld"));
				cmd.Param(_TSA("MaXeOld")).setAsInt64() = MaXeOld;
				cmd.Execute();
				SACommand select;
				select.setConnection(&con);
				select.setCommandText(_TSA("UPDATE Motobike SET TinhTrang = 1 WHERE MaXe = :MaXeNew"));
				select.Param(_TSA("MaXeNew")).setAsInt64() = MaXe;
				select.Execute();
			}
			catch (invalid_argument& exception) {
				cout << "\t" << exception.what() << endl;
			}
			//int MaKH = -1;
			//bool kt = false;
			//for (unsigned int i = 0; i < Customer.size(), kt = false; ++i) {
			//	for (unsigned int j = 0; j < Customer[i]->getListRental().size(), kt = false; ++j) {
			//		if ((*rentalmoto) == (*Customer[i]->getListRental()[j])) {
			//			MaKH = Customer[i]->getMaKH(); kt = true;
			//		}
			//	}
			//}
			break;
		}
		case 2:
		{
			string TenKH, CMND;
			try {
				cout << "\tTen Khach Hang Rent: ";
				getline(cin, TenKH);
				cout << "\tCMND Khach Hang Rent: ";
				getline(cin, CMND);
				int pos_KH = this->searchCustomerByCMNDAndName(CMND, TenKH);
				if (pos_KH == -1)  throw invalid_argument("Note: Khach Hang Not Exist !");
				//int MaXe = -1;
				//for (unsigned int i = 0, bool flasg = false; i < TypeMoto.size(), flasg == false; ++i) {
				//	for (unsigned int j = 0; j < TypeMoto[i]->getListMoto().size(), flasg == false; ++j) {
				//		for (unsigned int k = 0; k < TypeMoto[i]->getListMoto()[j]->getListRental().size(), flasg == false; ++k) {
				//			if ((*TypeMoto[i]->getListMoto()[j]->getListRental()[k]) == (*rentalmoto)) {
				//				MaXe = TypeMoto[i]->getListMoto()[j]->getMaXe(); flasg = true;
				//			}
				//		}
				//	}
				//}
				SACommand update;
				SADateTime Rent_date(rentalmoto->getRentDay().getYear(), rentalmoto->getRentDay().getMonth(), rentalmoto->getRentDay().getDay());
				update.setConnection(&con);
				update.setCommandText(_TSA("UPDATE Rental SET MaKH = :MaKHNew WHERE MaRental = :MaRental AND Rent_date = :Rent_date"));
				update.Param(_TSA("MaKHNew")).setAsInt64() = Customer[pos_KH]->getMaKH();
				update.Param(_TSA("MaRental")).setAsInt64() = rentalmoto->getMaRental();
				update.Param(_TSA("Rent_date")).setAsDateTime() = Rent_date;
				update.Execute();
				bool flasg = false;
				for (unsigned int i = 0; i < Customer.size(); ++i) {
					if (flasg) break;
					for (unsigned int j = 0; j < Customer[i]->getListRental().size(); ++j) {
						if ((*Customer[i]->getListRental()[j]) == (*rentalmoto)) {
							Customer[pos_KH]->addRental(rentalmoto); // add rental new into customer other
							Customer[i]->removeRental(j); // remove rental of customer old
							flasg = true;
							break;
						}
					}
				}
			}
			catch (invalid_argument& exception) {
				cout << "\t" << exception.what() << endl;
			}
			break;
		}
		case 3: case 4: case 5:
		{
			Date date_new;
			bool status;
			switch (choose) {
			case 3:
			{
				cout << "\tUpdate Ngay Thue";
				date_new.scan();
				SADateTime date_Rent(date_new.getYear(), date_new.getMonth(), date_new.getDay());
				SACommand updateRent;
				updateRent.setConnection(&con);
				updateRent.setCommandText(_TSA("UPDATE Rental SET Rent_date = :Rent_dateNew WHERE MaRental = :MaRental"));
				updateRent.Param(_TSA("MaRental")).setAsInt64() = rentalmoto->getMaRental();
				updateRent.Param(_TSA("Rent_dateNew")).setAsDateTime() = date_Rent;
				updateRent.Execute();
				break;
			}
			case 4:
			{
				cout << "\tUpdate Ngay Tra";
				date_new.scan();
				SADateTime dateNew(date_new.getYear(), date_new.getMonth(), date_new.getDay());
				SACommand updateReturn;
				updateReturn.setConnection(&con);
				updateReturn.setCommandText(_TSA("UPDATE Rental SET Return_date = :Return_dateNew WHERE MaRental = :MaRental"));
				updateReturn.Param(_TSA("MaRental")).setAsInt64() = rentalmoto->getMaRental();
				updateReturn.Param(_TSA("Return_dateNew")).setAsDateTime() = dateNew;
				updateReturn.Execute();
				break;
			}
			case 5:
			{
				bool flash = false;
				do {
					try {
						string TinhTrang;
						cout << "\tUpdate Tinh Trang: ";
						getline(cin, TinhTrang);
						if (!(TinhTrang == "da tra" || TinhTrang == "chua tra")) throw invalid_argument("Note: Tinh Trang phai la da tra hoac chua tra!");
						status = (TinhTrang == "da tra") ? 1 : 0;
						flash = true;
					}
					catch (invalid_argument& exception) {
						cout << "\t" << exception.what() << endl;
					}
				} while (!flash);
				SACommand update;
				update.setConnection(&con);
				update.setCommandText(_TSA("UPDATE Rental SET TinhTrang = :TinhTrang WHERE MaRental = :MaRental"));
				update.Param(_TSA("MaRental")).setAsInt64() = rentalmoto->getMaRental();
				update.Param(_TSA("TinhTrang")).setAsBool() = status;
				update.Execute();
				break;
			}
			}
			bool flasg = false;
			for (unsigned int i = 0; i < Customer.size(); ++i) {
				if (flasg) break;
				for (unsigned int j = 0; j < Customer[i]->getListRental().size(); ++j) {
					if ((*Customer[i]->getListRental()[j]) == (*rentalmoto)) {
						if (choose == 3) {
							Customer[i]->getListRental()[j]->setRentDay(date_new.getDay(), date_new.getMonth(), date_new.getYear());
						}
						else if (choose == 4) {
							Customer[i]->getListRental()[j]->setReturnDay(date_new.getDay(), date_new.getMonth(), date_new.getYear());
						}
						else {
							Customer[i]->getListRental()[j]->setStatus(status);
						}
						flasg = true;
						break;
					}
				}
			}
			flasg = false;
			for (unsigned int i = 0; i < TypeMoto.size(); ++i) {
				if (flasg) break;
				for (unsigned int j = 0; j < TypeMoto[i]->getListMoto().size(); ++j) {
					if (flasg) break;
					for (unsigned int k = 0; TypeMoto[i]->getListMoto()[j]->getListRental().size(); ++k) {
						if (rentalmoto->getMaRental() == TypeMoto[i]->getListMoto()[j]->getListRental()[k]->getMaRental()) {
							if (choose == 3) {
								TypeMoto[i]->getListMoto()[j]->getListRental()[k]->setRentDay(date_new.getDay(), date_new.getMonth(), date_new.getYear());
							}
							else if (choose == 4) {
								TypeMoto[i]->getListMoto()[j]->getListRental()[k]->setReturnDay(date_new.getDay(), date_new.getMonth(), date_new.getYear());
							}
							else {
								TypeMoto[i]->getListMoto()[j]->getListRental()[k]->setStatus(status);
							}
							flasg = true;
							break;
						}
					}
				}
			}
			break;
		}
		case 0: 
		{
			check = true;
			break;
		}
		default:
			cout << "\tError: You choose invalid!" << endl;
			break;
		}
	} while (!check);
}

void QL_RentalMoto::ReturnMotobike() {
	cout << "\tRETURN MOTOBIKE" << endl;
	string NameKH, CMND;
	Date RentDay, ReturnDay;
	Rental* rentalMoto = nullptr;
	int maKH = -1, maMotobike = -1;
	bool success = false;
	bool kt = false;
	int pos_customer = -1;
	do {
		try {
			cout << "\tTen Khach Hang: ";
			getline(cin, NameKH);
			cout << "\tCMND: ";
			getline(cin, CMND);
			pos_customer = this->searchCustomerByCMNDAndName(CMND, NameKH);
			if (pos_customer == -1) throw invalid_argument("Error: Khach Hang Not Exist!");
			cout << "\tNgay Thue: ";
			RentDay.scan();
			for (unsigned int i = 0; i < Customer[pos_customer]->getListRental().size(); ++i) {
				if (Customer[pos_customer]->getListRental()[i]->getRentDay().getDay() == RentDay.getDay()) {
					kt = true;
					break;
				}
			}
			if (!kt) throw invalid_argument("Error: Rental Not Exists!");
			cout << "\tUpdate Ngay Tra: ";
			ReturnDay.scan();
			success = true;
		}
		catch (invalid_argument& exception) {
			cout << "\t" << exception.what() << endl;
		}
	} while (!success);
	for (unsigned int i = 0; i < Customer[pos_customer]->getListRental().size(); ++i) {
		if (Customer[pos_customer]->getListRental()[i]->getRentDay() == RentDay) {
			rentalMoto = Customer[pos_customer]->getListRental()[i];
			Customer[pos_customer]->getListRental()[i]->setReturnDay(ReturnDay.getDay(), ReturnDay.getMonth(), ReturnDay.getYear());
			Customer[pos_customer]->getListRental()[i]->setStatus(true);
			break;
		}
	}
	bool check = false;
	for (unsigned int i = 0; i < TypeMoto.size(); ++i) {
		if (check) break;
		for (unsigned j = 0; j < TypeMoto[i]->getListMoto().size(); ++j) {
			if (check) break;
			for (unsigned k = 0; k < TypeMoto[i]->getListMoto()[j]->getListRental().size(); ++k) {
				if ((*rentalMoto) == (*TypeMoto[i]->getListMoto()[j]->getListRental()[k])) {
					TypeMoto[i]->getListMoto()[j]->getListRental()[k]->setReturnDay(ReturnDay.getDay(), ReturnDay.getMonth(), ReturnDay.getYear());
					TypeMoto[i]->getListMoto()[j]->getListRental()[k]->setStatus(true);
					TypeMoto[i]->getListMoto()[j]->setStatus(false); // update xe ve trang thai chua thue
					maMotobike = TypeMoto[i]->getListMoto()[j]->getMaXe();
					check = true;
					break;
				}
			}
		}
	}
	SADateTime Return_Date(ReturnDay.getYear(), ReturnDay.getMonth(), ReturnDay.getDay());
	SADateTime Rent_Date(RentDay.getYear(), RentDay.getMonth(), RentDay.getDay());
	SACommand update;
	update.setConnection(&con);
	update.setCommandText(_TSA("UPDATE Rental SET Return_date = :Return_date, TinhTrang = :TinhTrang WHERE MaKH = :MaKH AND MaXe = :MaXe AND Rent_date = :Rent_date"));
	update.Param(_TSA("Return_date")).setAsDateTime() = Return_Date;
	update.Param(_TSA("TinhTrang")).setAsBool() = true;
	update.Param(_TSA("Rent_date")).setAsDateTime() = Rent_Date;
	update.Param(_TSA("MaKH")).setAsInt64() = Customer[pos_customer]->getMaKH();
	update.Param(_TSA("MaXe")).setAsInt64() = maMotobike;
	update.Execute();
	SACommand cmd;
	cmd.setConnection(&con);
	cmd.setCommandText(_TSA("UPDATE Motobike SET TinhTrang = :TinhTrang WHERE MaXe = :MaXe"));
	cmd.Param(_TSA("MaXe")).setAsInt64() = maMotobike;
	cmd.Param(_TSA("TinhTrang")).setAsBool() = false;
	cmd.Execute();
}

bool QL_RentalMoto::isValidEmailSame(const char* email) { // check same email
	for (unsigned int i = 0; i < Customer.size(); ++i) {
		if (email == Customer[i]->getEmail()) return false;
	}
	return true;
}
bool QL_RentalMoto::isValidCMNDSame(const char* CMND) { //check same CMND
	for (unsigned int i = 0; i < Customer.size(); ++i) {
		if (CMND == Customer[i]->getCMND()) return false;
	}
	return true;
}
bool QL_RentalMoto::isValidSDTSame(const char* SDT) { //check same SDT
	for (unsigned int i = 0; i < Customer.size(); ++i) {
		if (SDT == Customer[i]->getSDT()) return false;
	}
	return true;
}

bool QL_RentalMoto::isValidNameCategorySame(const char* name) {
	for (unsigned int i = 0; i < TypeMoto.size(); ++i) {
		if (string(TypeMoto[i]->getTenLoaiXe()) == string(name)) return false;
	}
	return true;
}

void QL_RentalMoto::searchCategory() {
	cout << "\tSEARCH CATEGORY" << endl;
	cout << "\t1. Search Ma Category Motobike" << endl;
	cout << "\t2. Searc Name Category Motrobike" << endl;
	int choose; bool check = false;
	do {
		cout << "\t=>Choose: ";
		cin >> choose;
		cin.ignore();
		switch (choose)
		{
		case 1:
		{
			int maCategory; bool flash = false;
			do {
				try {
					cout << "\tMa Loai Xe: ";
					cin >> maCategory; cin.ignore();
					int pos_Category = this->searchMaCategory(maCategory);
					if (pos_Category == -1) throw invalid_argument("Note: Ma Loai Xe Not Exists!");
					cout << *TypeMoto[pos_Category];
					flash = true;
				}
				catch (invalid_argument& exception) {
					cout << "\t" << exception.what() << endl;
				}
			} while (!flash);
			check = true;
			break;
		}
		case 2:
		{
			string nameCategory; bool flash = false;
			do {
				try {
					cout << "\tTen Loai Xe: ";
					getline(cin, nameCategory);
					int pos_Category = this->searchNameCategory(nameCategory);
					if (pos_Category == -1) throw invalid_argument("Note: Ten Loai Xe Not Exists!");
					cout << *TypeMoto[pos_Category];
					flash = true;
				}
				catch (invalid_argument& exception) {
					cout << "\t" << exception.what() << endl;
				}
			} while (!flash);
			check = true;
			break;
		}
		default:
			cout << "\tError: You Choose Invalid!" << endl;
			break;
		}
	} while (!check);
}

bool QL_RentalMoto::isValidBienSoMotobikeSame(const char* bienso) {
	for (unsigned int i = 0; i < TypeMoto.size(); ++i) {
		for (unsigned int j = 0; j < TypeMoto[i]->getListMoto().size(); ++j) {
			if (string(TypeMoto[i]->getListMoto()[j]->getBienSo()) == string(bienso)) return false;
		}
	}
	return true;
}

bool QL_RentalMoto::isValidNameMotobikeSame(const char* TenXe) {
	for (unsigned int i = 0; i < TypeMoto.size(); ++i) {
		for (unsigned int j = 0; j < TypeMoto[i]->getListMoto().size(); ++j) {
			if (string(TypeMoto[i]->getListMoto()[j]->getTenXe()) == string(TenXe)) return false;
		}
	}
	return true;
}

int QL_RentalMoto::searchCategoryOfMotobike(const char* TenXe) {
	static int err = -1;
	for (unsigned int i = 0; i < TypeMoto.size(); ++i) {
		for (unsigned int j = 0; j < TypeMoto[i]->getListMoto().size(); ++j) {
			if (string(TypeMoto[i]->getListMoto()[j]->getTenXe()) == string(TenXe)) return i;
		}
	}
	return err;
}

void QL_RentalMoto::deleteCustomers() {
	Customers* KhachHang = this->searchCustomer();
	if (KhachHang == NULL) return;
	vector<int> listMaRental;
	SACommand select;
	select.setConnection(&con);
	select.setCommandText(_TSA("SELECT MaRental, MaXe FROM Rental WHERE MaKH = :MaKH"));
	select.Param(_TSA("MaKH")).setAsInt64() = KhachHang->getMaKH();
	select.Execute();
	while (select.FetchNext()) {
		sa_int64_t MaRental = select.Field("MaRental").asInt64();
		sa_int64_t MaXe = select.Field("MaXe").asInt64();
		int pos_customer = this->searchMaCustomer(KhachHang->getMaKH());
		delete Customer[pos_customer];
		Customer[pos_customer] = nullptr;
		for (unsigned int i = pos_customer; i < Customer.size() - 1; ++i) {
			Customer[i] = Customer[i + 1];
		}
		Customer.reserve(Customer.size() - 1);
	}
}
