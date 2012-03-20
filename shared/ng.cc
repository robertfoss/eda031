#include <iostream>
#include <ctime>
#include <sstream>
# include "ng.h"

//#define DEBUG_NG
#ifdef DEBUG_NG
#define trace std::cout
#else
#define trace if(false) std::cout
#endif

using namespace std;
using data::Article;

namespace data{

Ng::Ng(const string& n, id_type ngId) {
	created = getTimeString();
	name = n;
	latestArtId = 0;
	id = ngId;		// Id numbers may not be reused
}


Ng::Ng(const std::string& strs){
	stringstream iss(strs);
	string str[4];
	trace << "Parsing article..\n\"" << strs << "\"\n";
	for(unsigned int i = 0; i < 4; ++i){
		unsigned int len;
		iss >> len;
		trace << "Length found: " << len << "..  " << endl;
		iss.ignore(1);
		
		char* buffer = new char [len];
		iss.read(buffer,len);
		str[i] = string(buffer, len);
		trace << "Parsed: " << str[i] << endl;
		delete[] buffer;
		iss.ignore(2);
		
	}
	created = str[0];
	name = str[1];
	iss >> id;
	iss.ignore(1); // ignore last newline

	//peek next char, if tab then is article else is ng endif.
}

std::stringstream&
Ng::toString(std::stringstream& o){
	o << created.size()<< " " << created << "\t\t";
	o << name.size() << " " << name << "\t\t";
	o << id << "\t\t\n";

   	map<id_type, data::Article>::iterator it = arts.begin(); 
	for (; it != arts.end(); ++it) {
		o << "\t";
		(*it).second.toString(o);
	}
	return o;
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
}
