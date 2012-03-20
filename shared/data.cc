#include<iostream>
#include "data.h"

using namespace std;
typedef unsigned int id_type;

Data::Data(){
	
}

map<id_type, Ng> Data::allNgs(){
	return ngs;
}

bool Data::createNg(const string& n){
	++latestNgId;
	for(map<id_type, Ng>::iterator it = ngs.begin(); it != ngs.end(); ++it){
		if((*it).second.name == n){ //the ngname alredy exists
			return false;
		}
	}
	Ng ng(n, latestNgId);
	ngs.insert(make_pair(latestNgId, ng));
	return true;
}

bool Data::delNg(id_type ngId){
	if(ngs.erase(ngId) == 1){
		return true;
	}
	return false;
}

map<id_type, Article> Data::allArtsInNg(id_type ngId){
	map<id_type, Ng>::iterator it = ngs.find(ngId);
	if(it == ngs.end()){ //Newsgroup doesn't exist
		map<id_type, Article> m;
		return m;
	}
	return (*it).second.arts;
}

bool Data::createArt(id_type ngId, const string& name, const string& auth, const string& text){
	map<id_type, Ng>::iterator it = ngs.find(ngId);
	if(it == ngs.end()){ //Newsgroup doesn't exist
		return false;
	}
	Article art(name, auth, text);
	(*it).second.newArt(art);
	return true;
}

bool Data::delArt(id_type ngId, id_type artId){
	map<id_type, Ng>::iterator it = ngs.find(ngId);
	if(it == ngs.end()){
		return false;
	}
	if((*it).second.arts.erase(artId) == 1) {
		return true;
	}
	return false;
}

Article Data::getArt(id_type ngId, id_type artId){
	map<id_type, Ng>::iterator it = ngs.find(ngId);
	if(it == ngs.end()){ //Newsgroup doesn't exist
		Article a;
		return a;
	}
	Ng ng = (*it).second;
	map<id_type, Article>::iterator it2 = ng.arts.find(artId);
	if(it2 == ng.arts.end()){ //Article doesn't exist
		Article a;
		return a;
	}
	return (*it2).second;
}

int main() {
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

	Article a = d.getArt(1, 4);
	cout << "id? " << a.id << endl;

}