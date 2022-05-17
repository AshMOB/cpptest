#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
int main(void)
{
    int sockfd; //客户端套接字描述符
    int len = 0;
    struct sockaddr_in address; //套接字协议地址
    char snd_buf[1024];         //发送消息缓冲区
    char rcv_buf[1024];         //接收消息缓冲区
    int result;
    int rcv_num; //接收消息长度
    pid_t cpid;  //客户进程标识符
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("客户端创建套接字失败！\n");
        return 1;
    }
    address.sin_family = AF_INET;                     //使用网络套接字
    address.sin_addr.s_addr = inet_addr("127.0.0.1"); //服务器地址
    address.sin_port = htons(9736);                   //服务器所监听的端口
    if (inet_aton("127.0.0.1", &address.sin_addr) < 0)
    {
        printf("inet_aton error.\n");
        return -1;
    }
    len = sizeof(address);
    cpid = getpid(); //获取客户进程标识符
    printf("1、客户机%ld开始connect服务器...\n", cpid);
    result = connect(sockfd, (struct sockaddr *)&address, len);
    if (result == -1)
    {
        perror("客户机connect服务器失败!\n");
        exit(1);
    }
    printf("-----------客户机%ld与服务器线程对话开始...\n", cpid);
    do
    { //客户机与服务器循环发送接收消息
        printf("2.客户机%ld--->服务器:sockfd=%d,请输入客户机要发送给服务器的消息：", cpid, sockfd);
        memset(snd_buf, 0, 1024);                //发送缓冲区清零
        scanf("%s", snd_buf);                    //键盘输入欲发送给服务器的消息字符串
        write(sockfd, snd_buf, sizeof(snd_buf)); //将消息发送到套接字
        if (strncmp(snd_buf, "!q", 2) == 0)
            break;                //若发送"!q"，则结束循环，通信结束
        memset(rcv_buf, 0, 1024); //接收缓冲区清零
        printf("客户机%ld,sockfd=%d 等待服务器回应...\n", cpid, sockfd);
        rcv_num = read(sockfd, rcv_buf, sizeof(rcv_buf));
        printf("客户机%ld,sockfd=%d 从服务器接收的消息长度=%d\n", cpid, sockfd, strlen(rcv_buf));
        printf("3.客户机%ld<---服务器:sockfd=%d,客户机从服务器接收到的消息是： (%d) :%s\n", cpid, sockfd, rcv_num, rcv_buf); //输出客户机从服务器接收的消息
        sleep(1);
        if (strncmp(rcv_buf, "quit", 4) == 0)
            break;                            //如果收到"quit"，则结束循环，通信结束
    } while (strncmp(rcv_buf, "!q", 2) != 0); //如果收到"!q"，则结束循环，通信结束
    printf("-----------客户机%ld,sockfd=%d 与服务器线程对话结束---------\n", cpid, sockfd);
    close(sockfd); //关闭客户机套接字
}