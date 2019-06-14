#include "eurovision.h"

//Participant-------------------------------------------------------------------

Participant::Participant(const string state, const string song, const int timeLength, const string singer)
        :state_name(state), song_name(song), song_duration(timeLength), singer_name(singer)
{
    is_registered = false;
}

bool Participant::isRegistered()const
{
    return this->is_registered;
}

string Participant::state() const
{
    return this->state_name;
}

string Participant::song() const
{
    return this->song_name;
}

string Participant::singer() const
{
    return this->singer_name;
}

int Participant::timeLength() const
{
    return this->song_duration;
}


void Participant::updateRegistered(bool status)
{
    this->is_registered = status;
}

void Participant::update(const string song, const int timeLength, const string singer)
{
    if (isRegistered()) return;
    if (song != "")
        this->song_name = song;
    if (singer != "")
        this->song_duration = timeLength;
    if (timeLength != 0)
        this->singer_name = singer;
}
//Voter-------------------------------------------------------------------------

Voter::Voter(string state, VoterType voterType) : state_name(state) {
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
    if (this->voter_type == Regular) {
        this->times_of_votes += 1;
    }
    return *this;
}
//Vote--------------------------------------------------------------------------

Vote::Vote(Voter& vr, string s1, string s2, string s3, string s4, string s5,
           string s6, string s7, string s8, string s9, string s10) : vr(vr),
                                                                     states{ s1, s2, s3, s4, s5, s6, s7, s8, s9, s10 }{}

Vote::~Vote()
{
    delete[] states;
}



//MainControl-------------------------------------------------------------------
MainControl::MainControl(int maxSongLength, int maxParticipants,
                         int maxRegularVotes) : maxSongLength(maxSongLength),
                                                maxParticipants(maxParticipants),
                                                maxRegularVotes(maxRegularVotes)
{
    this->contest_arr = new ParticipantWVotes[maxParticipants];
    this->phase = Registration;
}

void MainControl::setPhase(Phase phase)
{
    if ((phase - this->phase) != 1) return;
    this->phase = phase;
}

bool MainControl::legalParticipant(Participant p)
{
    bool illigal_result = p.timeLength() > this->maxSongLength ||
                          p.state() == "" || p.song() == "" || p.singer() == "";

    return !illigal_result;
}

MainControl::~MainControl()
{
    delete[] this->contest_arr;
}

MainControl& MainControl::operator+=(const Vote &v)
{
    Voter* voter_p = &(v.vr);

    //Regular:
    if (voter_p->voterType() == Regular){

        if(voter_p->timesOfVotes() == maxRegularVotes)
            return *this;
        if(!participate(voter_p->state()) || !participate(v.states[0]))
            return *this;
        if (voter_p->state() == v.states[0]) //todo: not sure if it's enough
            return *this;

        for (int i=0; i<(this->maxParticipants); ++i)
        {
            if (contest_arr[i].participant_ptr->state() == v.states[0]){ //todo: not sure if it's enough
                contest_arr[i].reg_votes += 1;
                voter_p->operator++(); //todo: not sure if it's enough
                return *this;
            }
        }
    }

    //Judges:
    if(voter_p->timesOfVotes() == 1)
        return *this;
    if(!participate(voter_p->state()))
        return *this;

    for (int i=0; i<10; ++i)
    {
        if(v.states[i] == voter_p->state())
            continue;
        if(!participate(v.states[i]))
            continue;

        bool judge_did_vote = false;
        for (int j=0; j<(this->maxParticipants); ++j)
        {
            if (contest_arr[j].participant_ptr->state() == v.states[i]) { //todo: not sure if it's enough
                if (i==0)
                    contest_arr[j].judge_votes += 12; // todo: define?
                if (i==1)
                    contest_arr[j].judge_votes += 10;
                else
                    contest_arr[j].judge_votes += (10-i);
                judge_did_vote = true;
            }
        }

        if (judge_did_vote)
            voter_p->operator++(); //todo: not sure if it's enough
    }
    return *this;
}



//eurovision--------------------------------------------------------------------

ParticipantWVotes::ParticipantWVotes(Participant* p, int regular_votes,
                                     int judge_votes) :
        participant_ptr(p), reg_votes(regular_votes), judge_votes(judge_votes) {}



// overload of the << operator--------------------------------------------------

//prints the data of a participant:
ostream& operator<<(ostream& os, const Participant& p)
{
    string const part1 = "[" + p.state() + "/" + p.song() + "/";
    string const part2 = "/" + p.singer() + "]";

    return os << part1 << p.timeLength() << part2;
}

//prints the data of a voter
ostream& operator<<(ostream& os, const Voter& v) {
    return os << '<' << v.state() << '/' << v.voterType() << '>';
}

int main(int argc, char* argv[])
{

}
