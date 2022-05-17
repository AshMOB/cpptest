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
int main()
{
    char buff[BUFF_SIZE]; //欲读取管道的数据缓冲区
    int real_read;        //读取管道的字节数
    int fd;               //管道描述符
    int rc = 1;           //管道读次数
    if (access(FIFO, F_OK) == -1)
    { //测试有名管道FIFO是否存在，若不存在，则用mkfifo创建该管道
        if ((mkfifo(FIFO, 0666) < 0) && (errno != EEXIST))
        { //创建管道"myfifo"，允许读写
            printf("Can NOT create fifo file!\n");
            exit(1);
        }
    }
    if ((fd = open(FIFO, O_RDONLY)) == -1)
    { //以只读方式打开FIFO，返回文件描述符fd
        printf("Open fifo error!\n");
        exit(1);
    }
    while (1)
    { //循环读管道，若读空，则结束循环
        memset(buff, 0, BUFF_SIZE);
        if ((real_read = read(fd, buff, BUFF_SIZE)) > 0)
            printf("第%d次读取管道: '%s'.\n", rc++, buff);
        else
            break;
    }
    close(fd);
    exit(0);
}
