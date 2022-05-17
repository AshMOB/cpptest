#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
typedef struct _msg_buf
{//消息结构体
	long type;		//消息类型
	char buf[100];		//消息内容
} msg_buf;
int main()
{
	int key, qid; 							//消息队列键值和标识符
	msg_buf buf; 							//消息缓冲区
	key = ftok(".", 10); 					//将当前目录和10转换为消息队列IPC键值
	qid = msgget(key, IPC_CREAT|0666); 		//创建或获得消息队列标识符
	printf("key: %d\nqid: %d\n", key, qid); 	//输出消息队列键值和标识符
while (1)
	{
		if (msgrcv(qid, (void *)&buf, 100, 0, 0) < 0)
		{//循环读取队列中消息到缓冲区
			perror("msgrcv");
			exit(-1);
		}
		printf("type:%ld\nget:%s\n", buf.type, buf.buf);	//输出所读信息
	}
	return 0;
}