#include <ctime>
#include <iostream>
#include <sstream>
# include "article.h"


//#define DEBUG_ARTICLE
#ifdef DEBUG_ARTICLE
#define trace cout
#else
#define trace if(false) cout
#endif

using namespace std;
namespace data{
Article::Article(const string& t, const string& a, const string& tx) {
	created = getTimeString();
	title = t;
	auth = a;
	text = tx;
}

Article::Article(const string& t, const string& a, const string& tx, const string& timestamp, id_type ids) {
	created = timestamp;
	title = t;
	auth = a;
	text = tx;
	id = ids;
}

Article::Article(const std::string& strs){
	
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
	title = str[1];
	auth = str[2];
	text = str[3];
	iss >> id;
	iss.ignore(1); // ignore last newline
}

std::stringstream&
Article::toString(std::stringstream& o){
	o << created.size()<< " " << created << "\t\t";
	o << title.size() << " " << title << "\t\t";
	o << auth.size() << " " << auth << "\t\t";
	o << text.size() << " " << text << "\t\t";
	o << id << "\t\t\n";
	trace << "toString() == " << o.str();
	return o;
}

Article::Article(){
	id = 0;
}

string Article::getTitle(){
	return title;
}

string Article::getAuth(){
	return auth;
}

string Article::getText(){
	return text;
}

string Article::getTimeString() {
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
}
