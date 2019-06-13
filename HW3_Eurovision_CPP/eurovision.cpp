#include "eurovision.h"



ParticipantWVotes::ParticipantWVotes(Participant* p, int regular_votes, int judge_votes) :
	participant_ptr(p), reg_votes(regular_votes), judge_votes(judge_votes){}

