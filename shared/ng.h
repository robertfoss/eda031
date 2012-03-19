#ifndef NG_H
#define NG_H

#include <string>
#include <map>
#include "article.h"
namespace data{
class Ng {
	typedef unsigned int id_type;
	friend class Data;
	friend bool operator==(const data::Ng& ng1, const data::Ng& ng);
public:
	Ng(const std::string& name, id_type id);
	void newArt(data::Article& art);
	id_type id;	//unique
private:
	id_type latestArtId;
	std::string getTimeString();
	std::string created;
	std::string name;
	std::map<id_type, data::Article> arts;
};
}
bool operator==(const data::Ng& ng1, const data::Ng& ng2);

#endif
