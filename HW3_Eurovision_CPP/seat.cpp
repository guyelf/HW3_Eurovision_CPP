#include "seat.h"

using std::to_string;


//NoPrice
char const* NoPrice::what() const noexcept
{
	return "Not For Sale !";
}

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
int GreenRoomSeat::price() const
{
	throw NoPrice();
}

string GreenRoomSeat::location() const
{
	return "Green Room-> " + Seat::location();
}


//Main Hall

//c'tor defined in the .h file

//special seat

//c'tor

//Golden Circle
string GoldenCircleSeat::location() const
{
	return "Golden Circle-> " + SpecialSeat::location();
}

//Disable Podium
//note base_price is fixed

//c'tor for 3 params 
//c'tor for 2 params


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

string RegularSeat::location() const
{
	string area_string = string(1,area);
	return "area: " + area_string + ", " + MainHallSeat::location();
}


//FrontRegularSeat--------------------------------------------------------------

string FrontRegularSeat::location() const
{
	return "Front-> " + RegularSeat::location();
}


//MiddleRegularSeat-------------------------------------------------------------

string MiddleRegularSeat::location() const
{
	return "Middle-> " + RegularSeat::location();
}


//RearRegularSeat---------------------------------------------------------------

string RearRegularSeat::location() const
{
	return "Rear-> " + RegularSeat::location();
}
