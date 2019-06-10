#ifndef EUROVISION_H_
#define EUROVISION_H_

#include <iostream>

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

	 const string state;
	 bool isRegistered;

public:

	string song;
	int timeLength;
	string singer;
	
	Participant(const string state,const string song,const int timeLength,const string singer);
	~Participant() = default;
	Participant(Participant& p) = delete;
	
	void updateRegistered(bool status);
	void update(const string song,const int timeLength,const string singer);

	 //todo: maybe needs to add the field ostream& os like in the presentation
	ostream& operator<<(const Participant& p);

	 //todo: overload << so you can print according to the format 
	// need to define here possibly c'tr and d'tr and ONLY methods that
	// are mentioned and demonstrated in the test example that has been published.
	// NO OTHER METHODS SHOULD APPEAR HERE.

	// NO friend is allowed here.
};

//---------------------------------------------------


class Voter
{
	// relevant private members can be defined here, if necessary.
	string state;
	VoterType voterType;

public:

	Voter(string state, VoterType voterType = Regular);
	~Voter() = default;
	Voter(Voter& v) = delete;
	//todo: provide getters for state & voterType	

	int timesOfVotes;
	//todo: overload ++ for timesOfVotes

	// need to define here possibly c'tr and d'tr and ONLY methods that
	// are mentioned and demonstrated in the test example that has been published.
	// NO OTHER METHODS SHOULD APPEAR HERE.

	// NO friend is allowed here.

};


// -----------------------------------------------------------

struct Vote
{
	Voter vr;

	//todo: how to save votes from voter. array? where?

	Vote(Voter vr, string state);// remember to check for judge here too
	Vote(Voter vr, string s1 = "", string s2 = "", string s3 = "", 
		string s4 = "",string s5 = "", string s6 = "", string s7 = "", 
		string s8 = "",string s9 = "", string s10 = ""); //overload for judge
	~Vote() = default;
	Vote(Vote& v) = delete;

	
	// ALL is public here.
	// need to define ONLY data members and c'tr and d'tr.
	// NO NEED to define anything else.

};

// -----------------------------------------------------------


class MainControl
{
	// relevant private members can be defined here, if necessary.
	Phase phase;

	int maxSongLength;
	int maxParticipants;
	int maxRegularVotes;

public:

	MainControl(int maxSongLength=180, int maxParticipants=26, int maxRegularVotes=5);
	void setPhase(Phase phase);


	/* the operation legalParticipant checks if all the following exists :
   the names of the state and the song and the singer are not empty "",
   as well as time length of the song is not big than the max allowed time length
   for any song. this max allowed value is determined when defining the
   system object. default is 180 (sec).
*/
	bool legalParticipant(Participant p);

	//checks if the given state participates in the Eurovision or not
	bool participate(string state_name);
	//todo: operator oveloading: 1. += for adding participants - register them
	//todo/////////////////////  2. += for voting in the system
	//todo/////////////////////  3. -= for voting in the system
	//todo/////////////////////  4. << for printing the info of eurovision according to the requirement in test1.cpp


	// need to define here possibly c'tr and d'tr and ONLY methods that
	// are mentioned and demonstrated in the test example that has been published.
	// NO OTHER METHODS SHOULD APPEAR HERE.

	// Also it's allowed here to define friend.

};

// -----------------------------------------------------------

#endif

