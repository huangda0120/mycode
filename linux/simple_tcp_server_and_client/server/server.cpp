#include "stdio.h"
#include <unistd.h>
#include "arpa/inet.h"
#include <errno.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <time.h>

void GetCalendar(tm* outRes)
{
	if (!outRes) return;

	time_t timer = 0;
	time(&timer);
	
	*outRes = *(localtime(&timer));	
}

static void PrintPeerInfomation(sockaddr_in const & info, tm const & cur)
{
	char const * strAddr= inet_ntoa(info.sin_addr);
	printf("[%u:%u:%u-%u:%u:%u]", cur.tm_year, cur.tm_mon, cur.tm_mday, cur.tm_hour, cur.tm_min, cur.tm_sec);
	printf(":%s,%d.\n", strAddr, ntohs(info.sin_port));
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

int main()
{
	sockaddr_in addrInfo;
	addrInfo.sin_family = AF_INET;
	addrInfo.sin_port = htons(60000);
	
	if (0 == inet_aton("192.168.0.82", &addrInfo.sin_addr))
	{
		printf("inet_aton failed.\n");
		return -1;
	}

	//get a socket
	int sid = socket(addrInfo.sin_family, SOCK_STREAM, 0);

	if (0 > sid)
	{
		printf("can not get socket.\n");
		return sid;
	}

	//bind a socket to a address.
	int tmp = bind(sid, (sockaddr *)&addrInfo, sizeof(addrInfo));

	if (0 != tmp) 
	{
		perror(strerror(errno));
		return -1;
	}

	//Listen
	if (0 != listen(sid, 1))
	{
		perror(strerror(errno));
		return -1;
	}

	//Print the socket options
	//PrintSockOptions(sid);

	
	unsigned int count = 0;
	sockaddr_in clientInfo;
	socklen_t len;

	while (true)
	{
		memset(&clientInfo, 0, sizeof(clientInfo));
		len = sizeof(clientInfo);
		
		int cid = accept(sid, (sockaddr *)&clientInfo, &len);

		tm cur;
		GetCalendar(&cur);
		count++;

		PrintPeerInfomation(clientInfo, cur);
		PrintSockOptions(cid);

		char buffer[1024] = {};

		while (true)
		{
			int byte = read(cid, &buffer, sizeof(buffer)-1);

			if (0 > byte)
			{
				perror(strerror(errno));

//				if (-1 ==  close(cid))
//					perror(strerror(errno));

				break;
			}
			else if (!byte)
			{
				printf("The peer was disconnect.\n");

				if (-1 ==  close(cid))
					perror(strerror(errno));

				break;
			}
			else
			{
				printf("Byte Num:[%d].\n", byte);
			}
		}
	}

	return 0;
}
