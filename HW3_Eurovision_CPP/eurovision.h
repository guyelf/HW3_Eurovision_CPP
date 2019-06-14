#ifndef EUROVISION_H_
#define EUROVISION_H_

#include <iostream>
#include <string>

// it's allowed to define here any using statements, according to needs.
// do NOT define here : using namespace std;

using std::string;
using std::ostream;
using std::istream;
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

	Participant(const string state,const string song,const int timeLength,const string singer);
	~Participant() = default;
	Participant(Participant& p) = delete;
	
	void updateRegistered(bool status);
	void update(const string song,const int timeLength,const string singer);


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

	explicit ParticipantWVotes(Participant* p= nullptr, int regular_votes = 0, int judge_votes = 0);
	ParticipantWVotes(ParticipantWVotes& s) = delete;
	~ParticipantWVotes() = default;
};


// -----------------------------------------------------------
struct Vote
{
	Voter& vr;
	string states [10];

	Vote(Voter& vr, string s1 = "", string s2 = "", string s3 = "", 
		string s4 = "",string s5 = "", string s6 = "", string s7 = "", 
		string s8 = "",string s9 = "", string s10 = ""); 

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

public:

	MainControl(int maxSongLength=180, int maxParticipants=26, int maxRegularVotes=5);
	void setPhase(Phase phase);
	~MainControl();

	bool legalParticipant(Participant p);

	//checks if the given state participates in the Eurovision or not
	bool participate(string state_name);

	MainControl& operator+=(const Participant& p);
	MainControl& operator+=(const Vote& v);


	/*
  unregister p5 from the system.
  it is allowed to do this only in the Registration phase.
  Moreover, if p5 is not registered, then just ignore the operation without
  further notice.

  can also do operations like

  (((eurovision -= p5) -= p3) += p4) -= p4;

*/
	MainControl& operator-=(const Vote& v);


	// need to define here possibly c'tr and d'tr and ONLY methods that
	// are mentioned and demonstrated in the test example that has been published.
	// NO OTHER METHODS SHOULD APPEAR HERE.

	// Also it's allowed here to define friend.

};

ostream& operator<<(ostream& os ,const Participant& p);


//todo: remove all the comments below after done implementing the overload
/*
  displays the main features of the system.
  All the display is between curly brackets ({ in the first line, then continue
  to next line, then all the relevant data, then } in the last line.
  first line after { displays the phase - can be one of Registration or Contest or Voting.
  if phase is Registration, then all Registered participant are displayed
  sorted regarding state name. each state in a different line, in the same format
  as operator<< to a participant.
  if phase is Contest, nothing more is displayed.
  if phase is Voting, then all votes (both Regular and Judge) are displayed,
  in respect to all states of registered participants.
  each state in a different line. the states are sorted in respect to state name.

  example :
{
Registration
[Australia/Song_Australia/180/Singer_Australia]
[Cyprus/Song_Cyprus/172/Singer_Cyprus]
[Israel/Song_Israel/175/Singer_Israel]
[UK/Song_UK/170/Singer_UK]
}

  another example relevant to Voting phase - explanation will follow later :
{
Voting
Australia : Regular(0) Judge(8)
Cyprus : Regular(6) Judge(12)
Israel : Regular(1) Judge(0)
UK : Regular(1) Judge(10)
}

*/
ostream& operator<<(ostream& os, const MainControl& main_control);
ostream& operator<<(ostream& os, const Voter& v);


// -----------------------------------------------------------

#endif

