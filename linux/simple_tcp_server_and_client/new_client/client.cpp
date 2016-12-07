#include "stdio.h"
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/tcp.h>

static void SetSockOptionals(int sockfd)
{	
	socklen_t tmpLen = 0;

	linger tmp = {};
	tmp.l_onoff = 1;
	tmp.l_linger = 0;
	tmpLen = sizeof(tmp);

	setsockopt(sockfd, SOL_SOCKET, SO_LINGER, &tmp, tmpLen);
}

static void PrintSockOptions(int sockfd)
{
	printf("-------------Current Server Optionals---------------\n");

	int tmp = 0;
	socklen_t tmpLen = sizeof(tmp);

	getsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, (void *)&tmp, &tmpLen);
	printf("Recieve Buff Size:[%d].\n", tmp);
	
	tmp = 0;
	getsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, (void *)&tmp, &tmpLen);
	printf("Send Buff Size:[%d].\n", tmp);

	tmp = 0;
	getsockopt(sockfd, SOL_SOCKET, SO_RCVLOWAT, (void *)&tmp, &tmpLen);
	printf("Recieve Buffer low-water mark:[%d].\n", tmp);

	tmp = 0;
	getsockopt(sockfd, SOL_SOCKET, SO_SNDLOWAT, (void *)&tmp, &tmpLen);
	printf("Send Buffer low-water mark:[%d].\n", tmp);
	
	timeval tRes = {};
	tmpLen = sizeof(tRes);
	getsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (void *)&tRes, &tmpLen);
	printf("Recieve Timeout:[%d:%d].\n", tRes.tv_sec, tRes.tv_usec);

	memset(&tRes, 0, sizeof(tRes));
	getsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, (void *)&tRes, &tmpLen);
	printf("Send Timeout:[%d:%d].\n", tRes.tv_sec, tRes.tv_usec);

	tmp = 0;
	getsockopt(sockfd, IPPROTO_IP, IP_TTL, (void *)&tmp, &tmpLen);
	printf("IP-TTL:[%d].\n", tmp);

	tmp = 0;
	getsockopt(sockfd, IPPROTO_TCP, TCP_MAXSEG, (void *)&tmp, &tmpLen);
	printf("TCP-MSS:[%d].\n", tmp);

	tmp = 0;
	getsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, (void *)&tmp, &tmpLen);
	printf("TCP-Nagle algorithm:[%d].\n", tmp);


	printf("---------------------------------------------------\n");
}

int main ()
{
	int sid = socket(AF_INET, SOCK_STREAM, 0);
	
	if (-1 == sid)
	{
		perror(strerror(errno));
		return 0;
	}

	sockaddr_in serverInfo;
	serverInfo.sin_family = AF_INET;
	serverInfo.sin_port = htons(60000);

	if (0 == inet_aton("192.168.0.82", &serverInfo.sin_addr))	
	{
		perror("inet_aton failed.");
		return 0;
	}

	if (0 != connect(sid, (sockaddr *)&serverInfo, sizeof(serverInfo)))
	{
		perror(strerror(errno));
		return 0;
	}
	SetSockOptionals(sid);
	PrintSockOptions(sid);
		
	printf("Connection established.\n");
	printf("Start sending data.\n");
	
	unsigned int byteNum = 0;
	unsigned int buffer[1024] = {};
		
	for (unsigned int i=0, size=sizeof(buffer)/4; i<size; ++i)
	{
		buffer[i] = i;
	}

	int tmp =  write(sid, (void *)buffer, sizeof(buffer));

	if (-1  == tmp)
	{
		perror(strerror(errno));
	}
	else
	{
		printf("Byte:%d.", tmp);
	}
//	close(sid);


		
	printf("\nCompleted.");
	getchar();


	return 0;
} 
