#include "eurovision.h"
#include <list>
#include <iterator>

using std::list;

//global variables
static const auto first_place = 12;
static const int second_place = 10;

//Participant---------------------------------------------------------

Participant::Participant(const string state, const string song,
                         const int timeLength, const string singer)
        : state_name(state), song_name(song), song_duration(timeLength),
          singer_name(singer) {
    is_registered = false;
}

bool Participant::isRegistered() const {
    return this->is_registered;
}

string Participant::state() const {
    return this->state_name;
}

string Participant::song() const {
    return this->song_name;
}

string Participant::singer() const {
    return this->singer_name;
}

int Participant::timeLength() const {
    return this->song_duration;
}


void Participant::updateRegistered(bool status) {
    this->is_registered = status;
}

void Participant::update(const string song, const int timeLength,
                         const string singer) {
    if (isRegistered()) return;

    if (song != "")
        this->song_name = song;
    if (timeLength != 0)
        this->song_duration = timeLength;
    if (singer != "")
        this->singer_name = singer;
}
//Voter---------------------------------------------------------------

Voter::Voter(string state, VoterType voterType) : state_name(state),
                                                  times_of_votes(0) {
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

Voter &Voter::operator++() {
    this->times_of_votes += 1;
    return *this;
}
//Vote----------------------------------------------------------------

Vote::Vote(Voter &vr, string s1, string s2, string s3, string s4, string s5,
           string s6, string s7, string s8, string s9, string s10) : vr(vr),
           states(new string[10]{
                   s1,
                   s2,
                   s3,
                   s4,
                   s5,
                   s6,
                   s7,
                   s8,
                   s9,
                   s10}){}

Vote::~Vote() {
    delete[] states;
}

MainControl &MainControl::operator+=(const Vote &v) {
    if (this->phase != Voting) return *this;

    //Regular:
  if (v.vr.voterType() == Regular) {
    if (v.vr.timesOfVotes() == maxRegularVotes||v.states[0]=="") return *this;
    if (!participate(v.vr.state()) || !participate(v.states[0])) return *this;
    if (v.vr.state() == v.states[0]) return *this;

        int i = 0;
        while ((i < this->maxParticipants) &&
               (contest_arr[i].participant_ptr != nullptr)) {
            if (contest_arr[i].participant_ptr->state() == v.states[0]) {
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
    for (int i = 0; i < 10; ++i) {
        if (v.states[i] == "") break; //end of the existing states
        if (v.states[i] == v.vr.state() || !participate(v.states[i])) continue;

        int j = 0;
        while ((j<maxParticipants)&&(contest_arr[j].participant_ptr!=nullptr)){
            if (contest_arr[j].participant_ptr->state() == v.states[i]){
                if (i == 0) contest_arr[j].judge_votes += first_place;
                else if (i == 1) contest_arr[j].judge_votes += second_place;
                else contest_arr[j].judge_votes += (second_place - i);

                judge_vote = true;
            }
            j++;
        }
    }

    if (judge_vote) ++v.vr;
    return *this;
}


//MainControl---------------------------------------------------------

MainControl::MainControl(int maxSongLength, int maxParticipants,
                         int maxRegularVotes) : maxSongLength(maxSongLength),
                                                maxParticipants(
                                                        maxParticipants),
                                                maxRegularVotes(
                                                        maxRegularVotes){
    this->contest_arr = new ParticipantWVotes[maxParticipants];
    this->phase = Registration;
}

void MainControl::setPhase(Phase phase) {
    if ((phase - this->phase) != 1) return;
    this->phase = phase;
}

bool MainControl::legalParticipant(const Participant &p) const {
    bool illigal_result = p.timeLength() > this->maxSongLength ||
                          p.state() == "" || p.song() == "" || p.singer() == "";

    return !illigal_result;
}


bool MainControl::isContestFull() {
    for (int i = 0; i < this->maxParticipants; i++) {
        if (contest_arr[i].participant_ptr == nullptr)
            return false;
    }
    return true;
}


bool MainControl::participate(string state_name) const {
    int i = 0;
    while (i < this->maxParticipants &&
        this->contest_arr[i].participant_ptr != nullptr) {
        if (this->contest_arr[i].participant_ptr->state() == state_name)
            return true;

        i++;
    }
    return false;
}

//ordered insertion:
MainControl &MainControl::operator+=(Participant &participant) {
//checks that all the prerequisites are valid
    const bool is_valid = this->legalParticipant(participant) &&
                          this->phase == Registration && !isContestFull() &&
                          !participate(participant.state());

    if (!is_valid) return *this;


    //adding the participantWVotes member to the contest_array

    int i = 0;
    //advancing i till the place where it's need to be added
    while (this->contest_arr[i].participant_ptr != nullptr &&
           i < this->maxParticipants &&
           this->contest_arr[i].participant_ptr->state()
                   .compare(participant.state()) < 0)
        i++;

    //separate into 2 cases:
    int j = i;
    if (this->contest_arr[j].participant_ptr == nullptr) {
        participant.updateRegistered(true);
        this->contest_arr[j].participant_ptr = &participant;
        return *this;
    } else {
        //advancing the i towards the last participant (not the end of the arr)
        while (this->contest_arr[i].participant_ptr != nullptr &&
               i < this->maxParticipants)
            i++;

        while (i > j) {
            swap(contest_arr[i], contest_arr[i - 1]);
            i--;
        }
        // reached to the place where needs to add the participant
        //and adding it
        participant.updateRegistered(true);
        contest_arr[i].participant_ptr = &participant;
        return *this;
    }
}


MainControl &MainControl::operator-=(Participant &participant) {
    const bool is_valid_state = this->phase == Registration &&
                                participant.isRegistered();
    //this->isParticipantRegistered(participant);
    if (!is_valid_state) return *this;


    //the state is registered for sure (was checked)
    int i = 0;
    while (this->contest_arr[i].participant_ptr->state() != participant.state())
        i++;

    //exist the loop and remove the current state
    participant.updateRegistered(false);
    this->contest_arr[i].participant_ptr = nullptr;

    //update the order in the array accordingly
    while (i + 1 < this->maxParticipants &&
        this->contest_arr[i + 1].participant_ptr != nullptr) {
        swap(this->contest_arr[i], this->contest_arr[i + 1]);
        i++;
    }

    return *this;
}


MainControl::~MainControl() {
    delete[] this->contest_arr;
}


//ParticipantWVotes---------------------------------------------------

ParticipantWVotes::ParticipantWVotes(Participant *p, int regular_votes,
                                     int judge_votes) :
        participant_ptr(p), reg_votes(regular_votes),
        judge_votes(judge_votes) {}


//overloading of the << operator--------------------------------------

//prints the data of a participant:
ostream &operator<<(ostream &os, const Participant &p) {
    string const part1 = "[" + p.state() + "/" + p.song() + "/";
    string const part2 = "/" + p.singer() + "]";

    return os << part1 << p.timeLength() << part2;
}

//prints the data of a voter
ostream &operator<<(ostream &os, const Voter &v) {
    switch (v.voterType()) {
        case 0:
            return os << "ALL Option was chosen";//just in case
        case 1:
            return os << '<' << v.state() << '/' << "Regular" << '>';
        case 2:
            return os << '<' << v.state() << '/' << "Judge" << '>';
        default:
            return os << "ENTERED DEFAULT IN ENUM"; //just in case of an error
    }
}


//prints the data of eurovision:
ostream &operator<<(ostream &os, const MainControl &eurovision) {
    os << "{" << endl;
    if (eurovision.phase == Contest) {
        os << "Contest" << endl;
        os << "}" << endl;
        return os;
    }
    if (eurovision.phase == Registration) {
        os << "Registration" << endl;
        for (int i = 0; i < eurovision.maxParticipants; i++) {
            if (eurovision.contest_arr[i].participant_ptr == nullptr) break;
            os <<"["<<eurovision.contest_arr[i].participant_ptr->state()<<"/";
            os << eurovision.contest_arr[i].participant_ptr->song() << "/";
            os << eurovision.contest_arr[i].participant_ptr->timeLength()
               << "/";
            os << eurovision.contest_arr[i].participant_ptr->singer() << "]"
               << endl;
        }
        os << "}" << endl;
        return os;
    } else {
        os << "Voting" << endl;
        for (int i = 0; i < eurovision.maxParticipants; i++) {
            if (eurovision.contest_arr[i].participant_ptr == nullptr) break;
            os << eurovision.contest_arr[i].participant_ptr->state() << " : ";
            os << "Regular(" << eurovision.contest_arr[i].reg_votes << ") ";
            os << "Judge(" << eurovision.contest_arr[i].judge_votes << ")"
               << endl;
        }
        os << "}" << endl;
        return os;
    }
}


//Part B.2------------------------------------------------------------


ParticipantWVotes *MainControl::begin() {
    return this->contest_arr;
}

ParticipantWVotes *MainControl::end() {
    int count = 0;
    while (count < this->maxParticipants &&
        this->contest_arr[count].participant_ptr != nullptr)
        count++;

    return (this->contest_arr + count);
}

MainControl::Iterator &MainControl::operator++() {
    //Maybe a handle of out of memory exception is needed here
    this->_iterator++;
    this->cur_iterator = this->contest_arr + _iterator;
    return this->cur_iterator;
}


bool MainControl::operator==(const Iterator &i) const {
    if ((this->contest_arr + _iterator)->participant_ptr->state() ==
        i->participant_ptr->state())
        return true;

    return false;
}


ostream &operator<<(ostream &os, const ParticipantWVotes &pwv) {
    return os << (*pwv.participant_ptr);
}


string MainControl::operator()(const int place, const VoterType vt) const {
    list<int> votes;
    string result = "";

    int i = 0;
    while (i<maxParticipants && this->contest_arr[i].participant_ptr!= nullptr){
        if (vt == Regular) votes.push_back(this->contest_arr[i].reg_votes);
        if (vt == Judge) votes.push_back(this->contest_arr[i].judge_votes);
        if (vt == All)votes.push_back(this->contest_arr[i].reg_votes+
                                      this->contest_arr[i].judge_votes);
        i++;
    }
    //sanity check
    if (i == 0) return result;

    //getting the desired place and checking if it's legal
    auto const get_result = Get(votes.begin(), votes.end(), place);
    if (get_result == votes.end()) return result;

    //sorting to find the relative place of the result
    votes.sort();

    //getting the value out of the place
    int const chosen_votes = *get_result;

    int relative_place = 0, j = 0;

    //note: this loop destroys the votes list
    while (j < place - 1) {
        if (votes.back() == chosen_votes) relative_place++;

        j++;
        votes.pop_back();
    }

    //i is pointing to the end location
    for (--i; i >= 0; i--) {
        if ((vt == Regular && this->contest_arr[i].reg_votes == chosen_votes)||
        (vt == Judge && this->contest_arr[i].judge_votes == chosen_votes)
        ||(vt == All && (this->contest_arr[i].reg_votes +
                         this->contest_arr[i].judge_votes) == chosen_votes)){
            if (relative_place == 0){
                result = this->contest_arr[i].participant_ptr->state();
                return result;
            }
            relative_place--;
        }
    }
    return result;// result = "" didn't enter to any of the if statements
}





