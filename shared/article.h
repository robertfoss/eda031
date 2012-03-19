#ifndef ART_H
#define ART_H

#include <string>
namespace data{
class Article {
	friend class Data;
	friend class Ng;
	friend bool CompArt(const Article& art1, const Article& art2);
	typedef unsigned int id_type;
public:
	Article(const std::string& name, const std::string& auth, const std::string& text);
	id_type id; //unique in ng
private:
	Article();
	std::string created;
	std::string getTimeString();
	std::string title;
	std::string auth;
	std::string text;
};
}
#endif
