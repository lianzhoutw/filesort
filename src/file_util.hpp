#ifndef FILE_UTIL_H_
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <string>
#include "string_util.hpp"
#define FILE_UTIL_H_
using namespace std;

/*
 * Utility class of file 
 */
class FileUtil{
public:

	// Check whether the file [file_path] exists
	static bool isFileExist(const char *file_path){
		return !access(file_path, R_OK);
	}

	// Check whether the directory [dir_path] exists
	static bool isDirExist(string dir_path){
		DIR *dp;
		if((dp = opendir(dir_path.data())) == NULL){
			return false;
		}
		closedir(dp);
		return true;
	}
	
	static bool createDir(string dir){
		int flag = mkdir(dir.data(),S_IRUSR | S_IWUSR | S_IXUSR | S_IRWXG | S_IRWXO);
		return !flag;
	}
	
	// Create a random directory
	static string mkRandomDir(string prefix){
		const int NUM = 10;
		string dir;
		for(int i = 0; i < NUM; i++){
			dir = prefix + StringUtil::genRandomStr(8);
			if(!FileUtil::isDirExist(dir) && createDir(dir)){
				return dir;
			}
		}
		return "";
	}
};

#endif
