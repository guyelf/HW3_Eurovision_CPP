
//nirnirnir
#include <iostream>
#include "eurovision.h"

using std::string;
using std::ostream;
using std::istream;

Voter::Voter(string state, VoterType voterType) {
    this->state_name = state;
    this->voter_type = voterType;
}

string Voter::state() const {
    return this->state_name;
}

VoterType Voter::voterType() const {
    return this->voter_type;
}

int Voter::timesOfVotes() const {
    return this->times_of_votes;
}

Voter& Voter::operator++() {
    this->times_of_votes += 1;
    return *this;
}

ostream& operator<<(ostream& os ,const Voter& v){
    return os << '<' << v.state() << '/' << v.voterType() << '>';
}