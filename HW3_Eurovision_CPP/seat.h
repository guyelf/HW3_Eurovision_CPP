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

// Green Room ---------------------------------------------
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

public:
	DisablePodiumSeat(int chair_num, int line_num,int base_price);
	DisablePodiumSeat(int chair_num, int line_num);
	~DisablePodiumSeat() = default;

	string location() const override;
	int price() const override;
};

//RegularSeat-------------------------------------------------------------------
class RegularSeat:public MainHallSeat
{
protected:
	char area;
public:
	RegularSeat(char area, int chair, int line, int base_price);
	virtual ~RegularSeat() = default;
	string location() const override = 0;
};


//FrontRegularSeat--------------------------------------------------------------
class FrontRegularSeat :public RegularSeat
{
public:
	FrontRegularSeat(char area, int chair, int line, int base_price);
	~FrontRegularSeat() = default;
	string location() const override;
};


//MiddleRegularSeat-------------------------------------------------------------
class MiddleRegularSeat :public RegularSeat
{
public:
	MiddleRegularSeat(char area, int chair, int line, int base_price);
	~MiddleRegularSeat() = default;
	string location() const override;
};


//RearRegularSeat---------------------------------------------------------------
class RearRegularSeat :public RegularSeat
{
public:
	RearRegularSeat(char area, int chair, int line, int base_price);
	~RearRegularSeat() = default;
	string location() const override;
};
