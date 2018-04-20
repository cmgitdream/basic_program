
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include <boost/regex.h>

//namespace qi = boost::spirit::qi;
using boost::spirit::ascii::space;
using namespace boost::spirit;
using namespace boost::phoenix;
using std::string;

/*
template <typename Iterator>

struct stringlist : qi::grammer<Iterator, std::map<string, string>()>
{
  // constructor, init base_type
  stringlist() : stringlist::base_type(query)
  {
    query = pair
  }
}
*/

std::string text = "quota.user.1000.max_bytes";

int main(int argc, char *argv[])
{
  string quota;
  string type;
  string uid;
  int id;
  string qt;
  std::vector<int> ids;

  std::string::iterator first = text.begin();
  std::string::iterator last = text.end();

  bool r = qi::parse(first, last,
	"quota." >> +(qi::alpha) >> '.' >> +(qi::digit) >> '.' >> (+qi::char_("a-zA-Z_"))
	,type, uid, qt);
/*
  bool r = qi::phrase_parse(first, last,
	("quota.user." >> +(qi::digit)[push_back(ref(ids), _1)]
	>> ".max" >> (+qi::alpha)),
	space);
*/
  std::cout << "parse result = " << r
	<< ", first != last = " << (first != last ? "true" : "false")
	<< ", quota = " << quota
	<< ", type = " << type
	<< ", uid = " << uid
	<< ", bytes = " << qt
	<< std::endl;
}


