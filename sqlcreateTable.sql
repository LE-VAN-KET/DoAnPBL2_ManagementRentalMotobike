create table Customers(
	MaKH int IDENTITY(1,1) not null primary key,
	FullName nvarchar(50) not null,
	Country nvarchar(50) not null,
	City nvarchar(50) not null,
	CMND nvarchar(50) not null,
	SDT nvarchar(50) not null,
	Email nvarchar(50) not null,
	Gender bit not null,
	BirthDay datetime not null,
)

create table Category(
	MaLoaiXe int IDENTITY(1,1) not null primary key,
	TenLoaiXe nvarchar(50) not null,
	Number int not null,
)

create table Motobike(
	MaXe int IDENTITY(1,1) not null,
	TenXe nvarchar(50) not null,
	BienSo nvarchar(50) not null,
	Color nvarchar(50) not null,
	MaLoaiXe int not null,
	PriceDay money not null,
	TinhTrang bit not null,
	primary key (MaXe),
	foreign key (MaLoaiXe) references Category(MaLoaiXe) ON DELETE CASCADE ON UPDATE CASCADE,
)

create table Rental(
	MaRental int IDENTITY(1,1) not null,
	MaXe int not null,
	MaKH int not null,
	Rent_date datetime not null,
	Return_date datetime not null,
	ThanhTien money not null,
	TinhTrang bit not null,
	primary key (MaRental),
	foreign key (MaXe) references Motobike(MaXe) ON DELETE CASCADE ON UPDATE CASCADE,
	foreign key (MaKH) references Customers(MaKH) ON DELETE CASCADE ON UPDATE CASCADE,
)


CREATE trigger Insert_Upate_TinhTien
on [dbo].[Rental]
for insert, update
as
begin
	declare @new_return_day datetime, @price money, @rent_day datetime, @MaRental int
	select @new_return_day = inserted.Return_date, @price = Motobike.PriceDay, @rent_day = inserted.Rent_date, @MaRental = inserted.MaRental
	from inserted inner join Motobike on inserted.MaXe = Motobike.MaXe
	Update Rental set ThanhTien = DATEDIFF(day, @rent_day,  @new_return_day) * @price Where MaRental = @MaRental
end