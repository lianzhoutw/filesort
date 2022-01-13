#include <stdio.h>
#include <getopt.h>
#include <iostream>
#include "file_sort.hpp"
#include "file_sort_opt.hpp"

using namespace std;

void genTestContent(const char* filePath, int num){
	const int BUF_LEN = 32;
	char buf[BUF_LEN];
	FILE *file = NULL;
	if((file = fopen(filePath , "w+")) == NULL){
		perror("open file error");
		exit(EXIT_FAILURE);
	}
	const int chunk_num = 100;
	int len, formatLen = 20;
	int last_rate, rate;
	last_rate = 0;
	int st, ed;
	st = 1;
	ed = chunk_num + 1;
	if(ed > num){
		ed = num + 1;
	}
	while(st <= num){
		vector<int> test_nums(ed - st);
		for(int n = num; n >= 1; n--){
			len = sprintf(buf, "%0*d\n", formatLen, n);
			fwrite(buf, 1, len, file);
			rate = (int)(((int64)(num - n) * 100) / num);
			if(rate > last_rate + 2 || rate == 100){
				printf("progress %d%%\n", rate);
				last_rate = rate;
			}
		}
		st = ed;
		ed = ed + chunk_num;
		if(ed > num){
			ed = num + 1;
		}
		// tangwulog here 没chunk_num个数字就打乱一下顺序
		break;
	}
	fclose(file);
}
 
int main(int argc, char *argv[])
{ 
	int c; 
	int digit_optind = 0; 
	string cmd;
	bool is_help;
	FileSortOpt opt;
	while (true) { 
		int this_option_optind = optind ? optind : 1; 
		int option_index = 0; 
		static struct option long_options[] = { 
			{"out-file",  	optional_argument, 0,  'o'}, 
			{"seg-size", 	optional_argument, 0,  's'},
			{"test-num", 	optional_argument, 0,  't'},
			{"cmd", 	  	optional_argument, 0,  'c'},
			{"help", 		no_argument,       0,  'h'},
			{0,         	0,                 0,   0} 
		}; 
		c = getopt_long(argc, argv, "i:o:s:t:c:h", long_options, &option_index); 
		if (c == -1) break; 
		switch (c) {
			case 'o': 
				opt.out_file = optarg; 
				break; 
			case 's': 
				opt.seg_size = atoi(optarg);
				break; 
			case 't': 
				opt.test_num = atoi(optarg);
				break; 
			case 'c': 
				opt.cmd = optarg;
				break; 
			case 'h': 
				is_help = true;
				break; 
			case '?': 
				//unknown option
				break; 
			default: 
				printf("?? getopt returned character code 0%o ??\n", c);
	   }
   }
   if(is_help){
	   cout << FileSortOpt::HELP_DESC << endl ;
	   return 0;
   }
   if(argc > 1){
	   opt.in_file = string(argv[argc - 1]);
   }
   cout << "opt.cmd: " << opt.cmd << endl;
   if(opt.cmd == "gen_test"){
	   genTestContent(opt.in_file.c_str(), opt.test_num);
   }else{
	   FileSort fileSort;
	   fileSort.sort(opt.in_file.c_str(), opt.out_file.c_str());
   }
   /*
	char *in_file = argv[1], *out_file = argv[2];
	printf("input file %s\noutput file %s\n", in_file, out_file);
	//genTestContent(in_file);
	*/
    return 0;
}
