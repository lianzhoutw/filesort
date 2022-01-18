#ifndef FILE_SORT_OPT_H_
#include <string>
#define FILE_SORT_OPT_H_

using namespace std;

/*
 * @brief Command line options
 */
class FileSortOpt{
public:
	string 	in_file;
	string 	out_file;	 
	int 	seg_size; 	// Size in bytes of each segment file 
	string 	cmd; 		// command
	int 	test_num; 	// Number of test numbers	

	const static string HELP_DESC;

	const static int MAX_SEG_SIZE = 262144000; //250M
	const static int MIN_SEG_SIZE = 1048576; //1M

	FileSortOpt(){
		seg_size = 10485760; //10M
		test_num = 1000000;
	}

	string to_string(){
		return "in_file:" + in_file + "\n" +
			"out_file:" + out_file + "\n" +
			"seg_size:" + std::to_string(seg_size) + "\n";
	}

};
const string FileSortOpt::HELP_DESC = "Sort a big file which can't be loaded into memory\n\
  Usage: filesort [OPTIONS] FILE\n\
  -h,--help 	Display this help and exit.\n\
  -o,--out-file	Output file which stores the sorting results\n\
  -t,--test-num	Number of test content, default is 1000000\n\
  -c,--cmd 		Command to execute\n\
  				- sort (default) 	Sort the FILE\n\
				- gen_test 			Generate test content to FILE\n";
#endif
