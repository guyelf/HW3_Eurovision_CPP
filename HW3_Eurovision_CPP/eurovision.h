#ifndef EUROVISION_H_
#define EUROVISION_H_

#include <iostream>
#include <string>
#include <list>
#include <iterator>

//--------------------------------------------------------------------

using std::string;
using std::ostream;
using std::istream;
using std::endl;
using std::swap;

//--------------------------------------------------------------------

enum VoterType { All, Regular, Judge };
enum Phase { Registration, Contest, Voting };

//Participant---------------------------------------------------------

class Participant
{
    const string state_name;
    string song_name;
    int song_duration;
    string singer_name;
    bool is_registered;

public:

    //returns the state name
    string state()const;
    //returns the song name
    string song() const;
    //returns the time length of the song
    int timeLength() const;
    //returns the singer name
    string singer() const;
    //returns whether the participated country is registered or not
    bool isRegistered() const;

    Participant(const string state, const string song, const int timeLength,
                const string singer);
    ~Participant() = default;
    Participant(Participant& p) = delete;

    //updating the registration status of the participant to the given status
    void updateRegistered(bool status);

     // updating the participant's values by the given values
    void update(const string song, const int timeLength, const string singer);
};

//Voter---------------------------------------------------------------

class Voter
{
    const string state_name;
    VoterType voter_type;
    int times_of_votes;

public:

    Voter(string state, VoterType voterType = Regular);
    ~Voter() = default;
    Voter(Voter& v) = delete;

    //returns the state name of the voter
    string state() const;
    //returns enum with the voter type
    VoterType voterType() const;

    //returns the amount of times the voter voted
    int timesOfVotes() const;

    //update times of votes
    Voter& operator++();

};

//ParticipantWVotes---------------------------------------------------

//An array of registered participants including voting information
struct ParticipantWVotes
{
    Participant* participant_ptr;
    int reg_votes;
    int judge_votes;

    explicit ParticipantWVotes(Participant* p = nullptr, int regular_votes = 0,
                               int judge_votes = 0);

    ~ParticipantWVotes() = default;
};

//Vote----------------------------------------------------------------

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

};

//MainControl---------------------------------------------------------

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

public:

    MainControl(int maxSongLength = 180, int maxParticipants = 26,
                int maxRegularVotes = 5);
    //sets the phase of the contest
    void setPhase(Phase phase);
    ~MainControl();

    //checks if the given participant is legal
    bool legalParticipant(const Participant& p) const;

    //checks if the given state participates in the Eurovision or not
    bool participate(string state_name) const;

    //add a participant to the contest
    MainControl& operator+=(Participant& participant);
    //remove participant from the contest
    MainControl& operator-=(Participant &participant);

    //submits a vote of the given voter
    MainControl& operator+=(const Vote& vote);
    //B.3
    string operator()(const int place, const VoterType vt) const;

    //prints the eurovision details
    friend ostream& operator<<(ostream& os, const MainControl& eurovision);

    typedef ParticipantWVotes* Iterator;

    //returns the beginning of the container
    Iterator begin();
    //returns the end of the container (1 place after the last element)
    Iterator end();
    //comparing iterators
    bool operator==(const Iterator& iterator) const;
    //advancing iterators
    Iterator& operator++();
};

//section of print operators:
ostream& operator<<(ostream& os, const Participant& p);
ostream& operator<<(ostream& os, const Voter& v);
ostream& operator<<(ostream& os, const MainControl& eurovision);
ostream& operator<<(ostream& os, const ParticipantWVotes& pwv);

//PartB---------------------------------------------------------------

//B.1
using std::list;
template<typename T_Iterator>
T_Iterator Get(T_Iterator begin, T_Iterator end, int place){
    //validity check
    if (begin == end || place < 1) return end;

    int size = 0;
    //gets the value the type that the iterator points to
    list<typename std::iterator_traits<T_Iterator>::value_type> temp_list;

    for (T_Iterator i = begin; i != end; ++i){
        temp_list.push_back(*i);
        size++;
    }

    //validity check- size is not available before the loop
    if (size < place) return end;
    //list is sorted in ascending order
    temp_list.sort();

    for (int i = 0; i < place - 1; ++i){
        temp_list.pop_back();
    }

    typename std::iterator_traits<T_Iterator>::value_type result_value =
            temp_list.back();

    for (T_Iterator i = begin; i != end; ++i){
        if (*i == result_value) return i;
    }
    return end;
}

#endif

