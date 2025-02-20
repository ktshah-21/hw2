#include <iostream>
#include <sstream>
#include <cctype>
#include <algorithm>
#include "util.h"

using namespace std;
std::string convToLower(std::string src)
{
    std::transform(src.begin(), src.end(), src.begin(), ::tolower);
    return src;
}

/** Complete the code to convert a string containing a rawWord
    to a set of words based on the criteria given in the assignment **/
std::set<std::string> parseStringToWords(string rawWords)
{


  string rawWord = convToLower(rawWords);
  int len = rawWord.size();
  std::set<std::string> retVal;
  std::string currHolder = "";
  for(int i=0; i<len; i++){
    if(!((rawWord[i]>='a' && rawWord[i] <='z') || (rawWord[i]>='A' && rawWord[i]<='Z'))){
      if(currHolder.size()>=2){
        retVal.insert(currHolder);
      }
      currHolder = "";
    }else{
      currHolder += rawWord[i];
    }
  }
  if(currHolder.size()>=2){
    retVal.insert(currHolder);
  }
  currHolder = "";
  return retVal;

}

/**************************************************
 * COMPLETED - You may use the following functions
 **************************************************/

// Used from http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
// trim from start
std::string &ltrim(std::string &s) {
    s.erase(s.begin(), 
	    std::find_if(s.begin(), 
			 s.end(), 
			 std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

// trim from end
std::string &rtrim(std::string &s) {
    s.erase(
	    std::find_if(s.rbegin(), 
			 s.rend(), 
			 std::not1(std::ptr_fun<int, int>(std::isspace))).base(), 
	    s.end());
    return s;
}

// trim from both ends
std::string &trim(std::string &s) {
    return ltrim(rtrim(s));
}

// int main(){
//   std::set<char>
//         p{'C', 'B', 'B', 'A'}, 
//         q{'E', 'D', 'E', 'C'};
//   std::set<char> o1 = setIntersection(p,q);
//   std::set<char> o2 = setUnion(p,q);
//   for (std::set<char>::iterator it = o1.begin(); it != o1.end(); ++it) {
//     std::cout << *it << " ";
//   }
//   std::cout<<std::endl;
//   for (std::set<char>::iterator it = o2.begin(); it != o2.end(); ++it) {
//     std::cout << *it << " ";
//   }
//   //std::cout<<__cplusplus<<endl;
// }