#include "eurovision.h"
#include <functional>

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
	if(song != "")
		this->song_name = song;
	if(singer != "")
		this->song_duration = timeLength;
	if(timeLength != 0)
		this->singer_name = singer;
}

//Note this function is not part of CLASS Participant but in charge of printing it
ostream& operator<<(ostream& os, const Participant& p)
{
	string const part1 = "[" + p.state() + "/" + p.song() + "/";
	string const part2 = "/" + p.singer() + "]";

	return os << part1 << p.timeLength() << part2;
}