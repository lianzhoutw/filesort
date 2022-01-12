#include <stdio.h>
#include "filesort.hpp"



void iniNumToFile(char* filePath){
	const int NUM = 100000000;
	const int BUF_LEN = 32;
	char buf[BUF_LEN];
	FILE *file = NULL;
	if((file = fopen(filePath , "w+")) == NULL){
		perror("open file error");
		exit(EXIT_FAILURE);
	}
	int len, formatLen = 20;
	int last_rate, rate;
	last_rate = 0;
	for(int n = NUM; n >= 1; n--){
		len = sprintf(buf, "%0*d\n", formatLen, n);
		fwrite(buf, 1, len, file);
		rate = (int)(((int64)(NUM - n) * 100) / NUM);
		if(rate > last_rate + 2 || rate == 100){
			printf("progress %d%%\n", rate);
			last_rate = rate;
		}
	}
	fclose(file);
}
 
int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("please enter input file!\n");
        return -1;
    }
    if (argc < 3)
    {
        printf("please enter output file!\n");
        return -1;
    }
	char *in_file = argv[1], *out_file = argv[2];
	printf("input file %s\noutput file %s\n", in_file, out_file);
	//iniNumToFile(in_file);
	FileSort fileSort;
	fileSort.sort(in_file, out_file);
    return 0;
}
