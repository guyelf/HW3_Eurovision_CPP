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

string MainHallSeat::location() const
{
	return "Main Hall-> " + this->location();
}
