#include <iostream>
#include <string>
using std::string;
using std::runtime_error;


//NoPrice-----------------------------------------------------------------------
class NoPrice:public runtime_error
{

public:
    NoPrice():runtime_error("Not For Sale !"){}
    virtual ~NoPrice()=default;
};


//Seat--------------------------------------------------------------------------
class Seat
{

protected:
    int chairNum;
    int lineNum;
    int basePrice;

public:
    Seat(int chair_num, int line_num, int base_price): chairNum(chair_num),
                                                       lineNum(line_num)
                                                       ,basePrice(base_price){}
    virtual ~Seat() = default;

    virtual int price() const;
    virtual string location() const = 0;
};


//GreenRoom---------------------------------------------------------------------
class GreenRoomSeat:public Seat
{
public:
    GreenRoomSeat(int line, int chair);
    //override to throw exception
    int price() const override;
    string location() const override;
};


//MainHall----------------------------------------------------------------------
class MainHallSeat:public Seat
{

public:
    //use the c'tor of Seat and add 100 to the base_price
    MainHallSeat(int chair_num, int line_num, int base_price);
    virtual ~MainHallSeat() = default;
};


//SpecialSeat-------------------------------------------------------------------
class SpecialSeat:public MainHallSeat
{
public:

};


//GoldenCircleSeat--------------------------------------------------------------
class GoldenCircleSeat:public SpecialSeat
{
};


//DisablePodiumSeat-------------------------------------------------------------
class DisablePodiumSeat:public SpecialSeat
{

};


//RegularSeat-------------------------------------------------------------------
class RegularSeat:public MainHallSeat
{

protected:
    char area;
public:
    RegularSeat(char area, int chair, int line, int base_price);
    virtual ~RegularSeat() = default;
    string location() const override;
};


//FrontRegularSeat--------------------------------------------------------------
class FrontRegularSeat:public RegularSeat
{
public:
    FrontRegularSeat(char area, int chair, int line, int base_price);
    ~FrontRegularSeat() = default;
    string location() const override;
};


//MiddleRegularSeat-------------------------------------------------------------
class MiddleRegularSeat:public RegularSeat
{
public:
    MiddleRegularSeat(char area, int chair, int line, int base_price);
    ~MiddleRegularSeat() = default;
    string location() const override;
};


//RearRegularSeat---------------------------------------------------------------
class RearRegularSeat:public RegularSeat
{
public:
    RearRegularSeat(char area, int chair, int line, int base_price);
    ~RearRegularSeat() = default;
    string location() const override;
};

