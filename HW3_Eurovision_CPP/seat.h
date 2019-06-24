#include <iostream>
#include <string>
using std::string;
using std::exception;


// ---------------------------------------------
class NoPrice:exception
{
};

// ---------------------------------------------
class Seat
{
	int chairNum;
	int lineNum;
	int basePrice;

public:
	Seat(int chair_num, int line_num, int base_price);
	virtual  ~Seat();

	virtual string location() const;
	virtual int price() const = 0;
	
};

// ---------------------------------------------
class GreenRoomSeat:Seat
{
public:
	int price() const override;
};

// ---------------------------------------------
class MainHallSeat:Seat
{
	int mainHallPrice;

public:
	MainHallSeat();
	virtual ~MainHallSeat();
	int price() const override;
};

// ---------------------------------------------
class SpecialSeat:MainHallSeat
{
public:
	int price() const override;
};

// ---------------------------------------------
class GoldenCircleSeat:SpecialSeat
{
};

// ---------------------------------------------
class DisablePodiumSeat:SpecialSeat
{
};

// ---------------------------------------------
class RegularSeat:MainHallSeat
{
	char area;
public:
	RegularSeat() = default;
	virtual ~RegularSeat();
	string location() const override;

};

// ---------------------------------------------
class FrontRegularSeat:RegularSeat
{
};

// ---------------------------------------------
class MiddleRegularSeat:RegularSeat
{
};

// ---------------------------------------------
class RearRegularSeat:RegularSeat
{
};

// ---------------------------------------------
