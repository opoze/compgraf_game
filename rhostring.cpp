/******************************************************************************\
* rhostring.cpp                                                                *
*                                                                              *
* Rossana B. Queiroz                                                           *
\******************************************************************************/

#include "rhostring.hpp"

// trim from end (right)
inline std::string& rtrim(std::string& s, const char* t)
{
	s.erase(s.find_last_not_of(t) + 1);
	return s;
}

// trim from beginning (left)
inline std::string& ltrim(std::string& s, const char* t)
{
	s.erase(0, s.find_first_not_of(t));
	return s;
}

// trim from both ends (left & right)
inline std::string& trim(std::string& s)
{
	return ltrim(rtrim(s, " \t\n\r\f\v"), " \t\n\r\f\v");
}

vector <string> tokenize(string buffer, string delim) {
   vector <string> tokens;
   string strbuffer(buffer);
   string token;
   int pos;

   do {
      pos = strbuffer.find(delim);
      if (pos > 0) {
         token = strbuffer.substr(0,pos);
         strbuffer = strbuffer.substr(pos+delim.size());
         tokens.push_back(token);
      }
      else tokens.push_back(strbuffer);
   } while(pos > 0);

   return tokens;
}

vector <string> tokenize(string buffer){
   vector <string> tokens;
   string token;
   istringstream sbuffer(buffer);

   do {
      token.clear();
      sbuffer >> token;
	  if (!token.empty())
		  trim(token);
         tokens.push_back(token);
   } while (!token.empty());
   return tokens;
}

void print_tokens(vector <string> tokens) {
   for (unsigned int i=0; i<tokens.size(); i++) {
      cout << tokens[i] << "\t";
   }
   cout << endl;
}
