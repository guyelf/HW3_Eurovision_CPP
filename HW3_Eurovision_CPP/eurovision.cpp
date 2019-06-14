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

bool MainControl::legalParticipant(const Participant& p)
{
	bool illigal_result = p.timeLength() > this->maxSongLength || 
			p.state() == "" || p.song() == "" || p.singer() == "";

	return !illigal_result;
}

bool MainControl::isContestFull()
{
	for (int i = 0; i < this->maxParticipants; i++)
	{
		if (contest_arr[i].participant_ptr == nullptr)
			return false;
	}
	return true;
}

bool MainControl::isParticipantRegistered(const Participant& participant)
{
	for (int i = 0; i <this->maxParticipants; ++i)
	{
		if (contest_arr[i].participant_ptr->state() == participant.state() &&
			contest_arr[i].participant_ptr->singer() == participant.singer() &&
			contest_arr[i].participant_ptr->song() == participant.song() &&
			contest_arr[i].participant_ptr->timeLength() == participant.timeLength())
			return true;		
	}
	return false;
}



MainControl& MainControl::operator+=(Participant& participant)
{
//checks that all the prerequisites for the participant and eurovision are valid
	const bool is_valid = this->legalParticipant(participant) && 
		this->phase == Registration && !isContestFull() && 
		!isParticipantRegistered(participant);

	if (!is_valid) return *this;

//adding the participantWVotes member to the contest_array
for (int i = 0; i < this->maxParticipants; ++i)
{
	if(this->contest_arr[i].participant_ptr==nullptr)
	{
		participant.updateRegistered(true);
		this->contest_arr[i].participant_ptr = &participant;
		break;
	}
}
return *this;
}

MainControl& MainControl::operator-=(Participant& participant)
{
	const bool is_valid_state = this->phase == Registration &&
		this->isParticipantRegistered(participant);
	if (!is_valid_state) return *this;

	for (int i = 0; i < maxParticipants; i++)
	{
		if(this->contest_arr[i].participant_ptr->state() == participant.state())
		{
			participant.updateRegistered(false);
			this->contest_arr[i].participant_ptr = nullptr;
			break;
		}
	}
	return *this;
}


MainControl::~MainControl()
{
	delete[] this->contest_arr;
}

//ParticipantWVotes -----------------------------------------------------------

ParticipantWVotes::ParticipantWVotes(Participant* p, int regular_votes,
													 int judge_votes) :
	participant_ptr(p), reg_votes(regular_votes), judge_votes(judge_votes) {}



// overloading of the << operator-----------------------------------------------

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


//Main ------------------------------------------------------------------------
int main(int argc, char* argv[])
{
	
}


