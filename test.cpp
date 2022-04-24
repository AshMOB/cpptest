#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
int main()
{
	pid_t pid;
	char *msg;
	int k;
	printf("观察父子进程执行的先后顺序，了解调度算法的特征\n");
	pid = fork();
	switch (pid)
	{
	case 0:
		msg = "子进程在运行";
		k = 3;
		break;
	case -1:
		msg = "进程创建失败";
		break;
	default:
		msg = "父进程在运行";
		k = 5;
		break;
	}
	while (k > 0)
	{
		puts(msg);
		sleep(1);
		k--;
	}
	exit(0);
}