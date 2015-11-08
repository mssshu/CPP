/*
@author Luda Shu, set 3G
@date November 7, 2015
*/
#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <map>
#include <locale>
using namespace std;

/*a struct to hold elements in containers to use between various functions*/
struct myMap{
	map<string, string> mymap;
	vector<string> tagColors;
	vector<string> tagNames;
};

/*function prototypes*/
void readFile(istream& in, string& s, map<string,string>& m);
bool validTag(string& s);
void input(string& line, myMap& m);
int findTags(string& line, myMap& m);
string highlight(string& s, myMap& m);

/*
This function reads the config.txt file and stores the escape codes in a map.
It will check if the tag names are alphanumeric, any duplicate keys
are automatically skipped because a map contains unique values.

@param in - an argument passed in through the stream.
@param s - a string to read the lines in the file.
@param m - an empty map to store the variables.

string tag - word for the tag.
string escape - word for the escape code.
*/
void readFile(istream& in, string& s, map<string,string>& m){
	string tag;
	string escape;
   	regex re(R"((\\)(e))");
  
	while(getline(in,s)){
		istringstream iss(s);

		if(iss>>tag && iss>>escape){ 	
			escape = regex_replace(escape, re, "\033");

			if(validTag(tag)){
				m.insert(make_pair(tag,escape));	

			}else{
				cerr << "Error! Invalid tag name.";
				return;
			}
		}
	}	
}

/*
This function checks if the tag is alphanumeric.

@param s - tag word.
*/
bool validTag(string& s){
	for(auto it = s.begin(); it!= s.end(); ++it){
		if(!isalnum(*it)){		
			return false;
		}
	}
	return true;
}

/*
this function is for reading from the standard input

@param line - words from the stream.
@param m - to access the map.

string s - to append multiple lines into one.
int count - line counter.
*/
void input(string& line, myMap& m){
	string s = "";
	int count = 0;

	while(!getline(cin,line).eof()){
		s += line + "\n";
		count++;
	}

	auto it = m.mymap.find("text");
	if(findTags(s,m) == 0){
		cout << it->second << "Error at line " << count - 1 << endl;
	}	
}
/*
This function first stores the matches for the text tags. Then it calls
a regex iterator to iterate through all other possible matches. 

It replaces all "&lt;" matches from the prefix, and returns the value
"lt" as the replaced prefix. It replaces all "&gt;" matches in the
new prefix and returns the value "gt" as another replaced prefix.
Then this new prefix is used for the rest of the iterator.

If the prefix of the text tag includes whitespace, the erase string 
method is called to erase all whitespace. If there are characters
other than a whitespace, it will return an error. 

If there are characters other than a whitespace after the </text> tag, 
it will return an error. Otherwise, whitespace is erased.

If there is an error from the highlight function, it returns an error.

If everything is okay, it prints out the prefix and then the highlighted 
result.

@param line - words from the input.
@param map - to access the map.

regex text - expression for opening text tag.
regex tag - all other tags.
regex gtRe - to replace to >.
regex ltRe - to replace to <.
smatch match - matching value.
string foundTag, prefix, suffix, ws, etc. - to store regex matches.
*/
int findTags(string& line, myMap& m){

	regex text(R"((\s*)(<text>))");
	regex text2(R"((</text>)(\s*))");
	regex tag(R"((<)((/?)([a-z]+))(>)|>|<)");
	regex gtRe(R"(&gt;)");
	regex ltRe(R"(&lt;)");
	smatch match;
	string foundTag, suffix, ws, ws2, textEnd, textTag, lt, gt;

	if ( regex_search(line, match, text) ){
		ws = match.str(1);
		textTag = match.str(2);
	}
	if ( regex_search(line, match, text2) ){
		textEnd = match.str(1);
		ws2 = match.str(2);
	}
	regex_iterator<string::iterator> rit( line.begin(), line.end(), tag );
  	regex_iterator<string::iterator> rend;

	while ( rit != rend ){

		foundTag = rit->str();
		suffix = rit->suffix();

		lt = regex_replace(rit->prefix().str(),ltRe,"<");
		gt = regex_replace(lt,gtRe,">");
		
		//beginning text tag
		if(foundTag == textTag){
			if(gt != ws){
				return 0;
			}else{
				gt.erase(0, gt.length());
			}
		}
		//ending text tag
		if(foundTag == textEnd){
			if(suffix != ws2){
				return 0;
			}else{
				suffix.erase(1);
			}
		}		
		//unmatched tag
		if(highlight(foundTag,m) == " "){
			return 0;
		}
		//invalid char
		if(foundTag == ">" || foundTag == "<"){
			return 0;
		}
		cout << gt << highlight(foundTag, m);
		++rit;
	}
	return 1;
}
/*
This is the highlight function. 

If it finds an opening tag, it will replace the tag to the 
corresponding color by finding it in the map. Then stores both 
the name and color code into seperate vectors.

If it finds a closing tag, it will check if it has a match.
If there is no match, the tags are overlapped or has a different name
or is missing. It returns an error.
If there is a match, the last elements from both vectors are popped 
off the container and the tag is replaced with the previous color 
from the tagColors vector (if there is one).

Vectors tagNames and tagColors act as a stack.

@param s - string.
@param m - to access the map.

regex tag - expression to find.
smatch match - match.
string closeTag, backName, insideTag, wholeTag - stores match results.
*/
string highlight(string& s, myMap& m){
	regex tag(R"((<)(.+)(>))");
	smatch match;
	string closeTag, backName, insideTag, wholeTag;

	if (regex_match(s, match, tag)){
		wholeTag = match.str();
		insideTag = match.str(2);

		//opening tag
		auto it = m.mymap.find(insideTag);
		if(it != m.mymap.end()){	
			wholeTag = regex_replace(wholeTag, tag, it->second);	
			m.tagColors.push_back(it->second);
			m.tagNames.push_back(insideTag);
			s = wholeTag;	
		}	
		//closing tag
		else if(insideTag.substr(0,1) == "/"){
			closeTag = insideTag.substr(1);
			backName = m.tagNames.back();

			if(closeTag == backName){
				m.tagNames.pop_back();
			
				if(m.tagColors.size() > 1){
					m.tagColors.pop_back();	
					wholeTag = regex_replace(wholeTag,tag,m.tagColors.back());	
				}
				wholeTag = regex_replace(wholeTag, tag, "");
				s = wholeTag;	
			}else{
				//unmatched tag
				s = " ";
			}
		}
	}
	return s;
}
int main(int argc, char*argv[]){
	string line;
	myMap m;

	if(argc != 2){
		cerr << "usage: " << argv[0] << "[srcfile]\n";
		return 1;
	}
	fstream in(argv[1], ios_base::in|ios_base::binary);

	if(!in){
		cerr << "unable to open " << argv[1] << endl;
		return 2;
	}
	readFile(in, line, m.mymap);

	#ifdef DEBUG
	for(auto& i : m.mymap){
		cout << i.second << i.first << endl;
	}

	#else
	input(line, m);

	#endif
}