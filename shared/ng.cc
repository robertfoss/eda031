#include <ctime>
#include <sstream>
# include "ng.h"

using namespace std;

Ng::Ng(const string& n, id_type ngId) {
	created = getTimeString();
	name = n;
	latestArtId = 0;
	id = ngId;		// Id numbers may not be reused
}

void Ng::newArt(Article& art){
	++latestArtId;
	art.id = latestArtId;
	arts.insert(make_pair(latestArtId, art));
}


string Ng::getTimeString() {
	time_t timer = time(0);
	tm* locTime = localtime(&timer);
	std::stringstream ss;
	ss << (1900 + locTime->tm_year);
	if((1 + locTime->tm_mon) < 10){
		ss << 0;
	}
	ss << (1 + locTime->tm_mon);
	
	if(locTime->tm_mday < 10){
		ss << 0;
	}
	ss << (locTime->tm_mday);
	ss << " ";

	if(locTime->tm_hour < 10){
		ss << 0;
	}
	ss << (locTime->tm_hour);
	ss << ":";
	
	if(locTime->tm_min < 10){
		ss << 0;
	}
	ss << (locTime->tm_min);
	ss << ":";

	if(locTime->tm_sec < 10){
		ss << 0;
	}
	ss << (locTime->tm_sec);
	return ss.str();
}

bool operator==(const Ng& ng1, const Ng& ng2){
	return ng1.id == ng2.id;
}

