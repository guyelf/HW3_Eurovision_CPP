#ifndef EUROVISION_H_
#define EUROVISION_H_

#include <iostream>
#include <string>
#include <list>
#include <iterator>

// it's allowed to define here any using statements, according to needs.
// do NOT define here : using namespace std;

using std::string;
using std::ostream;
using std::istream;
using std::endl;
using std::swap;
//---------------------------------------------------
enum VoterType { All, Regular, Judge };
enum Phase { Registration, Contest, Voting };


//---------------------------------------------------

class Participant
{
	// relevant private members can be defined here, if necessary.

	const string state_name;
	string song_name;
	int song_duration;
	string singer_name;
	bool is_registered;

public:

	string state()const;
	string song() const;
	int timeLength() const;
	string singer() const;
	bool isRegistered() const;

	Participant(const string state, const string song, const int timeLength, const string singer);
	~Participant() = default;
	Participant(Participant& p) = delete;

	void updateRegistered(bool status);
	void update(const string song, const int timeLength, const string singer);


	// need to define here possibly c'tr and d'tr and ONLY methods that
	// are mentioned and demonstrated in the test example that has been published.
	// NO OTHER METHODS SHOULD APPEAR HERE.

	// NO friend is allowed here.
};

//---------------------------------------------------


class Voter
{
	// relevant private members can be defined here, if necessary.
	const string state_name;
	VoterType voter_type;
	int times_of_votes;

public:

	Voter(string state, VoterType voterType = Regular);
	~Voter() = default;
	Voter(Voter& v) = delete;

	string state() const;
	VoterType voterType() const;
	int timesOfVotes() const;

	Voter& operator++();

	// need to define here possibly c'tr and d'tr and ONLY methods that
	// are mentioned and demonstrated in the test example that has been published.
	// NO OTHER METHODS SHOULD APPEAR HERE.

	// NO friend is allowed here.

};


// -----------------------------------------------------------
struct ParticipantWVotes
{
	Participant* participant_ptr;
	int reg_votes;
	int judge_votes;

	explicit ParticipantWVotes(Participant* p = nullptr, int regular_votes = 0, int judge_votes = 0);
	//ParticipantWVotes(ParticipantWVotes& s) = delete;
	~ParticipantWVotes() = default;
};

//Helper function: swap for ParticipantWvotes
//Helper Swap - still requires a bit of modification for it run.
//void swap(struct ParticipantWithVotes* p1, struct ParticipantWithVotes* p2);

// -----------------------------------------------------------
struct Vote
{
	Voter& vr;
	string* states;

	Vote(Voter& vr, string s1 = "", string s2 = "", string s3 = "",
		string s4 = "", string s5 = "", string s6 = "", string s7 = "",
		string s8 = "", string s9 = "", string s10 = "");

	Vote(Vote& v) = delete;
	Vote& operator=(const Vote&) = delete;
	~Vote();
	// ALL is public here.
	// need to define ONLY data members and c'tr and d'tr.
	// NO NEED to define anything else.
};

// -----------------------------------------------------------


class MainControl
{
	// relevant private members can be defined here, if necessary.
	Phase phase;

	const int maxSongLength;
	const int maxParticipants;
	const int maxRegularVotes;
	ParticipantWVotes* contest_arr;

	int _iterator = 0;
	ParticipantWVotes* cur_iterator;

	//checks if the contest array is full
	bool isContestFull();
	//checks if the given participant is already registered in the system
	bool isParticipantRegistered(const Participant& participant);
	int getParticipatingNum() const;
public:

	MainControl(int maxSongLength = 180, int maxParticipants = 26, int maxRegularVotes = 5);
	void setPhase(Phase phase);
	~MainControl();

	bool legalParticipant(const Participant& p) const;

	//checks if the given state participates in the Eurovision or not
	bool participate(string state_name) const;

	MainControl& operator+=(Participant& participant);
	MainControl& operator-=(Participant &participant);

	MainControl& operator+=(const Vote& vote);
	//B.3
	string operator()(const int place, const VoterType vt) const;


	friend ostream& operator<<(ostream& os, const MainControl& eurovision);

	typedef ParticipantWVotes* Iterator;

	Iterator begin();
	Iterator end();
	bool operator==(const Iterator& iterator) const;
	Iterator& operator++();

};

ostream& operator<<(ostream& os, const Participant& p);
ostream& operator<<(ostream& os, const Voter& v);
ostream& operator<<(ostream& os, const MainControl& eurovision);


//ostream& operator<<(ostream& os, const MainControl::Iterator& iterator);
ostream& operator<<(ostream& os, const ParticipantWVotes& pwv);
// -----------------------------------------------------------
//Part B:





//B.1
using std::list;
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
	if (size < place) //come back here to add the <= sign
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
// -----------------------------------------------------------



#endif
