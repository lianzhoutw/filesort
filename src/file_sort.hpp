#ifndef FILE_SORT_H_
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <queue>
#include <vector>
#include <algorithm>
#include "file_util.hpp"
#define FILE_SORT_H_

using namespace std;

typedef long long int64;

//文件的一行
class FileLine{
public:
	char *ptr;	
	int st, ed; //起始位置(不含ed)
	FileLine(char *_ptr, int _st, int _ed):ptr(_ptr), st(_st), ed(_ed) {}
	char* stPtr(){
		return ptr + st;
	}

	int size(){
		return ed - st;
	}
};

class FileSeg: public FileLine{
public:
	int left, right; //分段的区间(不含right)
	FileSeg(char *_ptr, int _st, int _ed, int _left, int _right):FileLine(_ptr, _st, _ed), 
		left(_left), right(_right) {}

	FileSeg(const FileSeg &a): FileLine(a.ptr, a.st, a.ed),
		left(a.left), right(a.right) {}

	bool toNext(){
		if(ed >= right) return false;
		st = ed;
		while(ed < right && ptr[ed] != '\n'){
			ed++;
		}
		ed++;
		return true;
	}
};

/*
 * @brief 比较函数
 * @param a < b 时是否返回ture
 */
bool fileLineSort(const FileLine &a, const FileLine &b, bool isLess){
	int i1, i2;
	for(i1 = a.st, i2 = b.st; i1 < a.ed || i2 < b.ed;){
		if(i1 >= a.ed ) return isLess;
		if(i2 >= b.ed) return !isLess;
		if(a.ptr[i1] > b.ptr[i2]) return !isLess;
		if(a.ptr[i1] < b.ptr[i2]) return isLess;
		i1++;
		i2++;
	}
	//字符相等
	return false;
}

bool fileLineSortLess(const FileLine &a, const FileLine &b){
	return fileLineSort(a, b, true);
}

bool fileLineSortGreater(const FileLine &a, const FileLine &b){
	return fileLineSort(a, b, false);
}

class FileLineLess{
public:
	// a < b
	bool operator()(FileLine &a, FileLine &b) const {
		return fileLineSort(a, b, true);
	}
};

class FileLineGreater{
public:
	// a > b
	bool operator()(FileLine &a, FileLine &b) const {
		return fileLineSort(a, b, false);
	}
};

class FileSort{
protected:
	int seg_size; 	//每个分段的长度
	int max_seg_size; //每个分段的最大长度

public:
	FileSort(){
		seg_size = 10485760;
		max_seg_size = 209715200; //200M
	}
	

	void setSegFileName(char* sort_file_name, const char* work_dir, int seq_num){
		sprintf(sort_file_name, "%s/sort_%d.txt", work_dir, seq_num);
	}

	int sort(const char *in_file, const char *out_file){
		int in_fd = 0, out_fd = 0;
		char *ptr = NULL, *out_ptr;
		struct stat buf = {0};
		if(!FileUtil::isFileExist(in_file)){
			fprintf(stderr, "Input file file %s can't be accessed\n", in_file);
			return -1;
		}
		if ((in_fd = open(in_file, O_RDWR)) < 0)
		{
			fprintf(stderr, "open file %s error\n", in_file);
			return -1;
		}

		if ((out_fd = open(out_file, O_RDWR | O_CREAT, 0766)) < 0)
		{
			fprintf(stderr, "open file %s error\n", out_file);
			return -1;
		}
	 
		if (fstat(in_fd, &buf) < 0)
		{
			printf("get file state error:%d\n", errno);
			close(in_fd);
			return -1;
		}
	 
		printf("mmap in_fd=%d\n", in_fd);
		ptr = (char *)mmap(NULL, buf.st_size, PROT_READ|PROT_WRITE, MAP_SHARED, in_fd, 0);
		if (ptr == MAP_FAILED)
		{
			printf("mmap failed errno:%d\n", errno);
			close(in_fd);
			return -1;
		}
		close(in_fd);
		printf("mmap out_fd=%d\n", out_fd);
		ftruncate(out_fd, buf.st_size);
		out_ptr = (char *)mmap(NULL, buf.st_size, PROT_READ|PROT_WRITE, MAP_SHARED, out_fd, 0);
		if (out_ptr == MAP_FAILED)
		{
			printf("mmap failed errno:%d\n", errno);
			close(out_fd);
			return -1;
		}
		close(out_fd);
		//创建临时目录
		const int FILE_NAME_MAX_LEN = 64;
		char sort_file_name[FILE_NAME_MAX_LEN];
		string work_dir_str = FileUtil::mkRandomDir("/tmp/sort_");	
		if(work_dir_str.size() == 0){
			printf("Create random work directory error\n");
			return -1;
		}
		const char *work_dir = work_dir_str.data();
		printf("work_dir:%s\n", work_dir);
		mkdir(work_dir, 0766);
		const int CBUF_LEN = 32;
		printf("length of the file is : %d\n", buf.st_size);
		int left, right, st, ed;
		priority_queue<FileSeg, vector<FileSeg>, FileLineGreater> pque;

		char cbuf[CBUF_LEN] = {0};
		int cbuf_len, file_name, file_num = 0;
		vector<char*> seg_fd_map_vec; //分段文件的内存映射指针
		vector<int> seg_len_vec; //分段文件大小
		int seg_fd, seg_file_size, read_file_len;
		char* seg_fd_map;
		vector<FileLine> file_line_vec;
		for(left = 0; left < buf.st_size;){
			right = left + seg_size;	
			if(right >= buf.st_size){
				right = buf.st_size;
			}
			while(right < buf.st_size){
				if(ptr[right] == '\n'){
					right++;
					break;
				}
				right++;
			}
			seg_file_size = right - left;
			if(seg_file_size > max_seg_size){
				fprintf(stderr, "seg_file_size(%d) is greater than max_seg_size(%d)\n", seg_file_size, max_seg_size);
				return -1;
			}
			setSegFileName(sort_file_name, work_dir, file_num);
			printf("File sort progress %d%% sort_file_name:%s\n", int(((int64)right * 100) / buf.st_size), sort_file_name);
			if((seg_fd = open(sort_file_name, O_RDWR | O_CREAT, 0766)) < 0){
				fprintf(stderr, "open sort file %s error\n", sort_file_name);
				return -1;
			}
			ftruncate(seg_fd, seg_file_size);
			seg_fd_map = (char *)mmap(NULL, seg_file_size, PROT_READ|PROT_WRITE, MAP_SHARED, seg_fd, 0);
			if (seg_fd_map == MAP_FAILED)
			{
				fprintf(stderr, "mmap failed errno:%d\n", errno);
				close(seg_fd);
				return -1;
			}
			close(seg_fd);
			seg_fd_map_vec.push_back(seg_fd_map);
			seg_len_vec.push_back(seg_file_size);
			st = ed = left;
			//对分块内容排序
			file_line_vec.clear();
			while(ed < right){
				if(ptr[ed] == '\n' || ed == (right - 1)){
					ed++;
					file_line_vec.push_back(FileLine(ptr, st, ed));
					st = ed;
				}else{
					ed++;
				}
			}
			std::sort(file_line_vec.begin(), file_line_vec.end(), fileLineSortLess);
			//写入分块文件
			st = 0;
			for(FileLine file_line : file_line_vec){
				strncpy(seg_fd_map + st, file_line.stPtr(), file_line.size());
				st += file_line.size();
			}
			msync(seg_fd_map, seg_file_size, MS_SYNC);
			left = right;
			file_num++;
		}
		munmap(ptr, buf.st_size);
		//多路归并排序并写入文件
		for(int i = 0; i < seg_fd_map_vec.size(); i++){
			FileSeg file_seg(seg_fd_map_vec[i], 0, 0, 0, seg_len_vec[i]);
			file_seg.toNext();
			pque.push(file_seg);
		}
		int out_len = 0;
		int last_rate = 0, rate;
		printf("Start to sort\n");
		while(!pque.empty()){
			FileSeg top(pque.top());
			strncpy(out_ptr + out_len, top.stPtr(), top.size());
			out_len += top.size();
			rate = int(((int64)out_len * 100) / buf.st_size);
			if(rate > last_rate + 2 || rate == 100){
				last_rate = rate;
				printf("Sort progress %d%%\n", rate);
			}
			pque.pop();
			// printf("top[%d,%d] [%d,%d]\n", top.st, top.ed, top.left, top.right);
			if(top.toNext()){
				// printf("new top[%d,%d] [%d,%d]\n", top.st, top.ed, top.left, top.right);
				pque.push(top);
			}
		}
		munmap(out_ptr, buf.st_size);
		bool del_tmp_file = true;
		for(int i = 0; i < seg_fd_map_vec.size(); i++){
			munmap(seg_fd_map_vec[i], seg_len_vec[i]);
			if(del_tmp_file){
				setSegFileName(sort_file_name, work_dir, i);
				remove(sort_file_name);
			}
		}
		printf("remove work_dir:%s\n", work_dir);
		remove(work_dir);
		return 0;
	}
};

#endif
