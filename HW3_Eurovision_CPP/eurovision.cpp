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
	if (timeLength != 0)
		this->song_duration = timeLength;
	if (singer != "")
		this->singer_name = singer;
}
//Voter-------------------------------------------------------------------------

Voter::Voter(string state, VoterType voterType) : state_name(state), 
times_of_votes(0) 
{	
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
	states(new string[10]{ s1, s2, s3, s4, s5, s6, s7, s8, s9, s10 }){}

Vote::~Vote()
{
	delete[] states;
}

MainControl& MainControl::operator+=(const Vote &v)
{
	if (this->phase != Voting) return *this;

	//Regular:
	if (v.vr.voterType() == Regular) {
		if (v.vr.timesOfVotes() == maxRegularVotes) return *this;
		if (v.states[0] == "") return *this;
		if (!participate(v.vr.state()) || !participate(v.states[0])) return *this;
		if (v.vr.state() == v.states[0]) return *this;

		int i = 0;
		while ((i < this->maxParticipants) && (contest_arr[i].participant_ptr != nullptr))
		{
			if (contest_arr[i].participant_ptr->state() == v.states[0])
			{
				contest_arr[i].reg_votes += 1;
				++(v.vr);
				return *this;
			}
			i++;
		}
	}

	//Judges:
	if (v.vr.timesOfVotes() > 0) return *this;
	if (!participate(v.vr.state())) return *this;

	bool judge_did_vote = false;
	for (int i = 0; i < 10; ++i)
	{
		if (v.states[i] == "") break;
		if (v.states[i] == v.vr.state()) continue;
		if (!participate(v.states[i])) continue;

		int j = 0;
		while ((j < this->maxParticipants) && (contest_arr[j].participant_ptr != nullptr))
		{
			if (contest_arr[j].participant_ptr->state() == v.states[i])
			{
				if (i == 0) contest_arr[j].judge_votes += 12;
				if (i == 1) contest_arr[j].judge_votes += 10;
				else contest_arr[j].judge_votes += (10 - i);
				judge_did_vote = true;
			}
			j++;
		}

		if (judge_did_vote) ++(v.vr);
	}
	return *this;
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

bool MainControl::legalParticipant(const Participant& p) const
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

int MainControl::getParticipatingNum() const
{
	int i = 0;
	while(this->contest_arr[i].participant_ptr)
	{
		i++;
	}
	return i;
}

bool MainControl::isParticipantRegistered(const Participant& participant)
{
	int i = 0;
	while(contest_arr[i].participant_ptr != nullptr)
	{
		if (contest_arr[i].participant_ptr->state() == participant.state() &&
			contest_arr[i].participant_ptr->singer() == participant.singer() &&
			contest_arr[i].participant_ptr->song() == participant.song() &&
			contest_arr[i].participant_ptr->timeLength() == participant.timeLength())
			return true;

		i++;
	}
	return false;
}

bool MainControl::participate(string state_name) const
{
	int i = 0;
	while(this->contest_arr[i].participant_ptr != nullptr)
	{
		if(this->contest_arr[i].participant_ptr->state() == state_name)
			return true;

		i++;
	}
	return false;
}

// not ordered insertion
//MainControl& MainControl::operator+=(Participant& participant)
//{
////checks that all the prerequisites for the participant and eurovision are valid
//	const bool is_valid = this->legalParticipant(participant) && 
//		this->phase == Registration && !isContestFull() && 
//		!isParticipantRegistered(participant);
//
//	if (!is_valid) return *this;
//
////adding the participantWVotes member to the contest_array
//for (int i = 0; i < this->maxParticipants; ++i)
//{
//	if(this->contest_arr[i].participant_ptr==nullptr)
//	{
//		participant.updateRegistered(true);
//		this->contest_arr[i].participant_ptr = &participant;
//		break;
//	}
//}
//return *this;
//}

//ordered insertion:
MainControl& MainControl::operator+=(Participant& participant)
{
	//checks that all the prerequisites for the participant and eurovision are valid
	const bool is_valid = this->legalParticipant(participant) &&
		this->phase == Registration && !isContestFull() &&
		!isParticipantRegistered(participant);

	if (!is_valid) return *this;


	//adding the participantWVotes member to the contest_array

	int i = 0;
	//advancing i till the place where it's need to be added
	while (this->contest_arr[i].participant_ptr != nullptr &&
			this->contest_arr[i].participant_ptr->state()
			.compare(participant.state()) < 0)
		i++;

	//separate into 2 cases:
	int j = i;
	if (this->contest_arr[j].participant_ptr == nullptr)
	{
		participant.updateRegistered(true);
		this->contest_arr[j].participant_ptr = &participant;
		return *this;
	}
	else
	{
		//advancing the i towards the last participant (not the end of the arr)
		while (this->contest_arr[i].participant_ptr != nullptr)
			i++;
		//running from the end backwards and swapping with nullptr(thus making 1 free slot to enter the new state)
		while(i>j)
		{
			swap(contest_arr[i], contest_arr[i - 1]);
			i--;
		}
		// reached to the place where needs to add the participant
		//and adding it
		//todo: check if i didn't miss by +1 / -1 the correct index
		participant.updateRegistered(true);
		contest_arr[i].participant_ptr = &participant;
		return *this;
	}

}


MainControl& MainControl::operator-=(Participant& participant)
{
	const bool is_valid_state = this->phase == Registration &&
		this->isParticipantRegistered(participant);
	if (!is_valid_state) return *this;

	
	//the state is registered for sure (was checked)
	int i = 0;
	while(this->contest_arr[i].participant_ptr->state() != participant.state())		
		i++;
	
	//exist the loop and remove the current state 
	participant.updateRegistered(false);
	this->contest_arr[i].participant_ptr = nullptr;

	//update the order in the array accordingly
	while(this->contest_arr[i+1].participant_ptr != nullptr)
	{
		swap(this->contest_arr[i], this->contest_arr[i + 1]);
		i++;
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
	switch (v.voterType())
	{
	//	case 0: break;//todo: add ALL here when ready
		case 1:
			return os << '<' << v.state() << '/' << "Regular" << '>';
		case 2:
			return  os << '<' << v.state() << '/' << "Judge" << '>';
	}
	return os;
}




//prints the data of eurovision:
ostream& operator<<(ostream& os, const MainControl& eurovision) {

	os << "{" << endl;

	if (eurovision.phase == Registration)
		{
		os << "Registration" << endl;
			for (int i = 0; i < eurovision.maxParticipants; i++)
			{
				if (eurovision.contest_arr[i].participant_ptr == nullptr) break;
				os << "[" << eurovision.contest_arr[i].participant_ptr->state() << "/";
				os << eurovision.contest_arr[i].participant_ptr->song() << "/";
				os << eurovision.contest_arr[i].participant_ptr->timeLength() << "/";
				os << eurovision.contest_arr[i].participant_ptr->singer() << "]" << endl;
			}

			os << "}" << endl;
			return os;
		}
	else
	{
		os << "Voting" << endl;
		for (int i = 0; i < eurovision.maxParticipants; i++)
		{
			if (eurovision.contest_arr[i].participant_ptr == nullptr) break;
			os << eurovision.contest_arr[i].participant_ptr->state() << " : ";
			os << "Regular(" << eurovision.contest_arr[i].reg_votes << ") ";
			os << "Judge(" << eurovision.contest_arr[i].judge_votes << ")" <<endl;
		}

		os << "}" << endl;
		return os;
	}
}

