#ifndef STRING_UTIL_H_
#define STRING_UTIL_H_
#include <time.h>

using namespace std;

/*
 * Utility class of string
 */
class StringUtil{
public:
	const static string RANDOM_CHARS;

	// Create a random directory
	static string genRandomStr(int len){
		srand((unsigned)time(NULL));
		string str;
		int k;
		for(int i = 0; i < len; i++){
			k = rand() % StringUtil::RANDOM_CHARS.size();
			str += StringUtil::RANDOM_CHARS.substr(k, 1);
		}
		return str;
	}
};
const string StringUtil::RANDOM_CHARS = "abcdefghijkmnpqrstuvwxyz0123456789";
#endif
