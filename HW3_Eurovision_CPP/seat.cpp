#include "eurovision.h"
#include "seat.h"

using std::to_string;


//Seat
int Seat::price() const
{
	return this->basePrice;
}

string Seat::location() const
{
	return "line: " + to_string(this->lineNum) + ", chair: " +
				    to_string(this->chairNum);
}

//GreenRoom
GreenRoomSeat::GreenRoomSeat(int line, int chair)
:Seat(chair, line, -1){}

int GreenRoomSeat::price() const
{
	throw NoPrice();
}

string GreenRoomSeat::location() const
{
	return "Green Room-> " + this->location();
}


//Main Hall
MainHallSeat::MainHallSeat(int chair_num, int line_num, int base_price)
						  :Seat(chair_num,line_num,base_price+100){}


//special seat
SpecialSeat::SpecialSeat(int chair_num, int line_num,int base_price)
:MainHallSeat(chair_num, line_num,base_price+300){}

//Golden Circle
GoldenCircleSeat::GoldenCircleSeat(int chair, int num, int base_price)
:SpecialSeat(chair, num,base_price+1000){}


string GoldenCircleSeat::location() const
{
	return "Golden Circle-> " + SpecialSeat::location();
}



//Disable Podium
//note base_price is fixed
DisablePodiumSeat::DisablePodiumSeat(int chair_num, int line_num,int base_price)
:SpecialSeat(chair_num,line_num,-1){}

DisablePodiumSeat::DisablePodiumSeat(int chair_num, int line_num)
:SpecialSeat(chair_num, line_num, -1){}


string DisablePodiumSeat::location() const
{
	return "Disable Podium-> " + SpecialSeat::location();
}

//price is fixed for 200
int DisablePodiumSeat::price() const
{
	return 200;
}

//RegularSeat-------------------------------------------------------------------
RegularSeat::RegularSeat(char area, int chair, int line, int base_price) :
	MainHallSeat(chair, line, base_price), area(area) {}

string RegularSeat::location() const
{
	return "area: " + to_string(area) + " " + MainHallSeat::location();
}


//FrontRegularSeat--------------------------------------------------------------
FrontRegularSeat::FrontRegularSeat(char area, int chair, int line,
	int base_price) : RegularSeat(area, chair,
		line, base_price + 500) {}

string FrontRegularSeat::location() const
{
	return "Front-> " + RegularSeat::location();
}


//MiddleRegularSeat-------------------------------------------------------------
MiddleRegularSeat::MiddleRegularSeat(char area, int chair, int line,
	int base_price) : RegularSeat(area, chair,
		line, base_price + 250) {}

string MiddleRegularSeat::location() const
{
	return "Middle-> " + RegularSeat::location();
}


//RearRegularSeat---------------------------------------------------------------
RearRegularSeat::RearRegularSeat(char area, int chair, int line,
	int base_price) : RegularSeat(area, chair,
		line, base_price) {}

string RearRegularSeat::location() const
{
	return "Rear-> " + RegularSeat::location();
}
