#include "eurovision.h"
#include <list>
#include <vector>
#include <iterator>
using std::list;
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
	this->times_of_votes += 1;
	return *this;
}
//Vote--------------------------------------------------------------------------

Vote::Vote(Voter& vr, string s1, string s2, string s3, string s4, string s5,
	string s6, string s7, string s8, string s9, string s10) : vr(vr),
	states(new string[10]{ s1, s2, s3, s4, s5, s6, s7, s8, s9, s10 }) {}

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
	
	bool judge_vote = false;
	for (int i = 0; i < 10; ++i)
	{
		if (v.states[i] == "") break; //end of the existing states
		if (v.states[i] == v.vr.state()) continue;
		if (!participate(v.states[i])) continue;

		int j = 0;
		while ((j < this->maxParticipants) && (contest_arr[j].participant_ptr != nullptr))
		{
			if (contest_arr[j].participant_ptr->state() == v.states[i])
			{
				if (i == 0) contest_arr[j].judge_votes += 12;
				else if (i == 1) contest_arr[j].judge_votes += 10;
				else contest_arr[j].judge_votes += (10 - i);

				judge_vote = true;
			}
			j++;
		}
	}

	if (judge_vote) ++v.vr;
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
	while (this->contest_arr[i].participant_ptr)
	{
		i++;
	}
	return i;
}

bool MainControl::isParticipantRegistered(const Participant& participant)
{
	int i = 0;
	while (contest_arr[i].participant_ptr != nullptr)
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
	while (this->contest_arr[i].participant_ptr != nullptr)
	{
		if (this->contest_arr[i].participant_ptr->state() == state_name)
			return true;

		i++;
	}
	return false;
}

//ordered insertion:
MainControl& MainControl::operator+=(Participant& participant)
{
	//checks that all the prerequisites for the participant and eurovision are valid
	const bool is_valid = this->legalParticipant(participant) &&
		this->phase == Registration && !isContestFull() &&
		!participate(participant.state());

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
		while (i > j)
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
	while (this->contest_arr[i].participant_ptr->state() != participant.state())
		i++;

	//exist the loop and remove the current state
	participant.updateRegistered(false);
	this->contest_arr[i].participant_ptr = nullptr;

	//update the order in the array accordingly
	while (this->contest_arr[i + 1].participant_ptr != nullptr)
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

//Helper function swap for ParticipantWvotes
//still requires a bit of modification in order for it to run
//void swap(ParticipantWithVotes* p1, ParticipantWithVotes* p2)
//{
//	ParticipantWithVotes temp = *p1;
//	*p1 = *p2;
//	*p2 = temp;
//}

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
	case 0: 
		return os << "ALL Option was chosen";//todo: fix appropriately
	case 1:
		return os << '<' << v.state() << '/' << "Regular" << '>';
	case 2:
		return  os << '<' << v.state() << '/' << "Judge" << '>';
	default:
		return os << "ENTERED DEFAULT IN ENUM";
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
			os << "Judge(" << eurovision.contest_arr[i].judge_votes << ")" << endl;
		}

		os << "}" << endl;
		return os;
	}
}


//B.1
template<typename T_Iterator>
T_Iterator Get(T_Iterator begin, T_Iterator end, int place)
{
	//validity check
	if (begin == end || place < 1)
		return end;

	int size = 0;
	//gets the value the type that the iterator points to
	list<typename std::iterator_traits<T_Iterator>::value_type> temp_list;

	for (T_Iterator i = begin; i != end; ++i)
	{
		temp_list.push_back(*i);
		size++;
	}

	//validity check- size is not available before the loop
	if (size <= place)
		return end;
	//list is sorted in ascending order
	temp_list.sort();

	for (int i = 0; i < place - 1; ++i)
	{
		temp_list.pop_back();
	}

	typename std::iterator_traits<T_Iterator>::value_type result_value = 
	temp_list.back();

	for (T_Iterator i = begin; i != end; ++i)
	{
		if (*i == result_value)
			return i;
	}
	return end;
}

//Part B.2


ParticipantWVotes* MainControl::begin()
{
	return this->contest_arr;
}

ParticipantWVotes* MainControl::end()
{
	int count = 0;
	while (this->contest_arr[count].participant_ptr != nullptr)
		count++;

	return (this->contest_arr+count);
}

MainControl::Iterator& MainControl::operator++()
{
	//Maybe a handle of out of memory exception is needed here
	this->_iterator++;
	Iterator temp = this->contest_arr + _iterator;
	return temp;
}


bool MainControl::operator==(const Iterator& i) const
{
	if ((this->contest_arr + _iterator)->participant_ptr->state() == i->participant_ptr->state())
		return true;

	return false;
}


ostream& operator<<(ostream& os, const ParticipantWVotes& pwv)
{
	return os << (*pwv.participant_ptr);
}


//Part B.3
using std::vector;
//string MainControl::operator()(const int place, const VoterType vt) const
//{
//	vector<int> votes;
//	int winner_votes;
//	string result = "";
//
//	if (vt == Regular)
//	{
//		int i = 0;
//		while (this->contest_arr[i].participant_ptr != nullptr) {
//			int temp_vote = this->contest_arr[i].reg_votes;
//			votes.push_back(temp_vote);
//			i++;
//		}
//		if (i == 0)
//			return result;
//		
//		auto get_result = Get(votes.begin(), votes.end(), place);
//		if (get_result == votes.end()) return result;
//
//		winner_votes = *get_result;
//
//		//i is pointing to the end location
//		for (--i; i >= 0; i--)
//		{
//			if (this->contest_arr[i].reg_votes == winner_votes) {
//				result = this->contest_arr[i].participant_ptr->state();
//				return result;
//			}
//		}
//	}
//	if (vt == Judge)
//	{
//		int i = 0;
//		while (this->contest_arr[i].participant_ptr != nullptr) {
//			int temp = this->contest_arr[i].judge_votes;
//			votes.push_back(temp);
//			i++;
//		}
//		if (i == 0)
//			return result;
//
//		auto get_result = Get(votes.begin(), votes.end(), place);
//		if (get_result == votes.end()) return result;
//
//		winner_votes = *get_result;
//
//		//i is pointing to the end location
//		for (--i; i >= 0; i--)
//		{
//			if (this->contest_arr[i].judge_votes == winner_votes) {
//				result = this->contest_arr[i].participant_ptr->state();
//				return result;
//			}
//		}
//	}
//	else
//	{
//		int i = 0;
//		while (this->contest_arr[i].participant_ptr != nullptr) {
//			int temp = (this->contest_arr[i].reg_votes) + (this->contest_arr[i].judge_votes);
//			votes.push_back(temp);
//			i++;
//		}
//		if (i == 0)
//			return result;
//
//		auto get_result = Get(votes.begin(), votes.end(), place);
//		if (get_result == votes.end()) return result;
//
//		winner_votes = *get_result;
//
//		//i is pointing to the end location
//		for (--i; i >= 0; i--)
//		{
//			if ((this->contest_arr[i].reg_votes) + (this->contest_arr[i].judge_votes) == winner_votes) {
//				result = this->contest_arr[i].participant_ptr->state();
//				return result;
//			}
//		}
//	}
//}
//


using std::vector;
string MainControl::operator()(const int place, const VoterType vt) const
{
	vector<int> votes;
	string result = "";

	int i = 0;
	while(this->contest_arr[i].participant_ptr != nullptr)
	{
		if (vt == Regular)
			votes.push_back(this->contest_arr[i].reg_votes);
		if (vt == Judge)
			votes.push_back(this->contest_arr[i].judge_votes);
		if(vt == All)
			votes.push_back(this->contest_arr[i].reg_votes +
								 this->contest_arr[i].judge_votes);
		i++;
	}
	//sanity check
	if (i == 0) 
		return result;

	//getting the desired place and checking if it's legal
	auto get_result = Get(votes.begin(), votes.end(), place);
	if (get_result == votes.end()) return result;

	//getting the value out of the place
	int winner_votes = *get_result;

	//i is pointing to the end location
	for (--i; i >= 0; i--)
	{
		if (vt == Regular && this->contest_arr[i].reg_votes == winner_votes) {
			result = this->contest_arr[i].participant_ptr->state();
			return result;
		}
		if (vt == Judge && this->contest_arr[i].judge_votes == winner_votes) {
			result = this->contest_arr[i].participant_ptr->state();
			return result;
		}
		if (vt == All && (this->contest_arr[i].reg_votes +
			this->contest_arr[i].judge_votes) == winner_votes) {
			result = this->contest_arr[i].participant_ptr->state();
			return result;
		}
	}
		return result;// result = "" didn't enter to any of the if statements
}





