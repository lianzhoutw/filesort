#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <queue>
#include <vector>


class FileSeg{
public:
	char *ptr;	
	int st, ed; //当前访问到的起始位置(不含ed)
	int left, right; //分段的区间(不含right)
	FileSeg(char *_ptr, int _st, int _ed, int _left, int _right){
		ptr = _ptr;
		st = _st;
		ed = _ed;
		left = _left;
		right = _right;
	}
};

class FileSegLess{
public:
	// a < b
	bool operator()(FileSeg &a, FileSeg &b) const {
		int i1, i2;
		for(i1 = a.st, i2 = b.st; i1 < a.ed || i2 < b.ed;){
			if(i1 >= a.ed ) return true;
			if(i2 >= b.ed) return false;
			if(a.ptr[i1] > b.ptr[i2]) return false;
			if(a.ptr[i1] < b.ptr[i2]) return true;
			i1++;
			i2++;
		}
		//字符相等
		return false;
	}
};

class FileSegGreater{
public:
	// a > b
	bool operator()(FileSeg &a, FileSeg &b) const {
		int i1, i2;
		for(i1 = a.st, i2 = b.st; i1 < a.ed || i2 < b.ed;){
			if(i1 >= a.ed ) return false;
			if(i2 >= b.ed) return true;
			if(a.ptr[i1] > b.ptr[i2]) return true;
			if(a.ptr[i1] < b.ptr[i2]) return false;
			i1++;
			i2++;
		}
		//字符相等
		return false;
	}
};

void iniNumToFile(char* filePath){
	const int NUM = 5;
	const int BUF_LEN = 32;
	char buf[BUF_LEN];
	FILE *file = NULL;
	if((file = fopen(filePath , "w+")) == NULL){
		perror("open file error");
		exit(EXIT_FAILURE);
	}
	int len, formatLen = 20;
	for(int n = NUM; n >= 1; n--){
		len = sprintf(buf, "%0*d\n", formatLen, n);
		fwrite(buf, 1, len, file);
	}
	fclose(file);
}
 
int main(int argc, char *argv[])
{
    int fd = 0;
    char *ptr = NULL;
    struct stat buf = {0};
    if (argc < 2)
    {
        printf("please enter a file!\n");
        return -1;
    }
	char *filePath = argv[1]; 
	// tangwulog 采用数字初始化文件
	iniNumToFile(filePath);
    if ((fd = open(argv[1], O_RDWR)) < 0)
    {
        printf("open file error\n");
        return -1;
    }
 
    if (fstat(fd, &buf) < 0)
    {
        printf("get file state error:%d\n", errno);
        close(fd);
        return -1;
    }
 
	printf("mmap fd=%d\n", fd);
    ptr = (char *)mmap(NULL, buf.st_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED)
    {
        printf("mmap failed errno:%d\n", errno);
        close(fd);
        return -1;
    }
 
    close(fd);
	const int SEG_LEN = 20;  //每20个字节一个分段
	const int CBUF_LEN = 32;
    printf("length of the file is : %d\n", buf.st_size);
	int left, right, st, ed;
	std::priority_queue<FileSeg, std::vector<FileSeg>, FileSegLess> pque;

	char cbuf[CBUF_LEN] = {0};
	int cbuf_len;
	for(left = 0; left < buf.st_size;){
		right = left + SEG_LEN;	
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
		st = ed = left;
		while(ed < right){
			if(ptr[ed] == '\n'){
				ed++;
				break;
			}
			ed++;
		}
		// tangwulog here 在内存里对区间[left, right]的数据排序然后写入到文件
		pque.push(FileSeg(ptr, st, ed, left, right));
		left = right;
	}
	while(!pque.empty()){
		FileSeg fileSeg = pque.top();
		cbuf_len = fileSeg.ed - fileSeg.st;
		strncpy(cbuf, fileSeg.ptr + fileSeg.st, cbuf_len);
		cbuf[cbuf_len] = 0;
		printf("cbuf:%s", cbuf);
		pque.pop();
	}
    munmap(ptr, buf.st_size);
    return 0;
}
