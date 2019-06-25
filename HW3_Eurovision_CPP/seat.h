#include <iostream>
#include <string>
using std::string;
using std::runtime_error;


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

public:
	Seat(int chair_num, int line_num, int base_price): chairNum(chair_num),
								  lineNum(line_num),basePrice(base_price){}
	virtual ~Seat() = default;

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
};


// ---------------------------------------------
class SpecialSeat:public MainHallSeat
{
public:
	SpecialSeat(int chair_num, int line_num,int base_price);
	virtual ~SpecialSeat() = default;
	int price() const override = 0;
};

// ---------------------------------------------
class GoldenCircleSeat:public SpecialSeat
{
public:
	GoldenCircleSeat(int chair, int line_num, int base_price);
	~GoldenCircleSeat() = default;

	string location() const override;
};

// ---------------------------------------------
class DisablePodiumSeat:public SpecialSeat
{
	DisablePodiumSeat(int chair_num, int line_num);
	~DisablePodiumSeat() = default;

	string location() const override;
	int price() const override;
};

// ---------------------------------------------
class RegularSeat:public MainHallSeat
{

protected:
	char area;
public:
	RegularSeat() = default;
	virtual ~RegularSeat();
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
