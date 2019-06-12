
#include <iostream>
#include "eurovision.h"

using std::string;
using std::ostream;
using std::istream;

Voter::Voter(string state, VoterType voterType): state_name(state) {
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
    if (this->voter_type == Regular){
        this->times_of_votes += 1;
    }
    return *this;
}

ostream& operator<<(ostream& os ,const Voter& v){
    return os << '<' << v.state() << '/' << v.voterType() << '>';
}

///////////////////////////////////VOTE.CPP
Vote::Vote(Voter& vr, string s1, string s2, string s3, string s4, string s5,
           string s6, string s7, string s8, string s9, string s10): vr(vr),
                                                                    states {s1, s2, s3, s4, s5, s6, s7, s8, s9, s10}{
}