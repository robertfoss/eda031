#ifndef NG_H
#define NG_H

#include <string>
#include <map>
#include "article.h"

class Ng {
	typedef unsigned int id_type;
	friend class Data;
	friend bool operator==(const Ng& ng1, const Ng& ng);
public:
	Ng(const std::string& name, id_type id);
	void newArt(Article& art);
	id_type id;	//unique
private:
	id_type latestArtId;
	std::string getTimeString();
	std::string created;
	std::string name;
	std::map<id_type, Article> arts;
};

bool operator==(const Ng& ng1, const Ng& ng2);

#endif
