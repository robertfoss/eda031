#ifndef ART_H
#define ART_H

#include <string>
#include <sstream>

namespace data{
class Article {
	friend class Data;
	friend class Ng;
	friend bool CompArt(const Article& art1, const Article& art2);
	typedef unsigned int id_type;
public:
	Article(const std::string& str);
	Article(const std::string& name, const std::string& auth, const std::string& text);
	Article(const std::string& t, const std::string& a, const std::string& tx, const std::string& timestamp, id_type ids);
	std::stringstream& toString(std::stringstream& oss);
	id_type id; //unique in ng
	std::string getTitle();
	std::string getAuth();
	std::string getText();
	std::string toRealString();
private:
	Article();
	std::string title;
	std::string created;
	std::string getTimeString();
	std::string auth;
	std::string text;
};
}
#endif
