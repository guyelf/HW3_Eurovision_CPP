#include "eurovision.h"

MainControl::MainControl(int maxSongLength, int maxParticipants,
	int maxRegularVotes): maxSongLength(maxSongLength), 
						  maxParticipants(maxParticipants),
						  maxRegularVotes(maxRegularVotes)
{
	this->phase = Registration;
}

void MainControl::setPhase(Phase phase)
{
	if ((phase - this->phase) != 1) return;
	this->phase = phase;
}

bool MainControl::legalParticipant(Participant p)
{
	bool illigal_result = p.timeLength() > this->maxSongLength || p.state() == "" ||
		p.song() == "" || p.singer() ==  "";

	return !illigal_result;
}



