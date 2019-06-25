#ifndef SEAT_H_
#define SEAT_H_

#include <iostream>
#include <string>
using std::string;
using std::runtime_error;
using std::exception;

// ---------------------------------------------
class NoPrice:public runtime_error
{

public:
	NoPrice():runtime_error("Not For Sale !"){}
	virtual ~NoPrice()=default;
};

// ---------------------------------------------
class Seat
{

protected:
	int chairNum;
	int lineNum;
	int basePrice;

protected:
	Seat(int line_num, int chair_num, int base_price) :chairNum(chair_num),
		lineNum(line_num),basePrice(base_price){}
public:
	virtual ~Seat() = default;

	virtual int price() const;
	virtual string location() const;
};

// Green Room ---------------------------------------------
class GreenRoomSeat:public Seat
{
public:
	GreenRoomSeat(int line, int chair):Seat(line, chair, -1){}
	//override to throw exception
	int price() const override;
	string location() const override;
};


// ---------------------------------------------
class MainHallSeat:public Seat
{
protected:
//use the c'tor of Seat and add 100 to the base_price
MainHallSeat(int line_num, int chair_num, int base_price)
:Seat(line_num, chair_num, base_price + 100) {}

public:
	virtual ~MainHallSeat() = default;
};


// ---------------------------------------------
class SpecialSeat:public MainHallSeat
{
protected:
	SpecialSeat(int line_num, int chair_num, int base_price)
		:MainHallSeat(line_num, chair_num, base_price + 300) {}
public:
	virtual ~SpecialSeat() = default;
};

// ---------------------------------------------
class GoldenCircleSeat:public SpecialSeat
{
public:
	GoldenCircleSeat(int line_num, int chair_num, int base_price)
	:SpecialSeat(line_num, chair_num,base_price + 1000) {}
	~GoldenCircleSeat() = default;

	string location() const override;
};

// ---------------------------------------------
class DisablePodiumSeat:public SpecialSeat
{

public:
	DisablePodiumSeat(int line_num, int chair_num, int base_price)
	:SpecialSeat(line_num, chair_num, -1) {}

	DisablePodiumSeat(int line_num, int chair_num)
	:SpecialSeat(line_num,chair_num,-1){}

	string location() const override;
	int price() const override;
};

//RegularSeat-------------------------------------------------------------------
class RegularSeat:public MainHallSeat
{
protected:
	char area;
	RegularSeat(char area, int line, int chair, int base_price)
		:MainHallSeat(line, chair, base_price), area(area){}
public:
	virtual ~RegularSeat() = default;
	string location() const override;
};


//FrontRegularSeat--------------------------------------------------------------
class FrontRegularSeat :public RegularSeat
{
public:
	FrontRegularSeat(char area, int line, int chair, int base_price)
	:RegularSeat(area,line,chair, base_price + 500){}
	~FrontRegularSeat() = default;
	string location() const override;
};


//MiddleRegularSeat-------------------------------------------------------------
class MiddleRegularSeat :public RegularSeat
{
public:
	MiddleRegularSeat(char area, int line, int chair, int base_price)
	:RegularSeat(area, line,chair, base_price + 250) {}
	~MiddleRegularSeat() = default;
	string location() const override;
};


//RearRegularSeat---------------------------------------------------------------
class RearRegularSeat:public RegularSeat
{
public:
	RearRegularSeat(char area, int line, int chair, int base_price)
	:RegularSeat(area, line,chair, base_price) {}
	~RearRegularSeat() = default;
	string location() const override;
};

#endif
