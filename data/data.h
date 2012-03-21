#ifndef DATA_H
#define DATA_H

#include <string>
#include "ng.h"
namespace data{

	enum res {OK, NO_NG, NO_ART};

class Data {
typedef unsigned int id_type;

public:
	Data();
	std::map<id_type, Ng> allNgs();
	int createNg(const std::string& n);
	int delNg(id_type ngId);
	std::pair<int, std::map<id_type, Article> > allArtsInNg(id_type ngId);
	int createArt(id_type ngId, const std::string& name, const std::string& auth, const std::string& text);
	int delArt(id_type ngId, id_type artId);
	std::pair<int, Article> getArt(id_type ngId, id_type artId);
	void saveToFile();
	bool loadFromFile();
	std::map<id_type, Ng> ngs;
	std::string toString();
private:
	id_type latestNgId;
};
}

#endif
