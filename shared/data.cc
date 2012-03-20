#include<iostream>
#include "data.h"

using namespace std;
typedef unsigned int id_type;
using data::Article;
using data::Ng;
namespace data{

Data::Data(){
	latestNgId = 0;
}

map<id_type, Ng> Data::allNgs(){
	return ngs;
}

bool Data::saveToFile(){
	return true;
}

int Data::createNg(const string& n){
	++latestNgId;
	for(map<id_type, Ng>::iterator it = ngs.begin(); it != ngs.end(); ++it){
		if((*it).second.name == n){ //the ngname alredy exists
			return NO_NG;
		}
	}
	Ng ng(n, latestNgId);
	ngs.insert(make_pair(latestNgId, ng));
	return OK;
}

int Data::delNg(id_type ngId){
	if(ngs.erase(ngId) == 1){
		return OK;
	}
	return NO_NG;
}

pair<int, map<id_type, Article> > Data::allArtsInNg(id_type ngId){
	map<id_type, Ng>::iterator it = ngs.find(ngId);
	if(it == ngs.end()){ //Newsgroup doesn't exist
		map<id_type, Article> m;
		return make_pair(NO_NG, m);
	}
	return make_pair(OK, (*it).second.arts);
}

int Data::createArt(id_type ngId, const string& name, const string& auth, const string& text){
	map<id_type, Ng>::iterator it = ngs.find(ngId);
	if(it == ngs.end()){ //Newsgroup doesn't exist
		return NO_NG;
	}
	Article art(name, auth, text);
	(*it).second.newArt(art);
	return OK;
}

int Data::delArt(id_type ngId, id_type artId){
	map<id_type, Ng>::iterator it = ngs.find(ngId);
	if(it == ngs.end()){ //Newsgroup doesn't exist
		return NO_NG;
	}
	if((*it).second.arts.erase(artId) == 1) {
		return OK;
	} else { //Article doesn't exist
		return NO_ART;
	}
}

pair<int, Article> Data::getArt(id_type ngId, id_type artId){
	map<id_type, Ng>::iterator it = ngs.find(ngId);
	if(it == ngs.end()){ //Newsgroup doesn't exist
		Article a;
		return make_pair(NO_NG, a);
	}
	Ng ng = (*it).second;
	map<id_type, Article>::iterator it2 = ng.arts.find(artId);
	if(it2 == ng.arts.end()){ //Article doesn't exist
		Article a;
		return make_pair(NO_ART, a);
	}
	return make_pair(OK, (*it2).second);
}
}

int main() {
/*
	using namespace data;
	Data d;

	if(d.createNg("Gelicas ng")){
		cout << "yay! I own the only ng ^^" << endl;
	} else {
		cout << "no ng for me :(" << endl;
	}
	if(d.createArt(1, "a", "b", "c")){
		cout << "1 art!" << endl;
	} else {
		cout << "fuk art" << endl;
	}
	
	map<id_type, Article> all = d.allArtsInNg(1);
	for(map<id_type, Article>::iterator it = all.begin(); it != all.end(); ++it){
		cout << "oj" << endl;
	}


	stringstream ss0(stringstream::in | stringstream::out);
	cout << "Printing all newsgroups.." << endl;

	for(map<id_type, Ng>::iterator it = d.ngs.begin(); it != d.ngs.end(); ++it){
		it->second.toString(ss0);
		cout << ss0.str();
	}
	//*/
	/*
	Article a = d.getArt(1, 4);
	cout << "id? " << a.id << endl;
	*/
	/*
	Article a("111 11", "222 22", "3333 33", "4444 44", 123);

	stringstream ss(stringstream::in | stringstream::out);
	Article b("aaaa  aa", "bbb  bb", "ccccc  cc", "dddd  dd", 123);
	b.toString(ss);
	cout << "B: " << ss.str() << endl;*/



/*	Article c(ss.str());
//	stringstream ss2(stringstream::in | stringstream::out);
//	c.toString(ss2);
//	cout << "C: " << ss2.str();
	
	Article c(ss.str());
	stringstream ss2(stringstream::in | stringstream::out);
	c.toString(ss2);
	cout << "C: " << ss2.str();
	*/
}
