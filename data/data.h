#ifndef DATA_H
#define DATA_H

#include <string>
//#include <map>
#include "ng.h"

class Data {
typedef unsigned int id_type;
public:
	Data();
	std::map<id_type, Ng> allNgs();
	bool createNg(const std::string& n);
	bool delNg(id_type ngId);
	std::map<id_type, Article> allArtsInNg(id_type ngId);
	bool createArt(id_type ngId, const std::string& name, const std::string& auth, const std::string& text);
	bool delArt(id_type ngId, id_type artId);
	Article getArt(id_type ngId, id_type artId);
private:
	id_type latestNgId;
	std::map<id_type, Ng> ngs;
};


#endif
