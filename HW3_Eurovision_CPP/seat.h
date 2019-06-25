#include <iostream>
#include <string>
using std::string;
using std::exception;


// ---------------------------------------------
class NoPrice:public exception
{

public:
	NoPrice():exception("Not For Sale !"){}
	virtual ~NoPrice()=default;
};

// ---------------------------------------------
class Seat
{

protected:
	int chairNum;
	int lineNum;
	int basePrice;

public:
	Seat(int chair_num, int line_num, int base_price): chairNum(chair_num),
								  lineNum(line_num),basePrice(base_price){}
	virtual ~Seat();

	virtual int price() const;
	virtual string location() const = 0;
};

// ---------------------------------------------
class GreenRoomSeat:public Seat
{
public:
	GreenRoomSeat(int line, int chair);
	//override to throw exception
	int price() const override;
	string location() const override;
};


// ---------------------------------------------
class MainHallSeat:public Seat
{

public:
	//use the c'tor of Seat and add 100 to the base_price
	MainHallSeat(int chair_num, int line_num, int base_price);
	virtual ~MainHallSeat() = default;

	string location() const override;
};


// ---------------------------------------------
class SpecialSeat:public MainHallSeat
{
public:

};

// ---------------------------------------------
class GoldenCircleSeat:public SpecialSeat
{
};

// ---------------------------------------------
class DisablePodiumSeat:public SpecialSeat
{

};

// ---------------------------------------------
class RegularSeat:public MainHallSeat
{

protected:
	char area;
public:
	RegularSeat() = default;
	virtual ~RegularSeat();
	string location() const override;

};

// ---------------------------------------------
class FrontRegularSeat:public RegularSeat
{
};

// ---------------------------------------------
class MiddleRegularSeat:public RegularSeat
{
};

// ---------------------------------------------
class RearRegularSeat:public RegularSeat
{
};

// ---------------------------------------------
