#ifndef SEAT_H_
#define SEAT_H_

#include <string>

using std::string;
using std::exception;

//NoPrice-------------------------------------------------------------

class NoPrice : public exception {

public:
    char const *what() const noexcept override; //Error message in
    //case of error
    virtual ~NoPrice() = default;
};

//Seat----------------------------------------------------------------

class Seat {

protected:
    int lineNum;
    int chairNum;
    int basePrice;

    Seat(int line_num, int chair_num, int base_price) : lineNum(line_num),
                                                        chairNum(chair_num),
                                                        basePrice(base_price) {}

public:
    virtual ~Seat() = default;

    //Provides information about chair price
    virtual int price() const;

    //Provides information about type, area (if relevant), row and chair num
    virtual string location() const;
};

//GreenRoom-----------------------------------------------------------

class GreenRoomSeat : public Seat {
public:
    GreenRoomSeat(int line, int chair) : Seat(line, chair, -1) {}

    //override to throw exception
    int price() const override;

    string location() const override;
};

//Main Hall-----------------------------------------------------------

class MainHallSeat : public Seat {
protected:
//use the c'tor of Seat and add 100 to the base_price
    MainHallSeat(int line_num, int chair_num, int base_price)
            : Seat(line_num, chair_num, base_price + 100) {}

public:
    virtual ~MainHallSeat() = default;
};

//SpecialSeat---------------------------------------------------------

class SpecialSeat : public MainHallSeat {
protected:
    SpecialSeat(int line_num, int chair_num, int base_price)
            : MainHallSeat(line_num, chair_num, base_price + 300) {}

public:
    virtual ~SpecialSeat() = default;
};

//Golden Circle-------------------------------------------------------

class GoldenCircleSeat : public SpecialSeat {
public:
    GoldenCircleSeat(int line_num, int chair_num, int base_price)
            : SpecialSeat(line_num, chair_num, base_price + 1000) {}

    ~GoldenCircleSeat() = default;

    string location() const override;
};

//DisablePodium-------------------------------------------------------

class DisablePodiumSeat : public SpecialSeat {

public:
    DisablePodiumSeat(int line_num, int chair_num, int base_price)
            : SpecialSeat(line_num, chair_num, -1) {}

    DisablePodiumSeat(int line_num, int chair_num)
            : SpecialSeat(line_num, chair_num, -1) {}

    string location() const override;

    int price() const override;
};

//RegularSeat---------------------------------------------------------

class RegularSeat : public MainHallSeat {
protected:
    char area;

    RegularSeat(char area, int line, int chair, int base_price)
            : MainHallSeat(line, chair, base_price), area(area) {}

public:
    virtual ~RegularSeat() = default;

    string location() const override;
};

//FrontRegularSeat----------------------------------------------------

class FrontRegularSeat : public RegularSeat {
public:
    FrontRegularSeat(char area, int line, int chair, int base_price)
            : RegularSeat(area, line, chair, base_price + 500) {}

    ~FrontRegularSeat() = default;

    string location() const override;
};

//MiddleRegularSeat---------------------------------------------------

class MiddleRegularSeat : public RegularSeat {
public:
    MiddleRegularSeat(char area, int line, int chair, int base_price)
            : RegularSeat(area, line, chair, base_price + 250) {}

    ~MiddleRegularSeat() = default;

    string location() const override;
};

//RearRegularSeat-----------------------------------------------------

class RearRegularSeat : public RegularSeat {
public:
    RearRegularSeat(char area, int line, int chair, int base_price)
            : RegularSeat(area, line, chair, base_price) {}

    ~RearRegularSeat() = default;

    string location() const override;
};

#endif
