#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#define FIFO "myfifo" //有名管道名字
#define BUFF_SIZE 1024
int main(int argc, char *argv[])
{
	char buff[BUFF_SIZE]; //欲写入管道的数据缓冲区
	int real_write;		  //写入管道的字节数
	int fd;				  //管道描述符
	int rw = 1;			  //管道写次数
	if (access(FIFO, F_OK) == -1)
	{ //测试有名管道FIFO是否存在，若不存在，则用mkfifo创建该管道
		if ((mkfifo(FIFO, 0666) < 0) && (errno != EEXIST))
		{ //创建管道"myfifo"，允许读写
			printf("Can NOT create fifo file!\n");
			exit(1);
		}
	}
	if ((fd = open(FIFO, O_WRONLY)) == -1)
	{ //调用open以只写方式打开FIFO，返回文件描述符fd
		printf("Open fifo error!\n");
		exit(1);
	}
	do
	{
		printf("请输入要写入管道的内容：");
		gets(buff);
		if ((real_write = write(fd, buff, BUFF_SIZE)) > 0)
			printf("第%d次写入管道: '%s'.\n", rw++, buff);
	} while (strlen(buff) != 0);
	close(fd);
	exit(0);
}
