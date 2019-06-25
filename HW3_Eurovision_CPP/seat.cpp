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
	return "Green Room->" + this->location();
}


//Main Hall
MainHallSeat::MainHallSeat(int chair_num, int line_num, int base_price)
						  :Seat(chair_num,line_num,base_price+100){}


//special seat
SpecialSeat::SpecialSeat(int chair_num, int line_num,int base_price)
:MainHallSeat(chair_num, line_num,base_price){}


//Golden Circle
GoldenCircleSeat::GoldenCircleSeat(int chair, int num, int base_price)
:SpecialSeat(chair, num,base_price+1000){}

string GoldenCircleSeat::location() const
{
	return "Golden Circle->" + SpecialSeat::location();
}



//Disable Podium
DisablePodiumSeat::DisablePodiumSeat(int chair_num, int line_num)
:SpecialSeat(chair_num,line_num,-1){}


string DisablePodiumSeat::location() const
{
	return "Disable Podium->" + SpecialSeat::location();
}

//price is fixed for 200
int DisablePodiumSeat::price() const
{
	return 200;
}

